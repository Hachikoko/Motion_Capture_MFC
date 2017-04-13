//Copyright (C) 2008 Ehsan Kamrani
//This program is distributed under the terms of the GNU Lesser General Public License.
//Note: Some classes and functions of this class are from the miniviewer_gl project available at 
//http://download.gna.org/cal3d/sources/	
//minviewer_gl has been released under the terms of GNU LGPL license
// I have improved and changed some functions and added more functions to deal with cal3D models

#include "StdAfx.h"
#include "Cal3DChild.h"

Cal3DChild::Cal3DChild()
: m_currentAnimationId(0)
, m_animationCount(0)
, m_vertexCount(0)
, m_faceCount(0)
, m_LODLevel(1)
, m_materialCount(0)
, m_meshCount(0)
{
	m_calModel = NULL;
}

Cal3DChild::~Cal3DChild()
{
}

bool Cal3DChild::Init( CalCoreModel* coreModel )
{
    m_calModel = new CalModel( coreModel );
    // attach all meshes to the model
    for(int meshId = 0; meshId < coreModel->getCoreMeshCount(); meshId++)
    {
        m_calModel->attachMesh(meshId);
    }

    // set the material set of the whole model
    m_calModel->setMaterialSet(0);
	m_animationCount = coreModel->getCoreAnimationCount();
	m_materialCount = coreModel->getCoreMaterialCount();
	m_meshCount = coreModel->getCoreMeshCount();
   if(  m_animationCount > 0)
   {
	   m_currentAnimationId = 0;
	   //coreModel->getCoreAnimation(m_currentAnimationId)->getDuration();
	   blendCycle(m_currentAnimationId, 1.0f, 0.0f);
   }
   else
   {
	   m_currentAnimationId = -1;
	   //m_leftAnimationTime = -1.0f;
   }
   // we're done
   return true;
}

void Cal3DChild::executeAction( int animationID, float delayIn, float delayOut, float weightTarget, bool autoLock)
{
	m_calModel->getMixer()->executeAction(animationID, delayIn, delayOut, weightTarget, autoLock );
}

void Cal3DChild::blendCycle( int animationID, float delayIn, float delayOut )
{
	m_calModel->getMixer()->blendCycle( animationID, delayIn, delayOut );

}

void Cal3DChild::clearCycle( int animationID, float delay )
{
	m_calModel->getMixer()->clearCycle( animationID, delay );
}

void Cal3DChild::Update( float elapsedTime )
{
	m_calModel->update( elapsedTime );
}

void Cal3DChild::RenderModel( float elapsedTime, bool isTextured )
{
    CalRenderer *pCalRenderer = m_calModel->getRenderer();
	m_faceCount = 0;
	m_vertexCount = 0;

    // begin the rendering loop
    if(pCalRenderer->beginRendering())
    {
	    // we will use vertex arrays, so enable them
		glEnableClientState( GL_VERTEX_ARRAY );
		glEnableClientState( GL_NORMAL_ARRAY );
        // get the number of meshes
        int meshCount = pCalRenderer->getMeshCount();

        // render all meshes of the model
        for(int meshId = 0; meshId < meshCount; meshId++)
        {
            // get the number of submeshes
            int submeshCount = pCalRenderer->getSubmeshCount(meshId);
            
            // render all submeshes of the mesh
            for(int submeshId = 0; submeshId < submeshCount; submeshId++)
            {
                // select mesh and submesh for further data access
                if(pCalRenderer->selectMeshSubmesh(meshId, submeshId))
                {
                    unsigned char meshColor[4];
                    GLfloat materialColor[4];

                    // set the material ambient color
                    pCalRenderer->getAmbientColor(&meshColor[0]);
                    materialColor[0] = (GLfloat)meshColor[0] / 255.0f;  materialColor[1] = (GLfloat)meshColor[1] / 255.0f; materialColor[2] = (GLfloat)meshColor[2] / 255.0f; materialColor[3] = (GLfloat)meshColor[3] / 255.0f;
					mat.FrontAmbient( materialColor );

                    // set the material diffuse color
                    pCalRenderer->getDiffuseColor(&meshColor[0]);
                    materialColor[0] = meshColor[0] / 255.0f;  materialColor[1] = meshColor[1] / 255.0f; materialColor[2] = meshColor[2] / 255.0f; materialColor[3] = 1.0;
					mat.FrontDiffuse( materialColor );

                    // set the material specular color
                    pCalRenderer->getSpecularColor(&meshColor[0]);
                    materialColor[0] = meshColor[0] / 255.0f;  materialColor[1] = meshColor[1] / 255.0f; materialColor[2] = meshColor[2] / 255.0f; materialColor[3] = meshColor[3] / 255.0f;
					mat.FrontSpecular( materialColor );

                    // set the material shininess factor
                    //float shininess = pCalRenderer->getShininess();
					mat.FrontShininess( 25.0f );


                    // get the transformed vertices of the submesh
                    static GLfloat meshVertices[3000][3];
                    int vertexCount = pCalRenderer->getVertices(&meshVertices[0][0]);

                    // get the transformed normals of the submesh
                    static GLfloat meshNormals[3000][3];
                    int normalCount = pCalRenderer->getNormals(&meshNormals[0][0]);
                    
                    // get the texture coordinates of the submesh
                    static GLfloat meshTextureCoordinates[3000][2];
                    int textureCoordinateCount = pCalRenderer->getTextureCoordinates(0, &meshTextureCoordinates[0][0]);
					// get the faces of the submesh
                    static CalIndex meshFaces[3000][3];
                    int faceCount = pCalRenderer->getFaces(&meshFaces[0][0]);

                        // set the vertex and normal buffers
                        glVertexPointer(3, GL_FLOAT, 0, &meshVertices[0][0]);
                        glNormalPointer(GL_FLOAT, 0, &meshNormals[0][0]);

						if( (pCalRenderer->getMapCount() > 0) && ( textureCoordinateCount > 0 ) && isTextured )
						{
							lightModel.EnableSeparateSpecular();
							glEnable(GL_TEXTURE_2D);
							glEnableClientState(GL_TEXTURE_COORD_ARRAY);
							glEnable(GL_COLOR_MATERIAL);

							// set the texture id we stored in the map user data
							glBindTexture(GL_TEXTURE_2D, (GLuint)pCalRenderer->getMapUserData(0));
	                        
							// set the texture coordinate buffer
							glTexCoordPointer(2, GL_FLOAT, 0, &meshTextureCoordinates[0][0] );
							glColor3f(1.0f, 1.0f, 1.0f);
						}

						// draw the submesh
						if(sizeof(CalIndex)==2)
							glDrawElements(GL_TRIANGLES, faceCount * 3, GL_UNSIGNED_SHORT, &meshFaces[0][0]);
						else
							glDrawElements(GL_TRIANGLES, faceCount * 3, GL_UNSIGNED_INT, &meshFaces[0][0]);

                    // disable the texture coordinate state if necessary

                    if((pCalRenderer->getMapCount() > 0) && (textureCoordinateCount > 0) && isTextured )
                    {
						lightModel.DisableSepatrateSpecular();
                        glDisable(GL_COLOR_MATERIAL);
                        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
                        glDisable(GL_TEXTURE_2D);
                    }
				// adjust the vertex and face counter
			    m_vertexCount += vertexCount;
				m_faceCount += faceCount;
				}
			}
		}
        // clear vertex array state
        glDisableClientState(GL_NORMAL_ARRAY);
        glDisableClientState(GL_VERTEX_ARRAY);

		pCalRenderer->endRendering();
	}
	Update( elapsedTime );
}
bool Cal3DChild::Release()
{
	if( m_calModel )
	{
		CalRenderer *pCalRenderer = m_calModel->getRenderer();
		if( pCalRenderer )
		{
			int meshCount = pCalRenderer->getMeshCount();
			for(int meshId = 0; meshId < meshCount; meshId++)
			{
				// get the number of submeshes
				int submeshCount = pCalRenderer->getSubmeshCount(meshId);
				for(int submeshId = 0; submeshId < submeshCount; submeshId++)
				{
					// select mesh and submesh for further data access
					if(pCalRenderer->selectMeshSubmesh(meshId, submeshId))
					{
						static float meshTextureCoordinates[3000][2];
						int textureCoordinateCount = pCalRenderer->getTextureCoordinates(0, &meshTextureCoordinates[0][0]);
						if((pCalRenderer->getMapCount() > 0) && (textureCoordinateCount > 0))
						{
							//release texture data
							GLuint textureId = (GLuint)pCalRenderer->getMapUserData(0);
							glDeleteTextures( 1,&textureId );
						}//if 
							
					}//if
				}//for( int submeshId...
			}//for( int meshId...
		}
	}

	if( m_calModel )
        delete m_calModel;

	return true;
}

void Cal3DChild::setLodLevel(float lodLevel )
{
	m_calModel->setLodLevel( lodLevel );
	m_LODLevel = lodLevel;
}

int Cal3DChild::GetAnimationCount(void)
{
	return m_animationCount;
}

int Cal3DChild::GetCurrentAnimationId(void)
{
	return m_currentAnimationId;
}

void Cal3DChild::SetCurrentAnimationId(int Id)
{
	m_currentAnimationId = Id;
}

void Cal3DChild::RenderSkeleton()
{
	// draw the bone lines
	glPushAttrib( GL_LINE_BIT | GL_POINT_BIT );
  
	float lines[1024][2][3];
	int nrLines = m_calModel->getSkeleton()->getBoneLines(&lines[0][0][0]);

	glLineWidth(3.0f);
	glColor3f(1.0f, 1.0f, 0.0f);
	glBegin(GL_LINES);
	for(int currLine = 0; currLine < nrLines; currLine++)
	{
	glVertex3f(lines[currLine][0][0], lines[currLine][0][1], lines[currLine][0][2]);
	glVertex3f(lines[currLine][1][0], lines[currLine][1][1], lines[currLine][1][2]);
	}
	glEnd();

	// draw the bone points
	float points[1024][3];
	int nrPoints = m_calModel->getSkeleton()->getBonePoints(&points[0][0]);

	glPointSize(5.0f);
	glBegin(GL_POINTS);
	glColor3f(1.0f, 0.0f, 0.0f);
	for(int currPoint = 0; currPoint < nrPoints; currPoint++)
	{
	glVertex3f(points[currPoint][0], points[currPoint][1], points[currPoint][2]);
	}
	glEnd();
	glPopAttrib();
	glColor3f( 1.0f, 1.0f, 1.0f );
}

int Cal3DChild::GetFaceCount(void)
{
	return m_faceCount;
}

int Cal3DChild::GetVertexCount(void)
{
	return m_vertexCount;
}

float Cal3DChild::GetLODLevel(void)
{
	return m_LODLevel;
}

int Cal3DChild::GetMeshCount(void)
{
	return m_meshCount;
}

int Cal3DChild::GetMaterialCount(void)
{
	return m_materialCount;
}
