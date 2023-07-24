/*
* Copyright (c) 2020,�Ϻ��������ܿƼ����޹�˾
* All rights reserved.
* 
* �ļ����ƣ�CComponentEchartsDlg.h
* �ļ���ʶ��
* ժ    Ҫ����ͨ��������á��Ӵ��ڡ���������ͼ��
*
* ȡ���汾��1.0 
* ԭ����  ��Mark
* ������ڣ�2020��2��25��
*/
#pragma once
#include "afxwin.h"
#include "json/json.h"
#include "afxcolorbutton.h"
#include "ComponentDlgBase.h"
// CComponentEcharts �Ի���

class CComponentEcharts : public CComponentDlgBase
{
	DECLARE_DYNAMIC(CComponentEcharts)

public:
	CComponentEcharts(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CComponentEcharts();

// �Ի�������
	enum { IDD = IDD_DIALOG_GENE_COMP_SETTING_Echarts };

	void RefreshControls( Json::Value jsonRoot);
	Json::Value GenerateJson();
	virtual bool IsMe(string & strStyle){return strStyle == "echarts";}
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	// �󶨵�
	CString m_strBindPoint;
	// ����Ԥ���߶�
	int m_nTitleRsvdHeight;
	// ���������С
	int m_nTitleFontSize;
	// �������
	CComboBox m_comboxTitleAlignment;
	// ���������ϸ
	CComboBox m_comboxTitleFontWeight;
	// x��̶ȱ�ǩԤ�����
	int m_nXWidth;
	// Y��̶ȱ�ǩԤ�����
	int m_nYWidth;
	// ������ȡֵ��Χ0~500�����óɶ�����0~max��
	int m_nMax;
	// ������ɫ
	CMFCColorButton m_colorbtTitleColor;
	// X��̶ȱ�ǩ��ɫ
	CMFCColorButton m_colorbtXColor;
	// Y��̶ȱ�ǩ��ɫ
	CMFCColorButton m_colorbtYColor;
	// ��������ɫ
	CMFCColorButton m_colorbtGridLineColor;
	// ����������ɫ
	CMFCColorButton m_colorbtGridAreaColor;

	UINT	CstrRGBToUIntBGR(CString cstrRGB);
	string	UIntBGRToStrRGB(UINT nBGRColor);
	CString UIntBGRToCStrRGB(UINT nBGRColor);

	string	GetTitleAlignmentStr(void);
	void	SetTitleAlignmentComBox(string str);
	string	GetTitleFontWeightStr(void);
	void	SetTitleFontWeightComBox(string str);

	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedButtonSeletepoint();
	afx_msg void OnBnClickedButtonClearpoint();
};
