#pragma once
#include "afxwin.h"
#include "ComponentDlgBase.h"

#include "json/json.h"
#include "./Tools/CustomTools/CustomTools.h"
#include "gridctrl/GridCtrl.h"

// ComponentAcumbarDlg �Ի���
//�Զ�������Ӵ��� ������״ͼ���
class ComponentAcumbarDlg : public CComponentDlgBase
{
	DECLARE_DYNAMIC(ComponentAcumbarDlg)

public:
	ComponentAcumbarDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~ComponentAcumbarDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_GENE_COMP_SETTING_accumbar };

	virtual void RefreshControls( Json::Value jsonRoot);
	virtual Json::Value GenerateJson();
	virtual bool IsMe(string & strStyle){return strStyle == "accumbar";}
	

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

	virtual BOOL OnInitDialog();
private:
	afx_msg void OnBnClickedButtonSetGridRow();
	afx_msg void OnBnClickedButtonAddPoint();
	/*
	 *	@brief ���ñ�����������Ʊ�ͷ����
	*/
	void SetGridRowCount(void);
	// ��ʾ����
	int m_radio_ShowStyle;
	// ֵ����-���ֵ
	int m_edit_nMax;
	// ֵ����-��Сֵ
	int m_edit_nMin;
	// ��Ҫ����ʾ���б�
	CGridCtrl m_grid_list;
	//�����еı༭��
	int m_nGridRow;

};
