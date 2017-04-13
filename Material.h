//Copyright (C) 2008 Ehsan Kamrani
//This program is distributed under the terms of the GNU Lesser General Public License.


#pragma once

#include "OpenGL.h"

class Material
{
public:
    GLvoid FrontAmbient( GLfloat* FAmbient );
	GLvoid BackAmbient( GLfloat* BAmbient );
	GLvoid FrontDiffuse( GLfloat* FDiffuse );
	GLvoid BackDiffuse( GLfloat* BDiffuse );
	GLvoid FrontSpecular( GLfloat* FSpecular );
	GLvoid BackSpecular( GLfloat* BSpecular );
	GLvoid FrontShininess( GLfloat FShininess );
	GLvoid BackShininess( GLfloat BShininess );
	GLvoid FrontEmission( GLfloat FEmission );
	GLvoid BackEmission( GLfloat BEmission );
};

