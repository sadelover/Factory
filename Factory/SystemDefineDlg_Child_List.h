#pragma once
#include "./json/json.h"
#include "afxwin.h"

// CSystemDefineDlg_Child_List �Ի���

class CSystemDefineDlg_Child_List : public CDialogEx
{
	DECLARE_DYNAMIC(CSystemDefineDlg_Child_List)

public:
	CSystemDefineDlg_Child_List(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSystemDefineDlg_Child_List();
	/*
		@breif:��ʼ������
		@details:����m_JsonValue_SysDefine��m_JsonValue_ChillerPlantRoom�Ĳ���
		@param:
			rCstr[in] һ��json��ʽ���ַ���
		@rtrval:
			-1:json��ʽ����
			 0:���ó�ʼ���ɹ�
			 1:û��ChillerPlantRoom�ֶ�
	*/
	int SetData(CString & rCstr);
	/*
		@breif:����m_JsonValue_SysDefine��m_JsonValue_ChillerPlantRoom�Ĳ�����ϵ�json��ʽ�ַ���
		@param:
			rCstr[out] һ��json��ʽ���ַ���
	*/
	void GetData(CString & rCstr);

	afx_msg void OnCbnSelchangeComboRommnamelist();
	afx_msg void OnBnClickedButtonNewitem();
	afx_msg void OnBnClickedButtonDeleteitem();
	afx_msg void OnEnChangeEditPrichwpgrouplist();
	afx_msg void OnEnChangeEditRoomname();
	afx_msg void OnEnChangeEditChgrouplist();
	afx_msg void OnEnChangeEditCwpgrouplist();
	afx_msg void OnEnChangeEditCtgrouplist();
	afx_msg void OnEnChangeEditChcapacitylist();
	afx_msg void OnEnChangeEditIschillerconnectglycolpumponetoone();
	afx_msg void OnEnChangeEditIschillerconnecticehxonetoone();
	afx_msg void OnEnChangeEditGlycolpumpgrouplist();
	afx_msg void OnEnChangeEditBasechillerlist();
	afx_msg void OnEnChangeEditIcehxgrouplist();
	afx_msg void OnEnChangeEditExceptchgrouplist();
	afx_msg void OnEnChangeEditPreferchgrouplist();
	afx_msg void OnEnChangeEditAcarea();
	afx_msg void OnEnSecChWPGroupList();
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
// �Ի�������
	enum { IDD = IDD_DIALOG_SYSTEM_DEFINE_List };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	/*
		@breif:���ý���Ĳ���
		@param:
			nItem[in] m_JsonValue_ChillerPlantRoom��һ�����飬�������ó���������������
	*/
	void Interface_SetParam(unsigned int nIndex);
	/*
		@breif:��ҳ,����ѡ����Ͽ��������ý����еĲ���
		@param:
			nItem[in] m_JsonValue_ChillerPlantRoom��һ�����飬�������ó���������������
	*/
	void Interface_TurnPage(int nIndex);
	/*
		@brief: ��ս�����������ݣ����������˵��е�����,��ֹ��json��������ý����л�ʱ�ظ����
	*/
	void ResetAll(void);

	// RoomName�б�ؼ�
	CComboBox m_ComboBox_RoomName;
	//���combo��һ��ѡ�е�ֵ����ֹˢ��
	int m_ComboBox_Index;

	//ȫ��
	Json::Value  m_JsonValue_SysDefine;
	//key:ChillerPlantRoom
	Json::Value m_JsonValue_ChillerPlantRoom;

	 // һ���䶳����
	CString m_cstrPriChWPGroupList;
	// ��������
	CString m_cstrRoomName;
	// �����
	CString m_cstrChGroupList;
	// ��ȴ����
	CString m_cstrCWPGroupList;
	// ��ȴ����
	CString m_cstrCTGroupList;
	// ���������kW��
	CString m_cstrChCapacityList;
	// �Ҷ�������
	CString m_cstrGlycolPumpGroupList;
	// ������Ҷ������Ƿ�Ϊһһ��Ӧ��1-�ǣ�0-��
	CString m_cstrIsChillerConnectGlycolPumpOneToOne;
	// ���������廻�Ƿ�Ϊһһ��Ӧ��1-�ǣ�0-��
	CString m_cstrIsChillerConnectIceHXOneToOne;
	// ����������
	CString m_cstrBaseChillerList;
	// ����廻��
	CString m_cstrIceHXGroupList;
	// ������һ�����е�������
	CString m_cstrExceptChGroupList;
	// �������е�������
	CString m_cstrPreferChGroupList;
	// �յ������m2��
	CString m_cstrACArea;
	// �����䶳����
	CString m_cstrSecChWPGroupList;
};
