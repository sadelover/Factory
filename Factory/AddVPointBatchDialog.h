#pragma once
#include "afxwin.h"


// CAddVPointBatchDialog �Ի���

class CAddVPointBatchDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CAddVPointBatchDialog)

public:
	CAddVPointBatchDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAddVPointBatchDialog();

// �Ի�������
	enum { IDD = IDD_DIALOG_POINT_ADD_BATCH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:

	CString m_strVpointBatchlist;
};
