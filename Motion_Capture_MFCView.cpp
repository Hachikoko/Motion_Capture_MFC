// 这段 MFC 示例源代码演示如何使用 MFC Microsoft Office Fluent 用户界面 
// (“Fluent UI”)。该示例仅供参考，
// 用以补充《Microsoft 基础类参考》和 
// MFC C++ 库软件随附的相关电子文档。  
// 复制、使用或分发 Fluent UI 的许可条款是单独提供的。  
// 若要了解有关 Fluent UI 许可计划的详细信息，请访问 
// http://go.microsoft.com/fwlink/?LinkId=238214。
//
// 版权所有(C) Microsoft Corporation
// 保留所有权利。

// Motion_Capture_MFCView.cpp : CMotion_Capture_MFCView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "Motion_Capture_MFC.h"
#endif

#include "Motion_Capture_MFCDoc.h"
#include "Motion_Capture_MFCView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMotion_Capture_MFCView

IMPLEMENT_DYNCREATE(CMotion_Capture_MFCView, CView)

BEGIN_MESSAGE_MAP(CMotion_Capture_MFCView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMotion_Capture_MFCView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_CREATE()
END_MESSAGE_MAP()

// CMotion_Capture_MFCView 构造/析构

CMotion_Capture_MFCView::CMotion_Capture_MFCView()
{
	// TODO: 在此处添加构造代码

}

CMotion_Capture_MFCView::~CMotion_Capture_MFCView()
{
}

BOOL CMotion_Capture_MFCView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CMotion_Capture_MFCView 绘制

void CMotion_Capture_MFCView::OnDraw(CDC* /*pDC*/)
{
	CMotion_Capture_MFCDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CMotion_Capture_MFCView 打印


void CMotion_Capture_MFCView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMotion_Capture_MFCView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CMotion_Capture_MFCView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CMotion_Capture_MFCView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CMotion_Capture_MFCView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMotion_Capture_MFCView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CMotion_Capture_MFCView 诊断

#ifdef _DEBUG
void CMotion_Capture_MFCView::AssertValid() const
{
	CView::AssertValid();
}

void CMotion_Capture_MFCView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMotion_Capture_MFCDoc* CMotion_Capture_MFCView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMotion_Capture_MFCDoc)));
	return (CMotion_Capture_MFCDoc*)m_pDocument;
}
#endif //_DEBUG


// CMotion_Capture_MFCView 消息处理程序


void CMotion_Capture_MFCView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
//	SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_ROTATE)));
	CView::OnMouseMove(nFlags, point);
}


int CMotion_Capture_MFCView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码

	//Intitialize OpenGL window
	CRect rcClient(100,100,800,600);
	//GetClientRect(&rcClient);
	//rcClient.DeflateRect(7, 32, 207, 46);

	//Create the OpenGLw window here
	if (!m_view.Create(NULL, NULL, WS_BORDER | WS_VISIBLE | WS_CHILD , rcClient, this, 0))
	{
		PostQuitMessage(0);
		return FALSE;
	}
	GLenum err = glewInit();
	if( err != GLEW_OK )
		AfxMessageBox( _T("Couldn't initialize GLEW!") );
	
	else
	{
		m_view.DestroyWindow();
		m_view.m_IsMultiSampling = true;
		if (!m_view.Create(NULL, NULL, WS_BORDER | WS_VISIBLE | WS_CHILD , rcClient, this, 0))
		{
			m_view.DestroyWindow();
			m_view.m_IsMultiSampling = false;
			if (!m_view.Create(NULL, NULL, WS_BORDER | WS_VISIBLE | WS_CHILD  , rcClient, this, 0))
			{
				PostQuitMessage(0);
				return FALSE;
			}
		}
	}
	

	m_view.InitAll();
	m_view.ShowWindow(SW_SHOW);
//	if (m_view.pCal3DchildModel->GetAnimationCount())
//	{
//		m_view.pCal3DchildModel->clearCycle(m_view.pCal3DchildModel->GetCurrentAnimationId(), 0.0f);
//		m_view.pCal3DchildModel->SetCurrentAnimationId(2);
////		m_view.pCal3DchildModel->blendCycle(m_view.pCal3DchildModel->GetCurrentAnimationId(), 1.0f, 0.0f);
//		m_view.pCal3DchildModel->executeAction(m_view.pCal3DchildModel->GetCurrentAnimationId(), 1.0f, 0.0f);
//
//	}
	

	return true;
}
