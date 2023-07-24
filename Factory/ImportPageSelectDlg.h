#pragma once

#include "afxcmn.h"
#include "afxwin.h"
#include "../ComponentDraw/EqmDrawProject.h"
#include "../ControlEx/SkinDlg.h"
#include "../ControlEx/CommonButton.h"
#include "DataPoint/DataPointManager.h"
// CImportPageSelectDlg �Ի���

class CProjectSqlite;
class CDataPointManager;
class CEqmDrawPage;
class CImportPageSelectDlg : public CSkinDlg
{
	DECLARE_DYNAMIC(CImportPageSelectDlg)

public:
	CImportPageSelectDlg(bool bTemplate = false,CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CImportPageSelectDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_IMPORT_PAGE };

private:
	CString	m_strSysPath;
	BOOL	m_bSelectAll;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonImportPageSelect();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBtnSelectAll();
	afx_msg LRESULT OnMyMsgInitFinish(WPARAM wParam,LPARAM lParam);

	virtual BOOL OnInitDialog();
	/**
	* @brief: ��ȡͼƬID�Ͳ��뵽4db��
	* @details: ���뵼��ҳ���ͼƬID(nPicID)�����ͼƬ�Ƿ��ڵ���ҳ���е�4db���ڡ�
		����Ƿ���ͬһ�β������Ѿ����룬��������򷵻���ID��
		���赼���4db�в���nPicID�������ͼƬ�Ƿ���ڣ������򷵻ش�ͼƬID������������롣
	*/
	int	GetNewPicIDAndInsertIntoLib(int nMaxPicID, int nPicID);
	int	GetNewAnimationIDAndInsertIntoLib(int nMaxAniID, int nAniID);

	void	EnableControl(bool bResult);
	void	ImportSelectPage();
	void	ImportSelectPages();	//��Դ�Ѽ���
	void	ImportSelectPages_CurrentPageResource();	//ֻ����ѡ��ҳ����Դ
	void	ClearEventInfoPageId(void);

	static UINT WINAPI ThreadFunc_LoadProject(LPVOID lparam);

	bool	ReplacePagePoint(CEqmDrawPage *pPage, CString strOld, CString strNew, CString& strResult);

	wstring Replace(const wstring& orignStr, const wstring& oldStr, const wstring& newStr, bool& bReplaced);
	wstring CreatePointIfNotFind(CProjectSqlite* project_sqlite, wstring strPointName);

	vector<int>	m_vecImportNewPicID;			//���²���
	vector<int> m_vecExistPicID;				//����ID��ͬ ��ͼƬһ��
	vector<int> m_vecExistButNewPicID;			//����ID��ͬ ��ͼƬ��ͬ

	vector<NewPicIDInfo> m_vecNewPicID;
	vector<NewPicIDInfo> m_vecNewAniID;

	int				m_nPointMaxIndex;
	CListCtrl		m_list_page;
	CEdit			m_page_import_path;
	CString			m_strPath; 
	CString			m_strFileName;
	bool			m_bInitImport;
	bool			m_bConfigEsixt;
	bool			m_bImporSelectResourceReady;		//ֻ����һ��ȫ��ͼƬ
	bool			m_bImporSelectResource;
	bool			m_bResourceChanged;					//ͼƬ��Դ�Ѹ�
	int				m_nMaxPicID;
	int				m_nMaxAniID;
	bool			m_bTemplate;						//����ģ��ҳ
	_Import_Setting	m_setting;
	int				m_nID;
	
	//��Ҫ�����ҳ�����ڵ�4db������
	CProjectSqlite*				m_project_sqlite;
	//��Ҫ������Ĺ�����
	CEqmDrawProject				m_project;
	CDataPointManager			m_pointmap;	
	HTREEITEM					m_hItem;
	HANDLE                      m_threadhandle_LoadProject;
	vector<int>					m_vecImportPageID;
	CCommonButton				m_btnOk;
	CCommonButton				m_btnCancel;
	CCommonButton				m_btnReplace;
	CCommonButton				m_btnSelectAll;
};
