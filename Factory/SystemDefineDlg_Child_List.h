#pragma once
#include "./json/json.h"
#include "afxwin.h"

// CSystemDefineDlg_Child_List 对话框

class CSystemDefineDlg_Child_List : public CDialogEx
{
	DECLARE_DYNAMIC(CSystemDefineDlg_Child_List)

public:
	CSystemDefineDlg_Child_List(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSystemDefineDlg_Child_List();
	/*
		@breif:初始化界面
		@details:设置m_JsonValue_SysDefine、m_JsonValue_ChillerPlantRoom的参数
		@param:
			rCstr[in] 一个json格式的字符串
		@rtrval:
			-1:json格式错误
			 0:设置初始化成功
			 1:没有ChillerPlantRoom字段
	*/
	int SetData(CString & rCstr);
	/*
		@breif:返回m_JsonValue_SysDefine、m_JsonValue_ChillerPlantRoom的参数组合的json格式字符串
		@param:
			rCstr[out] 一个json格式的字符串
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
// 对话框数据
	enum { IDD = IDD_DIALOG_SYSTEM_DEFINE_List };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	/*
		@breif:设置界面的参数
		@param:
			nItem[in] m_JsonValue_ChillerPlantRoom是一个数组，参数不得超过这个数组的上限
	*/
	void Interface_SetParam(unsigned int nIndex);
	/*
		@breif:换页,重新选择组合框，重新设置界面中的参数
		@param:
			nItem[in] m_JsonValue_ChillerPlantRoom是一个数组，参数不得超过这个数组的上限
	*/
	void Interface_TurnPage(int nIndex);
	/*
		@brief: 清空界面的所有内容，包括下拉菜单中的内容,防止在json界面和配置界面切换时重复添加
	*/
	void ResetAll(void);

	// RoomName列表控件
	CComboBox m_ComboBox_RoomName;
	//存放combo上一次选中的值，防止刷新
	int m_ComboBox_Index;

	//全部
	Json::Value  m_JsonValue_SysDefine;
	//key:ChillerPlantRoom
	Json::Value m_JsonValue_ChillerPlantRoom;

	 // 一次冷冻泵组
	CString m_cstrPriChWPGroupList;
	// 机房名称
	CString m_cstrRoomName;
	// 冷机组
	CString m_cstrChGroupList;
	// 冷却泵组
	CString m_cstrCWPGroupList;
	// 冷却塔组
	CString m_cstrCTGroupList;
	// 冷机容量（kW）
	CString m_cstrChCapacityList;
	// 乙二醇泵组
	CString m_cstrGlycolPumpGroupList;
	// 冷机与乙二醇泵是否为一一对应（1-是；0-否）
	CString m_cstrIsChillerConnectGlycolPumpOneToOne;
	// 冷机与蓄冷板换是否为一一对应（1-是；0-否）
	CString m_cstrIsChillerConnectIceHXOneToOne;
	// 基载主机组
	CString m_cstrBaseChillerList;
	// 蓄冷板换组
	CString m_cstrIceHXGroupList;
	// 不允许一起运行的冷机组合
	CString m_cstrExceptChGroupList;
	// 优先运行的冷机组合
	CString m_cstrPreferChGroupList;
	// 空调面积（m2）
	CString m_cstrACArea;
	// 二次冷冻泵组
	CString m_cstrSecChWPGroupList;
};
