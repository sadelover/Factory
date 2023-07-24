#pragma once
/*
* Copyright (c) 2020,�Ϻ��������ܿƼ����޹�˾
* All rights reserved.
* 
* ժ    Ҫ������׼�豸���á�����
* �ࣺCStandardEquip
*/
#include "afxwin.h"
#include "afxcmn.h"

#include "../ComponentDraw/EqmDrawMapNavigate.h"


#include "../ComponentDraw/pageTemplateInfo.h"


#define SYSTEAM_TEMPLATE  L"template.4db"

class CMainFrame;

// CStandardEquip dialog

class CStandardEquip : public CDialogEx
{
	DECLARE_DYNAMIC(CStandardEquip)

public:
	CStandardEquip(CWnd* pParent = NULL);   // standard constructor
	virtual ~CStandardEquip();


// Dialog Data
	enum { IDD = IDD_DIALOG_STANDARD_EQUIP };
public:
	int					GetLayer(void) const;
	
	void				SetLayer(const int nLayer);
	int                 GetEquipType(void)const;
	void                SetEquipType(const int nEquipType);
	int                 GetEquipNo(void)const;
	void                SetEquipNo(const int nEquipNo);
	void				SetMapPageInfo(const vector<CMapPageInfo>& vecMapPageInfo);
	vector<CMapPageInfo> GetMapPageInfo(void) const;
	int                 GetEquipStyle(void)const;
	void                SetEquipStyle(const int nEquipStyle);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	void	InitData(void);

	int m_nLayer;
	
	vector<GroupPage> m_vecPageGroupNameList;//ģ��ҳ��Ŀ¼
	vector<CPageTemplateInfo> m_vecPageTemplateInfoList;
	vector<CString> m_strParamValueList;
	vector<CMapPageInfo>	m_vecMapPageInfo;

	
	
	void	InitControls(void);
	//CComboBox m_combEquipstyle;
	//void SetShowPicCombination(const int nType��const int nstyle);
	void InsertButtonGroupIntoOpenS3db(const vector<PictureInfo>& vecPicInf, vector<int>& vecId);
	void SetDeviceStruct(const CString arrName, const CString arrPath, vector<PictureInfo>& vecBtn);//
	//CProjectSqlite* m_pProjectSqlite;
	//void LoadPreButtonImage(void);//��ʼ����
	//afx_msg void OnCbnSelchangeEquipType();//�豸����������
	
	
	int m_nTemplatePageId;
	int m_nTempateGroupId;
	CString m_strTemplatePageName;
	CString m_strTemplateParam;
	CEqmDrawMapNavigate * ppMapNavigate;
	bool m_bCreate;
	CString m_cstrTemplateName;
	// ��ֹ��ʾѡ��trueΪ��ֹ
	BOOL m_bBanOnDisplay;
	

private:
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeComboLayer();
	afx_msg void OnBnClickedOk();
	afx_msg void OnCbnSelchangeComboPreCombine();
	afx_msg void OnLvnItemchangedListConfig(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCbnSelchangeComboEquipTemplate();
	afx_msg void OnCbnSelchangeComboEquipTemplateGroup();
	//afx_msg void OnHdnItemdblclickListConfig(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkListConfig(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCbnSelchangeComboTemplatesort();

	void SetListSection(void);
	void InitCombox_TemplateSort();
	void GetInstallTemplateList(vector<CString> &fileList);
	void SelchangeComboEquipTemplate();
	vector<CString> m_listTemplateList;
	CMainFrame* pMainFrame;
	bool m_bFindTemplate;
	CString m_cstrTemplateName4db;

	CComboBox m_comboLayer;
	CComboBox m_combTemplate;//��ҳ��ģ�塱��Ͽ�
	CComboBox m_combTemplateGroup;//��ģ����ࡱ��Ͽ�
	CComboBox m_comboxTemplateSort;// ���л�ģ�塱 ��Ͽ�
	CListCtrl m_listSection;
	
	int m_nTemplateGroupLastChoice;
	int m_nTemplateLastChoice;
	
};
