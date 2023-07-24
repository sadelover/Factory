/*
* Copyright (c) 2020,�Ϻ��������ܿƼ����޹�˾
* All rights reserved.
* 
* �ļ����ƣ�ComponentParamQueryDlg.h
* �ļ���ʶ��
* ժ    Ҫ����ͨ��������á��Ӵ��ڡ��������ٲ�ѯ�����
*
* ȡ���汾��1.0 
* ԭ����  ��Mark
* ������ڣ�2020��2��25��
*/
#pragma once
#include "afxwin.h"
#include "ComponentDlgBase.h"
#include "json/json.h"
#include "gridctrl/GridCtrl.h"
#include "DataPoint/DataPointEntry.h"
#include "PointParamEditDlg.h"
#include "afxcmn.h"
#include "ComponentDlgBase.h"
// CComponentParamQuery �Ի���

class CComponentParamQuery : public CComponentDlgBase
{
	DECLARE_DYNAMIC(CComponentParamQuery)

public:
	CComponentParamQuery(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CComponentParamQuery();

// �Ի�������
	enum { IDD = IDD_DIALOG_GENE_COMP_SETTING_ParamQuery };

	void RefreshControls( Json::Value jsonRoot);
	Json::Value GenerateJson();
	virtual bool IsMe(string & strStyle){return strStyle == "ParamQuery";}
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	DECLARE_MESSAGE_MAP()
	
private:
	/**
	 *	@breif m_treeCtrl�������¼����������£�
	*/
	afx_msg void OnTvnEndlabeleditTree1(NMHDR *pNMHDR, LRESULT *pResult);
	/**
	 *	@breif ���һ������
	*/
	afx_msg void OnBnClickedButtonAddRoot();
	/**
	 *	@breif ���һ������
	*/
	afx_msg void OnBnClickedButtonAddSub();
	/**
	 *	@breif ɾ����
	*/
	afx_msg void OnBnClickedButtonDeleteItem();
	/**
	 *	@breif �����
	*/
	afx_msg void OnBnClickedButtonSeletepoint();

	/**
	 *	@brief ��ȡһ��m_treeCtrl����һ�����ڵڼ���
	 *	@param[in]	m_treeCtrl��ȡ����ı���
	 *	@return		0�������� ,1:��һ�㣬�Դ����ơ�
	*/
	int GetTreeItemLevel(HTREEITEM item);

	//���ؼ�
	CTreeCtrl m_treeCtrl;
	//m_treeCtrl ���б�
	vector<HTREEITEM> m_rootList;
};
