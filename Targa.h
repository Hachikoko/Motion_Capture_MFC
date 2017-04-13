//Copyright (C) 2008 Ehsan Kamrani
//This program is distributed under the terms of the GNU Lesser General Public License.


#pragma once

/************************************************
*************************************************
*************HOW TO USE THE TGA CLASS************
******Docment Written by Ehsan Kamrani***********

Files:
Targa.h
Targa.cpp

Usage:
1) Include the header file:
   #include "Targa.h"

2)Define a Targa object:
   	Targa targa_texture;

3)To load a tga file:
	if( !targa_texture.Load( fileName ) )
		return false;

4)To unload a tga file:
	targa_texture.Unload();

************************************************/
#include <stdio.h>
#include "OpenGL.h"
class Targa
{
public:
	Targa();
	~Targa();

	int width;
	int height;
    int bpp;
	GLubyte* imageData;
	bool Load( char* filename );
	void Unload();
};
