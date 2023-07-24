// ImagePreviewDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ImagePreviewDlg.h"
#include "afxdialogex.h"
const int g_nMaxSize = 300;

// CImagePreviewDlg dialog

IMPLEMENT_DYNAMIC(CImagePreviewDlg, CDialog)

CImagePreviewDlg::CImagePreviewDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CImagePreviewDlg::IDD, pParent)
	, m_pBitmap(NULL)
{

}

CImagePreviewDlg::~CImagePreviewDlg()
{
}

void CImagePreviewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CImagePreviewDlg, CDialog)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

bool CImagePreviewDlg::ShowPreviewImage( Image* pBitmap, CPoint point )
{
	if(pBitmap)
	{
		//移动对话框
		/*int nWidth = 0;
		int nHeight = 0;
		nWidth = pBitmap->GetWidth();
		nHeight = pBitmap->GetHeight();
	
		CDC* pDC = GetWindowDC();
		Bitmap bmp(nWidth,nHeight);
		Graphics bmpGraphics(&bmp);
		bmpGraphics.SetSmoothingMode(SmoothingModeInvalid);

		bmpGraphics.DrawImage(pBitmap,10,30,nWidth,nHeight);

		Graphics graphics(pDC->m_hDC);
		CachedBitmap cachedBmp(&bmp,&graphics);
		graphics.DrawCachedBitmap(&cachedBmp,0,0);
		graphics.ReleaseHDC(pDC->m_hDC);*/

		m_pBitmap = pBitmap;

		return true;
	}
	return false;
}

BOOL CImagePreviewDlg::OnEraseBkgnd( CDC* pDC )
{
	if(m_pBitmap)
	{
		//移动对话框
		int nWidth = 0;
		int nHeight = 0;
		nWidth = (m_pBitmap->GetWidth()>g_nMaxSize)?g_nMaxSize:m_pBitmap->GetWidth();
		nHeight = (m_pBitmap->GetHeight()>g_nMaxSize)?g_nMaxSize:m_pBitmap->GetHeight();

		CRect rect;  
		GetClientRect(rect);  
		DWORD backColor = RGB(255, 255, 255);
		pDC->FillSolidRect(rect,backColor);

		Bitmap bmp(nWidth,nHeight);
		Graphics bmpGraphics(&bmp);
		bmpGraphics.SetSmoothingMode(SmoothingModeInvalid);

		bmpGraphics.DrawImage(m_pBitmap,0,0,nWidth,nHeight);



		Graphics graphics(pDC->m_hDC);
		CachedBitmap cachedBmp(&bmp,&graphics);
		graphics.DrawCachedBitmap(&cachedBmp,10,10);
		graphics.ReleaseHDC(pDC->m_hDC);
	}

	return TRUE;
}


// CImagePreviewDlg message handlers
