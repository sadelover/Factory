#include "StdAfx.h"
#include "DebugToolFileTransfer.h"
#include "../Tools/CustomTools/CustomTools.h"
#include "../Tools/Util/UtilsFile.h"


CDebugToolFileTransfer::CDebugToolFileTransfer(int nPort)
	:m_nPort(nPort)
{
	m_bIsTransmitting = FALSE;
	m_bIsStop = FALSE;

	m_hThreadListen = NULL;
	m_hThreadSend = NULL;

	m_bExitThread = false;

	m_bNewFileRecved = false;
	m_bFileSent = false;

	m_bRecvS3DB = true;
}

CDebugToolFileTransfer::CDebugToolFileTransfer()
{
	m_bIsTransmitting = FALSE;
	m_bIsStop = FALSE;

	m_hThreadListen = NULL;
	m_hThreadSend = NULL;

	m_bExitThread = false;

	m_bNewFileRecved = false;
	m_bFileSent = false;
	m_nPort = 3456;

	m_bRecvS3DB = true;
}


CDebugToolFileTransfer::~CDebugToolFileTransfer(void)
{
}



void CDebugToolFileTransfer::SendFile(CSocket &senSo)
{
	m_bIsTransmitting = TRUE;

	//打开要发送的文件
	CFile file;
	if(!file.Open(m_strPath, CFile::modeRead | CFile::typeBinary))
	{
		AfxMessageBox(_T("文件打开失败"));

		senSo.Close();

		return ;
	}


	int nSize = 0, nLen = 0;
	DWORD dwCount = 0;
	char buf[BLOCKSIZE] = {0};
	file.Seek(0, CFile::begin);
	//开始传送文件
	for(;;)
	{
		//一次读取BLOCKSIZE大小的文件内容
		nLen = file.Read(buf, BLOCKSIZE);
		if(nLen == 0)
			break;

		//发送文件内容
		nSize = senSo.Send(buf, nLen);

		dwCount += nSize;
		CString strTransfersSize;
		strTransfersSize.Format(L"%ld 字节", dwCount);

		if(m_bIsStop)
		{
			m_bIsStop = FALSE;
			break;
		}

		if(nSize == SOCKET_ERROR)
			break;
	}
	//关闭文件
	file.Close();
	//关闭套接字
	senSo.Close();

	m_bIsTransmitting = FALSE;	
	m_bFileSent = true;
}


void CDebugToolFileTransfer::ReceiveFile(CSocket &recSo)//文件接收处理
{
	_tprintf(L"开始接收文件下装...\r\n");
	int nSize = 0;
	DWORD dwCount = 0;
	char buf[BLOCKSIZE] = {0};	

	CString strDir;
	Project::Tools::GetSysPath(strDir);
	
	CString strNewFileName;
	if(m_bRecvS3DB)
	{
		COleDateTime tnow = COleDateTime::GetCurrentTime();

		strNewFileName.Format(L"%s\\BEOPDATA%s.s3db",strDir, tnow.Format(_T("%Y%m%d%H%M%S")));

	}
	else
		strNewFileName =m_strPath;

	CFile file(strNewFileName, CFile::modeCreate|CFile::modeWrite);
	//开始接收文件
	for(;;)
	{
		//每次接收BLOCKSIZE大小的文件内容
		nSize = recSo.Receive(buf, BLOCKSIZE);
		if(nSize == 0)
			break;

		//将接收到的文件写到新建的文件中去
		file.Write(buf, nSize);
		dwCount += nSize;

		//用户是否要停止接收
		if(m_bIsStop)
		{
			m_bIsStop = FALSE;
			break;
		}
	}
	//关闭文件
	file.Close(); 
	//关闭套接字
	recSo.Close();

	m_bIsTransmitting = FALSE;	
	m_bNewFileRecved = true;

	CString strTemp;
	strTemp.Format(L"完成接收文件下装(文件大小:%.2f MB)\r\n", dwCount/1000.f/1000.f);
	_tprintf(strTemp.GetString());


}

bool CDebugToolFileTransfer::Exit()
{
	m_bExitThread = true;
	
	::WaitForSingleObject(m_hThreadListen, INFINITE);

	return true;
}

void CDebugToolFileTransfer::StartAsSender(CString strIPConnectTo, int nPortConnectTo)
{
	m_strIPConnectTo = strIPConnectTo;
	m_nPortConnectTo = nPortConnectTo;
	m_hThreadListen = CreateThread(NULL, 0, _SendThread, this, 0, NULL); 
}


void CDebugToolFileTransfer::StartAsReceiver()
{
	m_hThreadSend = CreateThread(NULL, 0, _ListenThread, this, 0, NULL); 
}


//监听接收文件线程
DWORD WINAPI CDebugToolFileTransfer::_ListenThread( LPVOID lpParameter )
{
	CDebugToolFileTransfer *pTransfer = (CDebugToolFileTransfer *) lpParameter;
	//创建套接字
	while(!pTransfer->m_bExitThread)
	{

		CSocket sockSrvr;
		if(!sockSrvr.Create(pTransfer->m_nPort))
		{
			return -1;
		}

		//开始监听
		if(!sockSrvr.Listen())
		{
			return -1;
		}

		//接受连接
		CSocket recSo;
		if(!sockSrvr.Accept(recSo))
		{
			return -1;
		}

		sockSrvr.Close();

		pTransfer->ReceiveFile(recSo);

		Sleep(2000);

	}


	return 0;
}


DWORD WINAPI CDebugToolFileTransfer::_SendThread( LPVOID lpParameter )
{
	CDebugToolFileTransfer *pTransfer = (CDebugToolFileTransfer *) lpParameter;

	//创建客户端套接字
	CSocket sockClient;
	if(!sockClient.Create())
	{
		return -1;
	}
	//与服务器建立连接

	if(!sockClient.Connect(pTransfer->m_strIPConnectTo, pTransfer->m_nPortConnectTo))
	{
		pTransfer->m_bIsTransmitting = false;
	}

	pTransfer->SendFile(sockClient);

	return 0;
}