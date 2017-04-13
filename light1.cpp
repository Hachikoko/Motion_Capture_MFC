//Copyright (C) 2008 Ehsan Kamrani
//This program is distributed under the terms of the GNU Lesser General Public License.


#include "StdAfx.h"
#include "light1.h"

GLvoid light1::Enable()
{
	glEnable( GL_LIGHT1 );
}

GLvoid light1::Disable()
{
	glDisable( GL_LIGHT1 );
}

GLvoid light1::Position( GLfloat* PositionArray )
{
	glLightfv( GL_LIGHT1, GL_POSITION, PositionArray );
}

GLvoid light1::Ambient( GLfloat* AmbientArray )
{
	glLightfv( GL_LIGHT1, GL_AMBIENT, AmbientArray );
}

GLvoid light1::Diffuse( GLfloat* DiffuseArray )
{
	glLightfv( GL_LIGHT1, GL_DIFFUSE, DiffuseArray );
}

GLvoid light1::Specular( GLfloat* SpecularArray )
{
	glLightfv( GL_LIGHT1, GL_SPECULAR, SpecularArray );
}

GLvoid light1::SpotDirection( GLfloat* SpotArray )
{
	glLightfv( GL_LIGHT1, GL_SPOT_DIRECTION, SpotArray );
}

GLvoid light1::SpotAngle( GLfloat Angle )
{
	glLightf( GL_LIGHT1, GL_SPOT_CUTOFF, Angle );
}

GLvoid light1::SpotExponent( GLfloat Exponent )
{
	glLightf( GL_LIGHT1, GL_SPOT_EXPONENT, Exponent );
}

GLvoid light1::ConstantAttenuation( GLfloat Cfactor )
{
	glLightf( GL_LIGHT1, GL_CONSTANT_ATTENUATION, Cfactor );
}

GLvoid light1::LinearAttenuation( GLfloat Lfactor )
{
	glLightf( GL_LIGHT1, GL_LINEAR_ATTENUATION, Lfactor );
}

GLvoid light1::QuadraticAttenuation( GLfloat Qfactor )
{
	glLightf( GL_LIGHT1, GL_QUADRATIC_ATTENUATION, Qfactor );
}

GLvoid light1::Reset()
{
	GLfloat Ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	glLightfv( GL_LIGHT1, GL_AMBIENT, Ambient );

	GLfloat Diffuse[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	glLightfv( GL_LIGHT1, GL_DIFFUSE, Diffuse );

	GLfloat Specular[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	glLightfv( GL_LIGHT1, GL_SPECULAR, Specular );

	GLfloat Position[] = { 0.0f, 0.0f, 1.0f, 0.0 };
	glLightfv( GL_LIGHT1, GL_POSITION, Position );

	GLfloat Direction[] = { 0.0f, 0.0f, -1.0f };
	glLightfv( GL_LIGHT1, GL_SPOT_DIRECTION, Direction );

	glLightf( GL_LIGHT1, GL_SPOT_EXPONENT, 0.0f );

	glLightf( GL_LIGHT1, GL_SPOT_CUTOFF, 180.0f );

	glLightf( GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1.0f );

	glLightf( GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.0f );

	glLightf( GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.0f );
}
