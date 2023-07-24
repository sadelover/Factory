/*
* Copyright (c) 2020,�Ϻ��������ܿƼ����޹�˾
* All rights reserved.
* 
* �ļ����ƣ�GeneralComponentSettingDlg.h
* �ļ���ʶ��
* ժ    Ҫ����ͨ��������á�����
*
* ��ǰ�汾�� 1.1
* �� �ߣ� Mark
* ������ڣ� 2020��11��11��
*
* ȡ���汾��1.0 
* ԭ����  ��Mark
* ������ڣ�2020��2��25��
*/
#pragma once


#include "afxwin.h"
#include "ComponentEnergyDlg.h"//��Դ������� energy
#include "ComponentParamQueryDlg.h"//���ݲ�ѯ��� ParamQuery
#include "ComponentReportHistoryDlg.h"//�����������
#include "ComponentEchartsDlg.h"//��������ͼ echarts
#include "ComponentBigDataAnalysisDlg.h"//����������Է������ BigDataAnalysis
#include "ComponentEvaluationBarDlg.h"//��������� evaluationBar
#include "ComponentModelControlDlg.h"//ģʽ�����Զ������
#include "ComponentAcumbarDlg.h"//������״ͼ���
#include "ComponenthHeatmapTableDlg.h"//ͨ�ñ����ͼ���

// CGeneralComponentSettingDlg �Ի���

class CGeneralComponentSettingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CGeneralComponentSettingDlg)

public:
	CGeneralComponentSettingDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CGeneralComponentSettingDlg();
// �Ի�������
	enum { IDD = IDD_DIALOG_GENE_COMP_SETTING };

	virtual BOOL OnInitDialog();

	int m_nLayer;//ͼ��
	CString m_strCfgValue;//Json����������ڵ���Ҫ��ڲ���
	//CString m_strCfgValue_Backup;//�������󱸷ݵ�json�ű�
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

private:
	/**
	 *	@breif ComBox�л��¼�������
	*/
	afx_msg void OnCbnSelchangeComboTemplate();
	/**
	 * @breif ���л���ͼ������������
	*/
	afx_msg void OnBnClickedSwitchCodeView();
	/**
	 * @breif ok
	*/
	afx_msg void OnBnClickedOk();

	//�������ͼ����¼�
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	
	/**
	 *	@brief ͨ���ַ���ȥ�ȶԳ�������combox���ĸ�����ָ��Ľ���
	 *	@details �ַ��������ڽ��������json���е�type�ؼ���
	 *	@return	����һ����comboBox�ж�Ӧ������
	*/
	int		GetIndexInComboBoxType(string& strType);

	/**
	 * @brief ѡ����ʾ�Ľ���,��ʼ������
	 * @param[in] nCurSel combo box��������Ӧͨ��combo box�ķ�����ȡ������Ϊ����
	 * @param[in] JsonRoot ��ʼ������ʹ�õ�����
	*/
	void	SwitchShowWindow(int nCurSel, Json::Value JsonRoot);
	/**
	 * @brief ѡ����ʾ�Ľ��棬����ʼ������
	*/
	void	SwitchShowWindow(int nCurSel);
	/**
	 * @brief �������е��Ӵ���
	*/
	void	HideAllChildDlg(void);

	//���������Ŀؼ���ʼ��
	void	InitControls(void);

	/**
	 * @brief �����ʼ��
	 * @details �ڴ˴��������������
	*/
	void	InitComponent(void);
	//�������� ���ڳ�ʼ��combo boxʱʹ�� ��������ɾ��
	typedef enum {
		None = 0,
		energy,
		ParamQuery,
		reportHistory,
		echarts,
		BigDataAnalysis,
		evaluationBar,
		modelControl,
		accumbar,
		heatmap_table
	} eTempalteComboBoxIndex;
	//����Ӵ��������� ����������̳���CComponentDlgBase�࣬
	CComponentEnergy			m_ChildPanel_Energy;//��Դ������� energy
	CComponentParamQuery		m_ChildPanel_ParamQuery;//���ݲ�ѯ��� ParamQuery
	CComponentReportHistory		m_ChildPanel_ReportHistory;//�����������
	CComponentEcharts			m_ChildPanel_Echarts;//��������ͼ echarts
	CComponentBigDataAnalysis	m_ChildPanel_BigDataAnalysis;//����������Է������ BigDataAnalysis
	CComponentEvaluationBar		m_ChildPanel_EvaluationBar;//��������� evaluationBar
	CComponentModelControlDlg	m_ChildPanel_ModeControl;//ģʽ�����Զ������
	ComponentAcumbarDlg			m_ChildPanel_Accumbar;//������״ͼ���
	CComponenthHeatmapTableDlg	m_ChildPanel_HeatmapTable;//ͨ�ñ����ͼ���

	//ͼ��ѡ��ؼ�
	CComboBox m_comboLayer;
	//�������ѡ���
	CComboBox m_comboTempalte;
	//�����Ͽ���һ�ε�ѡ��
	int m_nSelectedTempalte;

//��ȡ�༭����ʾ��״̬
#define IsJsonEditVisible()	GetDlgItem(IDC_EDIT_CFG_VALUE)->IsWindowVisible()
};

