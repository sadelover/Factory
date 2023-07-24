#pragma once

// CCheckMissImage 对话框
#include "ComponentDraw/ProjectSqlite.h"
#include "afxcmn.h"
#include "gridctrl\gridctrl.h"
class CCheckMissImageDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCheckMissImageDlg)

public:
	CCheckMissImageDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCheckMissImageDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_CHECK_MISS_IMAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnBnClickedButtonExport();
	void CheckMissingImage(void);
	static UINT Thread_Check(LPVOID pParam);
	typedef struct _PageInfo
	{
		_PageInfo(CString csGroupName, int nGroupId, CString csPageName,int nPageId)
			: m_csGroupName(csGroupName)
			, m_nGroupId(nGroupId)
			, m_csPageName(csPageName)
			, m_nPageId(nPageId)
		{

		}
		CString m_csGroupName;
		int m_nGroupId;
		CString m_csPageName;
		int m_nPageId;
	}CheckDlg_PageInfo;

	typedef struct _MissImageDeviceInfo
	{
		CString m_csGroupName;
		CString m_csPageName;
		CString m_csPageID;
		CString m_csDeviceID;//显示图片的那个元素的ID
		CString m_csPictureID;//被显示的图片的ID
		CString m_csBindPictureID;
		_MissImageDeviceInfo(CString csGroupName, CString csPageName, int csPageID, int csDeviceID, int csMissPictureID)
			: m_csGroupName(csGroupName)
			, m_csPageName(csPageName)
			, m_csPageID(L"")
			, m_csDeviceID(L"")
			, m_csPictureID(L"")
			, m_csBindPictureID(L"")
		{
			m_csPageID.Format(L"%d",csPageID);
			m_csDeviceID.Format(L"%d",csDeviceID);
			m_csPictureID.Format(L"%d",csMissPictureID);
			m_csBindPictureID = L"      ";
		}
		_MissImageDeviceInfo(CString csGroupName, CString csPageName, int csPageID, int csDeviceID, int csMissPictureID, int csBindPictureID)
			: m_csGroupName(csGroupName)
			, m_csPageName(csPageName)
			, m_csPageID(L"")
			, m_csDeviceID(L"")
			, m_csPictureID(L"")
			, m_csBindPictureID(L"")
		{
			m_csPageID.Format(L"%d",csPageID);
			m_csDeviceID.Format(L"%d",csDeviceID);
			m_csPictureID.Format(L"%d",csMissPictureID);
			m_csBindPictureID.Format(L"%d",csBindPictureID);
		}
	}MissImageDeviceInfo;
private:
	void InitPageInfo(void);
	void InitDeviceInfo(void);
	void InitPictureList(void);
	void GetPageAndGroupNameForPageId(int nPageId, CString& csPageName, CString& csGroupName);
	bool FindPictureId(int nPicture);
	void InsertItemListCrtl(MissImageDeviceInfo* pItem, int nRow);	
	list<CProjectSqlite::CheckDlg_DeviceInfo> m_deviceInfoList;
	list<CheckDlg_PageInfo> m_pageInfoList;
	list<int> m_pictureList;;
	list<MissImageDeviceInfo> m_MissImageInfoList;//找到的
	//CListCtrl m_listCtrl;
	CGridCtrl m_gridCtrl;
	CMainFrame * m_pMainFrame;
	CLoadingProjectDlg m_progressDlg;
	CProgressCtrl m_progressCtrl;
};

