#include "stdafx.h"
#include "SerialDataProc.h"
#include "BoneDataOfFrame.h"
#include "OpenGLWindow.h"
#include "cal3d/error.h"
#include "Timer.h"

static char frame_buf[1024];


CSerialDataProc::CSerialDataProc()
{
}


CSerialDataProc::~CSerialDataProc()
{
}

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
//	TRACE("type:%c,index:%d,ID:%d,w:%d,x:%d,y:%d,z:%d\r\n",buf[0], *(int*)(buf + 1), *(buf + 5), *(short*)(buf + 24), *(short*)(buf + 26), *(short*)(buf + 28), *(short*)(buf + 30));
	//为数据消息
	if ('D' == buf[0])
	{
		
		
		if (current_frame_index != *(int*)(buf + 1))
		{
			
			OpenGLWindow* pOpenGLView = &(pFrameViewDoc->pView->m_view);
			if (current_frame_index != -1) {
				
				memset(frame_buf, 0, 1024);
				makeKeyFrame(pdoc->frameDataArray.GetAt(pdoc->frameDataArray.GetCount() - 1), frame_buf);
				int id = pOpenGLView->pCal3DbaseModel->addCoreAnimationByBuf(frame_buf);
				TRACE("%d\n", id);
				if (-1 == id) {
					return ADD_ANIMATION_FAIL;
				}
				if (pOpenGLView->pCal3DchildModel->GetAnimationCount()) {
					//if (-1 == pOpenGLView->pCal3DbaseModel->GetCore()->unloadCoreAnimation(id - 1)) {
					//	TRACE("%s,%s.%d\n", CalError::getLastErrorDescription().c_str(), CalError::getLastErrorFile().c_str(), CalError::getLastErrorLine());
					//}
					pOpenGLView->pCal3DchildModel->executeAction(id,0.0f,0.0f);
				}
			}

			current_frame_index = *(int*)(buf + 1);
			float time = timer_for_serial->GetElapsedSeconds();
			CFrameData* pFrameData = new CFrameData(pOpenGLView->pCal3DbaseModel, time*2, 23);
			((CBoneDataOfFrame*)(pFrameData->boneData.GetAt(12)))->r_w = ((float) *(short*)(buf + 24)) / 10000.0f;
			((CBoneDataOfFrame*)(pFrameData->boneData.GetAt(12)))->r_y = ((float) *(short*)(buf + 26)) / 10000.0f;
			((CBoneDataOfFrame*)(pFrameData->boneData.GetAt(12)))->r_x = ((float) *(short*)(buf + 28)) / 10000.0f;
			((CBoneDataOfFrame*)(pFrameData->boneData.GetAt(12)))->r_z = ((float) *(short*)(buf + 30)) / 10000.0f;
			pdoc->frameDataArray.Add(pFrameData);
		}
		else
		{

		}
	}

	return 0;
}

int CSerialDataProc::makeKeyFrame(CObject * pFrameData,char frame_buf[])
{
	char*temp = frame_buf;
	CFrameData* ptempFrameData = (CFrameData*)pFrameData;
	int tempNum_Bones = ptempFrameData->num_bones;
	CObArray* ptempBoneDataArray =&(ptempFrameData->boneData);
	writeBytes(&temp, ptempFrameData->magic_token, 4);
	writeInteger(&temp, ptempFrameData->version);
	writeFloat(&temp, ptempFrameData->duration);
	writeInteger(&temp, ptempFrameData->num_bones);

	for (int i = 0; i < tempNum_Bones;i++)
	{
		writeInteger(&temp, ((CBoneDataOfFrame*)ptempBoneDataArray->GetAt(i))->bone_id);
		writeInteger(&temp, ((CBoneDataOfFrame*)ptempBoneDataArray->GetAt(i))->num_frame);

		writeFloat(&temp, ((CBoneDataOfFrame*)ptempBoneDataArray->GetAt(i))->time);
		writeFloat(&temp, ((CBoneDataOfFrame*)ptempBoneDataArray->GetAt(i))->t_x);
		writeFloat(&temp, ((CBoneDataOfFrame*)ptempBoneDataArray->GetAt(i))->t_y);
		writeFloat(&temp, ((CBoneDataOfFrame*)ptempBoneDataArray->GetAt(i))->t_z);
		writeFloat(&temp, ((CBoneDataOfFrame*)ptempBoneDataArray->GetAt(i))->r_x);
		writeFloat(&temp, ((CBoneDataOfFrame*)ptempBoneDataArray->GetAt(i))->r_y);
		writeFloat(&temp, ((CBoneDataOfFrame*)ptempBoneDataArray->GetAt(i))->r_z);
		writeFloat(&temp, ((CBoneDataOfFrame*)ptempBoneDataArray->GetAt(i))->r_w);
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
