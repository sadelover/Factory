/*
* Copyright (c) 2020,�Ϻ��������ܿƼ����޹�˾
* All rights reserved.
* 
* �ļ����ƣ�ComponentModelControlDlg.h
* �ļ���ʶ��
* ժ    Ҫ����ͨ��������á��Ӵ��ڡ�ģʽ�����Զ��������
*
* ȡ���汾��1.0 
* ԭ����  ��Mark
* ������ڣ�2020��2��25��
*/
#pragma once
#include "afxwin.h"
#include "json/json.h"
#include "ComponentDlgBase.h"
// CComponentModelControlDlg �Ի���

class CComponentModelControlDlg : public CComponentDlgBase
{
	DECLARE_DYNAMIC(CComponentModelControlDlg)

public:
	CComponentModelControlDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CComponentModelControlDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_GENE_COMP_SETTING_ModelControl };

	void RefreshControls( Json::Value jsonRoot);
	Json::Value GenerateJson();
	virtual bool IsMe(string & strStyle){return strStyle == "modelControl";}
	CString m_cstrJsonPacket;//���ջ򷵻ص�json��

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	DECLARE_MESSAGE_MAP()

private:
	virtual BOOL OnInitDialog();

	int m_nModelType;// ģʽ����
};
