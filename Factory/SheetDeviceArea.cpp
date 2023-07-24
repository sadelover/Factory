// DeviceAreaSheet.cpp : implementation file
//

#include "stdafx.h"
#include "SheetDeviceArea.h"
#include "MainFrm.h"


// CSheetDeviceArea

IMPLEMENT_DYNAMIC(CSheetDeviceArea, CPropertySheet)

CSheetDeviceArea::CSheetDeviceArea(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	: CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
	, m_pMainFrm(NULL)
{
	Init();
}

CSheetDeviceArea::CSheetDeviceArea(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	: CPropertySheet(pszCaption, pParentWnd, iSelectPage)
	, m_pMainFrm(NULL)
{
	Init();
}

CSheetDeviceArea::~CSheetDeviceArea()
{
}


BEGIN_MESSAGE_MAP(CSheetDeviceArea, CPropertySheet)
END_MESSAGE_MAP()


// CSheetDeviceArea message handlers

void CSheetDeviceArea::Init(void)
{
	AddPage(&m_pageAreaShow);
	AddPage(&m_pageDeviceShow);

	m_pMainFrm = static_cast<CMainFrame*>(AfxGetMainWnd());
	ASSERT(m_pMainFrm != NULL);
	if (m_pMainFrm != NULL)
	{
		if (m_pMainFrm->m_pPrjSqlite != NULL)
		{
			m_vecDevInf = m_pMainFrm->m_pPrjSqlite->m_vecTextDevInf;
			m_vecAreaInf = m_pMainFrm->m_pPrjSqlite->m_vecTextAreaInf;
		}
	}
}

void CSheetDeviceArea::UpdateDbData(void)
{
	if (m_pMainFrm != NULL)
	{
		if (m_pMainFrm->m_pPrjSqlite != NULL)
		{
			m_pMainFrm->m_pPrjSqlite->m_vecTextDevInf = m_vecDevInf;
			m_pMainFrm->m_pPrjSqlite->m_vecTextAreaInf = m_vecAreaInf;
		}
	}
}

vector<TextDeviceInfo> CSheetDeviceArea::GetDeviceInfo(void) const
{
	return m_vecDevInf;
}

void CSheetDeviceArea::AppendDeviceInfo(const TextDeviceInfo& info)
{
	m_vecDevInf.push_back(info);
}

void CSheetDeviceArea::DeleteDeviceInfo(const int nIndex)
{
	vector<TextDeviceInfo>::const_iterator iter;
	int i = 0;
	for (iter=m_vecDevInf.begin(); iter!=m_vecDevInf.end(); iter++,i++)
	{
		if (nIndex == i)
		{
			m_vecDevInf.erase(iter);
			break;
		}
	}
}

void CSheetDeviceArea::ModifyDeviceInfo(const int nIndex, TextDeviceInfo& info)
{
	vector<TextDeviceInfo>::iterator iter;
	int i = 0;
	for (iter=m_vecDevInf.begin(); iter!=m_vecDevInf.end(); iter++,i++)
	{
		if (nIndex == i)
		{
			iter->strRegion = info.strRegion;
			iter->nBindPageId = info.nBindPageId;
			iter->strBindPageName = info.strBindPageName;
			break;
		}
	}
}

CString CSheetDeviceArea::GetDeviceSelectName(void) const
{
	return m_strDevSelName;
}

void CSheetDeviceArea::SetDeviceSelectName(const CString strName)
{
	m_strDevSelName = strName;
}

vector<TextAreaInfo> CSheetDeviceArea::GetAreaInfo(void) const
{
	return m_vecAreaInf;
}

void CSheetDeviceArea::AppendAreaInfo(const TextAreaInfo& info)
{
	m_vecAreaInf.push_back(info);
}

void CSheetDeviceArea::DeleteAreaInfo(const int nIndex)
{
	vector<TextAreaInfo>::const_iterator iter;
	int i = 0;
	for (iter=m_vecAreaInf.begin(); iter!=m_vecAreaInf.end(); iter++,i++)
	{
		if (nIndex == i)
		{
			m_vecAreaInf.erase(iter);
			break;
		}
	}
}

void CSheetDeviceArea::ModifyAreaInfo(const int nIndex, const TextAreaInfo& info)
{
	vector<TextAreaInfo>::iterator iter;
	int i = 0;
	for (iter=m_vecAreaInf.begin(); iter!=m_vecAreaInf.end(); iter++,i++)
	{
		if (nIndex == i)
		{
			iter->nPageId = info.nPageId;
			iter->strPageName = info.strPageName;
			iter->strRegion = info.strRegion;
			break;
		}
	}
}

CString CSheetDeviceArea::GetAreaSelectName(void) const
{
	return m_strAreaSelName;
}

void CSheetDeviceArea::SetAreaSelectName(const CString strName)
{
	m_strAreaSelName = strName;
}
