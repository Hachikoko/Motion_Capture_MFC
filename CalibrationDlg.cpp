// CalibrationDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Motion_Capture_MFC.h"
#include "CalibrationDlg.h"
#include "afxdialogex.h"


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
END_MESSAGE_MAP()


// CCalibrationDlg 消息处理程序
