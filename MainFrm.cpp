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

// MainFrm.cpp : CMainFrame ���ʵ��
//

#include "stdafx.h"
#include "Motion_Capture_MFC.h"
#include "SerialDataProc.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnUpdateApplicationLook)
	ON_COMMAND(ID_VIEW_CAPTION_BAR, &CMainFrame::OnViewCaptionBar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_CAPTION_BAR, &CMainFrame::OnUpdateViewCaptionBar)
	ON_COMMAND(ID_TOOLS_OPTIONS, &CMainFrame::OnOptions)
	ON_COMMAND(ID_FILE_PRINT, &CMainFrame::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CMainFrame::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMainFrame::OnFilePrintPreview)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_PREVIEW, &CMainFrame::OnUpdateFilePrintPreview)
	ON_COMMAND(ID_OPEN_SERIALPORT, &CMainFrame::OnOpenSerialport)
	ON_COMMAND(ID_SERIALPORT_SELECT, &CMainFrame::OnSerialportSelect)
	ON_COMMAND(ID_SERIALPORT_BAUDRATE, &CMainFrame::OnSerialportBaudrate)
END_MESSAGE_MAP()

// CMainFrame ����/����

CMainFrame::CMainFrame()
:p_ComboBox_SerialportSelect(nullptr)
, p_Motion_Capture_MFCView(nullptr)
{
	// TODO: �ڴ���ӳ�Ա��ʼ������
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_OFF_2007_BLACK);
	comm_vector.clear();
	p_serialPort = CSerialPort::GetSerialPortInstance();

	ptrForFrameAppDoc.pDoc = nullptr;
	ptrForFrameAppDoc.pMainFrame = nullptr;
	ptrForFrameAppDoc.pView = nullptr;

	for (int i = 0; i < 23;i++)
	{
		joint_num_and_node_num_pair[i] = -1;
	}

	joint_num_and_node_num_pair[12] = 6;
}

CMainFrame::~CMainFrame()
{
}
static unsigned int pre_n = 0;
DWORD WINAPI readPortFunc(LPVOID lpParameter)
{
	char buf[1024];
	int n = 32;
	CSerialPort*p_serialPort = CSerialPort::GetSerialPortInstance();
	memset(buf, 0, n);
	while (p_serialPort->is_open())
	{
		n = 32;
		if (OK_SERIALPORT != p_serialPort->readSerialPort(buf, n)) {
			continue;
		}
		else
		{
			if (n > 0) {
				if (n != 32) {
					TRACE("n = %d,type:%c\r\n", n, buf[0]);
				}
				else {
					//TRACE("type:%c,index:%d,ID:%d,w:%d,x:%d,y:%d,z:%d\r\n", buf[0], buf[1], *(int*)(buf + 2), *(short*)(buf + 24), *(short*)(buf + 26), *(short*)(buf + 28), *(short*)(buf + 30));
					CSerialDataProc::dataProc((struct PtrForFrameAppDoc *)lpParameter, buf);
				}
//				

//				TRACE("type:%c,index:%d,ID:%d,w:%d,x:%d,y:%d,z:%d\r\n", buf[0], buf[1], *(int*)(buf + 2), *(short*)(buf + 24), *(short*)(buf + 26), *(short*)(buf + 28), *(short*)(buf + 30));
			}
			else
			{
				continue;
			}
		}
		memset(buf, 0, 1024);
	}

	return 0;
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	BOOL bNameValid;

	m_wndRibbonBar.Create(this);
	m_wndRibbonBar.LoadFromResource(IDR_RIBBON);
	p_ComboBox_SerialportSelect = DYNAMIC_DOWNCAST(CMFCRibbonComboBox, m_wndRibbonBar.FindByID(ID_SERIALPORT_SELECT));
	p_ComboBox_SerialportbaudRate = DYNAMIC_DOWNCAST(CMFCRibbonComboBox, m_wndRibbonBar.FindByID(ID_SERIALPORT_BAUDRATE));
	p_Button_OpenSerialPort = DYNAMIC_DOWNCAST(CMFCRibbonButton, m_wndRibbonBar.FindByID(ID_OPEN_SERIALPORT));
	EnumerateSerialPorts(comm_vector);
	

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("δ�ܴ���״̬��\n");
		return -1;      // δ�ܴ���
	}

	CString strTitlePane1;
	CString strTitlePane2;
	bNameValid = strTitlePane1.LoadString(IDS_STATUS_PANE1);
	ASSERT(bNameValid);
	bNameValid = strTitlePane2.LoadString(IDS_STATUS_PANE2);
	ASSERT(bNameValid);
	m_wndStatusBar.AddElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE1, strTitlePane1, TRUE), strTitlePane1);
	m_wndStatusBar.AddExtendedElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE2, strTitlePane2, TRUE), strTitlePane2);

	// ���� Visual Studio 2005 ��ʽͣ��������Ϊ
	CDockingManager::SetDockingMode(DT_SMART);
	// ���� Visual Studio 2005 ��ʽͣ�������Զ�������Ϊ
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// �������񽫴�������࣬��˽���ʱ��������ͣ��: 
	EnableDocking(CBRS_ALIGN_TOP | CBRS_ALIGN_BOTTOM | CBRS_ALIGN_RIGHT);

	// ���������á�Outlook��������: 
	if (!CreateOutlookBar(m_wndNavigationBar, ID_VIEW_NAVIGATION, m_wndTree, m_wndCalendar, 250))
	{
		TRACE0("δ�ܴ�����������\n");
		return -1;      // δ�ܴ���
	}

	// ����������: 
	if (!CreateCaptionBar())
	{
		TRACE0("δ�ܴ���������\n");
		return -1;      // δ�ܴ���
	}

	// �Ѵ��� Outlook ����Ӧ���������ͣ����
	EnableDocking(CBRS_ALIGN_LEFT);
	EnableAutoHidePanes(CBRS_ALIGN_RIGHT);
	// ���ڳ־�ֵ�����Ӿ�����������ʽ
	OnApplicationLook(theApp.m_nAppLook);


	return 0;
}

int CMainFrame::get_JointID_by_NodeID(const int node_id)
{
	if (node_id < 1) {
		return -1;
	}

	for (int i = 0; i < 23; i++)
	{
		if (joint_num_and_node_num_pair[i] == node_id) {
			return i;
		}
	}

	return -1;
}

int CMainFrame::set_JointID_by_NodeID(const int node_id, const int joint_id)
{
	if (node_id < 1 || node_id > 22)
	{
		return -1;
	}

	if (joint_id < 1 || joint_id > 22)
	{
		return -2;
	}

	joint_num_and_node_num_pair[node_id] = joint_id;

	return 0;
}

int CMainFrame::set_joint_and_node_pair_negtive(void)
{
	for (int i = 0; i < 23; i++)
	{
		joint_num_and_node_num_pair[i] = -1;
	}
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return TRUE;
}

BOOL CMainFrame::CreateOutlookBar(CMFCOutlookBar& bar, UINT uiID, CMFCShellTreeCtrl& tree, CCalendarBar& calendar, int nInitialWidth)
{
	bar.SetMode2003();

	BOOL bNameValid;
	CString strTemp;
	bNameValid = strTemp.LoadString(IDS_SHORTCUTS);
	ASSERT(bNameValid);
	if (!bar.Create(strTemp, this, CRect(0, 0, nInitialWidth, 32000), uiID, WS_CHILD | WS_VISIBLE | CBRS_LEFT))
	{
		return FALSE; // δ�ܴ���
	}

	CMFCOutlookBarTabCtrl* pOutlookBar = (CMFCOutlookBarTabCtrl*)bar.GetUnderlyingWindow();

	if (pOutlookBar == NULL)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	pOutlookBar->EnableInPlaceEdit(TRUE);

	static UINT uiPageID = 1;

	// �ɸ��������Զ����أ��ɵ�����С�������ܹر�
	DWORD dwStyle = AFX_CBRS_FLOAT | AFX_CBRS_AUTOHIDE | AFX_CBRS_RESIZE;

	CRect rectDummy(0, 0, 0, 0);
	const DWORD dwTreeStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS;

	tree.Create(dwTreeStyle, rectDummy, &bar, 1200);
	bNameValid = strTemp.LoadString(IDS_FOLDERS);
	ASSERT(bNameValid);
	pOutlookBar->AddControl(&tree, strTemp, 2, TRUE, dwStyle);

	calendar.Create(rectDummy, &bar, 1201);
	bNameValid = strTemp.LoadString(IDS_CALENDAR);
	ASSERT(bNameValid);
	pOutlookBar->AddControl(&calendar, strTemp, 3, TRUE, dwStyle);

	bar.SetPaneStyle(bar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

	pOutlookBar->SetImageList(theApp.m_bHiColorIcons ? IDB_PAGES_HC : IDB_PAGES, 24);
	pOutlookBar->SetToolbarImageList(theApp.m_bHiColorIcons ? IDB_PAGES_SMALL_HC : IDB_PAGES_SMALL, 16);
	pOutlookBar->RecalcLayout();

	BOOL bAnimation = theApp.GetInt(_T("OutlookAnimation"), TRUE);
	CMFCOutlookBarTabCtrl::EnableAnimation(bAnimation);

	bar.SetButtonsFont(&afxGlobalData.fontBold);

	return TRUE;
}

BOOL CMainFrame::CreateCaptionBar()
{
	if (!m_wndCaptionBar.Create(WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, this, ID_VIEW_CAPTION_BAR, -1, TRUE))
	{
		TRACE0("δ�ܴ���������\n");
		return FALSE;
	}

	BOOL bNameValid;

	CString strTemp, strTemp2;
	bNameValid = strTemp.LoadString(IDS_CAPTION_BUTTON);
	ASSERT(bNameValid);
	m_wndCaptionBar.SetButton(strTemp, ID_TOOLS_OPTIONS, CMFCCaptionBar::ALIGN_LEFT, FALSE);
	bNameValid = strTemp.LoadString(IDS_CAPTION_BUTTON_TIP);
	ASSERT(bNameValid);
	m_wndCaptionBar.SetButtonToolTip(strTemp);

	bNameValid = strTemp.LoadString(IDS_CAPTION_TEXT);
	ASSERT(bNameValid);
	m_wndCaptionBar.SetText(strTemp, CMFCCaptionBar::ALIGN_LEFT);

	m_wndCaptionBar.SetBitmap(IDB_INFO, RGB(255, 255, 255), FALSE, CMFCCaptionBar::ALIGN_LEFT);
	bNameValid = strTemp.LoadString(IDS_CAPTION_IMAGE_TIP);
	ASSERT(bNameValid);
	bNameValid = strTemp2.LoadString(IDS_CAPTION_IMAGE_TEXT);
	ASSERT(bNameValid);
	m_wndCaptionBar.SetImageToolTip(strTemp, strTemp2);

	return TRUE;
}

// CMainFrame ���

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame ��Ϣ�������

void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_VS_2008:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_WINDOWS_7:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(TRUE);
		break;

	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
	}

	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}

void CMainFrame::OnViewCaptionBar()
{
	m_wndCaptionBar.ShowWindow(m_wndCaptionBar.IsVisible() ? SW_HIDE : SW_SHOW);
	RecalcLayout(FALSE);
}

void CMainFrame::OnUpdateViewCaptionBar(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndCaptionBar.IsVisible());
}

void CMainFrame::OnOptions()
{
	CMFCRibbonCustomizeDialog *pOptionsDlg = new CMFCRibbonCustomizeDialog(this, &m_wndRibbonBar);
	ASSERT(pOptionsDlg != NULL);

	pOptionsDlg->DoModal();
	delete pOptionsDlg;
}


void CMainFrame::OnFilePrint()
{
	if (IsPrintPreview())
	{
		PostMessage(WM_COMMAND, AFX_ID_PREVIEW_PRINT);
	}
}

void CMainFrame::OnFilePrintPreview()
{
	if (IsPrintPreview())
	{
		PostMessage(WM_COMMAND, AFX_ID_PREVIEW_CLOSE);  // ǿ�ƹرա���ӡԤ����ģʽ
	}
}

void CMainFrame::OnUpdateFilePrintPreview(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(IsPrintPreview());
}


void CMainFrame::OnOpenSerialport()
{
	// TODO: �ڴ���������������
	int ret = 0;

	if (ptrForFrameAppDoc.pDoc == nullptr )
	{
		ptrForFrameAppDoc.pDoc = (CMotion_Capture_MFCDoc *)CFrameWnd::GetActiveDocument();
	}

	if (ptrForFrameAppDoc.pMainFrame == nullptr)
	{
		ptrForFrameAppDoc.pMainFrame = this;
	}

	if (ptrForFrameAppDoc.pView == nullptr)
	{
		ptrForFrameAppDoc.pView = (CMotion_Capture_MFCView*)ptrForFrameAppDoc.pMainFrame->GetActiveView();
	}

	CString data_pre_comm = p_ComboBox_SerialportSelect->GetEditText();
	CString data_baudRate = p_ComboBox_SerialportbaudRate->GetEditText();
	
	if (data_baudRate.GetLength() <= 0) {
		AfxMessageBox(_T("������Ϊ��"));
		return;
	}
	if (data_pre_comm.GetLength() <= 0) {
		AfxMessageBox(_T("������ѡ��Ϊ��"));
		return;
	}
	CString data_comm("\\\\.\\");
	data_comm += data_pre_comm;

	if (!p_serialPort->is_open()) {
		
		ret = p_serialPort->openSeialPort(data_comm, _ttoi(data_baudRate));
		if (ret != OK_SERIALPORT) {
			AfxMessageBox(_T("���ڴ�ʧ��"));
			p_Button_OpenSerialPort->SetText(_T("��"));
			return;
		}
		p_Button_OpenSerialPort->SetText(_T("�ر�"));
		p_Button_OpenSerialPort->SetImageIndex(1, true);
		//�����������߳�
		hthread_SerialPort = CreateThread(NULL, 0, readPortFunc, &ptrForFrameAppDoc, 0, &threadID);
		TRACE("%d\n", threadID);

	}
	else
	{
		p_serialPort->closeSerialPort();
		p_Button_OpenSerialPort->SetText(_T("��"));
		p_Button_OpenSerialPort->SetImageIndex(0, true);
		return;
	}


	return;
}


// ö�ٳ�ϵͳ�п��õĴ���
void CMainFrame::EnumerateSerialPorts(std::vector<CString>& comm_vector)
{
	
	//ȷ�������������
	comm_vector.clear();

	for (UINT i = 0;i < 256;i++)
	{
		//��ʽ���豸����
		CString sPort;
		sPort.Format(_T("\\\\.\\COM%d"), i);

		//���Դ򿪴���
		BOOL bSuccess = FALSE;
		HANDLE hPort = ::CreateFile(sPort, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0);

		if (hPort == INVALID_HANDLE_VALUE)
		{
			DWORD dwError = GetLastError();
			//Check to see if the error was because some other app had the port open or a general failure  
			if (dwError == ERROR_ACCESS_DENIED || dwError == ERROR_GEN_FAILURE)
				bSuccess = TRUE;
		}
		else
		{
			//The port was opened successfully  
			bSuccess = TRUE;
			//Don't forget to close the port, since we are going to do nothing with it anyway  
			CloseHandle(hPort);
		}
		//Add the port number to the array which will be returned  
		if (bSuccess) {
			sPort.Format(_T("COM%d"), i);
			comm_vector.push_back(sPort);
			errno_t err = p_ComboBox_SerialportSelect->AddItem(sPort);
			if (CB_ERR == err || CB_ERRSPACE  == err) {
				MessageBox(_T("��Ӵ������ƴ���"));
			}
		}
	}
}


void CMainFrame::OnSerialportSelect()
{
	// TODO: �ڴ���������������
	//�������ڿ���״̬ʱ�������ȹرգ��ڴ��µĴ���
	if (p_serialPort->is_open()) {
		p_serialPort->closeSerialPort();
		p_Button_OpenSerialPort->SetText(_T("��"));
		p_Button_OpenSerialPort->SetImageIndex(0, true);
		Sleep(10);
		OnOpenSerialport();
	}

}


void CMainFrame::OnSerialportBaudrate()
{
	// TODO: �ڴ���������������
	//�������ڿ���״̬ʱ�������ȹرգ������µĲ����ʴ�
	if (p_serialPort->is_open()) {
		p_serialPort->closeSerialPort();
		p_Button_OpenSerialPort->SetText(_T("��"));
		p_Button_OpenSerialPort->SetImageIndex(0, true);
		Sleep(10);
		OnOpenSerialport();
	}
}
