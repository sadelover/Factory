/*
* Copyright (c) 2020,�Ϻ��������ܿƼ����޹�˾
* All rights reserved.
* 
* �ļ����ƣ�ComponentEvaluationBarDlg.h
* �ļ���ʶ��
* ժ    Ҫ����ͨ��������á��Ӵ��ڡ������������
*
* ȡ���汾��1.0 
* ԭ����  ��Mark
* ������ڣ�2020��2��25��
*/

#pragma once
#include "afxwin.h"
#include "json/json.h"
#include "afxcolorbutton.h"
#include "../DataPoint/DataPointManager.h"
#include "ComponentDlgBase.h"
// CComponentEvaluationBar �Ի���

class CComponentEvaluationBar : public CComponentDlgBase
{
	DECLARE_DYNAMIC(CComponentEvaluationBar)

public:
	CComponentEvaluationBar(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CComponentEvaluationBar();

// �Ի�������
	enum { IDD = IDD_DIALOG_GENE_COMP_SETTING_EvaluationBar };

	void RefreshControls( Json::Value jsonRoot);
	Json::Value GenerateJson();
	virtual bool IsMe(string & strStyle){return strStyle == "evaluationBar";}
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	afx_msg void OnBnClickedButtonGenerate();
	afx_msg void OnBnClickedCheckBgallow();
	afx_msg void OnBnClickedButtonSeletepoint();
	afx_msg void OnBnClickedButtonClearpoint();

	UINT	CstrRGBToUIntBGR(CString cstrRGB);
	string	UIntBGRToStrRGB(UINT nBGRColor);
	CString UIntBGRToCStrRGB(UINT nBGRColor);

	// ����
	CString m_cstrTitle;
	// �󶨵���
	CString m_cstrBindingPoint;
	// ��ʼֵ
	float m_fStart;
	// ����
	float m_fEnd;
	// �̶�
	CString m_cstrSegments;
	// ������ɫ
	CString m_cstrAreaColor;
	// ��N������
	CString m_cstrArea;
	// ������ɫ
	CMFCColorButton m_colorBTArreaColor;
	// �̶��ߡ����֡��������ɫ
	CMFCColorButton m_colorbtThemesColor;
	// ����ɫ
	CMFCColorButton m_colorbtBGColor;
	// ����ɫ����λ
	BOOL m_bBGclolor;
	//��������ʽ
	int m_nSubType;


	// ��ͷ��ɫ�ؼ�
	CMFCColorButton m_colorbtArrow;
};
