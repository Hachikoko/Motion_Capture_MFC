// SerialPort.cpp : 实现文件
//

#include "stdafx.h"
#include "Motion_Capture_MFC.h"
#include "SerialPort.h"


// CSerialPort
CSerialPort * CSerialPort::GetSerialPortInstance()
{
	// TODO: 在此处插入 return 语句
	 static CSerialPort m_SertialPort;
	 return &m_SertialPort;
}


//************************************
// 方法名称:  openSeialPort
// 方法全名:  CSerialPort::openSeialPort
// 权限:      public 
// 返回值:    int	
// Qualifier:
// 参数:      CString & comm_name
// 参数:      UINT baudRate
// 描述：	  打开一个串口，除串口名与波特率可选，其他一切参数默认
//************************************
int CSerialPort::openSeialPort(const CString & comm_name, UINT baudRate)
{
	
	m_COM_Handle = ::CreateFile(comm_name, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
	if (INVALID_HANDLE_VALUE == m_COM_Handle) {
		AfxMessageBox(_T("打开串口失败：CreateFile"));
		openFlag = false;
		return ERR_CREATEFILE_FIAL;
	}

	//清空异步读写参数
	memset(&(m_overlapped_read), 0, sizeof(OVERLAPPED));
	memset(&(m_overlapped_write), 0, sizeof(OVERLAPPED));

	//设置DCB
	FillMemory(&(m_dcb), sizeof(m_dcb), 0);
	m_dcb.DCBlength = sizeof(DCB);
	GetCommState(m_COM_Handle, &m_dcb);
	m_dcb.BaudRate = baudRate; //波特率为115200
	m_dcb.ByteSize = 8; //每个字节有8位  
	m_dcb.Parity = NOPARITY; //无奇偶校验位  
	m_dcb.StopBits = ONE5STOPBITS; //一个停止位  

	//m_dcb.fBinary = TRUE;
	//m_dcb.fOutxDsrFlow = 0;
	//m_dcb.fDtrControl = DTR_CONTROL_ENABLE;
	//m_dcb.fOutxCtsFlow = 0;
	//m_dcb.fRtsControl = RTS_CONTROL_ENABLE;
	//m_dcb.fInX = m_dcb.fOutX = 1;
	//m_dcb.XonChar = 0X11;
	//m_dcb.XoffChar = 0X13;
	//m_dcb.XonLim = 100;
	//m_dcb.XoffLim = 100;
	//m_dcb.fParity = TRUE;

	SetCommState(m_COM_Handle, &m_dcb);

	//设置超时
	COMMTIMEOUTS timeOuts;
	timeOuts.ReadIntervalTimeout = MAXDWORD;
	timeOuts.ReadTotalTimeoutMultiplier = 0;
	timeOuts.ReadTotalTimeoutConstant = 0;
	//timeOuts.ReadIntervalTimeout = 1;
	//timeOuts.ReadTotalTimeoutMultiplier = 1;
	//timeOuts.ReadTotalTimeoutConstant = 3;
	timeOuts.WriteTotalTimeoutMultiplier = 1;
	timeOuts.WriteTotalTimeoutConstant = 3;
	if (!SetCommTimeouts(m_COM_Handle, &timeOuts)) {
		openFlag = false;
		return  ERR_SETTIMEOUT_FAIL;
	}

	//设置缓冲区
	if (!SetupComm(m_COM_Handle, 1024, 1024)) {
		openFlag = false;
		return ERR_SETBUF_FAIL;
	}

	//清空缓冲区
	if (!PurgeComm(m_COM_Handle, PURGE_RXCLEAR | PURGE_TXCLEAR)) {
		openFlag = false;
		return ERR_CLEARBUF_FAIL;
	}
	openFlag = true;
	return OK_SERIALPORT;
}

int CSerialPort::closeSerialPort(const HANDLE & handle)
{
	CloseHandle(handle);
	openFlag = false;
	return 0;
}

//************************************
// 方法名称:  readSerialPort
// 方法全名:  CSerialPort::readSerialPort
// 权限:      public 
// 返回值:    int
// Qualifier:
// 参数:      CString & str
// 描述：
//************************************
int CSerialPort::readSerialPort(char str[], int& n)
{
	COMSTAT comStat;
	DWORD dwErrorFlags;
	DWORD dwBytesRead = n;
	DWORD dwRealRead = 0;
	BOOL breadState;
	DWORD dwRes;
	int ret = OK_SERIALPORT;

	

	memset(str, 0, n);
	memset(&m_overlapped_read, 0, sizeof(OVERLAPPED));
	m_overlapped_read.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	ClearCommError(m_COM_Handle, &dwErrorFlags, &comStat);
//	dwBytesRead = min(dwBytesRead, comStat.cbInQue);

	//读取数据
	
	breadState = ReadFile(m_COM_Handle, str, dwBytesRead, &dwRealRead, &m_overlapped_read);
	if (!breadState) {
		if (ERROR_IO_PENDING != GetLastError())
		{
			ret = ERR_READ_FAIL;
		}
		else
		{
			dwRes = WaitForSingleObject(m_overlapped_read.hEvent, 1000);
			switch (dwRes)
			{
			case WAIT_OBJECT_0:
				if (!GetOverlappedResult(m_COM_Handle, &m_overlapped_read, &dwRealRead, FALSE)) {
					ret = ERR_READ_FAIL;
				}
				else
				{
					n = dwRealRead;
					ret = OK_SERIALPORT;
				}
				break;
			case WAIT_TIMEOUT:
				ret = ERR_READ_FAIL;
				break;
			default:
				ret = ERR_READ_FAIL;
				break;
			}

		}
	}
	n = dwRealRead;
	PurgeComm(m_COM_Handle, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
	CloseHandle(m_overlapped_read.hEvent);
	return ret;
}

int CSerialPort::writeSerialPort(const char str[], const int n)
{
	COMSTAT comStat;
	DWORD dwErrorFlags;
	DWORD dwrealwrite;
	BOOL bwriteState;
	DWORD dwRes;
	int ret = OK_SERIALPORT;

	memset(&m_overlapped_write, 0, sizeof(OVERLAPPED));
	m_overlapped_write.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	ClearCommError(m_COM_Handle, &dwErrorFlags, &comStat);

	bwriteState = WriteFile(m_COM_Handle, str, n, &dwrealwrite, &m_overlapped_write);
	if (!bwriteState) {
		if (GetLastError() == ERROR_IO_PENDING) {
			dwRes = WaitForSingleObject(m_overlapped_write.hEvent, 1000);
			switch (dwRes)
			{
			case WAIT_OBJECT_0:
				if (!GetOverlappedResult(m_COM_Handle, &m_overlapped_write, &dwrealwrite, FALSE)) {
					ret = ERR_WRITE_FAIL;
				}
				else
				{
					if (dwrealwrite == n) {
						ret = OK_SERIALPORT;
					}
					else
					{
						ret = ERR_WRITE_FAIL;
					}
				}
				break;
			case WAIT_TIMEOUT:
				ret = ERR_WRITE_FAIL;
				break;
			default:
				ret = ERR_WRITE_FAIL;
				break;
			}
		}
	}

	PurgeComm(m_COM_Handle, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
	CloseHandle(m_overlapped_write.hEvent);
	return ret;
}

bool CSerialPort::is_open()
{
	return openFlag;
}


CSerialPort::~CSerialPort()
{
}


// CSerialPort 成员函数
