//Copyright (C) 2008 Ehsan Kamrani
//This program is distributed under the terms of the GNU Lesser General Public License.


#include "StdAfx.h"
#include "light2.h"

GLvoid light2::Enable()
{
	glEnable( GL_LIGHT2 );
}

GLvoid light2::Disable()
{
	glDisable( GL_LIGHT2 );
}

GLvoid light2::Position( GLfloat* PositionArray )
{
	glLightfv( GL_LIGHT2, GL_POSITION, PositionArray );
}

GLvoid light2::Ambient( GLfloat* AmbientArray )
{
	glLightfv( GL_LIGHT2, GL_AMBIENT, AmbientArray );
}

GLvoid light2::Diffuse( GLfloat* DiffuseArray )
{
	glLightfv( GL_LIGHT2, GL_DIFFUSE, DiffuseArray );
}

GLvoid light2::Specular( GLfloat* SpecularArray )
{
	glLightfv( GL_LIGHT2, GL_SPECULAR, SpecularArray );
}

GLvoid light2::SpotDirection( GLfloat* SpotArray )
{
	glLightfv( GL_LIGHT2, GL_SPOT_DIRECTION, SpotArray );
}

GLvoid light2::SpotAngle( GLfloat Angle )
{
	glLightf( GL_LIGHT2, GL_SPOT_CUTOFF, Angle );
}

GLvoid light2::SpotExponent( GLfloat Exponent )
{
	glLightf( GL_LIGHT2, GL_SPOT_EXPONENT, Exponent );
}

GLvoid light2::ConstantAttenuation( GLfloat Cfactor )
{
	glLightf( GL_LIGHT2, GL_CONSTANT_ATTENUATION, Cfactor );
}

GLvoid light2::LinearAttenuation( GLfloat Lfactor )
{
	glLightf( GL_LIGHT2, GL_LINEAR_ATTENUATION, Lfactor );
}

GLvoid light2::QuadraticAttenuation( GLfloat Qfactor )
{
	glLightf( GL_LIGHT2, GL_QUADRATIC_ATTENUATION, Qfactor );
}

GLvoid light2::Reset()
{
	GLfloat Ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	glLightfv( GL_LIGHT2, GL_AMBIENT, Ambient );

	GLfloat Diffuse[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	glLightfv( GL_LIGHT2, GL_DIFFUSE, Diffuse );

	GLfloat Specular[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	glLightfv( GL_LIGHT2, GL_SPECULAR, Specular );

	GLfloat Position[] = { 0.0f, 0.0f, 1.0f, 0.0 };
	glLightfv( GL_LIGHT2, GL_POSITION, Position );

	GLfloat Direction[] = { 0.0f, 0.0f, -1.0f };
	glLightfv( GL_LIGHT2, GL_SPOT_DIRECTION, Direction );

	glLightf( GL_LIGHT2, GL_SPOT_EXPONENT, 0.0f );

	glLightf( GL_LIGHT2, GL_SPOT_CUTOFF, 180.0f );

	glLightf( GL_LIGHT2, GL_CONSTANT_ATTENUATION, 1.0f );

	glLightf( GL_LIGHT2, GL_LINEAR_ATTENUATION, 0.0f );

	glLightf( GL_LIGHT2, GL_QUADRATIC_ATTENUATION, 0.0f );
}