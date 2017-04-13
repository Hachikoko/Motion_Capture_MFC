//Copyright (C) 2008 Ehsan Kamrani
//This program is distributed under the terms of the GNU Lesser General Public License.


#include "StdAfx.h"
#include "light0.h"

GLvoid light0::Enable()
{
	glEnable( GL_LIGHT0 );
}

GLvoid light0::Disable()
{
	glDisable( GL_LIGHT0 );
}

GLvoid light0::Position( GLfloat* PositionArray )
{
	glLightfv( GL_LIGHT0, GL_POSITION, PositionArray );
}

GLvoid light0::Ambient( GLfloat* AmbientArray )
{
	glLightfv( GL_LIGHT0, GL_AMBIENT, AmbientArray );
}

GLvoid light0::Diffuse( GLfloat* DiffuseArray )
{
	glLightfv( GL_LIGHT0, GL_DIFFUSE, DiffuseArray );
}

GLvoid light0::Specular( GLfloat* SpecularArray )
{
	glLightfv( GL_LIGHT0, GL_SPECULAR, SpecularArray );
}

GLvoid light0::SpotDirection( GLfloat* SpotArray )
{
	glLightfv( GL_LIGHT0, GL_SPOT_DIRECTION, SpotArray );
}

GLvoid light0::SpotAngle( GLfloat Angle )
{
	glLightf( GL_LIGHT0, GL_SPOT_CUTOFF, Angle );
}

GLvoid light0::SpotExponent( GLfloat Exponent )
{
	glLightf( GL_LIGHT0, GL_SPOT_EXPONENT, Exponent );
}

GLvoid light0::ConstantAttenuation( GLfloat Cfactor )
{
	glLightf( GL_LIGHT0, GL_CONSTANT_ATTENUATION, Cfactor );
}

GLvoid light0::LinearAttenuation( GLfloat Lfactor )
{
	glLightf( GL_LIGHT0, GL_LINEAR_ATTENUATION, Lfactor );
}

GLvoid light0::QuadraticAttenuation( GLfloat Qfactor )
{
	glLightf( GL_LIGHT0, GL_QUADRATIC_ATTENUATION, Qfactor );
}

GLvoid light0::Reset()
{
	GLfloat Ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	glLightfv( GL_LIGHT0, GL_AMBIENT, Ambient );

	GLfloat Diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glLightfv( GL_LIGHT0, GL_DIFFUSE, Diffuse );

	GLfloat Specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glLightfv( GL_LIGHT0, GL_SPECULAR, Specular );

	GLfloat Position[] = { 0.0f, 0.0f, 1.0f, 0.0 };
	glLightfv( GL_LIGHT0, GL_POSITION, Position );

	GLfloat Direction[] = { 0.0f, 0.0f, -1.0f };
	glLightfv( GL_LIGHT0, GL_SPOT_DIRECTION, Direction );

	glLightf( GL_LIGHT0, GL_SPOT_EXPONENT, 0.0f );

	glLightf( GL_LIGHT0, GL_SPOT_CUTOFF, 180.0f );

	glLightf( GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0f );

	glLightf( GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.0f );

	glLightf( GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.0f );
}