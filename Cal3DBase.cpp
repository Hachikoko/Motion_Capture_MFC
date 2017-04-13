//Copyright (C) 2008 Ehsan Kamrani
//This program is distributed under the terms of the GNU Lesser General Public License.
//Note: Some classes and functions of this class are from the miniviewer_gl project available at 
//http://download.gna.org/cal3d/sources/	
//minviewer_gl has been released under the terms of the GNU LGPL license
// I have improved and changed some functions and added more functions to deal with cal3D models

#include "StdAfx.h"
#include "Cal3DBase.h"


Cal3DBase::Cal3DBase()
{
	m_scale = 1.0f;
	m_path = "";
	m_calCoreModel = NULL;
}

Cal3DBase::~Cal3DBase()
{
}

bool Cal3DBase::Init(CStringW filename_CFG )
{
	std::string name = "CAL3D";
    m_calCoreModel = new CalCoreModel(name);
	if( ! parseModelConfiguration( filename_CFG ) )
	{
		return false;
	}
	// make one material thread for each material
    // NOTE: this is not the right way to do it, but this viewer can't do the right
    // mapping without further information on the model etc., so this is the only
    // thing we can do here.
   
    for(int materialId = 0; materialId < m_calCoreModel->getCoreMaterialCount(); materialId++)
    {
        // create the a material thread
        m_calCoreModel->createCoreMaterialThread(materialId);

        // initialize the material thread
        m_calCoreModel->setCoreMaterialId(materialId, 0, materialId);
    }

    // load all textures and store the opengl texture id in the corresponding map in the material
    for(int materialId = 0; materialId < m_calCoreModel->getCoreMaterialCount(); materialId++)
    {
        // get the core material
        CalCoreMaterial *pCoreMaterial = m_calCoreModel->getCoreMaterial(materialId);
        // loop through all maps of the core material
        for(int mapId = 0; mapId < pCoreMaterial->getMapCount(); mapId++)
        {
            // get the filename of the texture
            std::string strFilename = pCoreMaterial->getMapFilename(mapId);

            // allocate space for a zero-terminated copy of the string
			char *charPtrString = new char[m_path.size() + strFilename.size() + 10 ];

            // copy the string
			std::string tempStr = m_path + strFilename;
//			std::strcpy(charPtrString, tempStr.c_str() );
			strcpy_s(charPtrString,(m_path.size() + strFilename.size() + 10) ,tempStr.c_str());

            // load the texture from the file
            GLuint textureId;
			glGenTextures( 1, &textureId );
            //commonGL.LoadTargaTexture( charPtrString, textureId );
			commonGL.LoadAdvancedTargaTexture( charPtrString, textureId );
            // clean up the allocated space
         	delete[] charPtrString; 

            // store the OpenGL texture id in the user data of the map
            pCoreMaterial->setMapUserData(mapId, (Cal::UserData)textureId);
        }
	}
	return true;
}
//This function opens the cfg file and parses it
bool Cal3DBase::parseModelConfiguration( CStringW strFilename )
{
	// open the model configuration file
	std::ifstream file;
	file.open( strFilename, std::ios::in );
	//We may use uincode strings for our strFilename, but we don't use unicode strings 
	//inside our cfg file( we use pure english ).m_report is used for representing texts
	//inside the cfg file.
	CStringA m_report;

	if(!file)
	{
		//strFileName may contanin unicode strings
		CStringW m_report;
		file.close();
		m_report.Format( _T( "Couldn't open the CFG file: %s" ), strFilename );
		AfxMessageBox( m_report );
		return false;
	}

	// parse all lines from the model configuration file
	int line;
	for(line = 1; ; line++)
	{
		// read the next model configuration line
		std::string strBuffer;
		std::getline(file, strBuffer);

		// stop if we reached the end of file
		if(file.eof()) break;

		// check if an error happend while reading from the file
		if(!file)
		{
			m_report.Format( "Couldn't open the CFG file: %s", strBuffer.c_str() );
			MessageBoxA( NULL, m_report, "Error", MB_OK );
			return false;
		}

		// find the first non-whitespace character
		std::string::size_type pos;
		pos = strBuffer.find_first_not_of(" \t");

		// check for empty lines
		if((pos == std::string::npos) || (strBuffer[pos] == '\n') || (strBuffer[pos] == '\r') || (strBuffer[pos] == 0)) continue;

		// check for comment lines
		if(strBuffer[pos] == '#') continue;

		// get the key
		std::string strKey;
		strKey = strBuffer.substr(pos, strBuffer.find_first_of(" =\t\n\r", pos) - pos);
		pos += strKey.size();

		// get the '=' character
		pos = strBuffer.find_first_not_of(" \t", pos);
		if((pos == std::string::npos) || (strBuffer[pos] != '='))
		{
			return false;
		}

		// find the first non-whitespace character after the '=' character
		pos = strBuffer.find_first_not_of(" \t", pos + 1);

		// get the data
		std::string strData;
		strData = strBuffer.substr(pos, strBuffer.find_first_of("\n\r", pos) - pos);

		// handle the model creation
		if(strKey == "scale")
		{
			// set rendering scale factor
			m_scale = (float)atof(strData.c_str());
		}
		else if(strKey == "path")
        {
            // set the new path for the data files if one hasn't been set already
            m_path = strData;
        }

		else if(strKey == "skeleton")
		{
			// load core skeleton
			if(!m_calCoreModel->loadCoreSkeleton( m_path + strData))
			{
				m_report.Format( "Couldn't open the CSF file: %s", (m_path + strData).c_str() );
				MessageBoxA( NULL, m_report, "Error", MB_OK );

				return false;
			}
		}
		else if(strKey == "animation")
		{
			// load core animation
			if(m_calCoreModel->loadCoreAnimation(m_path + strData) == -1)
			{
				m_report.Format( "Couldn't open the CAF file: %s", (m_path + strData).c_str() );
				MessageBoxA( NULL, m_report, "Error", MB_OK );
				return false;
			}
		}
		else if(strKey == "mesh")
		{
			// load core mesh
			if(m_calCoreModel->loadCoreMesh(m_path + strData) == -1)
			{
				m_report.Format( "Couldn't open the CMF file: %s", ( m_path + strData ).c_str() );
				MessageBoxA( NULL, m_report, "Error", MB_OK );
				return false;
			}
		}
		else if(strKey == "material")
		{
			// load core material
			if(m_calCoreModel->loadCoreMaterial(m_path + strData) == -1)
			{
				m_report.Format( "Couldn't open the CRF file: %s", (m_path + strData).c_str() );
				MessageBoxA( NULL, m_report, "Error", MB_OK );
				return false;
			}
		}
		else
		{
			// everything else is ignored
		}
	}

	// explicitely close the file
	file.close();

	return true;
}
bool Cal3DBase::Release()
{
	if( m_calCoreModel )
        delete m_calCoreModel;
	return true;
}

CalCoreModel* Cal3DBase::GetCore()
{
	return m_calCoreModel;
}
