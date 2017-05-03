// ��� MFC ʾ��Դ������ʾ���ʹ�� MFC Microsoft Office Fluent �û����� 
// (��Fluent UI��)����ʾ�������ο���
// ���Բ��䡶Microsoft ������ο����� 
// MFC C++ ������渽����ص����ĵ���  
// ���ơ�ʹ�û�ַ� Fluent UI ����������ǵ����ṩ�ġ�  
// ��Ҫ�˽��й� Fluent UI ��ɼƻ�����ϸ��Ϣ������� 
// http://go.microsoft.com/fwlink/?LinkId=238214��
//
// ��Ȩ����(C) Microsoft Corporation
// ��������Ȩ����

// Motion_Capture_MFCView.cpp : CMotion_Capture_MFCView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
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
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMotion_Capture_MFCView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_CREATE()
END_MESSAGE_MAP()

// CMotion_Capture_MFCView ����/����

CMotion_Capture_MFCView::CMotion_Capture_MFCView()
{
	// TODO: �ڴ˴���ӹ������

}

CMotion_Capture_MFCView::~CMotion_Capture_MFCView()
{
}

BOOL CMotion_Capture_MFCView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CMotion_Capture_MFCView ����

void CMotion_Capture_MFCView::OnDraw(CDC* /*pDC*/)
{
	CMotion_Capture_MFCDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CMotion_Capture_MFCView ��ӡ


void CMotion_Capture_MFCView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMotion_Capture_MFCView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CMotion_Capture_MFCView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CMotion_Capture_MFCView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
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


// CMotion_Capture_MFCView ���

#ifdef _DEBUG
void CMotion_Capture_MFCView::AssertValid() const
{
	CView::AssertValid();
}

void CMotion_Capture_MFCView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMotion_Capture_MFCDoc* CMotion_Capture_MFCView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMotion_Capture_MFCDoc)));
	return (CMotion_Capture_MFCDoc*)m_pDocument;
}
#endif //_DEBUG


// CMotion_Capture_MFCView ��Ϣ�������


void CMotion_Capture_MFCView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
//	SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_ROTATE)));
	CView::OnMouseMove(nFlags, point);
}


int CMotion_Capture_MFCView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������

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
