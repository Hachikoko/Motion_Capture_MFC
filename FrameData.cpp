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

}

CFrameData::~CFrameData()
{

}


// CFrameData 成员函数
