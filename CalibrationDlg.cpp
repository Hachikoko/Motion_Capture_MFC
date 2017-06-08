// CalibrationDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Motion_Capture_MFC.h"
#include "CalibrationDlg.h"
#include "afxdialogex.h"
#include "MainFrm.h"

extern CMainFrame*pMainFrame;

// CCalibrationDlg 对话框

IMPLEMENT_DYNAMIC(CCalibrationDlg, CDialog)

CCalibrationDlg::CCalibrationDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_CALIBRATIONDLG, pParent)
{

}

CCalibrationDlg::~CCalibrationDlg()
{
}

void CCalibrationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCalibrationDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_RECALIBRATION, &CCalibrationDlg::OnBnClickedReCalibration)
	ON_BN_CLICKED(IDCANCEL, &CCalibrationDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_COMFIRM, &CCalibrationDlg::OnBnClickedComfirm)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_PROGRESS1, &CCalibrationDlg::OnNMCustomdrawProgress1)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CCalibrationDlg 消息处理程序




void CCalibrationDlg::OnBnClickedReCalibration()
{
	// TODO: 在此添加控件通知处理程序代码
	pMainFrame->ClearCalibrationFlag();
	pMainFrame->SetCalibrationStartFlag();
	p_BuOK = (CButton*)GetDlgItem(IDC_BUTTON_COMFIRM);
	p_BuOK->EnableWindow(FALSE);
	if (0 == SetTimer(1, 50, nullptr)) {
		MessageBox(_T("进度条定时器初始化错误！"));
	}
}


void CCalibrationDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialog::OnCancel();
}


void CCalibrationDlg::OnBnClickedComfirm()
{
	// TODO: 在此添加控件通知处理程序代码l
	pMainFrame->p_calibration_Dlg = nullptr;
	this->DestroyWindow();

}


void CCalibrationDlg::OnNMCustomdrawProgress1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}


BOOL CCalibrationDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	p_BuReCalibration = (CButton*)GetDlgItem(IDC_BUTTON_RECALIBRATION);
	p_BuOK = (CButton*)GetDlgItem(IDC_BUTTON_COMFIRM);
	p_BuCancel = (CButton*)GetDlgItem(IDCANCEL);

	pProgCtrl = (CProgressCtrl*)GetDlgItem(IDC_PROGRESS1);
	pProgCtrl->SetRange(0, pMainFrame->calibration_Length-1);

	

	if (0 == SetTimer(1, 50, nullptr)) {
		MessageBox(_T("进度条定时器初始化错误！"));
	}


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CCalibrationDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (1 == nIDEvent)
	{
		pProgCtrl = (CProgressCtrl*)GetDlgItem(IDC_PROGRESS1);
		pProgCtrl->SetPos(pMainFrame->calibration_Index);

		if (pMainFrame->calibration_Index == pMainFrame->calibration_Length - 1) {
			KillTimer(nIDEvent);
			p_BuOK = (CButton*)GetDlgItem(IDC_BUTTON_COMFIRM);
			p_BuOK->EnableWindow(TRUE);

		}
	}

	

	CDialog::OnTimer(nIDEvent);
}
