#include "stdafx.h"
#include "SerialDataProc.h"
#include "BoneDataOfFrame.h"
#include "OpenGLWindow.h"
#include "cal3d/error.h"
#include "cal3d/vector.h"
#include "cal3d/quaternion.h"
#include "Timer.h"


static char frame_buf[1024];
static CMotion_Capture_MFCDoc * pdoc;
static CMainFrame*pMainFrame;
static OpenGLWindow* pOpenGLView;
static CObArray* p_frameDataArray;


CSerialDataProc::CSerialDataProc()
{
}


CSerialDataProc::~CSerialDataProc()
{
}

static  int count = 0;

int CSerialDataProc::dataProc(struct PtrForFrameAppDoc * pFrameViewDoc, const char buf[])
{
    CMotion_Capture_MFCDoc * pdoc = pFrameViewDoc->pDoc;
	CMainFrame*pMainFrame = pFrameViewDoc->pMainFrame;
	static unsigned int current_frame_index = -1;
	static Timer *timer_for_serial = nullptr;
	if (timer_for_serial == nullptr)
	{
		timer_for_serial = new Timer;
		if (!timer_for_serial->Init())
		{
			//Report an error message
			MessageBox(NULL, _T("Cannot initialize the timer_for_serial"), _T("ERROR"), MB_OK | MB_ICONINFORMATION);
			return 0;
		}
	}
	
	//为帧数据消息
	if ('D' == buf[0])
	{	
		frameDataProc(pFrameViewDoc, buf);
	}

	return 0;
}

int CSerialDataProc::frameDataProc(struct PtrForFrameAppDoc*pFrameViewDoc, const char buf[])
{
	pdoc = pFrameViewDoc->pDoc;
	pMainFrame = pFrameViewDoc->pMainFrame;
	pOpenGLView = &(pFrameViewDoc->pView->m_view);
	p_frameDataArray = &(pdoc->frameDataArray);

	//数据包中的序列
	unsigned int buf_index = *(unsigned int*)(buf + 2);

	static unsigned int current_frame_index = -1;
	static Timer *timer_for_serial = nullptr;
	static CFrameData* pFrameData = nullptr;
	CBoneDataOfFrame* p_Bone_Data = nullptr;

	if (timer_for_serial == nullptr)
	{
		timer_for_serial = new Timer;
		if (!timer_for_serial->Init())
		{
			//Report an error message
			MessageBox(NULL, _T("Cannot initialize the timer_for_serial"), _T("ERROR"), MB_OK | MB_ICONINFORMATION);
			return -1;
		}
	}

	//校准程序
	if (pMainFrame->GetCalibrationStartFlag() && !pMainFrame->GetCalibrationFlag())
	{
		static int calibration_Index = 0;
		if (calibration_Index < pMainFrame->calibration_Length) {
			int joint_id = pMainFrame->get_JointID_by_NodeID(buf[1]);
			calibration_sum[joint_id].w += ((float) *((short*)(buf + 24)));
			calibration_sum[joint_id].x += ((float) *((short*)(buf + 26)));
			calibration_sum[joint_id].y += ((float) *((short*)(buf + 28)));
			calibration_sum[joint_id].z += (((float) *((short*)(buf + 30))));
			real_calibration_numbs[joint_id]++;
			pMainFrame->calibration_Index = calibration_Index;
			if (buf_index != current_frame_index) {
				calibration_Index++;
				current_frame_index = buf_index;
			}
			return 0;
		}
		//计算偏差
		calculate_bias(calibration_sum);
		pMainFrame->ClearCalibrationStartFlag();
		pMainFrame->SetCalibrationFlag();
		calibration_Index = 0;
		memset(real_calibration_numbs, 0, sizeof(short) * 23);
		for (int i =0; i < 23;i++)
		{
			calibration_sum[i].clear();
			calibration_sum[i].w = 0;
		}
	}

	//有新一帧数据时，将前一帧数据制作成关键帧
	if (current_frame_index != buf_index)
	{

		if (current_frame_index != -1) {
			memset(frame_buf, 0, 1024);
			((CFrameData*)p_frameDataArray->GetAt(p_frameDataArray->GetCount() - 1))->check_Bone_Flag(pMainFrame->joint_num_and_node_num_pair);

			//关键帧制作成功，加入动画序列
			if (0 == makeKeyFrame(p_frameDataArray, frame_buf)) {
				int id = pOpenGLView->pCal3DbaseModel->addCoreAnimationByBuf(frame_buf);
				if (-1 != id) {
					//执行动画
					pOpenGLView->pCal3DchildModel->executeAction(id, 0.0f, 0.0f);
				}		
			}
		}

		//将当前帧序列改为新的序列
		current_frame_index = buf_index;
		//获取间隔时间
		float time = timer_for_serial->GetElapsedSeconds();

		pFrameData = new CFrameData(pOpenGLView->pCal3DbaseModel, time * 3, 23);
		int joint_id = pMainFrame->get_JointID_by_NodeID(buf[1]);
		if (joint_id == -1)
		{
			return NO_SUCH_JOINT_ID;
		}

		//获取关节号，进行获得数据标记
		pFrameData->set_Bones_Data_Flag(joint_id);
		p_Bone_Data = (CBoneDataOfFrame*)pFrameData->boneData.GetAt(joint_id);
		proc_Frame_Data(p_Bone_Data, buf, joint_id);
		p_frameDataArray->Add(pFrameData);

		return 0;
	}
	else
	{
		//获取关节号，进行获得数据标记
		int joint_id = pMainFrame->get_JointID_by_NodeID(buf[1]);
		pFrameData->set_Bones_Data_Flag(joint_id);
		p_Bone_Data = (CBoneDataOfFrame*)((CFrameData*)p_frameDataArray->GetAt(p_frameDataArray->GetCount() - 1))->boneData.GetAt(joint_id);
		proc_Frame_Data(p_Bone_Data, buf, joint_id);
		return 1;
	}
}

int CSerialDataProc::makeKeyFrame(CObArray * p_frameDataArray,char frame_buf[])
{
	char*temp = frame_buf;
	CalQuaternion temp_quaternion;
	CFrameData* ptempFrameData = (CFrameData*)p_frameDataArray->GetAt(p_frameDataArray->GetCount() - 1);
	
	int tempNum_Bones = ptempFrameData->num_bones;
	CObArray* ptempBoneDataArray =&(ptempFrameData->boneData);
	writeBytes(&temp, ptempFrameData->magic_token, 4);
	writeInteger(&temp, ptempFrameData->version);
	writeFloat(&temp, ptempFrameData->duration);
	writeInteger(&temp, ptempFrameData->num_bones);

	for (int i = 0; i < tempNum_Bones;i++)
	{
		CBoneDataOfFrame* temp_BoneDataOfFrame = (CBoneDataOfFrame*)ptempBoneDataArray->GetAt(i);
		writeInteger(&temp, temp_BoneDataOfFrame->bone_id);
		writeInteger(&temp, temp_BoneDataOfFrame->num_frame);

		writeFloat(&temp, temp_BoneDataOfFrame->time);
		writeFloat(&temp, temp_BoneDataOfFrame->t_x);
		writeFloat(&temp, temp_BoneDataOfFrame->t_y);
		writeFloat(&temp, temp_BoneDataOfFrame->t_z);

		//当此节点存在可用的父节点（能够更新数据的父节点）时，计算相对旋转
		if ((-1 != pMainFrame->joint_num_and_node_num_pair[i]) && (-1 != pMainFrame->joint_num_and_node_num_pair[father_child_joint_pair[i][0]]))
		{
			CalQuaternion relative_rotation;
			if (CALCULATE_RELATIVE_ROTATION_FAIL == calculate_relative_rotation(p_frameDataArray, i, relative_rotation))
			{
				return NO_SUTABLE_DATA;
			}

			relative_rotation = relative_rotation * bias[i];

			writeFloat(&temp, relative_rotation.x);
			writeFloat(&temp, relative_rotation.y);
			writeFloat(&temp, relative_rotation.z);
			writeFloat(&temp, relative_rotation.w);
			continue;
		}

		//此节点可更新数据但没有更新的数据时
		if (-1 == ptempFrameData->bones_data_flag[i])
		{
			if (p_frameDataArray->GetCount() > 1) {
				ptempFrameData = (CFrameData*)p_frameDataArray->GetAt(p_frameDataArray->GetCount() - 2);
				if (-1 == ptempFrameData->bones_data_flag[i])
				{
					return NO_SUTABLE_DATA;
				}
			}
			else {
				return NO_SUTABLE_DATA;
			}
			temp_quaternion.set(temp_BoneDataOfFrame->r_x, temp_BoneDataOfFrame->r_y, temp_BoneDataOfFrame->r_z, temp_BoneDataOfFrame->r_w);
			temp_quaternion = temp_quaternion * bias[i];
			writeFloat(&temp, temp_quaternion.x);
			writeFloat(&temp, temp_quaternion.y);
			writeFloat(&temp, temp_quaternion.z);
			writeFloat(&temp, temp_quaternion.w);
			continue;
		}
		temp_quaternion.set(temp_BoneDataOfFrame->r_x, temp_BoneDataOfFrame->r_y, temp_BoneDataOfFrame->r_z, temp_BoneDataOfFrame->r_w);
		temp_quaternion = temp_quaternion * bias[i];
		writeFloat(&temp, temp_quaternion.x);
		writeFloat(&temp, temp_quaternion.y);
		writeFloat(&temp, temp_quaternion.z);
		writeFloat(&temp, temp_quaternion.w);
	}

	return 0;
}

void CSerialDataProc::writeBytes(char** dest, const void * pBuffer, int length)
{
	for (int i  = 0;i < length;i++)
	{
		(*dest)[i] = ((char*)pBuffer)[i];
	}
	(*dest) += length;
	return;
}

void CSerialDataProc::writeFloat(char** dest, float value)
{
	*(float*)(*dest) = value;
	(*dest) += sizeof(float);
	return;
}

void CSerialDataProc::writeInteger(char** dest, int value)
{
	*(int*)(*dest) = value;
	(*dest) += sizeof(int);
	return;
}

void CSerialDataProc::writeString(char** dest, const std::string & strValue)
{
	int lengh = strValue.length();
	for (int i = 0;i < lengh;i++)
	{
		(*dest)[i] = strValue.c_str()[i];
	}

	(*dest) += lengh;
	return;
}

int CSerialDataProc::calculate_relative_rotation(CObArray * p_frameDataArray,int child_joint_id, CalQuaternion & result_data)
{
	int father_joint_id = father_child_joint_pair[child_joint_id][0];
	CFrameData* p_Frame_Data_for_Child = (CFrameData*)p_frameDataArray->GetAt(p_frameDataArray->GetCount() - 1);
	CFrameData* p_Frame_Data_for_Father = (CFrameData*)p_frameDataArray->GetAt(p_frameDataArray->GetCount() - 1);


	//检查当前节点数据是否是更新的数据，没有使用上一条，再没有，返回
	if (-1 == p_Frame_Data_for_Child->bones_data_flag[child_joint_id])
	{
		if (p_frameDataArray->GetCount() > 1) {
			p_Frame_Data_for_Child = (CFrameData*)p_frameDataArray->GetAt(p_frameDataArray->GetCount() - 2);
		}

		if (-1 == p_Frame_Data_for_Child->bones_data_flag[child_joint_id])
		{
			return CALCULATE_RELATIVE_ROTATION_FAIL;
		}
	}



	//检查父节点数据是否为空，没有使用上一条，再没有，返回
	if (-1 == p_Frame_Data_for_Father->bones_data_flag[father_joint_id])
	{
		if (p_frameDataArray->GetCount() > 1) {
			p_Frame_Data_for_Father = (CFrameData*)p_frameDataArray->GetAt(p_frameDataArray->GetCount() - 2);
		}
		if (-1 == p_Frame_Data_for_Father->bones_data_flag[father_joint_id])
		{
			return CALCULATE_RELATIVE_ROTATION_FAIL;
		}
	}


	CBoneDataOfFrame*p_temp_father_bone_data = (CBoneDataOfFrame*)(p_Frame_Data_for_Father->boneData.GetAt(father_joint_id));
	CalQuaternion revers_father_quat(-p_temp_father_bone_data->r_x, -p_temp_father_bone_data->r_y, -p_temp_father_bone_data->r_z, p_temp_father_bone_data->r_w);


	CBoneDataOfFrame*p_temp_child_bone_data = (CBoneDataOfFrame*)(p_Frame_Data_for_Child->boneData.GetAt(child_joint_id));
	CalQuaternion child_quat(p_temp_child_bone_data->r_x, p_temp_child_bone_data->r_y, p_temp_child_bone_data->r_z, p_temp_child_bone_data->r_w);

	result_data = revers_father_quat*child_quat;


	return 0;
}






void CSerialDataProc::proc_Frame_Data(CBoneDataOfFrame* p_Bone_Data,const char* buf,int joint_id)
{
	CalQuaternion temp,temp_after_colibration;
	p_Bone_Data->a_x = ((float) *((short*)(buf + 6))) / 10000.0f;
	p_Bone_Data->a_y = ((float) *((short*)(buf + 8))) / 10000.0f;
	p_Bone_Data->a_z = ((float) *((short*)(buf + 10))) / 10000.0f;

	p_Bone_Data->g_x = ((float) *((short*)(buf + 12))) / 10.0f;
	p_Bone_Data->g_y = ((float) *((short*)(buf + 14))) / 10.0f;
	p_Bone_Data->g_z = ((float) *((short*)(buf + 16))) / 10.0f;

	p_Bone_Data->m_x = ((float) *((short*)(buf + 18))) / 100.0f;
	p_Bone_Data->m_y = ((float) *((short*)(buf + 20))) / 100.0f;
	p_Bone_Data->m_z = ((float) *((short*)(buf + 22))) / 100.0f;

	switch (joint_id)
	{
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
		break;
	case 7:
		break;
	case 8:
	case 9:
		break;
	case 10:
		break;
	case 11:
		break;
	case 12:
	case 13:
		p_Bone_Data->r_w = ((float) *((short*)(buf + 24))) / 10000.0f;			// temp_w <- buf_w; 												
		p_Bone_Data->r_x = -((float) *((short*)(buf + 28))) / 10000.0f;			// temp_x <- buf_y;
		p_Bone_Data->r_y = -((float) *((short*)(buf + 26))) / 10000.0f;			// temp_y <- buf_x;
		p_Bone_Data->r_z = (((float) *((short*)(buf + 30))) / 10000.0f);			// temp_z <- buf_z;
		break;
	case 14:
		break;
	case 15:
	case 16:
		break;
	case 17:
		break;
	case 18:
		break;
	case 19:
	case 20:
		p_Bone_Data->r_w = ((float) *((short*)(buf + 24))) / 10000.0f;			// temp_w <- buf_w; 												
		p_Bone_Data->r_x = -((float) *((short*)(buf + 28))) / 10000.0f;			// temp_x <- buf_y;
		p_Bone_Data->r_y = -((float) *((short*)(buf + 26))) / 10000.0f;			// temp_y <- buf_x;
		p_Bone_Data->r_z = -(((float) *((short*)(buf + 30))) / 10000.0f);			// temp_z <- buf_z;
		break;
	case 21:
		break;
	case 22:
		break;
	default:
		break;
	}
}


void CSerialDataProc::calculate_bias(CalQuaternion* p_calibration_sum)
{
	CalQuaternion temp_quat, revers_father_quat,relative_rotation, reverse_relative_rotation;
	for (int i = 0; i < 23; i++)
	{
		p_calibration_sum[i].w /= (((float)(real_calibration_numbs[i])) * (10000.0f));
		p_calibration_sum[i].x /= (((float)(real_calibration_numbs[i])) * (10000.0f));
		p_calibration_sum[i].y /= (((float)(real_calibration_numbs[i])) * (10000.0f));
		p_calibration_sum[i].z /= (((float)(real_calibration_numbs[i])) * (10000.0f));
		
		switch (i)
		{
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
			break;
		case 7:
			break;
		case 8:
		case 9:
			break;
		case 10:
			break;
		case 11:
			break;
		case 12:
			//取反
			temp_quat.w = p_calibration_sum[i].w;  
			temp_quat.x = p_calibration_sum[i].y;
			temp_quat.y = p_calibration_sum[i].x;
			temp_quat.z = -p_calibration_sum[i].z;
			bias[i] = temp_quat*standard_Rotation[i];
			break;
		case 13:
			revers_father_quat.w = p_calibration_sum[12].w;
			revers_father_quat.x = p_calibration_sum[12].y;
			revers_father_quat.y = p_calibration_sum[12].x;
			revers_father_quat.z = -p_calibration_sum[12].z;

			temp_quat.w = p_calibration_sum[i].w;
			temp_quat.x = -p_calibration_sum[i].y;
			temp_quat.y = -p_calibration_sum[i].x;
			temp_quat.z = p_calibration_sum[i].z;

			relative_rotation = revers_father_quat*temp_quat;
			reverse_relative_rotation.set(-relative_rotation.x, -relative_rotation.y, -relative_rotation.z, relative_rotation.w);

			bias[i] = reverse_relative_rotation*standard_Rotation[i];
			break;
		case 14:
			break;
		case 15:
		case 16:
		case 17:
			break;
		case 18:
			break;
		case 19:
		case 20:
		case 21:
			break;
		case 22:
			break;
		default:
			break;
		}

	}



}
