#pragma once
#include "json/json.h"

// CComponentDlgBase �Ի���
//�Զ����������
class CComponentDlgBase : public CDialogEx
{
	DECLARE_DYNAMIC(CComponentDlgBase)

public:
	CComponentDlgBase(UINT nIDTemplate, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CComponentDlgBase();

	//���ڳ�ʼ������Ľ���
	virtual void RefreshControls( Json::Value jsonRoot){};
	//���ڴ�����л�ȡ��json��������
	virtual Json::Value GenerateJson(){Json::Value jv;return jv;};
	//�����ж����������
	virtual bool IsMe(string & strStyle){return strStyle == "none";}

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	
	DECLARE_MESSAGE_MAP()
};
