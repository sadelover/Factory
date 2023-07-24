// PageReplaceLinkName.cpp : implementation file
//

#include "stdafx.h"
#include "PageReplaceLinkName.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "../ComponentDraw/CEqmDrawPage.h"
#include "../ComponentDraw/EqmDrawDevice.h"
#include "../ComponentDraw/EqmDrawButton.h"


// CPageReplaceLinkName dialog

IMPLEMENT_DYNAMIC(CPageReplaceLinkName, CDialogEx)

CPageReplaceLinkName::CPageReplaceLinkName(CEqmDrawPage *pPage, CWnd* pParent /*=NULL*/)
	: CDialogEx(CPageReplaceLinkName::IDD, pParent)
	, m_strPageName(_T(""))
	, m_strReplaceFrom(_T(""))
	, m_strReplaceTo(_T(""))
	, m_pPage(pPage)
{

}

CPageReplaceLinkName::~CPageReplaceLinkName()
{
}

void CPageReplaceLinkName::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PAGE_NAME, m_strPageName);
	DDX_Text(pDX, IDC_EDIT_REPLACE_FROM, m_strReplaceFrom);
	DDX_Text(pDX, IDC_EDIT_REPLACE_TO, m_strReplaceTo);
}


BEGIN_MESSAGE_MAP(CPageReplaceLinkName, CDialogEx)
	ON_BN_CLICKED(IDOK, &CPageReplaceLinkName::OnBnClickedOk)
END_MESSAGE_MAP()


// CPageReplaceLinkName message handlers


BOOL CPageReplaceLinkName::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	if (m_pPage != NULL)
	{
		CString strPageName;
		m_strPageName.Format(_T("%s"), m_pPage->GetPageName().c_str());
		UpdateData(FALSE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CPageReplaceLinkName::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CMainFrame* pMainFrame = static_cast<CMainFrame*>(AfxGetMainWnd());
	ASSERT(pMainFrame != NULL);
	if (pMainFrame == NULL)
	{
		return;
	}

	UpdateData(TRUE);
	if (m_pPage != NULL && m_strReplaceFrom.GetLength() > 0)
	{
		int nReplaceCount = 0;
		wstring strSourcePointName = m_strReplaceFrom.GetString();
		wstring strReplacePointName = m_strReplaceTo.GetString();
		wstring strNewPointName;
		bool bReplace = false;

		{
			// CEqmDrawDevice
			list<CEqmDrawDevice*>& deviceList = m_pPage->GetDrawDeviceList();
			list<CEqmDrawDevice*>::iterator iter = deviceList.begin();
			while (iter != deviceList.end())
			{
				int nPageId = (*iter)->GetEventInfo().nPageID;
				CEqmDrawPage* pPage = pMainFrame->m_DrawContext.GetProject()->GetPageByID(nPageId);
				ASSERT(pPage != NULL);
				if (pPage != NULL)
				{
					wstring strPageName = pPage->GetPageName();
					wstring strNewName = Replace(strPageName, strSourcePointName, strReplacePointName, bReplace);
					if (bReplace)
					{
						CEqmDrawPage* pNewPage = pMainFrame->m_DrawContext.GetProject()->GetPage(strNewName);
						if (pNewPage != NULL)
						{
							(*iter)->GetEventInfo().nPageID = pNewPage->GetID();
							nReplaceCount++;
						}
					}
				}
				iter++;
			}
		}
		{
			// CEqmDrawButton
			list<CEqmDrawButton*>& buttonList = m_pPage->GetButtonList();
			list<CEqmDrawButton*>::iterator iter = buttonList.begin();
			while (iter != buttonList.end())
			{
				int nPageId = (*iter)->GetLinkPageID();
				CEqmDrawPage* pPage = pMainFrame->m_DrawContext.GetProject()->GetPageByID(nPageId);
				ASSERT(pPage != NULL);
				if (pPage != NULL)
				{
					wstring strPageName = pPage->GetPageName();
					wstring strNewName = Replace(strPageName, strSourcePointName, strReplacePointName, bReplace);
					if (bReplace)
					{
						CEqmDrawPage* pNewPage = pMainFrame->m_DrawContext.GetProject()->GetPage(strNewName);
						if (pNewPage != NULL)
						{
							(*iter)->SetLinkPageID(pNewPage->GetID());
							nReplaceCount++;
						}
					}
				}
				iter++;
			}
		}
		CString strOut;
		strOut.Format(_T("¹²Ìæ»»%d´¦Á´½Ó!"), nReplaceCount);
		MessageBox(strOut);
	}

	CDialogEx::OnOK();
}

std::wstring CPageReplaceLinkName::Replace(const wstring& orignStr, const wstring& oldStr, const wstring& newStr, bool& bReplaced)
{
	size_t pos = 0;
	wstring tempStr = orignStr;
	wstring::size_type newStrLen = newStr.length();
	wstring::size_type oldStrLen = oldStr.length();
	bReplaced = false;
	while (true)
	{
		pos = tempStr.find(oldStr, pos);
		if (pos == wstring::npos) break;

		tempStr.replace(pos, oldStrLen, newStr);        
		pos += newStrLen;
		bReplaced = true;
	}

	return tempStr; 
}
