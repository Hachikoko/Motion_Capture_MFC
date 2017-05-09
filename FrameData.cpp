// FrameData.cpp : 实现文件
//

#include "stdafx.h"
#include "Motion_Capture_MFC.h"
#include "FrameData.h"
#include "cal3d/coreskeleton.h"


// CFrameData

CFrameData::CFrameData(Cal3DBase* basemodel, float duration, int num_bones)
{
	CalCoreSkeleton * temp = basemodel->GetCore()->getCoreSkeleton();
	magic_token[0] = 'C';
	magic_token[1] = 'A';
	magic_token[2] = 'F';
	magic_token[3] = '\0';
	version = 700;
	this->duration = duration;
	this->num_bones = num_bones;

	for (int i = 0;i < num_bones;i++)
	{
		boneData.Add(new CBoneDataOfFrame(temp, i, duration));
	}

	for (int i = 0; i < 23;i++)
	{
		bones_data_flag[i] = 0;
	}

}

CFrameData::~CFrameData()
{

}

int CFrameData::set_Bones_Data_Flag(int joint_id)
{
	if (joint_id < 0 || joint_id > (num_bones - 1))
	{
		return -1;
	}

	bones_data_flag[joint_id] = 1;

	return 0;
}

//************************************
// 方法名称:  check_Bone_Flag
// 方法全名:  CFrameData::check_Bone_Flag
// 权限:      public 
// 返回值:    void
// Qualifier:
// 参数:      const unsigned char * joint_num_and_node_num_pair
// 描述：	  检查此帧中缺少的骨骼数据，缺少在bones_data_flag中用-1标出
//************************************
void CFrameData::check_Bone_Flag(const char * joint_num_and_node_num_pair)
{
	int i = 0;
	while (i < num_bones)
	{
		if (joint_num_and_node_num_pair[i] > 0 && bones_data_flag[i] == 0) {
			bones_data_flag[i] = -1;
		}
		i++;
	}
	return;
}

// CFrameData 成员函数
