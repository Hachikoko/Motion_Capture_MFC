#pragma once
#include "Motion_Capture_MFCDoc.h"
#include "MainFrm.h"
#include "FrameData.h"
class CSerialDataProc
{
public:
	CSerialDataProc();
	~CSerialDataProc();

	enum DataProcState
	{
		ADD_ANIMATION_FAIL = -1,

	};

	static unsigned int pre_serialnum;

	static int dataProc(struct PtrForFrameAppDoc*pFrameViewDoc, const char buf[]);
	static int makeKeyFrame(CObject * pFrameData, char frame_buf[]);

	static void writeBytes(char** dest, const void *pBuffer, int length);
	static void writeFloat(char** dest, float value);
	static void writeInteger(char** dest, int value);
	static void writeString(char** dest, const std::string& strValue);

};

