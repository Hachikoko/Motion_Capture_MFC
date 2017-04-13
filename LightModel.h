//Copyright (C) 2008 Ehsan Kamrani
//This program is distributed under the terms of the GNU Lesser General Public License.


#pragma once

#include "OpenGL.h"

class LightModel
{
public:
	GLvoid GlobalAmbient( GLfloat* Ambient );
	GLvoid EnableLocalViewer();
	GLvoid DisableLocalViewer();
	GLvoid EnableTwoSide();
	GLvoid DisableTwoSide();
	GLvoid EnableSeparateSpecular();
	GLvoid DisableSepatrateSpecular();
	GLvoid Reset();
};

