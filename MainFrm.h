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

// MainFrm.h : CMainFrame ��Ľӿ�
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
	
protected: // �������л�����
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

private:
	//�������
	std::vector<CString> comm_vector;
	CMFCRibbonComboBox * p_ComboBox_SerialportSelect;
	CMFCRibbonComboBox * p_ComboBox_SerialportbaudRate;
	CMFCRibbonButton * p_Button_OpenSerialPort;
	CMotion_Capture_MFCView *p_Motion_Capture_MFCView;

	

	CSerialPort*p_serialPort;    //������ָ��
	HANDLE hthread_SerialPort;   //���ڶ��߳̾��
	DWORD threadID;				 //���ڶ��߳�ID

	//У׼���
	bool Calibration_Flag;
	bool Calibration_Start_Flag;


	CMFCRibbonComboBox * p_ComboBox_Calibration_Length;
	CMFCRibbonButton * p_Button_Calibration_Processing;
	CMFCRibbonCheckBox * p_CheckBox_Transmission_Model_Switch;

	//�Ӵ������
	CSplitterWnd m_wndSplitter;
		

// ����
public:
	char joint_num_and_node_num_pair[23];//��¼�ؽںŶ�Ӧ�Ľڵ�ţ��Թؽں�Ϊ�±꣨�ؽ������ǹ̶��������ڵ�žͲ����޶���23���ڣ�
	int calibration_Length; //��¼У׼֡��
	int calibration_Index;

	CCalibrationDlg* p_calibration_Dlg;
// ����
public:
	int get_JointID_by_NodeID(const int node_id);
	int set_JointID_by_NodeID(const int node_id,const int joint_id);
	int set_joint_and_node_pair_negtive(void);

	//У�����ƺ���
	void SetCalibrationFlag(void);
	void ClearCalibrationFlag(void);
	bool GetCalibrationFlag(void);
	void SetCalibrationStartFlag(void);
	void ClearCalibrationStartFlag(void);
	bool GetCalibrationStartFlag(void);


// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
//	friend DWORD WINAPI readPortFunc(LPVOID lpParameter);
// ʵ��
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // �ؼ���Ƕ���Ա
	CMFCRibbonBar     m_wndRibbonBar;
	CMFCRibbonApplicationButton m_MainButton;
	CMFCToolBarImages m_PanelImages;
	CMFCRibbonStatusBar  m_wndStatusBar;
	COutlookBar       m_wndNavigationBar;
	CMFCShellTreeCtrl m_wndTree;
	CCalendarBar      m_wndCalendar;
	CMFCCaptionBar    m_wndCaptionBar;

// ���ɵ���Ϣӳ�亯��
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
	//����Ҫָ��
	struct PtrForFrameAppDoc ptrForFrameAppDoc;
	
private:
	// ö�ٳ�ϵͳ�п��õĴ���
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




