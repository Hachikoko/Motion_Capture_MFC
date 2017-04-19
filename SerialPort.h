#pragma once

// CSerialPort 命令目标
//#include <CArray>

enum ERR
{
	OK_SERIALPORT = 0,
	ERR_SETTIMEOUT_FAIL = -1,
	ERR_SETBUF_FAIL = -2,
	ERR_CLEARBUF_FAIL = -3,
	ERR_READ_FAIL = -4,
	ERR_WRITE_FAIL = -5,
	ERR_CREATEFILE_FIAL = -6
};


class CSerialPort : public CObject
{
private:
	DCB m_dcb;		//串口配置结构体
	OVERLAPPED m_overlapped_read, m_overlapped_write;	//
	HANDLE m_COM_Handle;
	bool openFlag;

private: 
	CSerialPort() {
		openFlag = false;
	}
	CSerialPort(const CSerialPort &);
	CSerialPort & operator = (const CSerialPort &);

public:


public:
	virtual ~CSerialPort();
	static CSerialPort* GetSerialPortInstance();
	int openSeialPort(const CString& comm_name,UINT baudRate);
	int closeSerialPort(const HANDLE& handle);
	int readSerialPort(char str[],int& n);
	int writeSerialPort(const char str[], const int n);
	bool is_open();
};


