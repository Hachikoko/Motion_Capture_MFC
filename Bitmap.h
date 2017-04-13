//Copyright (C) 2008 Ehsan Kamrani
//This program is distributed under the terms of the GNU Lesser General Public License.
#pragma once

/************************************************
*************************************************
***********HOW TO USE THE BITMAP CLASS***********
******Docment Written by Ehsan Kamrani***********

Files: 
..\GraphicEngine\Bitmap.h
..\GraphicEngine\Bitmap.cpp

Usage:
1)Include the header file:
   #include "Bitmap.h"

2)Define a Bitmap object:
	Bitmap bitmap_texture;

3)To load a file:
	if( !bitmap_texture.Load( fileName ) )
		return false;

4)To unload a file:
	bitmap_texture.Unload();

************************************************/

#include <stdio.h>
class Bitmap
{
public:
	BITMAPFILEHEADER	bitmapFileHeader;	// bitmap file header
	BITMAPINFOHEADER    bitmapInfoHeader; 
	unsigned char		*bitmapImage;			// bitmap image data
	unsigned char       *bitmapImageWithAlpha;      
	Bitmap();
	~Bitmap();
	bool Load( char *filename );
	bool LoadWithAlpha( char* filename );
	void Unload();
};
