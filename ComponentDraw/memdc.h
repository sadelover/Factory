//////////////////////////////////////////////////
// CMemDC - memory DC
//
// Author: ZhuHuaWei
// Email:  hw.zhu@sohu.com
// Copyright 2008-2010, ZhuHuaWei
//
// You may freely use or modify this code provided this
// Copyright is included in all derived versions.
//
//
// Line 44 : Added bBg parameter.
// Line 83 ~ 87 : If bBg is TRUE, copy background.
//
#ifndef _MEMDC_H_
#define _MEMDC_H_
#pragma once
#include <afxwin.h>

class CMemDrawDC : public CDC
{
public:
	CMemDrawDC(CDC* pDC, const CRect* pRect = NULL, BOOL bBg = FALSE) : CDC()
	{
		ASSERT(pDC != NULL); 

		// Some initialization
		m_pDC = pDC;
		m_oldBitmap = NULL;
		m_bMemDC = !pDC->IsPrinting();

		// Get the rectangle to draw
		if (pRect == NULL)
		{
			pDC->GetClipBox(&m_rect);
		}else{
			m_rect = *pRect;
		}

		if (m_bMemDC)
		{
			// Create a Memory DC
			CreateCompatibleDC(pDC);
			pDC->LPtoDP(&m_rect);

			m_bitmap.CreateCompatibleBitmap(pDC, m_rect.Width(), m_rect.Height());
			m_oldBitmap = SelectObject(&m_bitmap);

			SetMapMode(pDC->GetMapMode());

			SetWindowExt(pDC->GetWindowExt());
			SetViewportExt(pDC->GetViewportExt());

			pDC->DPtoLP(&m_rect);
			SetWindowOrg(m_rect.left, m_rect.top);
		}else{
			// Make a copy of the relevent parts of the current DC for printing
			m_bPrinting = pDC->m_bPrinting;
			m_hDC       = pDC->m_hDC;
			m_hAttribDC = pDC->m_hAttribDC;
		}

		// Fill background 
		if( bBg )
			BitBlt(m_rect.left, m_rect.top, m_rect.Width(), m_rect.Height(),
			m_pDC, m_rect.left, m_rect.top, SRCCOPY);
		else
			FillSolidRect(m_rect, pDC->GetBkColor());
	}

	~CMemDrawDC()	
	{		
		if (m_bMemDC) 
		{
			// Copy the offscreen bitmap onto the screen.
			if (m_pDC->GetSafeHdc())
			{
				m_pDC->BitBlt(m_rect.left, m_rect.top, m_rect.Width(), m_rect.Height(),
					this, m_rect.left, m_rect.top, SRCCOPY);			

				//Swap back the original bitmap.
				//SelectObject(m_oldBitmap);		
			}
			
		}else{
			// All we need to do is replace the DC with an illegal value,
			// this keeps us from accidently deleting the handles associated with
			// the CDC that was passed to the constructor.			
			m_hDC = m_hAttribDC = NULL;
		}	
	}

	// Allow usage as a pointer	
	CMemDrawDC* operator->() 
	{
		return this;
	}	

	// Allow usage as a pointer	
	operator CMemDrawDC*() 
	{
		return this;
	}

private:	
	CBitmap		m_bitmap;		// Offscreen bitmap
	CBitmap*	m_oldBitmap;	// bitmap originally found in CMemDC
	CDC*		m_pDC;			// Saves CDC passed in constructor
	CRect		m_rect;			// Rectangle of drawing area.
	BOOL		m_bMemDC;		// TRUE if CDC really is a Memory DC.
};

#endif