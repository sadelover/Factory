#include "StdAfx.h"
#include "S7UDPCtrl.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "nodaveinclude/nodave.h"
#include <fcntl.h>
#include "nodaveinclude/log2.h"
#include "nodaveinclude/openSocket.h"
#include "Tools/CustomTools/CustomTools.h"
#define maxPBlockLen 0xDe	// real maximum 222 bytes

using namespace Project::Tools;

CS7UDPCtrl::CS7UDPCtrl()
{
	m_pPLCInterface = NULL;
	m_pPLCConnection = NULL;
	m_lpRecDataProc = NULL;
	m_nSlack = -1;
	m_nSlot = -1;
	m_bPLCConnectionSuccess = false;
}


CS7UDPCtrl::~CS7UDPCtrl(void)
{
	ExitPLCConnection();
}
VARENUM CS7UDPCtrl::GetPointDBPos(wstring strPLCAddress, int &nPos, int &nOffset, int &nBit,int &nBlock)
{
	CString strTemp = strPLCAddress.c_str();
	VARENUM varType = VT_INT;
	int nDB = strTemp.Find(L"DB");
	nBlock = daveDB;
	if(nDB>=0)
	{
		//find real
		int nDB2 = strTemp.Find(L",REAL", nDB);
		if(nDB2>nDB)
		{
			CString strDB =  strTemp.Mid(nDB+2, nDB2-nDB-2);
			nPos = _wtoi(strDB.GetString());
			varType = VT_R4;

			CString strOffset = strTemp.Mid(nDB2+5);
			nOffset =  _wtoi(strOffset.GetString());

			return varType;
		}

		//find int
		nDB2 = strTemp.Find(L",INT", nDB);
		if(nDB2>nDB)
		{
			CString strDB =  strTemp.Mid(nDB+2, nDB2-nDB-2);
			nPos = _wtoi(strDB.GetString());
			varType = VT_INT;

			CString strOffset = strTemp.Mid(nDB2+4);
			nOffset =  _wtoi(strOffset.GetString());

			return varType;
		}

		//find BYTE
		nDB2 = strTemp.Find(L",BYTE", nDB);
		if(nDB2>nDB)
		{
			CString strDB =  strTemp.Mid(nDB+2, nDB2-nDB-2);
			nPos = _wtoi(strDB.GetString());
			varType = VT_UI1;

			CString strOffset = strTemp.Mid(nDB2+5);
			nOffset =  _wtoi(strOffset.GetString());

			return varType;
		}

		//find WORD
		nDB2 = strTemp.Find(L",WORD", nDB);
		if(nDB2>nDB)
		{
			CString strDB =  strTemp.Mid(nDB+2, nDB2-nDB-2);
			nPos = _wtoi(strDB.GetString());
			varType = VT_UI2;

			CString strOffset = strTemp.Mid(nDB2+5);
			nOffset =  _wtoi(strOffset.GetString());

			return varType;
		}

		//find DWORD
		nDB2 = strTemp.Find(L",DWORD", nDB);
		if(nDB2>nDB)
		{
			CString strDB =  strTemp.Mid(nDB+2, nDB2-nDB-2);
			nPos = _wtoi(strDB.GetString());
			varType = VT_UI4;

			CString strOffset = strTemp.Mid(nDB2+6);
			nOffset =  _wtoi(strOffset.GetString());

			return varType;
		}

		////find DATE
		//nDB2 = strTemp.Find(L",DATE", nDB);
		//if(nDB2>nDB)
		//{
		//	CString strDB =  strTemp.Mid(nDB+2, nDB2-nDB-2);
		//	nPos = _wtoi(strDB.GetString());
		//	varType = VT_DATE;

		//	CString strOffset = strTemp.Mid(nDB2+5);
		//	nOffset =  _wtoi(strOffset.GetString());

		//	return varType;
		//}

		//find DINT
		nDB2 = strTemp.Find(L",DINT", nDB);
		if(nDB2>nDB)
		{
			CString strDB =  strTemp.Mid(nDB+2, nDB2-nDB-2);
			nPos = _wtoi(strDB.GetString());
			varType = VT_I4;

			CString strOffset = strTemp.Mid(nDB2+5);
			nOffset =  _wtoi(strOffset.GetString());

			return varType;
		}

		////find LREAL
		//nDB2 = strTemp.Find(L",LREAL", nDB);
		//if(nDB2>nDB)
		//{
		//	CString strDB =  strTemp.Mid(nDB+2, nDB2-nDB-2);
		//	nPos = _wtoi(strDB.GetString());
		//	varType = VT_R8;

		//	CString strOffset = strTemp.Mid(nDB2+6);
		//	nOffset =  _wtoi(strOffset.GetString());

		//	return varType;
		//}

		//find SINT
		nDB2 = strTemp.Find(L",SINT", nDB);
		if(nDB2>nDB)
		{
			CString strDB =  strTemp.Mid(nDB+2, nDB2-nDB-2);
			nPos = _wtoi(strDB.GetString());
			varType = VT_I1;

			CString strOffset = strTemp.Mid(nDB2+5);
			nOffset =  _wtoi(strOffset.GetString());

			return varType;
		}

		////find STRING
		//nDB2 = strTemp.Find(L",STRING", nDB);
		//if(nDB2>nDB)
		//{
		//	CString strDB =  strTemp.Mid(nDB+2, nDB2-nDB-2);
		//	nPos = _wtoi(strDB.GetString());
		//	varType = VT_BSTR;

		//	CString strOffset = strTemp.Mid(nDB2+7);
		//	nOffset =  _wtoi(strOffset.GetString());

		//	return varType;
		//}

		////find TIME
		//nDB2 = strTemp.Find(L",TIME", nDB);
		//if(nDB2>nDB)
		//{
		//	CString strDB =  strTemp.Mid(nDB+2, nDB2-nDB-2);
		//	nPos = _wtoi(strDB.GetString());
		//	varType = VT_I4;

		//	CString strOffset = strTemp.Mid(nDB2+5);
		//	nOffset =  _wtoi(strOffset.GetString());

		//	return varType;
		//}

		//find UDINT
		nDB2 = strTemp.Find(L",UDINT", nDB);
		if(nDB2>nDB)
		{
			CString strDB =  strTemp.Mid(nDB+2, nDB2-nDB-2);
			nPos = _wtoi(strDB.GetString());
			varType = VT_UI4;

			CString strOffset = strTemp.Mid(nDB2+6);
			nOffset =  _wtoi(strOffset.GetString());

			return varType;
		}

		//find UINT
		nDB2 = strTemp.Find(L",UINT", nDB);
		if(nDB2>nDB)
		{
			CString strDB =  strTemp.Mid(nDB+2, nDB2-nDB-2);
			nPos = _wtoi(strDB.GetString());
			varType = VT_UINT;

			CString strOffset = strTemp.Mid(nDB2+5);
			nOffset =  _wtoi(strOffset.GetString());

			return varType;
		}

		//find USINT
		nDB2 = strTemp.Find(L",USINT", nDB);
		if(nDB2>nDB)
		{
			CString strDB =  strTemp.Mid(nDB+2, nDB2-nDB-2);
			nPos = _wtoi(strDB.GetString());
			varType = VT_UI1;

			CString strOffset = strTemp.Mid(nDB2+6);
			nOffset =  _wtoi(strOffset.GetString());

			return varType;
		}

		//find bool
		nDB2 = strTemp.Find(L",X", nDB);
		if(nDB2>nDB)
		{
			CString strDB =  strTemp.Mid(nDB+2, nDB2-nDB-2);
			nPos = _wtoi(strDB.GetString());
			varType = VT_BOOL;

			int nDotPos = strTemp.Find(L".", nDB2+1);
			if(nDotPos>nDB2)
			{
				CString strOffset = strTemp.Mid(nDB2+2, nDotPos - nDB2-2);
				nOffset =  _wtoi(strOffset.GetString());

				CString strBit = strTemp.Mid(nDotPos+1);
				nBit =  _wtoi(strBit.GetString());

				return varType;
			}
		}
	}
	else
	{
		return GetPointDBPos_200(strPLCAddress,nPos,nOffset,nBit,nBlock);	
	}

	return varType;
}

bool CS7UDPCtrl::ExitPLCConnection()
{
	Project::Tools::Scoped_Lock<Mutex>	scopelock(m_lock);
	m_bPLCConnectionSuccess = false;
	if(m_pPLCConnection)
		daveDisconnectPLC(m_pPLCConnection);

	if(m_pPLCInterface)
		daveDisconnectAdapter(m_pPLCInterface);

	m_pPLCConnection = NULL;
	m_pPLCInterface = NULL;
	return true;
}

bool CS7UDPCtrl::InitPLCConnection( wstring strPLCIPAddress ,int& nSlack_,int& nSlot_,LPRecDataProc proc1/*=NULL*/,DWORD userdata/*=NULL*/)
{
	m_lpRecDataProc = proc1;
	m_dUserData = userdata;
	for(int nSlack=0; nSlack<=3; ++nSlack)
	{
		for(int nSlot=3; nSlot>=0; --nSlot)
		{
			if(AutoConnectPLC(strPLCIPAddress,nSlack,nSlot))
			{
				nSlack_ = nSlack;
				nSlot_ = nSlot;
				return true;
			}
		}
	}
	return false;
}

bool CS7UDPCtrl::AutoConnectPLC( wstring strPLCIPAddress,int& nSlack,int& nSlot )
{
	int i, initSuccess,useProto, useSlot;
	HANDLE fd;
	unsigned long res;

	_daveOSserialType fds;
	PDU p,p2;

	uc pup[]= {			// Load request
		0x1A,0,1,0,0,0,0,0,9,
		0x5F,0x30,0x42,0x30,0x30,0x30,0x30,0x34,0x50, // block type code and number
		//     _    0    B   0     0    0    0    4    P
		//		SDB		
		0x0D,
		0x31,0x30,0x30,0x30,0x32,0x30,0x38,0x30,0x30,0x30,0x31,0x31,0x30,0	// file length and netto length
		//     1   0     0    0    2    0    8    0    0    0    1    1    0
	};

	uc pablock[]= {	// parameters for parts of a block
		0x1B,0
	};

	uc progBlock[1260]= {
		0,maxPBlockLen,0,0xFB,	// This seems to be a fix prefix for program blocks
	};

	uc paInsert[]= {		// sended after transmission of a complete block,
		// I this makes the CPU link the block into a program
		0x28,0x00,0x00,0x00, 
		0x00,0x00,0x00,0xFD, 
		0x00,0x0A,0x01,0x00, 
		0x30,0x42,0x30,0x30,0x30,0x30,0x34,0x50, // block type code and number	
		0x05,'_','I','N','S','E',
	};

	daveSetDebug(daveDebugPrintErrors);
	useProto=daveProtoISOTCP;
	useSlot=0; //golding

	wchar_t wcChar[1024];

	wsprintf(wcChar,_T("尝试连接到PLC(%s, Slack=%d, Slot =%d)...\n"), strPLCIPAddress.c_str(), nSlack, nSlot);
	if(m_lpRecDataProc)
	{
		m_lpRecDataProc(Project::Tools::WideCharToAnsi(wcChar).c_str(),m_dUserData);
	}

	string strIPAnsi = Project::Tools::WideCharToAnsi(strPLCIPAddress.c_str());
	ExitPLCConnection();

	fds.rfd = (HANDLE)openSocket(102, strIPAnsi.data());

	fds.wfd=fds.rfd;

	if (fds.rfd>0) 
	{ 
		m_pPLCInterface = daveNewInterface(fds,"IF1",0, useProto, daveSpeed187k);
		m_pPLCInterface->timeout=5000000;
		m_pPLCConnection = daveNewConnection(m_pPLCInterface, nSlack, nSlack, nSlot);  // insert your rack and slot here
		initSuccess=0;
		for(i=0;i<=1;i++)
		{
			if (0==daveConnectPLC(m_pPLCConnection)) 
			{
				initSuccess=1;
				break;
			} 
			else  
			{
				daveDisconnectPLC(m_pPLCConnection);
			}
		}
		if (initSuccess) //PLC连接成功
		{ 
			printf("Success!\r\n");	
			if(m_lpRecDataProc)
			{
				m_lpRecDataProc("Success!",m_dUserData);
			}
			m_bPLCConnectionSuccess = true;
		}
		else 
		{
			m_pPLCConnection = NULL;
			daveDisconnectAdapter(m_pPLCInterface);
			m_bPLCConnectionSuccess = false;
			printf("Couldn't connect to PLC\n");
			if(m_lpRecDataProc)
			{
				m_lpRecDataProc("Couldn't connect to PLC!",m_dUserData);
			}
			return false;
		}    

	} 
	else
	{
		_tprintf(_T("错误:无法寻址连接到PLC"));	
		_tprintf(strPLCIPAddress.c_str());
		_tprintf(L"\r\n");
		if(m_lpRecDataProc)
		{
			m_lpRecDataProc("错误:无法寻址连接到PLC",m_dUserData);
		}
		
		return false;
	}
	return true;
}

bool CS7UDPCtrl::ReadPLCData( wstring strPLCAddress, double& result )
{
	Project::Tools::Scoped_Lock<Mutex>	scopelock(m_lock);
	bool bReadSuccess = false;
	if(!m_bPLCConnectionSuccess)
		return bReadSuccess;

	int nDBPos = 0;
	int nOffset = 0;
	int nBit = 0;
	int nBlock = daveDB;
	VARENUM varType = GetPointDBPos(strPLCAddress, nDBPos, nOffset, nBit,nBlock);

	int number, blockCont;
	int nResult;
	int blockNumber,rawLen,netLen;
	char blockType;
	PDU p;

	if(m_pPLCConnection)
		davePrepareReadRequest(m_pPLCConnection, &p);

	if(varType == VT_R4)
	{
		daveAddVarToReadRequest(&p,nBlock, nDBPos,nOffset,4);
	}
	/*else if(varType == VT_R8)
	{
	daveAddVarToReadRequest(&p,daveDB, nDBPos,nOffset,8);
	}*/
	else if(varType == VT_INT)
	{
		daveAddVarToReadRequest(&p,nBlock, nDBPos,nOffset,2);
	}
	else if(varType == VT_BOOL)
	{
		daveAddBitVarToReadRequest(&p, nBlock, nDBPos, nOffset*8+nBit, 1);
	}
	else if(varType == VT_UI1)
	{
		daveAddVarToReadRequest(&p,nBlock, nDBPos,nOffset,1);
	}
	else if(varType == VT_UI2)
	{
		daveAddVarToReadRequest(&p,nBlock, nDBPos,nOffset,2);
	}
	else if(varType == VT_UI4)
	{
		daveAddVarToReadRequest(&p,nBlock, nDBPos,nOffset,4);

	}
	else if(varType == VT_I1)
	{
		daveAddVarToReadRequest(&p,nBlock, nDBPos,nOffset,1);
	}
	else if(varType == VT_I4)
	{
		daveAddVarToReadRequest(&p,nBlock, nDBPos,nOffset,4);
	}
	/*else if(varType == VT_BSTR)
	{
		daveAddVarToReadRequest(&p,daveDB, nDBPos,nOffset,256);
	}*/
	else if(varType == VT_UINT)
	{
		daveAddVarToReadRequest(&p,nBlock, nDBPos,nOffset,2);
	}
	/*else if(varType == VT_DATE)
	{
		daveAddVarToReadRequest(&p,daveDB, nDBPos,nOffset,2);
	}*/

	if(!m_bPLCConnectionSuccess)
		return bReadSuccess;

	daveResultSet rs;
	nResult = daveExecReadRequest(m_pPLCConnection, &p, &rs);
	wchar_t wcChar[1024];

	DATE date;

	if(nResult!=daveResOK)
	{
		wstring wstrErr = AnsiToWideChar(daveStrerror(nResult));
		wsprintf(wcChar, _T("错误 :in SiemensTCP(%s) 读取: %d=%s\r\n"),strPLCAddress.c_str(),nResult, wstrErr.c_str());
		_tprintf(wcChar);
		if(m_lpRecDataProc)
		{
			m_lpRecDataProc(Project::Tools::WideCharToAnsi(wcChar).c_str(),m_dUserData);
		}
		return bReadSuccess;
	}
	if(!m_bPLCConnectionSuccess)
		return bReadSuccess;

	nResult = daveUseResult(m_pPLCConnection, &rs, 0); // first result
	if(nResult!=daveResOK)
	{
		wstring wstrErr = AnsiToWideChar(daveStrerror(nResult));
		wsprintf(wcChar,_T("错误 :in SiemensTCP(%s)查询 : %d=%s\r\n"),strPLCAddress.c_str(),nResult, wstrErr.c_str());
		_tprintf(wcChar);
		if(m_lpRecDataProc)
		{
			m_lpRecDataProc(Project::Tools::WideCharToAnsi(wcChar).c_str(),m_dUserData);
		}
		return bReadSuccess;
	}

	if(varType == VT_R4)
	{
		float fValue = daveGetFloat(m_pPLCConnection);
		result = (double)fValue;
		bReadSuccess = true;
	}
	//else if(varType == VT_R8)
	//{
	//	DWORD d1 = daveGetU32(m_pPLCConnection);
	//	DWORD d2 = daveGetU32(m_pPLCConnection);
	//	ULONGLONG l1 = (d1&0x00000000ffffffff)<<32;
	//	l1 != (d2&0x00000000ffffffff);
	//	VARIANT vt;
	//	vt.ullVal = l1;
	//	result = vt.dblVal;
	//	bReadSuccess = true;

	//	/*float fValue = daveGetFloat(m_pPLCConnection); 
	//	float fValue1 = daveGetFloat(m_pPLCConnection); 
	//	fValue = ((fValue)<<32)&(fValue1);
	//	result = (double)fValue;
	//	bReadSuccess = true;*/
	//}
	else if(varType == VT_INT)
	{
		int nValue = daveGetS16(m_pPLCConnection);
		result = (double)nValue;
		bReadSuccess = true;
	}
	else if(varType == VT_UINT)
	{
		unsigned int nValue = daveGetU16(m_pPLCConnection);
		result = (double)nValue;
		bReadSuccess = true;
	}
	else if(varType == VT_BOOL)
	{
		short  nValue = daveGetU8(m_pPLCConnection);
		result = nValue;
		bReadSuccess = true;
	}
	else if(varType == VT_UI1)
	{
		unsigned short nValue = daveGetU8(m_pPLCConnection);
		result = (double)nValue;
		bReadSuccess = true;
	}
	else if(varType == VT_UI2)
	{
		unsigned int nValue = daveGetU16(m_pPLCConnection);
		result = (double)nValue;
		bReadSuccess = true;
	}
	else if(varType == VT_UI4)
	{
		unsigned long nValue = daveGetU32(m_pPLCConnection);
		result = (double)nValue;
		bReadSuccess = true;
	}
	else if(varType == VT_I1)
	{
		short nValue = daveGetS8(m_pPLCConnection);
		result = (double)nValue;
		bReadSuccess = true;
	}
	else if(varType == VT_I4)
	{
		long nValue = daveGetS32(m_pPLCConnection);
		result = (double)nValue;
		bReadSuccess = true;
	}
	/*else if(varType == VT_BSTR)
	{

	}
	else if(varType == VT_DATE)
	{
		float fValue = daveGetFloat(m_pPLCConnection); 
		VARIANT vt;
		vt.fltVal = fValue;
		date = vt.date;
		COleDateTime odt = COleDateTime(date);
		CString str;
		str = odt.Format(_T("%Y-%m-%d %H:%M:%S"));
		result = (double)fValue;
		bReadSuccess = true;
	}*/

	daveFreeResults(&rs);	

	return true;
}

bool CS7UDPCtrl::WritePLCData( wstring strPLCAddress, double& fValue )
{
	Project::Tools::Scoped_Lock<Mutex>	scopelock(m_lock);
	bool bWriteSuccess = false;
	if(!m_bPLCConnectionSuccess)
		return bWriteSuccess;

	int nDBPos = 0;
	int nOffset = 0;
	int nBitOffset = 0;
	int nBlock = daveDB;
	VARENUM varType = GetPointDBPos(strPLCAddress, nDBPos, nOffset, nBitOffset,nBlock);
	wchar_t wcChar[1024];
	if(varType==VT_R4)
	{
		//write
		float fWriteInto = fValue;
		fWriteInto = toPLCfloat(fWriteInto);
		int nOperResuslt = daveWriteBytes(m_pPLCConnection,nBlock,nDBPos,nOffset,4,&fWriteInto);
		if(nOperResuslt!=0)
		{
			wsprintf(wcChar, _T("错误 :SiemensTCP写入操作: %d=%s\r\n"),nOperResuslt, Project::Tools::AnsiToWideChar(daveStrerror(nOperResuslt)).c_str());
			_tprintf(wcChar);
			if(m_lpRecDataProc)
			{
				m_lpRecDataProc(Project::Tools::WideCharToAnsi(wcChar).c_str(),m_dUserData);
			}
			return bWriteSuccess;
		}		
	}
	//else if(varType==VT_R8)
	//{
	//	//write
	//	float fWriteInto = fValue;
	//	fWriteInto = toPLCfloat(fWriteInto);
	//	int nOperResuslt = daveWriteBytes(m_pPLCConnection,daveDB,nDBPos,nOffset,8,&fWriteInto);
	//	if(nOperResuslt!=0)
	//	{
	//		wsprintf(wcChar, _T("错误 :SiemensTCP写入操作: %d=%s\r\n"), nOperResuslt, Project::Tools::AnsiToWideChar(daveStrerror(nOperResuslt)).c_str());
	//		_tprintf(wcChar);
	//		if(m_lpRecDataProc)
	//		{
	//			m_lpRecDataProc(Project::Tools::WideCharToAnsi(wcChar).c_str(),m_dUserData);
	//		}
	//		return bWriteSuccess;
	//	}
	//}
	else if(varType==VT_INT)
	{
		//write
		short nWriteInto = (int) fValue;
		nWriteInto = daveSwapIed_16(nWriteInto);
		int nOperResuslt = daveWriteBytes(m_pPLCConnection,nBlock,nDBPos,nOffset,2,&nWriteInto);

		if(nOperResuslt!=0)
		{
			wsprintf(wcChar, _T("错误 :SiemensTCP写入操作: %d=%s\r\n"),nOperResuslt, Project::Tools::AnsiToWideChar(daveStrerror(nOperResuslt)).c_str());
			if(m_lpRecDataProc)
			{
				m_lpRecDataProc(Project::Tools::WideCharToAnsi(wcChar).c_str(),m_dUserData);
			}
			return bWriteSuccess;
		}
	}
	else if(varType==VT_UINT)
	{
		//write
		unsigned char buffer[256] = {0};
		davePut16(buffer,(int)fValue);
		int nOperResuslt = daveWriteBytes(m_pPLCConnection, nBlock, nDBPos, nOffset, 2, buffer);
		if(nOperResuslt!=0)
		{
			wsprintf(wcChar, _T("错误 :SiemensTCP写入操作: %d=%s\r\n"),nOperResuslt, Project::Tools::AnsiToWideChar(daveStrerror(nOperResuslt)).c_str());
			_tprintf(wcChar);
			if(m_lpRecDataProc)
			{
				m_lpRecDataProc(Project::Tools::WideCharToAnsi(wcChar).c_str(),m_dUserData);
			}
			return bWriteSuccess;
		}
	}
	else if(varType==VT_BOOL)
	{
		//write
		bool bValue = (((int)fValue) ==1);
		int nOperResuslt = 0;
		short nValue = (short) fValue;
		nOperResuslt = daveWriteBits(m_pPLCConnection, nBlock, nDBPos, nOffset*8+nBitOffset, 1, &nValue);
		if(nOperResuslt!=0)
		{
			wsprintf(wcChar, _T("错误 :SiemensTCP写入操作: %d=%s\r\n"),nOperResuslt, Project::Tools::AnsiToWideChar(daveStrerror(nOperResuslt)).c_str());
			if(m_lpRecDataProc)
			{
				m_lpRecDataProc(Project::Tools::WideCharToAnsi(wcChar).c_str(),m_dUserData);
			}
			return bWriteSuccess;
		}
	}
	else if(varType==VT_I1)
	{
		//write
		unsigned char buffer[256] = {0};
		davePut8(buffer,(int)fValue);
		int nOperResuslt = daveWriteBytes(m_pPLCConnection, nBlock, nDBPos, nOffset, 1, buffer);
		if(nOperResuslt!=0)
		{
			wsprintf(wcChar, _T("错误 :SiemensTCP写入操作: %d=%s\r\n"),nOperResuslt, Project::Tools::AnsiToWideChar(daveStrerror(nOperResuslt)).c_str());
			_tprintf(wcChar);
			if(m_lpRecDataProc)
			{
				m_lpRecDataProc(Project::Tools::WideCharToAnsi(wcChar).c_str(),m_dUserData);
			}
			return bWriteSuccess;
		}
	}
	else if(varType==VT_I4)
	{
		//write
		unsigned char buffer[256] = {0};
		davePut32(buffer,(int)fValue);
		int nOperResuslt = daveWriteBytes(m_pPLCConnection, nBlock, nDBPos, nOffset, 4, buffer);
		if(nOperResuslt!=0)
		{
			wsprintf(wcChar, _T("错误 :SiemensTCP写入操作: %d=%s\r\n"),nOperResuslt, Project::Tools::AnsiToWideChar(daveStrerror(nOperResuslt)).c_str());
			_tprintf(wcChar);
			if(m_lpRecDataProc)
			{
				m_lpRecDataProc(Project::Tools::WideCharToAnsi(wcChar).c_str(),m_dUserData);
			}
			return bWriteSuccess;
		}
	}
	else if(varType==VT_UI1)
	{
		//write
		//WORD nValue = (WORD)fValue;
		unsigned char buffer[256] = {0};
		davePut8(buffer,(int)fValue);
		int nOperResuslt = daveWriteBytes(m_pPLCConnection, nBlock, nDBPos, nOffset, 1, buffer);
		if(nOperResuslt!=0)
		{
			wsprintf(wcChar, _T("错误 :SiemensTCP写入操作: %d=%s\r\n"),nOperResuslt, Project::Tools::AnsiToWideChar(daveStrerror(nOperResuslt)).c_str());
			if(m_lpRecDataProc)
			{
				m_lpRecDataProc(Project::Tools::WideCharToAnsi(wcChar).c_str(),m_dUserData);
			}
			return bWriteSuccess;
		}
	}
	else if(varType==VT_UI2)
	{
		//write
		unsigned char buffer[256] = {0};
		davePut16(buffer,(int)fValue);
		int nOperResuslt = daveWriteBytes(m_pPLCConnection, nBlock, nDBPos, nOffset, 2, buffer);

		if(nOperResuslt!=0)
		{
			wsprintf(wcChar, _T("错误 :SiemensTCP写入操作: %d=%s\r\n"), nOperResuslt, Project::Tools::AnsiToWideChar(daveStrerror(nOperResuslt)).c_str());
			if(m_lpRecDataProc)
			{
				m_lpRecDataProc(Project::Tools::WideCharToAnsi(wcChar).c_str(),m_dUserData);
			}
			return bWriteSuccess;
		}
	}
	else if(varType==VT_UI4)
	{
		//write
		unsigned char buffer[256] = {0};
		davePut32(buffer,(int)fValue);
		int nOperResuslt = daveWriteBytes(m_pPLCConnection, nBlock, nDBPos, nOffset, 4, buffer);
		if(nOperResuslt!=0)
		{
			wsprintf(wcChar, _T("错误 :SiemensTCP写入操作: %d=%s\r\n"), nOperResuslt, Project::Tools::AnsiToWideChar(daveStrerror(nOperResuslt)).c_str());
			if(m_lpRecDataProc)
			{
				m_lpRecDataProc(Project::Tools::WideCharToAnsi(wcChar).c_str(),m_dUserData);
			}
			return bWriteSuccess;
		}
	}
	//else if(varType==VT_DATE)
	//{
	//	//
	//}
	//else if(varType==VT_BSTR)
	//{
	//	//write

	//}
	return true;
}

VARENUM CS7UDPCtrl::GetPointDBPos_200( wstring strPLCAddress, int &nPos, int &nOffset, int &nBit,int &nBlock )
{
	CString strTemp = strPLCAddress.c_str();
	VARENUM varType = VT_INT;
	nBlock = daveDB;
	nBit = 0;
	CString strFirstLetter = strTemp.Mid(0, 1);
	if(strFirstLetter == _T("V"))				//变量存储器
	{
		nBlock = daveDB;
		nPos = 1;
		return GetPointPos_200(strTemp.GetString(),nOffset,nBit);
	}
	else if(strFirstLetter == _T("I") || strFirstLetter == _T("E"))			//输入映像寄存器
	{
		nBlock = daveInputs;
		nPos = 0;
		return GetPointPos_200(strTemp.GetString(),nOffset,nBit);
	}
	else if(strFirstLetter == _T("Q"))			//输出映像寄存器
	{
		nBlock = daveOutputs;
		nPos = 0;
		return GetPointPos_200(strTemp.GetString(),nOffset,nBit);
	}
	else if(strFirstLetter == _T("M") || strFirstLetter == _T("F"))			//内部标志位
	{
		nBlock = daveFlags;
		nPos = 0;
		return GetPointPos_200(strTemp.GetString(),nOffset,nBit);
	}
	else if(strFirstLetter == _T("T"))			//定时器
	{
		nPos = 0;
		nBlock = daveTimer200;
		varType = VT_UI2;
		CString strOffset = strTemp.Mid(1);
		nOffset =  _wtoi(strOffset.GetString());
		return varType;
	}
	else if(strFirstLetter == _T("C") || strFirstLetter == _T("Z"))			//计数器
	{
		nPos = 0;
		nBlock = daveCounter200;
		varType = VT_UI2;
		CString strOffset = strTemp.Mid(1);
		nOffset =  _wtoi(strOffset.GetString());
		return varType;
	}
	else if(strFirstLetter == _T("S"))		//SF(SM)   S
	{
		CString strTwoLetter = strTemp.Mid(0, 2);
		if(strTwoLetter == _T("SF") || strTwoLetter == _T("SM"))
		{
			nBlock = daveSysFlags;
			nPos = 0;
			return GetPointPos_200_(strTemp.GetString(),nOffset,nBit);
		}
		else
		{
			nBlock = daveSysDataS5;
			nPos = 0;
			return GetPointPos_200(strTemp.GetString(),nOffset,nBit);
		}
	}
	else if(strFirstLetter == _T("A"))		//AA(AQ)  AE(AI)
	{
		CString strTwoLetter = strTemp.Mid(0, 2);
		if(strTwoLetter == _T("AA") || strTwoLetter == _T("AQ"))
		{
			nBlock = daveAnaOut;
			nPos = 0;
			return GetPointPos_200_(strTemp.GetString(),nOffset,nBit);
		}
		else if(strTwoLetter == _T("AE") || strTwoLetter == _T("AI"))
		{
			nBlock = daveAnaIn;
			nPos = 0;
			return GetPointPos_200_(strTemp.GetString(),nOffset,nBit);
		}
	}
	else if(strFirstLetter == _T("P"))		//PI 或 PE
	{
		CString strTwoLetter = strTemp.Mid(0, 2);
		if(strTwoLetter == _T("PE") || strTwoLetter == _T("PI"))
		{
			nBlock = daveP;
			nPos = 0;
			return GetPointPos_200_(strTemp.GetString(),nOffset,nBit);
		}
	}
	return varType;
}

VARENUM CS7UDPCtrl::GetPointPos_200( wstring strPLCAddress, int &nOffset, int &nBit )
{
	CString strTemp = strPLCAddress.c_str();
	VARENUM varType = VT_INT;
	CString strTypeLetter = strTemp.Mid(1, 1);
	if(strTypeLetter == _T("B"))			//字节
	{
		varType = VT_UI1;
		CString strOffset = strTemp.Mid(2);
		nOffset =  _wtoi(strOffset.GetString());
		return varType;
	}
	else if(strTypeLetter == _T("W"))		//字
	{
		varType = VT_UI2;
		CString strOffset = strTemp.Mid(2);
		nOffset =  _wtoi(strOffset.GetString());
		return varType;
	}
	else if(strTypeLetter == _T("D"))		//双字
	{
		varType = VT_UI4;
		CString strOffset = strTemp.Mid(2);
		nOffset =  _wtoi(strOffset.GetString());
		return varType;
	}
	else									//位
	{
		varType = VT_BOOL;
		int nDotPos = strTemp.Find(L".",1);
		if(nDotPos>1)
		{
			CString strOffset = strTemp.Mid(1, nDotPos - 1);
			nOffset =  _wtoi(strOffset.GetString());
			CString strBit = strTemp.Mid(nDotPos+1);
			nBit =  _wtoi(strBit.GetString());
			return varType;
		}
	}
	return varType;
}

VARENUM CS7UDPCtrl::GetPointPos_200_( wstring strPLCAddress, int &nOffset, int &nBit )
{
	CString strTemp = strPLCAddress.c_str();
	VARENUM varType = VT_INT;
	CString strTypeLetter = strTemp.Mid(2, 1);
	if(strTypeLetter == _T("B"))			//字节
	{
		varType = VT_UI1;
		CString strOffset = strTemp.Mid(3);
		nOffset =  _wtoi(strOffset.GetString());
		return varType;
	}
	else if(strTypeLetter == _T("W"))		//字
	{
		varType = VT_UI2;
		CString strOffset = strTemp.Mid(3);
		nOffset =  _wtoi(strOffset.GetString());
		return varType;
	}
	else if(strTypeLetter == _T("D"))		//双字
	{
		varType = VT_UI4;
		CString strOffset = strTemp.Mid(3);
		nOffset =  _wtoi(strOffset.GetString());
		return varType;
	}
	else									//位
	{
		varType = VT_BOOL;
		int nDotPos = strTemp.Find(L".",2);
		if(nDotPos>1)
		{
			CString strOffset = strTemp.Mid(2, nDotPos - 2);
			nOffset =  _wtoi(strOffset.GetString());
			CString strBit = strTemp.Mid(nDotPos+1);
			nBit =  _wtoi(strBit.GetString());
			return varType;
		}
	}
	return varType;
}
