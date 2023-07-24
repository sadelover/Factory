#pragma once

#include "afxsock.h"

#define BLOCKSIZE 1024*256         //缓冲区大小


class CDebugToolFileTransfer
{
public:
	CDebugToolFileTransfer();
	CDebugToolFileTransfer(int nPort);
	~CDebugToolFileTransfer(void);

	void ReceiveFile(CSocket &recSo);//文件接收处理
	void SendFile(CSocket &senSo);//文件发送处理

	void StartAsSender(CString strIPConnectTo, int nPortConnectTo);
	void StartAsReceiver();
	bool Exit();

	static DWORD WINAPI _ListenThread(LPVOID lpParameter);
	static DWORD WINAPI _SendThread(LPVOID lpParameter );

	BOOL m_bIsTransmitting;
	BOOL m_bIsStop;
	CString m_strPath;

	HANDLE m_hThreadListen;

	bool m_bExitThread;


	int m_nPort;
	bool m_bRecvS3DB;

	//sender

	CString m_strIPConnectTo;
	int m_nPortConnectTo;
	bool m_bNewFileRecved;
	bool m_bFileSent;
	HANDLE m_hThreadSend;

};

