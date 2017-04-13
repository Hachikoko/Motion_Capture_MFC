//Copyright (C) 2008 Ehsan Kamrani
//This program is distributed under the terms of the GNU Lesser General Public License.


#include "StdAfx.h"
#include "LightModel.h"
#include "LightModel.h"


GLvoid LightModel::GlobalAmbient( GLfloat* Ambient )
{
	glLightModelfv( GL_LIGHT_MODEL_AMBIENT, Ambient );
}

GLvoid LightModel::EnableLocalViewer()
{
	glLightModeli( GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE );
}

GLvoid LightModel::DisableLocalViewer()
{
	glLightModeli( GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE );
}

GLvoid LightModel::EnableTwoSide()
{
	glLightModeli( GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE );
}

GLvoid LightModel::DisableTwoSide()
{
	glLightModeli( GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE );
}

GLvoid LightModel::EnableSeparateSpecular()
{
	glLightModeli( GL_LIGHT_MODEL_COLOR_CONTROL,
		GL_SEPARATE_SPECULAR_COLOR );
}

GLvoid LightModel::DisableSepatrateSpecular()
{
	glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, 
        GL_SINGLE_COLOR );
}

GLvoid LightModel::Reset()
{
	GLfloat Ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	glLightModelfv( GL_LIGHT_MODEL_AMBIENT, Ambient );
	glLightModeli( GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE );
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE );
	glLightModeli( GL_LIGHT_MODEL_COLOR_CONTROL,
		GL_SINGLE_COLOR );
}