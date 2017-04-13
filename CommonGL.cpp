//Copyright (C) 2008 Ehsan Kamrani
//This program is distributed under the terms of the GNU Lesser General Public License.


#include "StdAfx.h"
#include "CommonGL.h"

bool CommonGL::LoadBitmapTexture( char *fileName, GLuint texID )
{
	Bitmap bitmap_texture;
	if( !bitmap_texture.Load( fileName ) )
	{
		MessageBox( NULL, _T( "Couln't open the bitmap file." ) , _T( "Engine Error" ) , MB_OK | MB_ICONINFORMATION );
		return false;
	}
	// setup the texture objext       
	glBindTexture(GL_TEXTURE_2D, texID);  
	glPixelStorei( GL_UNPACK_ALIGNMENT, 4 );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	if(  GLEW_EXT_texture_filter_anisotropic )
	{
		GLfloat m_maxAnisotropy;
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &m_maxAnisotropy);
		if( m_maxAnisotropy > 8 )
			m_maxAnisotropy = 8;
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, m_maxAnisotropy);
	}

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, bitmap_texture.bitmapInfoHeader.biWidth, bitmap_texture.bitmapInfoHeader.biHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, bitmap_texture.bitmapImage );
	bitmap_texture.Unload();

	return true;
}

bool CommonGL::LoadBitmapTextureWithAlpha( char* filename, GLuint texID )
{
	Bitmap bitmap_texture_with_alpha;
	if( !bitmap_texture_with_alpha.LoadWithAlpha( filename ) )
	{
		MessageBox( NULL, _T( "Couldn't load the bitmap file." ), _T( "Engine Error" ), MB_OK | MB_ICONINFORMATION );
		return false;
	}
	glBindTexture(GL_TEXTURE_2D, texID);  
	glPixelStorei( GL_UNPACK_ALIGNMENT, 4 );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, bitmap_texture_with_alpha.bitmapInfoHeader.biWidth,
		              bitmap_texture_with_alpha.bitmapInfoHeader.biHeight,
					  GL_BGRA_EXT, GL_UNSIGNED_BYTE, bitmap_texture_with_alpha.bitmapImageWithAlpha );

	bitmap_texture_with_alpha.Unload();
	return true;
}

bool CommonGL::LoadAdvancedTargaTexture( char* fileName, GLuint texID )
{
	  CTga *Tga;
	  Tga = new CTga();
	  if(Tga->ReadFile(fileName)==0)
	  {
		  CStringA s;
		  s.Format( "couldn't load the file %s", fileName );
		  MessageBoxA( NULL, s, "Engine Error", MB_OK );
		  Tga->Release();
		  return 0;
	  }

     int width = Tga->GetSizeX();
     int height = Tga->GetSizeY();

     glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

     glBindTexture(GL_TEXTURE_2D, texID);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

   	 if( GLEW_EXT_texture_filter_anisotropic )
	 {
		 GLfloat m_maxAnisotropy;
		 glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &m_maxAnisotropy);
		 if( m_maxAnisotropy > 8 )
			 m_maxAnisotropy = 8;
		 glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, m_maxAnisotropy);
	 }
  
	 if( Tga->Bpp() == 32 )
		 gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, width, height, GL_RGBA , GL_UNSIGNED_BYTE, (char*)Tga->GetPointer() );
	 else
		 gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, width, height, GL_RGB , GL_UNSIGNED_BYTE, (char*)Tga->GetPointer() );

	 Tga->Release();
	 return true;
}

bool CommonGL::LoadTargaTexture( char* fileName, GLuint texID )
{
	Targa targa_texture;
	if( !targa_texture.Load( fileName ) )
	{
		MessageBoxW( NULL, _T( "Couldn't load the TGA file." ), _T( "Engine Error" ), MB_OK | MB_ICONINFORMATION );
		MessageBoxA( NULL, fileName, "Missing TGA File", MB_OK );
		return false;
	}
	// setup the texture objext       
	glBindTexture(GL_TEXTURE_2D, texID);  
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );

	if( GLEW_EXT_texture_filter_anisotropic )
	{
		GLfloat m_maxAnisotropy;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &m_maxAnisotropy);
		if( m_maxAnisotropy > 8 )
			m_maxAnisotropy = 8;
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, m_maxAnisotropy);
	}

	if( targa_texture.bpp == 32 )
	{
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, targa_texture.width, targa_texture.height, GL_RGBA, GL_UNSIGNED_BYTE, targa_texture.imageData );
		//glHint(GL_TEXTURE_COMPRESSION_HINT, GL_NICEST);
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGBA, targa_texture.width, targa_texture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, targa_texture.imageData);

        //GLint wasCompressed;
        //glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_COMPRESSED, &wasCompressed);
        //if (!wasCompressed)
		//	return 0;
	}

	else if( targa_texture.bpp == 24 )
	{
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, targa_texture.width, targa_texture.height, GL_RGB, GL_UNSIGNED_BYTE, targa_texture.imageData );
		//glHint(GL_TEXTURE_COMPRESSION_HINT, GL_NICEST);
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGB, targa_texture.width, targa_texture.height, 0, GL_RGB, GL_UNSIGNED_BYTE, targa_texture.imageData);

        //GLint wasCompressed;
        //glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_COMPRESSED, &wasCompressed);
        //if (!wasCompressed)
		//	return 0;
	}

	targa_texture.Unload();

	return true;
}
bool CommonGL::LoadTextureWithDevil( char* fileName, GLuint texID )
{
	// HERE USE DEVIL
	ILuint imageId;
	ilGenImages(1, &imageId);
	ilBindImage(imageId);
	// Read in the image file into DevIL.
	if (ilLoadImage(fileName) == IL_FALSE )
	{
		// ERROR
		ilDeleteImages(1, &imageId);
		MessageBox( NULL, _T( "Couldn't load the image file" ), _T( "Engine Error" ), MB_OK |MB_ICONERROR );
		return false;
	}
	else {
			// opengl work
			glBindTexture(GL_TEXTURE_2D, texID ); /* Binding of texture name */
			glPixelStorei( GL_UNPACK_ALIGNMENT, 4 );
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			if( GLEW_EXT_texture_filter_anisotropic )
			{
				GLfloat m_maxAnisotropy;
				glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &m_maxAnisotropy);
				if( m_maxAnisotropy > 8 )
					m_maxAnisotropy = 8;
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, m_maxAnisotropy);
			}

			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

			// create mipmaps and upload texture to video card memory
			gluBuild2DMipmaps( GL_TEXTURE_2D, ilGetInteger(IL_IMAGE_CHANNELS), ilGetInteger(IL_IMAGE_WIDTH),
			ilGetInteger(IL_IMAGE_HEIGHT), ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE,
			ilGetData()	);

			// release memory, now opengl have it !
			ilDeleteImages(1, &imageId);
	}
	return true;
}
int CommonGL::WriteTGAFile( char *fileName, short int width, short int height, unsigned char* imageData )
{
	unsigned char byteSkip;
	short int shortSkip;
	unsigned char imageType;
	int colorMode;
	unsigned char colorSwap;
	int imageIdx;
	unsigned char bitDepth;
	long imageSize;
	FILE *filePtr;
	errno_t err;
//	filePtr = fopen( fileName, "wb" );
	err = fopen_s(&filePtr, fileName, "wb");
//	if(!filePtr )
	if(err)
	{
		fclose( filePtr );
		return 0;
	}
	imageType = 2;
	bitDepth = 24;
	colorMode = 3;

	byteSkip = 0;
	shortSkip = 0;

	fwrite( &byteSkip, sizeof( unsigned char ), 1, filePtr );
	fwrite( &byteSkip, sizeof( unsigned char ), 1, filePtr );
	fwrite( &imageType, sizeof( unsigned char ), 1, filePtr );
	fwrite( &shortSkip, sizeof( short int ), 1, filePtr );
	fwrite( &shortSkip, sizeof( short int ), 1, filePtr );
	fwrite( &byteSkip, sizeof( unsigned char ), 1, filePtr );
	fwrite( &shortSkip, sizeof( short int ), 1, filePtr );
	fwrite( &shortSkip, sizeof( short int ), 1, filePtr );
	fwrite( &width, sizeof( short int ), 1, filePtr );
	fwrite( &height, sizeof( short int ), 1, filePtr );
	fwrite( &bitDepth, sizeof( unsigned char ), 1, filePtr );
	fwrite( &byteSkip, sizeof( unsigned char ), 1, filePtr );

	imageSize = width * height * colorMode;

	for( imageIdx = 0; imageIdx < imageSize; imageIdx += colorMode )
	{
		colorSwap = imageData[ imageIdx ];
		imageData[ imageIdx ] = imageData[ imageIdx + 2 ];
		imageData[ imageIdx + 2 ] = colorSwap;
	}

	fwrite( imageData, sizeof( unsigned char ), imageSize, filePtr );
    
	fclose( filePtr );

	return 1;
}

void CommonGL::SaveScreenShot( char* fileName, short int width, short int height )
{
	unsigned char* imageData = (unsigned char*)malloc( width * height * 3 );
	memset( imageData, 0, width * height * 3 );
	glReadPixels( 0, 0, width - 1, height -1, GL_RGB, GL_UNSIGNED_BYTE, imageData );
	WriteTGAFile( fileName, width, height, imageData );
	free( imageData );
}

GLvoid CommonGL::DrawQuad( float* p, bool isTextured )
{    
	glPushAttrib( GL_CURRENT_BIT | GL_ENABLE_BIT );
	glDisable( GL_LIGHTING );
	glDisable( GL_TEXTURE_2D );
	static int index = 0;
	glBegin( GL_QUADS );
	   for( int loop = 0; loop < 4; loop++ )
	   {
		   if( isTextured )
		   {
			   glTexCoord2f( p[loop*5 + index ], p[loop*5+1 + index ] );
			   glVertex3f( p[loop*5+2 + index ], p[loop*5+3 + index ], p[loop*5+4 + index ] );
		   }
		   else
		   {

			   GLboolean lighting = glIsEnabled( GL_LIGHTING );				   glDisable( GL_LIGHTING );
			   glColor3f( p[loop*5 + index ], p[loop*5+1 + index ], p[loop*5+2 + index ]);
   			   glVertex3f( p[loop*5+3 + index ], p[loop*5+4 + index ], p[loop*5+5 + index ] );
		   }
	   index++;
	   }
    glEnd();
    glPopAttrib();
	index = 0;
}

GLvoid CommonGL::StencilPattern( int n, float* Array, int type, int bit )
{
    glDepthMask( GL_FALSE );

	// disable modification of all color components
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

    // enable stencil testing
    glEnable(GL_STENCIL_TEST);
	// setup the stencil buffer for a function reference value
    glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
    glStencilFunc(GL_ALWAYS, 1, 1);

	switch( type )
	{
	case POLYGON:
		{
			glBegin( GL_POLYGON );
			for( int loop = 0; loop < n; loop++ )
				glVertex3f( Array[loop*3], Array[loop*3+1], Array[loop*3+2] );
			glEnd();
		}break;
	case QUAD:
		{
			glBegin( GL_QUADS );
			for( int loop = 0; loop < n; loop++ )
				glVertex3f( Array[loop*3], Array[loop*3+1], Array[loop*3+2] );
			glEnd();
		}break;
	default:
		{
			glBegin( GL_POLYGON );
			for( int loop = 0; loop < n; loop++ )
				glVertex3f( Array[loop*3], Array[loop*3+1], Array[loop*3+2] );
			glEnd();
		}break;
	}
    // enable modification of all color components
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

	// enable depth testing
    glDepthMask( GL_TRUE );

    // make it so we can only render where the stencil buffer is equal to 1
    glStencilFunc(GL_EQUAL, bit, 1); // bit = 0 or 1
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
}

GLvoid CommonGL::BillboardingWithUserVectors( float centerX, float centerY, float centerZ, 
		GLuint texID, float sizeX, float sizeY,  Vector& up,  Vector& right )
{
	
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	right.Magnitude();
	right.Normalize();
	up.Magnitude();
	up.Normalize();
	Vector rightVec( right.i, right.j, right.k );
	Vector upVec( up.i, up.j, up.k );
	Vector p( centerX, centerY, centerZ );

	glEnable( GL_TEXTURE_2D );
	Vector temp;
	glBindTexture( GL_TEXTURE_2D, texID );
	glBegin( GL_QUADS );
	glTexCoord2f( 0.0f, 0.0f );
	Vector tempRight = rightVec * sizeX;
	Vector tempUp = upVec * sizeY;
	temp = p + ( tempRight + tempUp ) * -1.0;
	glVertex3f( temp.i, temp.j, temp.k );
	glTexCoord2f( 1.0f, 0.0f );
	temp = p + ( tempRight - tempUp );
	glVertex3f( temp.i, temp.j, temp.k );
	glTexCoord2f( 1.0f, 1.0f );
	temp = p + (tempRight + tempUp );
	glVertex3f( temp.i, temp.j, temp.k );
	glTexCoord2f( 0.0f, 1.0f );
	temp = p + ( tempUp - tempRight );
	glVertex3f( temp.i, temp.j, temp.k );
	glEnd();
	glDisable( GL_TEXTURE_2D );
	glDisable( GL_BLEND );
}

GLvoid CommonGL::Billboarding( float p1, float p2, float p3, 
				   GLuint texID, float sizex, float sizey )
{
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	float mat[16];
	glGetFloatv( GL_MODELVIEW_MATRIX, mat );

	Vector right( mat[0], mat[4], mat[8] );
	Vector up( mat[1], mat[5], mat[9] );
	Vector p( p1, p2, p3 );

	glEnable( GL_TEXTURE_2D );
	Vector temp;
	glBindTexture( GL_TEXTURE_2D, texID );
	glBegin( GL_QUADS );
	glTexCoord2f( 0.0f, 0.0f );
	Vector tempRight = right * sizex;
	Vector tempUp = up * sizey;
	temp = p + ( tempRight + tempUp ) * -1.0;
	glVertex3f( temp.i, temp.j, temp.k );
	glTexCoord2f( 1.0f, 0.0f );
	temp = p + ( tempRight - tempUp );
	glVertex3f( temp.i, temp.j, temp.k );
	glTexCoord2f( 1.0f, 1.0f );
	temp = p + (tempRight + tempUp );
	glVertex3f( temp.i, temp.j, temp.k );
	glTexCoord2f( 0.0f, 1.0f );
	temp = p + ( tempUp - tempRight );
	glVertex3f( temp.i, temp.j, temp.k );
	glEnd();
	glDisable( GL_TEXTURE_2D );
	glDisable( GL_BLEND );
}

GLvoid CommonGL::DrawCWBox( bool debugMode, Vector &Min, Vector &Max )
{
	if( debugMode )
	{
		glPushAttrib( GL_CURRENT_BIT | GL_ENABLE_BIT | GL_LINE_BIT );
        glDisable( GL_BLEND );
		glDisable( GL_FOG );
		glDisable( GL_LIGHTING );
		glDisable( GL_TEXTURE_2D );
		glColor3f( 1.0f, 1.0f, 1.0f );
		glLineWidth( 2.0f );

		
		glBegin( GL_LINE_LOOP );
		glVertex3f( Max.i, Max.j, Min.k );					// Top Right Of The Quad (Top)
		glVertex3f(Min.i, Max.j, Min.k);					// Top Left Of The Quad (Top)
		glVertex3f(Min.i, Max.j, Max.k);					// Bottom Left Of The Quad (Top)
		glVertex3f( Max.i, Max.j, Max.k);					// Bottom Right Of The Quad (Top)
		glEnd();

		glBegin( GL_LINE_LOOP );
		glVertex3f( Max.i,Min.j, Max.k);					// Top Right Of The Quad (Bottom)
		glVertex3f(Min.i,Min.j, Max.k);					// Top Left Of The Quad (Bottom)
		glVertex3f(Min.i,Min.j, Min.k);					// Bottom Left Of The Quad (Bottom)
		glVertex3f( Max.i,Min.j, Min.k);					// Bottom Right Of The Quad (Bottom)
		glEnd();

		glBegin( GL_LINE_LOOP );
		glVertex3f( Max.i, Max.j, Max.k);					// Top Right Of The Quad (Front)
		glVertex3f(Min.i, Max.j, Max.k);					// Top Left Of The Quad (Front)
		glVertex3f(Min.i,Min.j, Max.k);					// Bottom Left Of The Quad (Front)
		glVertex3f( Max.i,Min.j, Max.k);					// Bottom Right Of The Quad (Front)
		glEnd();
		
		glBegin( GL_LINE_LOOP );
		glVertex3f( Max.i,Min.j,Min.k);					// Top Right Of The Quad (Back)
		glVertex3f(Min.i,Min.j,Min.k);					// Top Left Of The Quad (Back)
		glVertex3f(Min.i, Max.j,Min.k);					// Bottom Left Of The Quad (Back)
		glVertex3f( Max.i, Max.j,Min.k);					// Bottom Right Of The Quad (Back)
		glEnd();
		glPopAttrib();

	}
	else
	{
		glBegin(GL_QUADS);								// Draw A Quad
		glVertex3f( Min.i, Max.j, Min.k );					// Top Right Of The Quad (Top)
		glVertex3f(Min.i, Max.j, Min.k);					// Top Left Of The Quad (Top)
		glVertex3f(Min.i, Max.j, Max.k);					// Bottom Left Of The Quad (Top)
		glVertex3f( Max.i, Max.j, Max.k);					// Bottom Right Of The Quad (Top)

		glVertex3f( Max.i,Min.j, Max.k);					// Top Right Of The Quad (Bottom)
		glVertex3f(Min.i,Min.j, Max.k);					// Top Left Of The Quad (Bottom)
		glVertex3f(Min.i,Min.j, Min.k);					// Bottom Left Of The Quad (Bottom)
		glVertex3f( Max.i,Min.j, Min.k);					// Bottom Right Of The Quad (Bottom)

		glVertex3f( Max.i, Max.j, Max.k);					// Top Right Of The Quad (Front)
		glVertex3f(Min.i, Max.j, Max.k);					// Top Left Of The Quad (Front)
		glVertex3f(Min.i,Min.j, Max.k);					// Bottom Left Of The Quad (Front)
		glVertex3f( Max.i,Min.j, Max.k);					// Bottom Right Of The Quad (Front)

		glVertex3f( Max.i,Min.j,Min.k);					// Top Right Of The Quad (Back)
		glVertex3f(Min.i,Min.j,Min.k);					// Top Left Of The Quad (Back)
		glVertex3f(Min.i, Max.j,Min.k);					// Bottom Left Of The Quad (Back)
		glVertex3f( Max.i, Max.j,Min.k);					// Bottom Right Of The Quad (Back)

		glVertex3f(Min.i, Max.j, Max.k);					// Top Right Of The Quad (Left)
		glVertex3f(Min.i, Max.j,Min.k);					// Top Left Of The Quad (Left)
		glVertex3f(Min.i,Min.j,Min.k);					// Bottom Left Of The Quad (Left)
		glVertex3f(Min.i,Min.j, Max.k);					// Bottom Right Of The Quad (Left)

		glVertex3f( Max.i, Max.j,Min.k);					// Top Right Of The Quad (Right)
		glVertex3f( Max.i, Max.j, Max.k);					// Top Left Of The Quad (Right)
		glVertex3f( Max.i,Min.j, Max.k);					// Bottom Left Of The Quad (Right)
		glVertex3f( Max.i,Min.j,Min.k);					// Bottom Right Of The Quad (Right)

		glEnd();											// Done Drawing The Quad
	}//else
}

GLvoid CommonGL::DrawCCWBox( bool debugMode, Vector &Min, Vector &Max )
{
	if( debugMode )
	{
		glPushAttrib( GL_CURRENT_BIT | GL_ENABLE_BIT );
        glDisable( GL_BLEND );
		glDisable( GL_FOG );
		glDisable( GL_LIGHTING );
		glDisable( GL_TEXTURE_2D );
		glLineWidth( 2.0f );
		glColor3f( 1.0f, 1.0f, 1.0f );

		glBegin( GL_LINE_LOOP );
		glVertex3f( Max.i, Max.j,Min.k);					// Top Right Of The Quad (Top)
		glVertex3f( Max.i, Max.j, Max.k);					// Bottom Right Of The Quad (Top)
		glVertex3f(Min.i, Max.j, Max.k);					// Bottom Left Of The Quad (Top)
		glVertex3f(Min.i, Max.j,Min.k);					// Top Left Of The Quad (Top)
        glEnd();
		glBegin( GL_LINE_LOOP );
		glVertex3f( Max.i,Min.j, Max.k);					// Top Right Of The Quad (Bottom)
		glVertex3f( Max.i,Min.j,Min.k);					// Bottom Right Of The Quad (Bottom)
		glVertex3f(Min.i,Min.j,Min.k);					// Bottom Left Of The Quad (Bottom)
		glVertex3f(Min.i,Min.j, Max.k);					// Top Left Of The Quad (Bottom)
        glEnd();
		glBegin( GL_LINE_LOOP );
		glVertex3f( Max.i, Max.j, Max.k);					// Top Right Of The Quad (Front)
		glVertex3f( Max.i,Min.j, Max.k);					// Bottom Right Of The Quad (Front)
		glVertex3f(Min.i,Min.j, Max.k);					// Bottom Left Of The Quad (Front)
		glVertex3f(Min.i, Max.j, Max.k);					// Top Left Of The Quad (Front)
        glEnd();
		glBegin( GL_LINE_LOOP );
		glVertex3f( Max.i,Min.j,Min.k);					// Top Right Of The Quad (Back)
		glVertex3f( Max.i, Max.j,Min.k);					// Bottom Right Of The Quad (Back)
		glVertex3f(Min.i, Max.j,Min.k);					// Bottom Left Of The Quad (Back)
		glVertex3f(Min.i,Min.j,Min.k);					// Top Left Of The Quad (Back)
        glEnd();
		glBegin( GL_LINE_LOOP );
		glVertex3f(Min.i, Max.j, Max.k);					// Top Right Of The Quad (Left)
		glVertex3f(Min.i,Min.j, Max.k);					// Bottom Right Of The Quad (Left)
		glVertex3f(Min.i,Min.j,Min.k);					// Bottom Left Of The Quad (Left)
		glVertex3f(Min.i, Max.j,Min.k);					// Top Left Of The Quad (Left)
        glEnd();
		glBegin( GL_LINE_LOOP );
		glVertex3f( Max.i, Max.j,Min.k);					// Top Right Of The Quad (Right)
		glVertex3f( Max.i,Min.j,Min.k);					// Bottom Right Of The Quad (Right)
		glVertex3f( Max.i,Min.j, Max.k);					// Bottom Left Of The Quad (Right)
		glVertex3f( Max.i, Max.j, Max.k);					// Top Left Of The Quad (Right)
		glEnd();											// Done Drawing The Quad
		glPopAttrib();
	}
	else
	{
	glBegin(GL_QUADS);								// Draw A Quad

	glVertex3f( Max.i, Max.j,Min.k);					// Top Right Of The Quad (Top)
	glVertex3f( Max.i, Max.j, Max.k);					// Bottom Right Of The Quad (Top)
	glVertex3f(Min.i, Max.j, Max.k);					// Bottom Left Of The Quad (Top)
	glVertex3f(Min.i, Max.j,Min.k);					// Top Left Of The Quad (Top)

	glVertex3f( Max.i,Min.j, Max.k);					// Top Right Of The Quad (Bottom)
	glVertex3f( Max.i,Min.j,Min.k);					// Bottom Right Of The Quad (Bottom)
	glVertex3f(Min.i,Min.j,Min.k);					// Bottom Left Of The Quad (Bottom)
	glVertex3f(Min.i,Min.j, Max.k);					// Top Left Of The Quad (Bottom)

	glVertex3f( Max.i, Max.j, Max.k);					// Top Right Of The Quad (Front)
	glVertex3f( Max.i,Min.j, Max.k);					// Bottom Right Of The Quad (Front)
	glVertex3f(Min.i,Min.j, Max.k);					// Bottom Left Of The Quad (Front)
	glVertex3f(Min.i, Max.j, Max.k);					// Top Left Of The Quad (Front)

	glVertex3f( Max.i,Min.j,Min.k);					// Top Right Of The Quad (Back)
	glVertex3f( Max.i, Max.j,Min.k);					// Bottom Right Of The Quad (Back)
	glVertex3f(Min.i, Max.j,Min.k);					// Bottom Left Of The Quad (Back)
	glVertex3f(Min.i,Min.j,Min.k);					// Top Left Of The Quad (Back)

	glVertex3f(Min.i, Max.j, Max.k);					// Top Right Of The Quad (Left)
	glVertex3f(Min.i,Min.j, Max.k);					// Bottom Right Of The Quad (Left)
	glVertex3f(Min.i,Min.j,Min.k);					// Bottom Left Of The Quad (Left)
	glVertex3f(Min.i, Max.j,Min.k);					// Top Left Of The Quad (Left)

	glVertex3f( Max.i, Max.j,Min.k);					// Top Right Of The Quad (Right)
	glVertex3f( Max.i,Min.j,Min.k);					// Bottom Right Of The Quad (Right)
	glVertex3f( Max.i,Min.j, Max.k);					// Bottom Left Of The Quad (Right)
	glVertex3f( Max.i, Max.j, Max.k);					// Top Left Of The Quad (Right)
    glEnd();											// Done Drawing The Quad
	}
}

void CommonGL::SetShadowMatrix(GLfloat destMat[16], float lightPos[4], float plane[4])
{
	GLfloat dot; 

	dot = plane[0] * lightPos[0] + 
	      plane[1] * lightPos[1] +
		  plane[2] * lightPos[2] +
		  plane[3] * lightPos[3];

	destMat[0] = dot - lightPos[0] * plane[0];
	destMat[4] = 0.0f - lightPos[0] * plane[1];
	destMat[8] = 0.0f - lightPos[0] * plane[2];
	destMat[12] = 0.0f - lightPos[0] * plane[3];

	destMat[1] = 0.0f - lightPos[1] * plane[0];
	destMat[5] = dot - lightPos[1] * plane[1];
	destMat[9] = 0.0f - lightPos[1] * plane[2];
	destMat[13] = 0.0f - lightPos[1] * plane[3];

	destMat[2] = 0.0f - lightPos[2] * plane[0];
	destMat[6] = 0.0f - lightPos[2] * plane[1];
	destMat[10] = dot - lightPos[2] * plane[2];
	destMat[14] = 0.0f - lightPos[2] * plane[3];

	destMat[3] = 0.0f - lightPos[3] * plane[0];
	destMat[7] = 0.0f - lightPos[3] * plane[1];
	destMat[11] = 0.0f - lightPos[3] * plane[2];
	destMat[15] = dot - lightPos[3] * plane[3];

}
