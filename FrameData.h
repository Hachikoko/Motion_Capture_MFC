#pragma once

// CFrameData 命令目标
#include "BoneDataOfFrame.h"
#include "Cal3DBase.h"



class CFrameData : public CObject
{
public:
	CFrameData(Cal3DBase* basemodel,float duration,int num_bones);
	virtual ~CFrameData();

	int set_Bones_Data_Flag(int bone_id);
	void check_Bone_Flag(const char* joint_num_and_node_num_pair);

public:
	char magic_token[4];
	int version;
	float duration;
	int num_bones;

	CObArray boneData;
	char bones_data_flag[23];  //用于记录每一帧中存在的骨骼数据信息,以关节号为下标
};


