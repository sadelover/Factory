/*
* Copyright (c) 2020,�Ϻ��������ܿƼ����޹�˾
* All rights reserved.
* 
* �ļ����ƣ�ComponentReportHistoryDlg.h
* �ļ���ʶ��
* ժ    Ҫ����ͨ��������á��Ӵ��ڡ��������������
*
* ȡ���汾��1.0 
* ԭ����  ��Mark
* ������ڣ�2020��2��25��
*/
#pragma once

#include "afxwin.h"
#include "json/json.h"
#include "ComponentDlgBase.h"
// CComponentReportHistory �Ի���

class CComponentReportHistory : public CComponentDlgBase
{
	DECLARE_DYNAMIC(CComponentReportHistory)

public:
	CComponentReportHistory(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CComponentReportHistory();

// �Ի�������
	enum { IDD = IDD_DIALOG_GENE_COMP_SETTING_ReportHistory };

	void RefreshControls( Json::Value jsonRoot);
	Json::Value GenerateJson();
	virtual bool IsMe(string & strStyle){return strStyle == "reportHistory";}

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
