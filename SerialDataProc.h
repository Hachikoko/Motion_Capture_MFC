#pragma once
#include "Motion_Capture_MFCDoc.h"
#include "MainFrm.h"
#include "FrameData.h"

static char father_child_joint_pair[23][2] = {
	-1,0,
	0,1,
	1,2,
	2,3,
	3,4,
	4,5,
	5,6,
	5,7,
	7,8,
	8,9,
	9,10,
	5,11,
	11,12,
	12,13,
	13,14,
	1,15,
	15,16,
	16,17,
	17,18,
	1,19,
	19,20,
	20,21,
	21,22
};

class CSerialDataProc
{
public:
	CSerialDataProc();
	~CSerialDataProc();

	enum DataProcState
	{
		ADD_ANIMATION_FAIL = -1,
		CALCULATE_RELATIVE_ROTATION_FAIL = -2,
		NO_SUTABLE_DATA = -3,
		NO_SUCH_JOINT_ID = -4,

	};

	static unsigned int pre_serialnum;

	static int dataProc(struct PtrForFrameAppDoc*pFrameViewDoc, const char buf[]);
	static int frameDataProc(struct PtrForFrameAppDoc*pFrameViewDoc, const char buf[]);
	static int makeKeyFrame(CObArray* p_frameDataArray, char frame_buf[]);

	static void writeBytes(char** dest, const void *pBuffer, int length);
	static void writeFloat(char** dest, float value);
	static void writeInteger(char** dest, int value);
	static void writeString(char** dest, const std::string& strValue);

	static int calculate_relative_rotation(CObArray * p_frameDataArray,int joint_id,CalQuaternion&result_data);
};

