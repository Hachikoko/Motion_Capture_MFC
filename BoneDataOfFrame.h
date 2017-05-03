#pragma once

// CBoneDataOfFrame ÃüÁîÄ¿±ê
#include "cal3d/corebone.h"
#include "cal3d/coreskeleton.h"

class CBoneDataOfFrame : public CObject
{
public:
	CBoneDataOfFrame(CalCoreSkeleton*pCoreSkeleton, int id, float time);
	virtual ~CBoneDataOfFrame();

public:
	int bone_id;
	int num_frame;

	float time;
	float t_x;
	float t_y;
	float t_z;

	float r_x;
	float r_y;
	float r_z;
	float r_w;

	short a_x;
	short a_y;
	short a_z;

	short g_x;
	short g_y;
	short g_z;

	short m_x;
	short m_y;
	short m_z;
};


