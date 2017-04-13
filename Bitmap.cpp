//Copyright (C) 2008 Ehsan Kamrani
//This program is distributed under the terms of the GNU Lesser General Public License.


#include "StdAfx.h"
#include "Bitmap.h"

Bitmap::Bitmap()
{
	bitmapImage = NULL;
	bitmapImageWithAlpha = NULL;
	memset( &bitmapFileHeader, 0, sizeof( BITMAPFILEHEADER ) );
	memset( &bitmapInfoHeader, 0, sizeof( BITMAPINFOHEADER ) );
}

Bitmap::~Bitmap()
{
}

bool Bitmap::Load( char *filename)
{
	FILE *filePtr;			// the file pointer
	errno_t err;
	int	imageIdx = 0;		// image index counter
	// open filename in "read binary" mode
//	filePtr = fopen(filename, "rb");
	err = fopen_s(&filePtr, filename, "rb");
	if (err != 0)
		return false;
	// read the bitmap file header
	fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
		
	// verify that this is a bitmap by checking for the universal bitmap id
	if (bitmapFileHeader.bfType != 0x4D42 )
	{
		fclose(filePtr);
		return false;
	}
	// read the bitmap information header
	fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
	// move file pointer to beginning of bitmap data
	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);
	// allocate enough memory for the bitmap image data
	bitmapImage = new unsigned char[bitmapInfoHeader.biSizeImage];
	// verify memory allocation
	if (!bitmapImage)
	{
		delete[] bitmapImage;
		fclose(filePtr);
		return false;
	}

	// read in the bitmap image data
	fread(bitmapImage, 1, bitmapInfoHeader.biSizeImage, filePtr);
	// make sure bitmap image data was read
	if (bitmapImage == NULL)
	{
		fclose(filePtr);
   		return false;
	}

	// close the file and return the bitmap image data
	fclose(filePtr);

	return true;
}

bool Bitmap::LoadWithAlpha( char* filename )
{
	if( Load( filename ) )
	{
		  bitmapImageWithAlpha = new unsigned char[bitmapInfoHeader.biSizeImage * 4 / 3];
		  if( bitmapImageWithAlpha == NULL )
			  return false;

    	  for (unsigned int src = 0, dst = 0; src < bitmapInfoHeader.biSizeImage; src +=3, dst +=4)
          {
              // if the pixel is black, set the alpha to 0. Otherwise, set it to 255.
              if (bitmapImage[src] == 0 && bitmapImage[src+1] == 0 && bitmapImage[src+2] == 0)
                  bitmapImageWithAlpha[dst+3] = 0;
              else
                  bitmapImageWithAlpha[dst+3] = 0xFF;

              // copy pixel data over
              bitmapImageWithAlpha[dst] = bitmapImage[src];
              bitmapImageWithAlpha[dst+1] = bitmapImage[src+1];
              bitmapImageWithAlpha[dst+2] = bitmapImage[src+2];
          }

		  return true;
	}
	return false;
}

void Bitmap::Unload()
{
	if( bitmapImage != NULL )
	{
		delete [] bitmapImage;
		bitmapImage = NULL;
	}
	if( bitmapImageWithAlpha != NULL )
	{
		delete[] bitmapImageWithAlpha;
		bitmapImageWithAlpha = NULL;
	}
    memset( &bitmapInfoHeader, 0, sizeof( bitmapInfoHeader ) );
	memset( &bitmapFileHeader, 0, sizeof( bitmapFileHeader ) );
}
