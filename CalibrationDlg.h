#pragma once


// CCalibrationDlg 对话框

class CCalibrationDlg : public CDialog
{
	DECLARE_DYNAMIC(CCalibrationDlg)

public:
	CCalibrationDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCalibrationDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CALIBRATIONDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
