// RectanglePointInfoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "RectanglePointInfoDlg.h"
#include "Tools/CustomTools/CustomTools.h"
#include <algorithm>
#include "MetaFileSelectDlg.h"
#include "CanvasDlg.h"
#include "MainFrm.h"
#include "../ComponentDraw/ProjectSqlite.h"

// CRectanglePointInfoDlg 对话框

IMPLEMENT_DYNAMIC(CRectanglePointInfoDlg, CSkinDlg)


	CRectanglePointInfoDlg::CRectanglePointInfoDlg(CCanvasDlg* pCanvalDlg, CWnd* pParent /*=NULL*/)
	: CSkinDlg(CRectanglePointInfoDlg::IDD, pParent)
	//, m_nValue(0)
	, m_nPicID(0)
	//, m_nFrame(0)
	//, m_nInterval(0)
	, m_pCanvasDlg(pCanvalDlg)
	, m_bAnimationMode(false)
	, m_csPointName(_T(""))
{
	m_pAnimationView = NULL;
	m_pPictureView = NULL;
}

CRectanglePointInfoDlg::~CRectanglePointInfoDlg()
{
	if(m_pAnimationView)
	{
		delete m_pAnimationView;
		m_pAnimationView = NULL;
	}

	if(m_pPictureView)
	{
		delete m_pPictureView;
		m_pPictureView = NULL;
	}
}

void CRectanglePointInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CSkinDlg::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PATH, m_nPicID);
	//DDX_Text(pDX, IDC_EDIT_FRAME, m_nFrame);
	//DDX_Text(pDX, IDC_EDIT_INTERVAL, m_nInterl);
	DDX_Control(pDX, IDOK, m_btnOk);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Text(pDX, IDC_EDIT_POINT, m_csPointName);
}


BEGIN_MESSAGE_MAP(CRectanglePointInfoDlg, CSkinDlg)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_PIC, &CRectanglePointInfoDlg::OnBnClickedButtonPic)
	//ON_BN_CLICKED(IDC_BUTTON_ANI, &CRectanglePointInfoDlg::OnBnClickedButtonAni)
	ON_BN_CLICKED(IDOK, &CRectanglePointInfoDlg::OnBnClickedOk)
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BUTTON_SELECT_POINT, &CRectanglePointInfoDlg::OnBnClickedButtonSelectPoint)
END_MESSAGE_MAP()


// CRectanglePointInfoDlg 消息处理程序


BOOL CRectanglePointInfoDlg::OnInitDialog()
{
	CSkinDlg::OnInitDialog();

	if(m_nFrame == 1)
	{
		m_bAnimationMode = false;
		if(m_pProjectSqlite)
		{
			const size_t size = m_pProjectSqlite->m_vPictureList.size();
			for (size_t i=0;i<size;++i)
			{
				if(m_pProjectSqlite->m_vPictureList[i].ID == m_nPicID)
				{
					m_pPictureView = new PictureView();
					m_pPictureView->pBitmap = m_pProjectSqlite->m_vPictureList[i].pBitmap;
					m_pPictureView->strName = m_pProjectSqlite->m_vPictureList[i].szName;
					m_pPictureView->nID = m_pProjectSqlite->m_vPictureList[i].ID;
					m_pPictureView->etype_equip = m_pProjectSqlite->m_vPictureList[i].etype_equip;
				}
			}
		}
	}
	else if(m_nFrame>1)
	{
		m_bAnimationMode = true;
		if(m_pProjectSqlite)
		{
			const size_t size = m_pProjectSqlite->m_vAnimationList.size();
			for (size_t i=0;i<size;++i)
			{
				if(m_pProjectSqlite->m_vAnimationList[i].ID == m_nPicID)
				{
					m_pAnimationView = new AnimationView();
					const size_t size_ani = m_pProjectSqlite->m_vAnimationList[i].vecPicture.size();
					m_pAnimationView->nID = m_pProjectSqlite->m_vAnimationList[i].ID;
					for (size_t j=0;j<size_ani;++j)
					{
						m_pAnimationView->vec_pBitmap.push_back(m_pProjectSqlite->m_vAnimationList[i].vecPicture[j].pBitmap);
					}
					m_pAnimationView->strName = m_pProjectSqlite->m_vAnimationList[i].szName.c_str();
				}
			}
			if(m_pAnimationView)
			{
				SetTimer(CALC_INDEX_TIMER_ID, CALC_INDEX_INTERVAL, NULL);
				SetTimer(REDRAW_ANIMATION_TIMER_ID,REDRAW_ANIMATION_TIMER_INTERVAL,NULL);
			}
		}
	}
	// TODO:  在此添加额外的初始化
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CRectanglePointInfoDlg::OnBnClickedButtonPic()
{
	CMetaFileSelectDlg dlg(this);
	dlg.SetWndType(e_picture);
	dlg.SetDivideMode(true);
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	dlg.SetCurrentPicID(pFrame->m_nSelectPicID);
	if (dlg.DoModal() == IDOK)
	{
		m_nPicID = dlg.m_nID;
		m_nFrame = dlg.m_nPicNum;

		// 导入图元数据
		int nPicId = 0;
		ASSERT(m_pCanvasDlg != NULL);
		if (m_pCanvasDlg != NULL)
		{
			m_pCanvasDlg->InsertImageIntoOpenS3db(m_nPicID, nPicId);

			// update id
			m_nPicID = nPicId;
			pFrame->m_nSelectPicID = m_nPicID;
		}
		ChangePictureByID();
		Invalidate(TRUE);
		UpdateData(FALSE);
	}
}


//void CRectanglePointInfoDlg::OnBnClickedButtonAni()
//{
//	CMetaFileSelectDlg dlg(this);
//	dlg.SetWndType(e_animation);
//	dlg.SetDivideMode(true);
//	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
//	dlg.SetCurrentPicID(pFrame->m_nSelectPicID);
//	if (dlg.DoModal() == IDOK)
//	{
//		m_nPicID = dlg.m_nID;
//		m_nFrame = dlg.m_nPicNum;
//
//		// 导入动画数据
//		int nPicId = 0;
//		ASSERT(m_pCanvasDlg != NULL);
//		if (m_pCanvasDlg != NULL)
//		{
//			m_pCanvasDlg->InsertAnimationIntoOpenS3db(m_nPicID, nPicId);
//
//			// update id
//			m_nPicID = nPicId;
//			pFrame->m_nSelectPicID = m_nPicID;
//		}
//		ChangePictureByID();
//		UpdateData(FALSE);
//	}
//}

bool CRectanglePointInfoDlg::CompareFunction( const int pElementA,const int pElementB )
{
	return pElementA < pElementB;
}

void CRectanglePointInfoDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	OnOK();
}

void CRectanglePointInfoDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	RECT rc;
	GetClientRect(&rc);
	int nWidth = 150;
	int nHeight = 150;

	Bitmap bmp(int(rc.right),int(rc.bottom));
	Graphics bmpGraphics(&bmp);
	bmpGraphics.SetSmoothingMode(SmoothingModeInvalid);
	bool bNoImage = true;

	if(!m_bAnimationMode)		//图片
	{
		if(m_pPictureView)
		{
			bmpGraphics.DrawImage(m_pPictureView->pBitmap,rc.right-170,rc.bottom -240,nWidth,nHeight);
			bNoImage = false;
		}
	}
	else
	{
		if(m_pAnimationView)
		{		
			bmpGraphics.DrawImage(m_pAnimationView->vec_pBitmap[m_pAnimationView->nIndex],rc.right-170,rc.bottom -240,nWidth,nHeight);
			bNoImage = false;
		}
	}

	if(!bNoImage)
	{
		Gdiplus::Pen pen((ARGB)Color::White,1.0);
		bmpGraphics.DrawRectangle(&pen,rc.right-170,rc.bottom -240,nWidth,nHeight);
	}
	else
	{
		Gdiplus::Pen pen((ARGB)Color::Red,1.0);
		bmpGraphics.DrawRectangle(&pen,rc.right-170,rc.bottom -240,nWidth,nHeight);
	}

	Graphics graphics(dc.m_hDC);
	CachedBitmap cachedBmp(&bmp,&graphics);
	graphics.DrawCachedBitmap(&cachedBmp,0,0);
	graphics.ReleaseHDC(dc.m_hDC);
}

void CRectanglePointInfoDlg::OnTimer( UINT_PTR nIDEvent )
{
	if(nIDEvent == REDRAW_ANIMATION_TIMER_ID)
	{
		RECT rc;
		GetClientRect(&rc);
		int nWidth = 150;
		int nHeight = 150;
		rc.left = rc.right-170;
		rc.right = rc.left + nWidth;
		rc.top = rc.bottom -240;
		rc.bottom = rc.top+nHeight;
		InvalidateRect(&rc,TRUE);
	}
	else if (nIDEvent == CALC_INDEX_TIMER_ID)
	{
		if(m_pAnimationView)
		{
			const size_t size_ani = m_pAnimationView->vec_pBitmap.size();
			if ((int)size_ani-1 > m_pAnimationView->nIndex)
			{
				m_pAnimationView->nIndex++;
			}
			else
			{
				m_pAnimationView->nIndex = 0;
			}
		}
	}
	CDialog::OnTimer(nIDEvent);
}

void CRectanglePointInfoDlg::SetProjectSqlPointer( CProjectSqlite* pProjectSqlite )
{
	m_pProjectSqlite = pProjectSqlite;
}

void CRectanglePointInfoDlg::ChangePictureByID()
{
	KillTimer(CALC_INDEX_TIMER_ID);
	KillTimer(REDRAW_ANIMATION_TIMER_ID);
	if(m_nFrame == 1)
	{
		m_bAnimationMode = false;
		if(m_pPictureView)
		{
			delete m_pPictureView;
			m_pPictureView = NULL;
		}
		if(m_pProjectSqlite)
		{
			const size_t size = m_pProjectSqlite->m_vPictureList.size();
			for (size_t i=0;i<size;++i)
			{
				if(m_pProjectSqlite->m_vPictureList[i].ID == m_nPicID)
				{
					m_pPictureView = new PictureView();
					m_pPictureView->pBitmap = m_pProjectSqlite->m_vPictureList[i].pBitmap;
					m_pPictureView->strName = m_pProjectSqlite->m_vPictureList[i].szName;
					m_pPictureView->nID = m_pProjectSqlite->m_vPictureList[i].ID;
					m_pPictureView->etype_equip = m_pProjectSqlite->m_vPictureList[i].etype_equip;
				}
			}
		}
	}
	else if(m_nFrame>1)
	{
		m_bAnimationMode = true;
		if(m_pAnimationView)
		{
			delete m_pAnimationView;
			m_pAnimationView = NULL;
		}
		if(m_pProjectSqlite)
		{
			const size_t size = m_pProjectSqlite->m_vAnimationList.size();
			for (size_t i=0;i<size;++i)
			{
				if(m_pProjectSqlite->m_vAnimationList[i].ID == m_nPicID)
				{
					m_pAnimationView = new AnimationView();
					const size_t size_ani = m_pProjectSqlite->m_vAnimationList[i].vecPicture.size();
					m_pAnimationView->nID = m_pProjectSqlite->m_vAnimationList[i].ID;
					for (size_t j=0;j<size_ani;++j)
					{
						m_pAnimationView->vec_pBitmap.push_back(m_pProjectSqlite->m_vAnimationList[i].vecPicture[j].pBitmap);
					}
					m_pAnimationView->strName = m_pProjectSqlite->m_vAnimationList[i].szName.c_str();
				}
			}
			if(m_pAnimationView)
			{
				SetTimer(CALC_INDEX_TIMER_ID, CALC_INDEX_INTERVAL, NULL);
				SetTimer(REDRAW_ANIMATION_TIMER_ID,REDRAW_ANIMATION_TIMER_INTERVAL,NULL);
			}
		}
	}
}


BOOL CRectanglePointInfoDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return CSkinDlg::OnEraseBkgnd(pDC);
}

//#include "PointParamEditDlg.h"
#include "MainFrm.h"
#include "DataComConfigDlg.h"
void CRectanglePointInfoDlg::OnBnClickedButtonSelectPoint()
{
	// TODO: 在此添加控件通知处理程序代码
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CDataComConfigDlg dlg(pFrame->m_plcpointmap);
	if (dlg.DoModal() == IDOK)
	{
		m_csPointName = dlg.m_strPointName;
	}
	UpdateData(FALSE);
}
