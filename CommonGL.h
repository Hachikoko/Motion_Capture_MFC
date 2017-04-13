//Copyright (C) 2008 Ehsan Kamrani
//This program is distributed under the terms of the GNU Lesser General Public License.


#pragma once

/************************************************
***************Ehsan Kamrani*********************
*************************************************
***********HOW TO USE COMMON FUNCTIONS***********
******Document Written by Ehsan Kamrani**********
*************************************************

Files:
CommonGL.h
CommonGL.cpp

Usage:
1)Include the header file:
   #include "CommonGL.h"

2)To load a bitmap texture:
   GLuint value;
   value = glGenLists( 1 );   //Generate one unique value
   LoadBitmapTexture( "Your_File.bmp", value );
   glBindTexture( GL_TEXTURE_2D, value );
   //Draw your object with this texture here

3)To load a targa texture:
   GLuint value;
   value = glGenLists( 1 );   //Generate one unique value
   LoadTargaTexture( "Your_File.tga", value );
   glBindTexture( GL_TEXTURE_2D, value );
   //Draw your object with this texture here

4)To daw a quad, call the function drawQuad( float *P );
p is an array that consist of 4 points. See the following example:
   	float Sky[] = { 0.0f,0.0f, -800.0f,70.0f,800.0f,
                   3.0f,0.0f, -800.0f,70.0f,-800.0f,
				   3.0f,3.0f, 800.0f,70.0f,-800.0f,
				   0.0f,3.0f, 800.0f,70.0f,800.0f};

	glBindTexture(GL_TEXTURE_2D, SKY_TEXTURE );
    drawQuad( sky );  

5)By defalut we have choosen no bits for stencil buffer. For more information,
see the raster class. you should specify the stencil bits explicitly.
( common values are 16 and 32;more bits = more memory;-) ).
In the function StencilPattern:
 n= Number of vertics
 array= array of the stencil pattern that consist of n points
 type= type of the pattern( POLYGON, QUAD )
 bit = 0 or 1.
example:
float HouseStencilPatternA[] =
{ -3.0f,0.0f,-1.0f,
-3.0f,0.0f,1.0f,
-3.0f,2.0f,1.0f,
-3.0f,2.7f,0.0f,
-3.0f,2.0f,-1.0f };   // n = 5 = five points.

StencilPattern( 5, HouseStencilPatternA, POLYGON, 1 );

//Then do everything and then disable the stencil buffer:
glDisable( GL_STENCIL_TEST );

Note: you do not need to enable the stencil pattern before 
calling the function StencilPattern().It enables the stencil
pattern for you. 

6)To use billboarding, call the function Billboarding().
p1, p2 and p3 are the center of your object to be affected 
by this function. size is the size of your object.and texID
is the texture id.

************************************************/
#define POLYGON 0
#define QUAD  1
#define NOMINMAX
#include "OpenGL.h"
#include "tga.h"
#include "Bitmap.h"
#include "Targa.h"
#include "Vector.h"
#include "ImageLib.h"
class CommonGL
{
public:
	bool LoadBitmapTexture( char* filename, GLuint texID );
	bool LoadBitmapTextureWithAlpha( char* filename, GLuint texID );
	bool LoadAdvancedTargaTexture( char* fileName, GLuint texID );
	bool LoadTargaTexture( char* filename, unsigned int texID );
	int WriteTGAFile( char *fileName, short int width, short int height, unsigned char* imageData );
	void SaveScreenShot( char* fileName, short int width, short int height );
	GLvoid DrawQuad( float* p, bool isTextured = true );
	GLvoid DrawCWBox( bool debugMode, Vector &Min, Vector &Max );
	GLvoid DrawCCWBox( bool debugMode, Vector &Min, Vector &Max );
	GLvoid BillboardingWithUserVectors( float centerX, float centerY, float centerZ, 
		GLuint texID, float sizeX, float sizeY,  Vector& up,  Vector& right );
    GLvoid StencilPattern( int n, float* array, int type, int bit );
    GLvoid Billboarding( float p1, float p2, float p3, 
				         GLuint texID, float sizex, float sizey );
	bool LoadTextureWithDevil( char* fileName, GLuint texID );
	void SetShadowMatrix(GLfloat desrMat[16], float lightPos[4], float plane[4]);
};