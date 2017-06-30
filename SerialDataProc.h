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

static CalQuaternion standard_Rotation[23] = {
	CalQuaternion(0,0,0.707106,0.707106),
	CalQuaternion(0.499999,0.5,0.5,0.500001),
	CalQuaternion(0,0,0,1),
	CalQuaternion(0,0,0,1),
	CalQuaternion(0,0,0,1),
	CalQuaternion(0,0,0,1),
	CalQuaternion(0,0,0,1),
	CalQuaternion(0.632981, -0.000253104,0.774167,-0.000307361),
	CalQuaternion(0,0.106166,0,0.994348),
	CalQuaternion(0,0,-0.00908291,0.999959),
	CalQuaternion(0.706825,0,0,0.707388),
	CalQuaternion(-0.632981,0.000250957,0.774167,-0.000309116),
	CalQuaternion(0,-0.106166,0,0.994348),
	CalQuaternion(0,0,-0.00908291,0.999959),
	CalQuaternion(-0.706825,0,0,0.707388),
	CalQuaternion(0.0107283,0.99979,0.00155526,0.0173822),
	CalQuaternion(0,0,0.0127332,0.999919),
	CalQuaternion(-0.00177716,0.0173617,-0.00200319,0.999846),
	CalQuaternion(0,0,-0.707107,0.707107),
	CalQuaternion(0.0107282,0.99979,-0.00155655,-0.0173836),
	CalQuaternion(0,0,0.0127332,0.999919),
	CalQuaternion(0.00177708,-0.0173617,-0.00200319,0.999846),
	CalQuaternion(0,0,-0.707107,0.707107)
};

//存储校准偏置
static CalQuaternion bias[23] = {
	CalQuaternion(0,0,0,1),
	CalQuaternion(0,0,0,1),
	CalQuaternion(0,0,0,1),
	CalQuaternion(0,0,0,1),
	CalQuaternion(0,0,0,1),
	CalQuaternion(0,0,0,1),
	CalQuaternion(0,0,0,1),
	CalQuaternion(0,0,0,1),
	CalQuaternion(0,0,0,1),
	CalQuaternion(0,0,0,1),
	CalQuaternion(0,0,0,1),
	CalQuaternion(0,0,0,1),
	CalQuaternion(0,0,0,1),
	CalQuaternion(0,0,0,1),
	CalQuaternion(0,0,0,1),
	CalQuaternion(0,0,0,1),
	CalQuaternion(0,0,0,1),
	CalQuaternion(0,0,0,1),
	CalQuaternion(0,0,0,1),
	CalQuaternion(0,0,0,1),
	CalQuaternion(0,0,0,1),
	CalQuaternion(0,0,0,1),
	CalQuaternion(0,0,0,1),
};

static CalQuaternion calibration_sum[23] = {
	CalQuaternion(0,0,0,0),
	CalQuaternion(0,0,0,0),
	CalQuaternion(0,0,0,0),
	CalQuaternion(0,0,0,0),
	CalQuaternion(0,0,0,0),
	CalQuaternion(0,0,0,0),
	CalQuaternion(0,0,0,0),
	CalQuaternion(0,0,0,0),
	CalQuaternion(0,0,0,0),
	CalQuaternion(0,0,0,0),
	CalQuaternion(0,0,0,0),
	CalQuaternion(0,0,0,0),
	CalQuaternion(0,0,0,0),
	CalQuaternion(0,0,0,0),
	CalQuaternion(0,0,0,0),
	CalQuaternion(0,0,0,0),
	CalQuaternion(0,0,0,0),
	CalQuaternion(0,0,0,0),
	CalQuaternion(0,0,0,0),
	CalQuaternion(0,0,0,0),
	CalQuaternion(0,0,0,0),
	CalQuaternion(0,0,0,0),
	CalQuaternion(0,0,0,0),
};

static short real_calibration_numbs[23] = {0,0,0,0,0,
									0,0,0,0,0,
									0,0,0,0,0,
									0,0,0,0,0,
									0,0,0 };

static CStringList list;

static unsigned char pre_set_size = 0; //记录前一帧set中的值的数量
static unsigned char pre_pre_set_size = 0; //记录前一个固定的数量值，只有此值发生变化时，重绘树形空间

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



//	static unsigned int pre_serialnum;

	static void init_list(CStringList &list);
	static CStringList* getStringListPtr(void);

	static int dataProc(struct PtrForFrameAppDoc*pFrameViewDoc, const char buf[]);
	static int frameDataProc(struct PtrForFrameAppDoc*pFrameViewDoc, const char buf[]);
	static int makeKeyFrame(CObArray* p_frameDataArray, char frame_buf[]);

	static void writeBytes(char** dest, const void *pBuffer, int length);
	static void writeFloat(char** dest, float value);
	static void writeInteger(char** dest, int value);
	static void writeString(char** dest, const std::string& strValue);

	static int calculate_relative_rotation(CObArray * p_frameDataArray,int joint_id,CalQuaternion&result_data);

	static void proc_Frame_Data(CBoneDataOfFrame* p_Bone_Data, const char* buf, int joint_id);
	static void calculate_bias(CalQuaternion* p_calibration_bias);
};

