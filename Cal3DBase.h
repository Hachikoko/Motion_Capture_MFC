//Copyright (C) 2008 Ehsan Kamrani
//This program is distributed under the terms of the GNU Lesser General Public License.
//Note: Some classes and functions of this class are from the miniviewer_gl project available at 
//http://download.gna.org/cal3d/sources/	
//minviewer_gl has been released under the terms of the GNU LGPL license
// I have improved and changed some functions and added more functions to deal with cal3D models
#pragma once

#include "Cal3D/Cal3D.h"
#include "Material.h"
#include "OpenGL.h"
#include "CommonGL.h"
#include "LightModel.h"
#pragma comment( lib, "Cal3D.lib" )

class Cal3DBase
{
public:
    Cal3DBase();
	~Cal3DBase();
	bool Init( CStringW m_cfgFileName );
	bool Release();
	CalCoreModel* GetCore();
	float m_scale;
private:
	LightModel lightModel;
	std::string m_path;
	CalCoreModel* m_calCoreModel;
	CommonGL commonGL;
    Material mat;
    bool parseModelConfiguration( CStringW strFilename );
};
