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

	//��Ҫ���͵��ļ�
	CFile file;
	if(!file.Open(m_strPath, CFile::modeRead | CFile::typeBinary))
	{
		AfxMessageBox(_T("�ļ���ʧ��"));

		senSo.Close();

		return ;
	}


	int nSize = 0, nLen = 0;
	DWORD dwCount = 0;
	char buf[BLOCKSIZE] = {0};
	file.Seek(0, CFile::begin);
	//��ʼ�����ļ�
	for(;;)
	{
		//һ�ζ�ȡBLOCKSIZE��С���ļ�����
		nLen = file.Read(buf, BLOCKSIZE);
		if(nLen == 0)
			break;

		//�����ļ�����
		nSize = senSo.Send(buf, nLen);

		dwCount += nSize;
		CString strTransfersSize;
		strTransfersSize.Format(L"%ld �ֽ�", dwCount);

		if(m_bIsStop)
		{
			m_bIsStop = FALSE;
			break;
		}

		if(nSize == SOCKET_ERROR)
			break;
	}
	//�ر��ļ�
	file.Close();
	//�ر��׽���
	senSo.Close();

	m_bIsTransmitting = FALSE;	
	m_bFileSent = true;
}


void CDebugToolFileTransfer::ReceiveFile(CSocket &recSo)//�ļ����մ���
{
	_tprintf(L"��ʼ�����ļ���װ...\r\n");
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
	//��ʼ�����ļ�
	for(;;)
	{
		//ÿ�ν���BLOCKSIZE��С���ļ�����
		nSize = recSo.Receive(buf, BLOCKSIZE);
		if(nSize == 0)
			break;

		//�����յ����ļ�д���½����ļ���ȥ
		file.Write(buf, nSize);
		dwCount += nSize;

		//�û��Ƿ�Ҫֹͣ����
		if(m_bIsStop)
		{
			m_bIsStop = FALSE;
			break;
		}
	}
	//�ر��ļ�
	file.Close(); 
	//�ر��׽���
	recSo.Close();

	m_bIsTransmitting = FALSE;	
	m_bNewFileRecved = true;

	CString strTemp;
	strTemp.Format(L"��ɽ����ļ���װ(�ļ���С:%.2f MB)\r\n", dwCount/1000.f/1000.f);
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


//���������ļ��߳�
DWORD WINAPI CDebugToolFileTransfer::_ListenThread( LPVOID lpParameter )
{
	CDebugToolFileTransfer *pTransfer = (CDebugToolFileTransfer *) lpParameter;
	//�����׽���
	while(!pTransfer->m_bExitThread)
	{

		CSocket sockSrvr;
		if(!sockSrvr.Create(pTransfer->m_nPort))
		{
			return -1;
		}

		//��ʼ����
		if(!sockSrvr.Listen())
		{
			return -1;
		}

		//��������
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

	//�����ͻ����׽���
	CSocket sockClient;
	if(!sockClient.Create())
	{
		return -1;
	}
	//���������������

	if(!sockClient.Connect(pTransfer->m_strIPConnectTo, pTransfer->m_nPortConnectTo))
	{
		pTransfer->m_bIsTransmitting = false;
	}

	pTransfer->SendFile(sockClient);

	return 0;
}