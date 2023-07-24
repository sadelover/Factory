#pragma once
#include "Tools/CustomTools/CustomTools.h"
#include "nodaveinclude/nodave.h"

#include <vector>
#include <utility>
#include <string>
using std::vector;
using std::pair;
using std::wstring;

typedef LRESULT (*LPRecDataProc)(const char* buffer,DWORD userdata); 

class  CS7UDPCtrl
{
public:
	CS7UDPCtrl();
	~CS7UDPCtrl(void);

	bool	InitPLCConnection(wstring strPLCIPAddress,int& nSlack,int& nSlot,LPRecDataProc proc1=NULL,DWORD userdata=NULL);
	bool	AutoConnectPLC(wstring strPLCIPAddress,int& nSlack,int& nSlot);
	bool	ExitPLCConnection();
	bool	ReadPLCData(wstring strPLCAddress, double& result);
	bool	WritePLCData(wstring strPLCAddress, double& result);
	VARENUM GetPointDBPos(wstring strPLCAddress, int &nPos, int &nOffset, int &nBit,int &nBlock);
	VARENUM GetPointDBPos_200(wstring strPLCAddress, int &nPos, int &nOffset, int &nBit,int &nBlock);
	VARENUM GetPointPos_200(wstring strPLCAddress,  int &nOffset, int &nBit);			//单首字母
	VARENUM GetPointPos_200_(wstring strPLCAddress,  int &nOffset, int &nBit);			//双首字母
private:
	Project::Tools::Mutex		m_lock;		//用于操作连接
	int							m_nSlack;
	int							m_nSlot;
	
	//PLC Connection by DAVE
	daveInterface *				m_pPLCInterface;
	daveConnection *			m_pPLCConnection;
	bool						m_bPLCConnectionSuccess;		//连接标志位

	LPRecDataProc	m_lpRecDataProc;	//回调
	DWORD			m_dUserData;
};

