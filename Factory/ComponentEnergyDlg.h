/*
* Copyright (c) 2020,�Ϻ��������ܿƼ����޹�˾
* All rights reserved.
* 
* �ļ����ƣ�ComponentEnergyDlg.h
* �ļ���ʶ��
* ժ    Ҫ����ͨ��������á��Ӵ��ڡ���Դ����
*
* ȡ���汾��1.0 
* ԭ����  ��Mark
* ������ڣ�2020��2��25��
*/
#pragma once

#include "afxwin.h"
#include "json/json.h"
#include "ComponentDlgBase.h"
// CComponentEnergy �Ի���

class CComponentEnergy : public CComponentDlgBase
{
	DECLARE_DYNAMIC(CComponentEnergy)

public:
	CComponentEnergy(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CComponentEnergy();

// �Ի�������
	enum { IDD = IDD_DIALOG_GENE_COMP_SETTING_Energy };

	void RefreshControls( Json::Value jsonRoot);
	Json::Value GenerateJson();
	virtual bool IsMe(string & strStyle){return strStyle == "energy";}
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

private:
};
