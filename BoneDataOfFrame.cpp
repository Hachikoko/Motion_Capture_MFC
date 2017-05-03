// BoneDataOfFrame.cpp : 实现文件
//

#include "stdafx.h"
#include "Motion_Capture_MFC.h"
#include "BoneDataOfFrame.h"




// CBoneDataOfFrame

CBoneDataOfFrame::CBoneDataOfFrame(CalCoreSkeleton*pCoreSkeleton, int id, float time)
	: a_x(0)
	, a_y(0)
	, a_z(0)
	, g_x(0)
	, g_y(0)
	, g_z(0)
	, m_x(0)
	, m_y(0)
	, m_z(0)
{
	CalCoreBone* pCorebone = pCoreSkeleton->getCoreBone(id);

	bone_id = id;
	num_frame = 1;

	this->time = time;

	t_x = pCorebone->getTranslation().x;
	t_y = pCorebone->getTranslation().y;
	t_z = pCorebone->getTranslation().z;

	r_x = pCorebone->getRotation().x;
	r_y = pCorebone->getRotation().y;
	r_z = pCorebone->getRotation().z;
	r_w = pCorebone->getRotation().w;
}

CBoneDataOfFrame::~CBoneDataOfFrame()
{
}


// CBoneDataOfFrame 成员函数
