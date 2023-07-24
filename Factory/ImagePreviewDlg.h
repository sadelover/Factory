#pragma once


// CImagePreviewDlg dialog

class CImagePreviewDlg : public CDialog
{
	DECLARE_DYNAMIC(CImagePreviewDlg)

public:
	CImagePreviewDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CImagePreviewDlg();
// Dialog Data
	enum { IDD = IDD_DIALOG_IMAGE_PREVIEW };

	bool	ShowPreviewImage(Image* pBitmap, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()

private:
	Image* m_pBitmap;
};
