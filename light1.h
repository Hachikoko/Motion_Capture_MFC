//Copyright (C) 2008 Ehsan Kamrani
//This program is distributed under the terms of the GNU Lesser General Public License.


#pragma once

#include "OpenGL.h"

class light1
{
public:
	GLvoid Position( GLfloat* PositionArray );
	GLvoid Ambient( GLfloat* AmbientArray );
	GLvoid Diffuse( GLfloat* DiffuseArray );
	GLvoid Specular( GLfloat* SpecularArray );
	GLvoid SpotDirection( GLfloat* SpotArray );
	GLvoid SpotAngle( GLfloat Angle );
	GLvoid SpotExponent( GLfloat Exponent );
	GLvoid ConstantAttenuation( GLfloat Cfactor );
	GLvoid LinearAttenuation( GLfloat Lfactor );
	GLvoid QuadraticAttenuation( GLfloat Qfacor );
	GLvoid Reset();
    GLvoid Enable();
	GLvoid Disable();
};
