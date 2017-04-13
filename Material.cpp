//Copyright (C) 2008 Ehsan Kamrani
//This program is distributed under the terms of the GNU Lesser General Public License.


#include "StdAfx.h"
#include "Material.h"

GLvoid Material::FrontAmbient( GLfloat* FAmbient )
{
	glMaterialfv( GL_FRONT, GL_AMBIENT, FAmbient );
}

GLvoid Material::BackAmbient( GLfloat* BAmbient )
{
	glMaterialfv( GL_BACK, GL_AMBIENT, BAmbient );
}

GLvoid Material::FrontDiffuse( GLfloat* FDiffuse )
{
	glMaterialfv( GL_FRONT, GL_DIFFUSE, FDiffuse );
}

GLvoid Material::BackDiffuse( GLfloat* BDiffuse )
{
	glMaterialfv( GL_BACK, GL_DIFFUSE, BDiffuse );
}

GLvoid Material::FrontSpecular( GLfloat* FSpecular )
{
	glMaterialfv( GL_FRONT, GL_SPECULAR, FSpecular );
}

GLvoid Material::BackSpecular( GLfloat* BSpecular )
{
	glMaterialfv( GL_BACK, GL_SPECULAR, BSpecular );
}

GLvoid Material::FrontShininess( GLfloat FShininess )
{
	glMaterialf( GL_FRONT, GL_SHININESS, FShininess );
}

GLvoid Material::BackShininess( GLfloat BShininess )
{
	glMaterialf( GL_BACK, GL_SHININESS, BShininess );
}

GLvoid Material::FrontEmission( GLfloat FEmission )
{
	glMaterialf( GL_FRONT, GL_EMISSION, FEmission );
}

GLvoid Material::BackEmission( GLfloat BEmission )
{
	glMaterialf( GL_BACK, GL_EMISSION, BEmission );
}

