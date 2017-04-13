//Copyright (C) 2008 Ehsan Kamrani
//This program is distributed under the terms of the GNU Lesser General Public License.
//Note: Some classes and functions of this class are from the miniviewer_gl project available at 
//http://download.gna.org/cal3d/sources/	
//minviewer_gl has been released under the terms of the GNU LGPL license
//I have improved and changed some functions and added more functions to deal with cal3D models
#pragma once
#include "Cal3dbase.h"

class Cal3DChild
{
public:
    Cal3DChild();
	~Cal3DChild();
	bool Init( CalCoreModel* coreModel );
	void RenderModel( float elapsedTime, bool isTextured );
	bool Release();
	void executeAction( int animationID, float delayIn, float delayOut, float weightTarget = 1.00000f, bool autoLock = false );
	void blendCycle( int animationID, float delayIn, float delayOut );
	void clearCycle( int animationID, float delay );
	void Update( float elapsedTime );
	void setLodLevel(float lodLevel);
	int GetAnimationCount(void);
	int GetMeshCount(void);
	int GetMaterialCount(void);
	int GetCurrentAnimationId(void);
	void SetCurrentAnimationId(int Id);
	void RenderSkeleton();
	int GetFaceCount(void);
	int GetVertexCount(void);
	float GetLODLevel(void);

private:
    CalModel* m_calModel;
	LightModel lightModel;
	CommonGL commonGL;
    Material mat;
	int m_currentAnimationId;
	int m_animationCount;
	// Number of vertices of the models
	int m_vertexCount;
	// Number of faces of the model
	int m_faceCount;
	float m_LODLevel;
	int m_materialCount;
	int m_meshCount;
};
