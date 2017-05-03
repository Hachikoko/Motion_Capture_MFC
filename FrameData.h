#pragma once

// CFrameData ÃüÁîÄ¿±ê
#include "BoneDataOfFrame.h"
#include "Cal3DBase.h"


class CFrameData : public CObject
{
public:
	CFrameData(Cal3DBase* basemodel,float duration,int num_bones);
	virtual ~CFrameData();

public:
	char magic_token[4];
	int version;
	float duration;
	int num_bones;

	CObArray boneData;
	
};


