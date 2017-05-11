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

	//有新一帧数据时，将前一帧数据制作成关键帧
	if (current_frame_index != *(int*)(buf + 2))
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
		current_frame_index = *(int*)(buf + 2);
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

		p_Bone_Data->a_x = ((float) *((short*)(buf + 6))) / 10000.0f;
		p_Bone_Data->a_y = ((float) *((short*)(buf + 8))) / 10000.0f;
		p_Bone_Data->a_z = ((float) *((short*)(buf + 10))) / 10000.0f;

		p_Bone_Data->g_x = ((float) *((short*)(buf + 12))) / 10.0f;
		p_Bone_Data->g_y = ((float) *((short*)(buf + 14))) / 10.0f;
		p_Bone_Data->g_z = ((float) *((short*)(buf + 16))) / 10.0f;

		p_Bone_Data->m_x = ((float) *((short*)(buf + 18))) / 100.0f;
		p_Bone_Data->m_y = ((float) *((short*)(buf + 20))) / 100.0f;
		p_Bone_Data->m_z = ((float) *((short*)(buf + 22))) / 100.0f;

		p_Bone_Data->r_w = ((float) *((short*)(buf + 24))) / 10000.0f;
		p_Bone_Data->r_y = ((float) *((short*)(buf + 26))) / 10000.0f;
		p_Bone_Data->r_x = ((float) *((short*)(buf + 28))) / 10000.0f;
		p_Bone_Data->r_z = ((float) *((short*)(buf + 30))) / 10000.0f;
		p_frameDataArray->Add(pFrameData);

		return 0;
	}
	else
	{
		//获取关节号，进行获得数据标记
		int joint_id = pMainFrame->get_JointID_by_NodeID(buf[1]);
		pFrameData->set_Bones_Data_Flag(joint_id);
		p_Bone_Data = (CBoneDataOfFrame*)((CFrameData*)p_frameDataArray->GetAt(p_frameDataArray->GetCount() - 1))->boneData.GetAt(joint_id);

		p_Bone_Data->a_x = ((float) *(short*)(buf + 6)) / 10000.0f;
		p_Bone_Data->a_y = ((float) *(short*)(buf + 8)) / 10000.0f;
		p_Bone_Data->a_z = ((float) *(short*)(buf + 10)) / 10000.0f;

		p_Bone_Data->g_x = ((float) *(short*)(buf + 12)) / 10.0f;
		p_Bone_Data->g_y = ((float) *(short*)(buf + 14)) / 10.0f;
		p_Bone_Data->g_z = ((float) *(short*)(buf + 16)) / 10.0f;

		p_Bone_Data->m_x = ((float) *(short*)(buf + 18)) / 100.0f;
		p_Bone_Data->m_y = ((float) *(short*)(buf + 20)) / 100.0f;
		p_Bone_Data->m_z = ((float) *(short*)(buf + 22)) / 100.0f;

		p_Bone_Data->r_w = ((float) *(short*)(buf + 24)) / 10000.0f; //w
		p_Bone_Data->r_y = ((float) *(short*)(buf + 26)) / 10000.0f; //x
		p_Bone_Data->r_x = ((float) *(short*)(buf + 28)) / 10000.0f; //y
		p_Bone_Data->r_z = ((float) *(short*)(buf + 30)) / 10000.0f; //z

		return 1;
	}
}

int CSerialDataProc::makeKeyFrame(CObArray * p_frameDataArray,char frame_buf[])
{
	char*temp = frame_buf;
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

		//if (i == 13) {
		//	writeFloat(&temp, temp_BoneDataOfFrame->time);
		//	writeFloat(&temp, temp_BoneDataOfFrame->t_x + 10);
		//	writeFloat(&temp, temp_BoneDataOfFrame->t_y + 10);
		//	writeFloat(&temp, temp_BoneDataOfFrame->t_z + 10);
		//}
		//else
		//{
			writeFloat(&temp, temp_BoneDataOfFrame->time);
			writeFloat(&temp, temp_BoneDataOfFrame->t_x);
			writeFloat(&temp, temp_BoneDataOfFrame->t_y);
			writeFloat(&temp, temp_BoneDataOfFrame->t_z);
//		}



		//当此节点存在可用的父节点（能够更新数据的父节点）时，计算相对旋转
		if ((-1 != pMainFrame->joint_num_and_node_num_pair[i]) && (-1 != pMainFrame->joint_num_and_node_num_pair[father_child_joint_pair[i][0]]))
//		if ((-1 != pMainFrame->joint_num_and_node_num_pair[i]) )
		{
			CalQuaternion relative_rotation;
			if (CALCULATE_RELATIVE_ROTATION_FAIL == calculate_relative_rotation(p_frameDataArray, i, relative_rotation))
			{
				return NO_SUTABLE_DATA;
			}
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

			writeFloat(&temp, temp_BoneDataOfFrame->r_x);
			writeFloat(&temp, temp_BoneDataOfFrame->r_y);
			writeFloat(&temp, temp_BoneDataOfFrame->r_z);
			writeFloat(&temp, temp_BoneDataOfFrame->r_w);
			continue;
		}
		writeFloat(&temp, temp_BoneDataOfFrame->r_x);
		writeFloat(&temp, temp_BoneDataOfFrame->r_y);
		writeFloat(&temp, temp_BoneDataOfFrame->r_z);
		writeFloat(&temp, temp_BoneDataOfFrame->r_w);	
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
	CalQuaternion father_quat(-p_temp_father_bone_data->r_x, -p_temp_father_bone_data->r_y, -p_temp_father_bone_data->r_z, p_temp_father_bone_data->r_w);

	CBoneDataOfFrame*p_temp_child_bone_data = (CBoneDataOfFrame*)(p_Frame_Data_for_Child->boneData.GetAt(child_joint_id));
	CalQuaternion revers_child_quat(p_temp_child_bone_data->r_x, p_temp_child_bone_data->r_y, p_temp_child_bone_data->r_z, p_temp_child_bone_data->r_w);
	
	result_data = father_quat*revers_child_quat;


	return 0;
}



