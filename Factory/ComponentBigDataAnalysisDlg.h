/*
* Copyright (c) 2020,�Ϻ��������ܿƼ����޹�˾
* All rights reserved.
* 
* �ļ����ƣ�ComponentBigDataAnalysisDlg.h
* �ļ���ʶ��
* ժ    Ҫ����ͨ��������á��Ӵ��ڡ�����������Է��������
*
* ȡ���汾��1.0 
* ԭ����  ��Mark
* ������ڣ�2020��  ��  ��
*/
#pragma once
#include "afxwin.h"
#include "ComponentDlgBase.h"
#include "json/json.h"
#include "gridctrl/GridCtrl.h"
#include "DataPoint/DataPointEntry.h"
#include "PointParamEditDlg.h"

// CComponentBigDataAnalysis �Ի���

class CComponentBigDataAnalysis : public CComponentDlgBase
{
	DECLARE_DYNAMIC(CComponentBigDataAnalysis)

public:
	CComponentBigDataAnalysis(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CComponentBigDataAnalysis();

// �Ի�������
	enum { IDD = IDD_DIALOG_GENE_COMP_SETTING_BigDataAnalysis };

	void RefreshControls( Json::Value jsonRoot);
	Json::Value GenerateJson();
	virtual bool IsMe(string & strStyle){return strStyle == "BigDataAnalysis";}
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	
private:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonSetinputline();
	afx_msg void OnBnClickedButtonSetoutputline();
	//���벿��
	CGridCtrl m_gridInput;
	//�������
	CGridCtrl m_gridOutput;
	// ����������
	CString m_cstrBindName;
	// ���������
	int m_nInputRow;
	// ���������
	int m_nOutputRow;
};
