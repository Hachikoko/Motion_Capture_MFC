#pragma once


// OpenGLWindow
#include "Cal3DBase.h"
#include "Cal3DChild.h"
#include "light0.h"
#include "light1.h"
#include "light2.h"
#include "atltypes.h"
#include "vector.h"
//#include "bitmapslider.h"
#include "commonGL.h"
#include "Timer.h"
//#include <vector>

class OpenGLWindow : public CWnd
{
	DECLARE_DYNAMIC(OpenGLWindow)

public:
	OpenGLWindow();
	virtual ~OpenGLWindow();

	void Redraw();

	virtual bool SetupViewport(int cx, int cy);
	virtual void SetupPerspective(GLfloat theta, int cx, int cy);
	virtual void SetupPerspective(GLfloat theta);

	bool InitAll();
	void RenderWindow();

	// To hold the pixel format of an OpenGL window in which doesn't use multisampling
	int		m_PixelFormat;

	HGLRC	m_hRC;
	CDC*	m_pDC;
	Timer* m_timer;

	int	m_width, m_height;

	Cal3DBase* pCal3DbaseModel;
	Cal3DChild* pCal3DchildModel;

public:
	float m_plane[4];
	float m_shadowMatrix[16];
	float m_lightPos0[4];
	CommonGL commonGL;

	bool m_IsMultiSampling;

protected:
	DECLARE_MESSAGE_MAP()


	//functions and variables to deal with lighting
	// //Initializes and sets all the lights of the scene
	void initializeLights(void);
	// Key light
	light0 Light0;
	float LightPos0[4];
	// Fill light
	light1 Light1;
	float LightPos1[4];
	// Back light
	light2 Light2;
	float LightPos2[4];

	//initializes all the models at the beginning
	bool InitializeModels(void);
	//it loads a model whenever we press the + btn and want to load a model from the list
	bool LoadModel(CStringW modelPath);

	// To remove all the models and texture data
	//it also calls the ReleaseModels() function
	bool RemoveAll(void);

	//release the models
	void ReleaseModels(void);
	// Removes a model when we press the - btn to remove a model from the list
	bool removeModel(int index);

	// To hold the previous point of the mouse
	CPoint m_PrevPoint;

	bool m_IsLMouseDown;



	// Holds the differences of the previous and current  mouse positions  in X direction
	CPoint m_DeltaMouse;

	// To change the zoom effect with gluPerspective()
	GLfloat m_distance;

	// GL_LINE, GL_POINT, GL_FILL
	GLenum m_DrawMode;
	void GetDrawMode(void);
	// Default mode, I use fill mode( GL_FILL )
	void GetDefaultDrawMode(void);

	// to move the camera with middle mouse button
	Vector m_camera;
	// to rotate the object	
	Vector m_Rotation;

	// current cursor
	HCURSOR m_hCurrentCursor;

	float m_elapsedTime;

	void DrawGrid(void);
	void DrawShadow(void);
	//Draw the background color( a shaded square )
	void DrawBackground(void);

	bool m_isTextured;
	bool m_IsInteractive;
	bool m_drawBone;
	bool m_isLighting;
	bool m_enableRotation;

	bool m_IsRMouseDown;
	bool m_IsMMouseDown;
	bool m_IsMouseMoving;

	//Level of details
	//Note: I don't know why i can't change the level of detail of a model with textured coordinates
	//It crashes the program.There's the same problem in cally demo
	void setLodLevel(float LodLevel);

	//functions related to the animations of the models
	int GetAnimationCount(void);
	bool NextAnimation(void);
	bool PrevAnimation(void);

	bool IsModelValid(void);

	public:


		bool GetModelInfo(int& numberOfFaces, int& numberOfAnimations, int& numberOfVertices
			, int& numberOfMeshes, int& numberOfMaterials, float& LODLevel);

	public:
		bool m_drawGrid;
		afx_msg void OnTimer(UINT_PTR nIDEvent);
		afx_msg void OnPaint();
		afx_msg void OnSize(UINT nType, int cx, int cy);
		afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
//		afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
		afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
		afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
		afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
		afx_msg void OnMouseMove(UINT nFlags, CPoint point);
		afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
		afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
		afx_msg void OnDestroy();
};


