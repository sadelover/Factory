#pragma once

#include "PageAreaShow.h"
#include "PageDeviceShow.h"
#include "../ComponentDraw/EqmDrawDefines.h"


class CMainFrame;

// CSheetDeviceArea

class CSheetDeviceArea : public CPropertySheet
{
	DECLARE_DYNAMIC(CSheetDeviceArea)

public:
	CSheetDeviceArea(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CSheetDeviceArea(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CSheetDeviceArea();

public:
	void	UpdateDbData(void);

	vector<TextDeviceInfo> GetDeviceInfo(void) const;
	void	AppendDeviceInfo(const TextDeviceInfo& info);
	void	DeleteDeviceInfo(const int nIndex);
	void	ModifyDeviceInfo(const int nIndex, TextDeviceInfo& info);
	CString	GetDeviceSelectName(void) const;
	void	SetDeviceSelectName(const CString strName);

	vector<TextAreaInfo> GetAreaInfo(void) const;
	void	AppendAreaInfo(const TextAreaInfo& info);
	void	DeleteAreaInfo(const int nIndex);
	void	ModifyAreaInfo(const int nIndex, const TextAreaInfo& info);
	CString	GetAreaSelectName(void) const;
	void	SetAreaSelectName(const CString strName);

	CMainFrame*	m_pMainFrm;

private:
	void			Init(void);
	CPageDeviceShow	m_pageDeviceShow;
	CPageAreaShow	m_pageAreaShow;
	vector<TextDeviceInfo>	m_vecDevInf;
	vector<TextAreaInfo>	m_vecAreaInf;
	CString			m_strDevSelName;
	CString			m_strAreaSelName;

protected:
	DECLARE_MESSAGE_MAP()
};


