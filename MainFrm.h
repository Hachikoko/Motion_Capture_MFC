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

// MainFrm.h : CMainFrame 类的接口
//

#pragma once
#include "CalendarBar.h"
#include "Resource.h"
#include "SerialPort.h"
#include <vector>
#include <iostream>
#include "Motion_Capture_MFCView.h"
#include "Motion_Capture_MFCDoc.h"
#include "CalibrationDlg.h"

DWORD WINAPI readPortFunc(LPVOID lpParameter);
class CMainFrame;
struct PtrForFrameAppDoc
{
	CMainFrame*pMainFrame;
	CMotion_Capture_MFCView*pView;
	CMotion_Capture_MFCDoc*pDoc;

	bool wired_flag;
};

class COutlookBar : public CMFCOutlookBar
{
	virtual BOOL AllowShowOnPaneMenu() const { return TRUE; }
	virtual void GetPaneName(CString& strName) const { BOOL bNameValid = strName.LoadString(IDS_OUTLOOKBAR); ASSERT(bNameValid); if (!bNameValid) strName.Empty(); }
};

class CMainFrame : public CFrameWndEx
{
	
protected: // 仅从序列化创建
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

private:
	//串口相关
	std::vector<CString> comm_vector;
	CMFCRibbonComboBox * p_ComboBox_SerialportSelect;
	CMFCRibbonComboBox * p_ComboBox_SerialportbaudRate;
	CMFCRibbonButton * p_Button_OpenSerialPort;
	CMotion_Capture_MFCView *p_Motion_Capture_MFCView;

	

	CSerialPort*p_serialPort;    //串口类指针
	HANDLE hthread_SerialPort;   //串口读线程句柄
	DWORD threadID;				 //串口读线程ID

	//校准相关
	bool Calibration_Flag;
	bool Calibration_Start_Flag;


	CMFCRibbonComboBox * p_ComboBox_Calibration_Length;
	CMFCRibbonButton * p_Button_Calibration_Processing;
	CMFCRibbonCheckBox * p_CheckBox_Transmission_Model_Switch;

	//子窗口相关
	CSplitterWnd m_wndSplitter;
		

// 特性
public:
	char joint_num_and_node_num_pair[23];//记录关节号对应的节点号，以关节号为下标（关节数量是固定，这样节点号就不必限定在23以内）
	int calibration_Length; //记录校准帧长
	int calibration_Index;

	CCalibrationDlg* p_calibration_Dlg;
// 操作
public:
	int get_JointID_by_NodeID(const int node_id);
	int set_JointID_by_NodeID(const int node_id,const int joint_id);
	int set_joint_and_node_pair_negtive(void);

	//校正控制函数
	void SetCalibrationFlag(void);
	void ClearCalibrationFlag(void);
	bool GetCalibrationFlag(void);
	void SetCalibrationStartFlag(void);
	void ClearCalibrationStartFlag(void);
	bool GetCalibrationStartFlag(void);


// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
//	friend DWORD WINAPI readPortFunc(LPVOID lpParameter);
// 实现
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 控件条嵌入成员
	CMFCRibbonBar     m_wndRibbonBar;
	CMFCRibbonApplicationButton m_MainButton;
	CMFCToolBarImages m_PanelImages;
	CMFCRibbonStatusBar  m_wndStatusBar;
	COutlookBar       m_wndNavigationBar;
	CMFCShellTreeCtrl m_wndTree;
	CCalendarBar      m_wndCalendar;
	CMFCCaptionBar    m_wndCaptionBar;

// 生成的消息映射函数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	afx_msg void OnViewCaptionBar();
	afx_msg void OnUpdateViewCaptionBar(CCmdUI* pCmdUI);
	afx_msg void OnOptions();
	afx_msg void OnFilePrint();
	afx_msg void OnFilePrintPreview();
	afx_msg void OnUpdateFilePrintPreview(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()

	BOOL CreateOutlookBar(CMFCOutlookBar& bar, UINT uiID, CMFCShellTreeCtrl& tree, CCalendarBar& calendar, int nInitialWidth);
	BOOL CreateCaptionBar();

	int FindFocusedOutlookWnd(CMFCOutlookBarTabCtrl** ppOutlookWnd);

	CMFCOutlookBarTabCtrl* FindOutlookParent(CWnd* pWnd);
	CMFCOutlookBarTabCtrl* m_pCurrOutlookWnd;
	CMFCOutlookBarPane*    m_pCurrOutlookPage;
public:
	//各主要指针
	struct PtrForFrameAppDoc ptrForFrameAppDoc;
	
private:
	// 枚举出系统中可用的串口
	void EnumerateSerialPorts(std::vector<CString>& comm_vector);
public:
	afx_msg void OnSerialportSelect();
	afx_msg void OnSerialportBaudrate();
	afx_msg void OnOpenSerialport();
	afx_msg void OnCalibration();
	afx_msg void OnComboCalibrationLength();
	afx_msg void OnCheckTransmissionSwitch();
	afx_msg void OnUpdateCheckTransmissionSwitch(CCmdUI *pCmdUI);
	bool isCheckBoxTransmission();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void Send_by_SerialPort(char* buf, int length);
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
};




