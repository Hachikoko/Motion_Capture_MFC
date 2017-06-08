// CalibrationDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Motion_Capture_MFC.h"
#include "CalibrationDlg.h"
#include "afxdialogex.h"
#include "MainFrm.h"

extern CMainFrame*pMainFrame;

// CCalibrationDlg �Ի���

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


// CCalibrationDlg ��Ϣ�������




void CCalibrationDlg::OnBnClickedReCalibration()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	pMainFrame->ClearCalibrationFlag();
	pMainFrame->SetCalibrationStartFlag();
	p_BuOK = (CButton*)GetDlgItem(IDC_BUTTON_COMFIRM);
	p_BuOK->EnableWindow(FALSE);
	if (0 == SetTimer(1, 50, nullptr)) {
		MessageBox(_T("��������ʱ����ʼ������"));
	}
}


void CCalibrationDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialog::OnCancel();
}


void CCalibrationDlg::OnBnClickedComfirm()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������l
	pMainFrame->p_calibration_Dlg = nullptr;
	this->DestroyWindow();

}


void CCalibrationDlg::OnNMCustomdrawProgress1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}


BOOL CCalibrationDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	p_BuReCalibration = (CButton*)GetDlgItem(IDC_BUTTON_RECALIBRATION);
	p_BuOK = (CButton*)GetDlgItem(IDC_BUTTON_COMFIRM);
	p_BuCancel = (CButton*)GetDlgItem(IDCANCEL);

	pProgCtrl = (CProgressCtrl*)GetDlgItem(IDC_PROGRESS1);
	pProgCtrl->SetRange(0, pMainFrame->calibration_Length-1);

	

	if (0 == SetTimer(1, 50, nullptr)) {
		MessageBox(_T("��������ʱ����ʼ������"));
	}


	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void CCalibrationDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
