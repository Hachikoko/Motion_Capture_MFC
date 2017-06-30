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

// MainFrm.cpp : CMainFrame 类的实现
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
	ON_COMMAND(IDB_CORRECTION, &CMainFrame::OnCalibration)
	ON_COMMAND(ID_COMBO_CALIBRATION_LENGTH, &CMainFrame::OnComboCalibrationLength)
	ON_COMMAND(ID_CHECK_TRANSMISSION_SWITCH, &CMainFrame::OnCheckTransmissionSwitch)
	ON_UPDATE_COMMAND_UI(ID_CHECK_TRANSMISSION_SWITCH, &CMainFrame::OnUpdateCheckTransmissionSwitch)
	ON_WM_TIMER()
	ON_COMMAND(ID_BUTTONMAPNODE, &CMainFrame::OnButtonmapnode)
END_MESSAGE_MAP()

// CMainFrame 构造/析构

CMainFrame::CMainFrame()
:p_ComboBox_SerialportSelect(nullptr)
, p_Motion_Capture_MFCView(nullptr)
, p_calibration_Dlg(nullptr)
, after_map_node_number(false)
{
	// TODO: 在此添加成员初始化代码
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_OFF_2007_BLACK);
	comm_vector.clear();
	p_serialPort = CSerialPort::GetSerialPortInstance();

	ptrForFrameAppDoc.pDoc = nullptr;
	ptrForFrameAppDoc.pMainFrame = nullptr;
	ptrForFrameAppDoc.pView = nullptr;
	ptrForFrameAppDoc.wired_flag = false;

	for (int i = 0; i < 23;i++)
	{
		joint_num_and_node_num_pair[i] = -1;
	}

//	joint_num_and_node_num_pair[12] = 12;
//	joint_num_and_node_num_pair[13] = 6;

	Calibration_Flag = false;
	Calibration_Start_Flag = false;

	calibration_Index = 0;
}

CMainFrame::~CMainFrame()
{
//	delete p_calibration_Dlg;
}
static unsigned int pre_n = 0;
DWORD WINAPI readPortFunc(LPVOID lpParameter)
{
	char buf[1024];
	int n;
	unsigned int wired_transmission_index = 0;
	struct PtrForFrameAppDoc * p_ForFrameAppDoc = (struct PtrForFrameAppDoc *)lpParameter;
	CSerialPort*p_serialPort = CSerialPort::GetSerialPortInstance();
	memset(buf, 0, 1024);
	while (p_serialPort->is_open())
	{
		n = p_ForFrameAppDoc->wired_flag == true ? 1024 : 32;

		if (OK_SERIALPORT != p_serialPort->readSerialPort(buf, n)) {
			continue;
		}
		else
		{
			if (n > 0) {
				if (n == 32 && buf[0] == 'D') {
					CSerialDataProc::dataProc(p_ForFrameAppDoc, buf);
				}
				else if(buf[0] == '#'&& buf[2] == '#'){
					int n = buf[1];
					char node_data_buf[40];
					wired_transmission_index++;
					for (int i = 0;i < n;i++)
					{
						if (1 == buf[i * 41 + 3]) {
							memcpy(node_data_buf, (buf + i * 41 + 4), 40);
							*((unsigned int*)node_data_buf + 2) = wired_transmission_index;
//							CSerialDataProc::dataProc(p_ForFrameAppDoc, node_data_buf);
							TRACE("%c_%d_%d\r\n", node_data_buf[0], node_data_buf[1], wired_transmission_index);
						}
						memset(node_data_buf, 0, 40);
					}	

//					p_ForFrameAppDoc->pMainFrame->Send_by_SerialPort("S", 1);

				}
				else
				{
				}
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
	p_CheckBox_Transmission_Model_Switch = DYNAMIC_DOWNCAST(CMFCRibbonCheckBox, m_wndRibbonBar.FindByID(ID_CHECK_TRANSMISSION_SWITCH));
	p_Button_Map_Node_Number = DYNAMIC_DOWNCAST(CMFCRibbonButton, m_wndRibbonBar.FindByID(ID_BUTTONMAPNODE));

	EnumerateSerialPorts(comm_vector);

	p_ComboBox_Calibration_Length = DYNAMIC_DOWNCAST(CMFCRibbonComboBox, m_wndRibbonBar.FindByID(ID_COMBO_CALIBRATION_LENGTH));
	p_Button_Calibration_Processing = DYNAMIC_DOWNCAST(CMFCRibbonButton, m_wndRibbonBar.FindByID(IDB_CORRECTION));
	
	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}

	CString strTitlePane1;
	CString strTitlePane2;
	bNameValid = strTitlePane1.LoadString(IDS_STATUS_PANE1);
	ASSERT(bNameValid);
	bNameValid = strTitlePane2.LoadString(IDS_STATUS_PANE2);
	ASSERT(bNameValid);
	m_wndStatusBar.AddElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE1, strTitlePane1, TRUE), strTitlePane1);
	m_wndStatusBar.AddExtendedElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE2, strTitlePane2, TRUE), strTitlePane2);

	// 启用 Visual Studio 2005 样式停靠窗口行为
	CDockingManager::SetDockingMode(DT_SMART);
	// 启用 Visual Studio 2005 样式停靠窗口自动隐藏行为
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// 导航窗格将创建在左侧，因此将暂时禁用左侧的停靠: 
	EnableDocking(CBRS_ALIGN_TOP | CBRS_ALIGN_BOTTOM | CBRS_ALIGN_RIGHT);

	// 创建并设置“Outlook”导航栏: 
	if (!CreateOutlookBar(m_wndNavigationBar, ID_VIEW_NAVIGATION, m_wndTree, m_wndCalendar, 250))
	{
		TRACE0("未能创建导航窗格\n");
		return -1;      // 未能创建
	}

	// 创建标题栏: 
	if (!CreateCaptionBar())
	{
		TRACE0("未能创建标题栏\n");
		return -1;      // 未能创建
	}

	// 已创建 Outlook 栏，应允许在左侧停靠。
	EnableDocking(CBRS_ALIGN_LEFT);
	EnableAutoHidePanes(CBRS_ALIGN_RIGHT);
	// 基于持久值设置视觉管理器和样式
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

void CMainFrame::SetCalibrationFlag(void)
{
	Calibration_Flag = true;
}

void CMainFrame::ClearCalibrationFlag(void)
{
	Calibration_Flag = false;
}

bool CMainFrame::GetCalibrationFlag(void)
{
	return Calibration_Flag;
}

void CMainFrame::SetCalibrationStartFlag(void)
{
	Calibration_Start_Flag = true;
}

void CMainFrame::ClearCalibrationStartFlag(void)
{
	Calibration_Start_Flag = false;
}

bool CMainFrame::GetCalibrationStartFlag(void)
{
	return Calibration_Start_Flag;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return TRUE;
}

BOOL CMainFrame::CreateOutlookBar(CMFCOutlookBar& bar, UINT uiID, CMySuperGrid& tree, CCalendarBar& calendar, int nInitialWidth)
{
	bar.SetMode2003();

	BOOL bNameValid;
	CString strTemp;
	bNameValid = strTemp.LoadString(IDS_SHORTCUTS);
	ASSERT(bNameValid);
	if (!bar.Create(strTemp, this, CRect(0, 0, nInitialWidth, 32000), uiID, WS_CHILD | WS_VISIBLE | CBRS_LEFT))
	{
		return FALSE; // 未能创建
	}

	CMFCOutlookBarTabCtrl* pOutlookBar = (CMFCOutlookBarTabCtrl*)bar.GetUnderlyingWindow();

	if (pOutlookBar == NULL)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	pOutlookBar->EnableInPlaceEdit(TRUE);

	static UINT uiPageID = 1;

	// 可浮动，可自动隐藏，可调整大小，但不能关闭
	DWORD dwStyle = AFX_CBRS_FLOAT | AFX_CBRS_AUTOHIDE | AFX_CBRS_RESIZE;

	CRect rectDummy(0, 0, 0, 0);
	const DWORD dwTreeStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS;

	tree.Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP, rectDummy, this, 1200);
	tree.InitializeGrid();
	tree.SetExtendedStyle(0);
	tree.ModifyStyleEx(0, WS_EX_CLIENTEDGE);

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
		TRACE0("未能创建标题栏\n");
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

// CMainFrame 诊断

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


// CMainFrame 消息处理程序

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
		PostMessage(WM_COMMAND, AFX_ID_PREVIEW_CLOSE);  // 强制关闭“打印预览”模式
	}
}

void CMainFrame::OnUpdateFilePrintPreview(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(IsPrintPreview());
}


void CMainFrame::OnOpenSerialport()
{
	// TODO: 在此添加命令处理程序代码
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
		AfxMessageBox(_T("波特率为空"));
		return;
	}
	if (data_pre_comm.GetLength() <= 0) {
		AfxMessageBox(_T("串口名选择为空"));
		return;
	}
	CString data_comm("\\\\.\\");
	data_comm += data_pre_comm;

	if (!p_serialPort->is_open()) {
		
		ret = p_serialPort->openSeialPort(data_comm, _ttoi(data_baudRate));
		if (ret != OK_SERIALPORT) {
			AfxMessageBox(_T("串口打开失败"));
			p_Button_OpenSerialPort->SetText(_T("打开"));
			return;
		}
		p_Button_OpenSerialPort->SetText(_T("关闭"));
		p_Button_OpenSerialPort->SetImageIndex(1, true);
		//开启读串口线程
		hthread_SerialPort = CreateThread(NULL, 0, readPortFunc, &ptrForFrameAppDoc, 0, &threadID);

		if (ptrForFrameAppDoc.wired_flag) {
			Sleep(10);
			p_serialPort->writeSerialPort("N", 1);

			if (0 == SetTimer(1, 20, NULL)) {
				MessageBox(_T("有线定时器初始化错误！"));
				return;
			}
		}


	}
	else
	{
		p_serialPort->closeSerialPort();
		p_Button_OpenSerialPort->SetText(_T("打开"));
		p_Button_OpenSerialPort->SetImageIndex(0, true);

		if (ptrForFrameAppDoc.wired_flag) {
			KillTimer(1);
		}

		return;
	}


	return;
}


// 枚举出系统中可用的串口
void CMainFrame::EnumerateSerialPorts(std::vector<CString>& comm_vector)
{
	
	//确定清除所有数据
	comm_vector.clear();

	for (UINT i = 0;i < 256;i++)
	{
		//格式化设备名称
		CString sPort;
		sPort.Format(_T("\\\\.\\COM%d"), i);

		//尝试打开串口
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
				MessageBox(_T("添加串口名称错误"));
			}
		}
	}
}


void CMainFrame::OnSerialportSelect()
{
	// TODO: 在此添加命令处理程序代码
	//当串口在开启状态时，串口先关闭，在打开新的串口
	if (p_serialPort->is_open()) {
		p_serialPort->closeSerialPort();
		p_Button_OpenSerialPort->SetText(_T("打开"));
		p_Button_OpenSerialPort->SetImageIndex(0, true);
		Sleep(10);
		OnOpenSerialport();
	}

}


void CMainFrame::OnSerialportBaudrate()
{
	// TODO: 在此添加命令处理程序代码
	//当串口在开启状态时，串口先关闭，在以新的波特率打开
	if (p_serialPort->is_open()) {
		p_serialPort->closeSerialPort();
		p_Button_OpenSerialPort->SetText(_T("打开"));
		p_Button_OpenSerialPort->SetImageIndex(0, true);
		Sleep(10);
		OnOpenSerialport();
	}
}


void CMainFrame::OnCalibration()
{
	
	// TODO: 在此添加命令处理程序代码
	if (!p_serialPort->is_open()) {
		MessageBox(_T("串口未打开！"));
		return;
	}

	if (GetCalibrationFlag())
	{
		if (IDNO == MessageBox(_T("否重新校准？"), _T("校准"), MB_YESNO)) {
			return;
		}
	}

	calibration_Length = _ttoi(p_ComboBox_Calibration_Length->GetEditText());
	if (calibration_Length <= 0) {
		MessageBox(_T("错误的校准帧长设置！"));
		return;
	}

	ClearCalibrationFlag();
	SetCalibrationStartFlag();

	//建立校准进度条
	if (p_calibration_Dlg == nullptr) {
		p_calibration_Dlg = new CCalibrationDlg();
		p_calibration_Dlg->Create(IDD_CALIBRATIONDLG, this);
	}
	CRect rcDlg;
	p_calibration_Dlg->GetWindowRect(rcDlg);
	ScreenToClient(rcDlg);
	int screen_x = GetSystemMetrics(SM_CXSCREEN);
	int screen_y = GetSystemMetrics(SM_CYSCREEN);

	//调整对话框出现位置，
	int dlg_Pos_x, dlg_Pos_y;
	if (((float)screen_x) - (((float)screen_x) * 0.7) < (float)rcDlg.Width()){
		dlg_Pos_x = ((float)screen_x - (float)rcDlg.Width());
	}
	else
	{
		dlg_Pos_x = ((float)screen_x) * 0.7;
	}

	if (((float)screen_y) - (((float)screen_y) * 0.7) < (float)rcDlg.Height()) {
		dlg_Pos_y = ((float)screen_y - (float)rcDlg.Height());
	}
	else
	{
		dlg_Pos_y = ((float)screen_y) * 0.7;
	}


	p_calibration_Dlg->SetWindowPos(&CCalibrationDlg::wndTopMost, dlg_Pos_x, dlg_Pos_y, rcDlg.Width(), rcDlg.Height(), SWP_NOSIZE);
	
	p_calibration_Dlg->ShowWindow(SW_SHOW);




	return;
}


void CMainFrame::OnComboCalibrationLength()
{
	// TODO: 在此添加命令处理程序代码
	OnCalibration();
}


void CMainFrame::OnCheckTransmissionSwitch()
{
	// TODO: 在此添加命令处理程序代码
	if (p_serialPort->is_open()) {
		AfxMessageBox(_T("请先关闭串口！"));
		return;
	}
	ptrForFrameAppDoc.wired_flag = !ptrForFrameAppDoc.wired_flag;
}


void CMainFrame::OnUpdateCheckTransmissionSwitch(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->SetCheck(ptrForFrameAppDoc.wired_flag);
}


bool CMainFrame::isCheckBoxTransmission()
{
	return this->p_CheckBox_Transmission_Model_Switch->IsChecked(); 
}


void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	if (1 == nIDEvent) {
		p_serialPort->writeSerialPort("S", 1);
	}

	CFrameWndEx::OnTimer(nIDEvent);
}


void CMainFrame::Send_by_SerialPort(char* buf, int length)
{
	p_serialPort->writeSerialPort(buf, length);
}


BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO: 在此添加专用代码和/或调用基类

//	m_wndSplitter.Create(this, 2, 2, CSize(10, 10), pContext);

	return CFrameWndEx::OnCreateClient(lpcs, pContext);
}


bool CMainFrame::IsAfterMapNodeNumber()
{
	return after_map_node_number;
}


void CMainFrame::SetMapNodeNumberFlag()
{
	after_map_node_number = true;
}


void CMainFrame::ClearMapNodeNumberFalg()
{
	after_map_node_number = false;
}


CMySuperGrid* CMainFrame::getCMySuperGridPtr()
{
	return &m_wndTree;
}


void CMainFrame::OnButtonmapnode()
{
	// TODO: 在此添加命令处理程序代码

	if (!p_serialPort->is_open()) {
		MessageBox(_T("串口未打开！"));
		return;
	}

	if (!IsAfterMapNodeNumber())
	{
		int n = m_wndTree.GetRootCount();
		CItemInfo* temp;
		CStringList* p_list = CSerialDataProc::getStringListPtr();
		CString temp_str, list_str, defult_str(_T("未分配"));
		int nodeNumber, jointNumber;


		for (int i = 0; i < n; i++)
		{
			temp = m_wndTree.GetData(m_wndTree.GetRootItem(i));
			nodeNumber = temp->GetNodeNumber();
			temp_str = temp->GetSubItem(0);
			jointNumber = -1;

			POSITION temp_position = p_list->GetHeadPosition();
			while (temp_position)
			{
				list_str = p_list->GetNext(temp_position);
				if (defult_str == temp_str)
				{
					AfxMessageBox(_T("存在未分配节点！"));
					return;
				}

				jointNumber++;
				if (!temp_position && temp_str != list_str)
				{
					jointNumber = -1;
					break;
				}

				if (temp_str == list_str)
				{
					break;
				}

			}

			//		TRACE(_T("jointNumber:%d\r\n"), jointNumber);
			joint_num_and_node_num_pair[jointNumber] = nodeNumber;
			SetMapNodeNumberFlag();
			p_Button_Map_Node_Number->SetText(_T("重新分配"));
		}

	} 
	else
	{
		POSITION temp_root_positon;
		CItemInfo*lp;
		CMySuperGrid::CTreeItem* pItem;
		ClearMapNodeNumberFalg();
		for (int i = 0; i < 23; i++)
		{
			joint_num_and_node_num_pair[i] = -1;
		}


		temp_root_positon = m_wndTree.GetRootHeadPosition();
		if (temp_root_positon == nullptr)
		{
			AfxMessageBox(_T("不存在节点！"));
			return;
		}

		pItem = m_wndTree.GetNextRoot(temp_root_positon);
		lp = m_wndTree.GetData(pItem);
		lp->SetSubItemText(0, _T("未分配"));
		m_wndTree.UpdateData(pItem, lp, TRUE);
		while (temp_root_positon)
		{
			pItem = m_wndTree.GetNextRoot(temp_root_positon);
			lp = m_wndTree.GetData(pItem);
			lp->SetSubItemText(0, _T("未分配"));
			m_wndTree.UpdateData(pItem, lp, TRUE);
		}
		p_Button_Map_Node_Number->SetText(_T("确定分配"));
	}


}
