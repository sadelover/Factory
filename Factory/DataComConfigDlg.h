#pragma once

#include "../ControlEx/SkinDlg.h"
#include "../ControlEx/CommonButton.h"
#include "DataPoint/DataPointManager.h"
// CDataComConfigDlg dialog

class CDataComConfigDlg : public CSkinDlg
{
	DECLARE_DYNAMIC(CDataComConfigDlg)

public:
	CDataComConfigDlg(CDataPointManager& pointmap, CWnd* pParent = NULL);   // standard constructor
	virtual ~CDataComConfigDlg();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
	// Dialog Data
	enum { IDD = IDD_DATACOMCONFIGDLG };

	enum E_REFRESH_TYPE{
		e_type = 0,
		e_number,
		e_search, 
		e_last,

		e_refresh_max,
	};

	// Add by Bruce.xu 2011/4/13
	// Get the selected entry list.
	map<int, DataPointEntry> GetSelectedEntryLists();
	// Add by Bruce.xu 2011/4/14
	// Set the selected entry list.
	void SetSelectedEntryLists(const map<int, DataPointEntry>& dataPointMap);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	//virtual BOOL PreTranslateMessage(MSG* pMsg);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedConfirm();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnCbnSelchangeComboDatatype();
	afx_msg void OnCbnSelchangeComboNumber();
	//afx_msg void OnBtnSearch();
	//afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnEnChangeEditSearch();


	afx_msg void OnNMClickListDatapoint(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnItemchangedList(NMHDR *pNMHDR, LRESULT *pResult);  // add by Bruce.xu 2011/4/13 
	//afx_msg void OnNMCustomdrawListDatapoint(NMHDR *pNMHDR, LRESULT *pResult);



private:
	void    ParseSearchString();
	void	InitList();
	void	RefreshList(E_REFRESH_TYPE eType);
	void    UpdateEntryListBySearch();
	UINT    GetItemNumber(const DataPointEntry& entry);
	UINT    GetNumberFromString(const CString& str);

	// Add by Bruce.xu 2011/4/13
	// Insert the selected item to a list.
	void    InsertSelectedEntryToMap(int nSelectedId);
	void	ResetPointList(void);

public:
	CCommonButton           m_btnOK;
	CCommonButton           m_btnCancel;
	CString         m_strSearch;
	CListCtrl       m_pointlist;
	HANDLE          m_hReadOpcEvent;
	CString         m_strPointName;
	int             m_selectedId;
	int				m_selectedStoreType;

	UINT            m_itemNum;

	CDataPointManager&           m_refpointmap;		// we use a temp pointmap file here
	vector<DataPointEntry>   m_entryList;
	vector<DataPointEntry>   m_entryListBySearch;
	vector<DataPointEntry>   m_currentEntryList;  // add by Bruce.xu 2011/4/13

	map<int, DataPointEntry> m_selectedEntryMap;  // add by Bruce.xu 2011/4/13

private :
	bool            m_bCtrlKeyDown;                  // add by Bruce.xu 2011/4/15

protected:
	std::vector<std::wstring>   m_strVecSearch;
public:
	afx_msg void OnBnClickedBtnAddVpoint();
	afx_msg void OnBnClickedBtnSearch();
	afx_msg void OnBnClickedBtnClear();
	//afx_msg void OnBnClickedBtnAddChPoint();
};

