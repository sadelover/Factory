
#include "stdafx.h"

#include "EqmDrawText.h"
#include "Tools/ColorDefines.h"
#include "Tools/CustomTools/CustomTools.h"
#include "UtilDrawNumber.h"
#include "DrawObject.h"
#include "ParseStringParam3.h"

//////////////////////////////////////////////////////////////////////////
#pragma warning(disable:4244)
#define STRING_0 _T("0")
#define STRING_1 _T("1")
#define STRING_2 _T("2")
#define STRING_3 _T("3")

#define TEXTDEFAULTWIDTH		50		//文本框默认长度
#define TEXTDEFAULTHEIGHT		18		//文本框默认高度
#define TEXTFONTDEFAULTSIZE		15		//文本框默认字体大小
#define TEXTFONTDEFAULTSTYLE	0		//文本框默认字体风格

#define TEXTFONTDEFAULTCOLOR	RGB_BLACK	//默认字体颜色

#define TEXTBORDEREFAULTCOLOR	RGB_WHITE	//文本背景颜色

//液晶字体的3D背景的最小宽度。
#define MIN3DBORDERWIDTH  50

#define DEFAULTPOINTVALUE -1				//默认点位
#define OFFSET 2  //偏移间距

#define TEXTINITIALSTRING _T("Static Text")

//////////////////////////////////////////////////////////////////////////

CEqmDrawText::CEqmDrawText()
: CEqmDrawBase(),
  m_fontsize(TEXTFONTDEFAULTSIZE),
  m_fontname(_T("微软雅黑")),
  m_color(TEXTFONTDEFAULTCOLOR),
  m_showmode(MODE_VALUE),
  m_strInitialValue(TEXTINITIALSTRING),
  m_strShowValue(TEXTINITIALSTRING),
  m_bordertype(1),
  m_borderColor(TEXTBORDEREFAULTCOLOR)
  ,m_align(0)
  ,m_nTextBKType(0)
  ,m_nWordWidth(0)
  ,m_nWordHeight(0)
  ,m_textAttri(TEXT_ATTR_NO_USE)
  ,m_cstrUnit(L"")
{
	m_nWidth  = TEXTDEFAULTWIDTH;
	m_nHeight = TEXTDEFAULTHEIGHT;
	m_dTransparence = 0;
	m_pFont = NULL;
	m_pSolidBrush = NULL;
	m_pHatchBrush = NULL;
	m_pCommonBorderPen = NULL;
	m_hPen3DSunkenTopLeft = NULL;
	m_hPen3DSunkenBottomRight = NULL;
	m_hPen3DHumpTopLeft = NULL;
	m_hPen3DHumpBottomRight = NULL;
	m_strPhysicalIDBind = L"";
	m_nPointNum = 0;
	UpdatePos();
}

CEqmDrawText::CEqmDrawText( const Gdiplus::PointF& centerpos, int pageid, int id, int width, 
						   int height, int fontsize, CString fontstyle, wstring strPhysicalID, 
						   const wstring& value, int fontcolor, TEXTSHOWMODE showmode, double dTransparent,
						   int ifborder, int bordercolor, const CString strDevCfg, const bool bFaultDiag , const CString strBindScript)
			:CEqmDrawBase(EQMDRAW_TEXT, centerpos, pageid, id, width, height),
			 m_fontsize(fontsize), m_fontname(fontstyle),
			 m_strInitialValue(value), m_color(fontcolor),
			m_showmode(showmode), m_dTransparence(dTransparent),
			m_bordertype(ifborder),m_borderColor(bordercolor)
            ,m_align(0),m_strPhysicalIDBind(strPhysicalID)
			,m_textAttri(TEXT_ATTR_NO_USE)
			,m_strDeviceCfg(strDevCfg)
			,m_bFaultDiagnosis(bFaultDiag)
			,m_strBindScript(strBindScript)
{
	m_strShowValue = m_strInitialValue;
	m_dTransparence = 0;
	m_pFont = NULL;
	m_pSolidBrush = NULL;
	m_pHatchBrush = NULL;
	m_pCommonBorderPen = NULL;
	m_hPen3DSunkenTopLeft = NULL;
	m_hPen3DSunkenBottomRight = NULL;
	m_hPen3DHumpTopLeft = NULL;
	m_hPen3DHumpBottomRight = NULL;
	m_nPointNum = 0;
	m_nTextBKType = 0;
	m_cstrUnit = L"";
	UpdatePos();
}


int CEqmDrawText::GetFontSize() const
{
	return m_fontsize;
}

void CEqmDrawText::SetFontSize( int fontsize )
{
	m_fontsize = fontsize;
	m_bNeedSave = true;
}

void CEqmDrawText::SetFontName(const CString& fontstyle )
{
	m_fontname = fontstyle;
	m_bNeedSave = true;
}

void CEqmDrawText::SetFontColor( int color )
{
	m_color = color;
	m_bNeedSave = true;
}

CString CEqmDrawText::GetFontName() const
{
	return m_fontname;
}

wstring CEqmDrawText::GetInitialstring() const
{
	return m_strInitialValue;
}

void CEqmDrawText::SetInitialString( const wstring& initialvalue )
{
	m_strInitialValue = initialvalue;
	m_bNeedSave = true;
}

int CEqmDrawText::GetFontColor() const
{
	return m_color;
}

void CEqmDrawText::SetShowString( const wstring strShow,  SHOWTYPE showtype )
{
	if (SHOWTYPE_RUN == showtype )
	{
		m_strShowValue = strShow;
	}
	else
	{
		m_strShowValue = m_strInitialValue;
	}
	m_bNeedSave = true;
}

wstring CEqmDrawText::GetShowString() const
{
	return m_strShowValue;
}

TEXTSHOWMODE CEqmDrawText::GetTextShowMode() const
{
	return m_showmode;
}

void CEqmDrawText::SetTextShowMode( TEXTSHOWMODE showmode )
{
	m_showmode = showmode;
	m_bNeedSave = true;
}

void CEqmDrawText::Draw( Gdiplus::Graphics *pGraphics, Image *pBitmap )
{
	CreateFont();
	CreateSolidBrush();
	CreateHatchBrush();
	CreateCommonBorderPen();
	Create3DPenSunken();
	Create3DPenHump();

	//CParseSplitString& instance = CParseSplitString::Instance();
	//if (MODE_ENUM == m_showmode)
	//{
	//	if(m_strTextModeString.GetLength() > 0)
	//	{
	//		instance.ParseParam3(GetID(), m_strTextModeString);
	//		m_strShowValue = instance.GetText(GetID(),0);
	//	}
	//}
	DrawString(pGraphics);
}

void CEqmDrawText::SetTransparence( double tranparence )
{
	m_dTransparence = tranparence;
	m_bNeedSave = true;
}

double CEqmDrawText::GetTransparence() const
{
	return m_dTransparence;
}

bool CEqmDrawText::ChangeSize(const Gdiplus::PointF& actPt, Direction dir )
{
	UpdatePos();

	if(m_dScale == 0)
	{
		m_dScale = (double)m_nWidth/(double)m_nHeight;
	}
	switch ( dir )
	{
	case Dir_Left:
		if (actPt.X < m_posru_.X)
		{
			m_pos_.X = actPt.X;
			m_posld_.X = actPt.X;
			m_nWidth = m_posru_.X - actPt.X;
			m_dScale = (double)m_nWidth/(double)m_nHeight;
		}
		break;
	case Dir_Right:
		if (actPt.X > m_pos_.X)
		{
			m_nWidth = abs(actPt.X-m_pos_.X);
			m_posrd_.X = actPt.X;
			m_posru_.X = actPt.X;
			m_dScale = (double)m_nWidth/(double)m_nHeight;
		}
		break;
	case Dir_Up:
		if (actPt.Y < m_posld_.Y)
		{
			m_pos_.Y = actPt.Y;
			m_posru_.Y = actPt.Y;
			m_nHeight = m_posld_.Y - actPt.Y;
			m_dScale = (double)m_nWidth/(double)m_nHeight;
		}
		break;
	case Dir_Down:
		if (actPt.Y > m_pos_.Y)
		{
			m_nHeight = abs(actPt.Y-m_pos_.Y);
			m_posld_.Y = actPt.Y;
			m_posrd_.Y = actPt.Y;
			m_dScale = (double)m_nWidth/(double)m_nHeight;
		}
		break;
	case Dir_LeftUp:
		{
			if(actPt.X < m_posrd_.X && actPt.Y < m_posrd_.Y)
			{
				m_nWidth = abs(actPt.X - m_posrd_.X);
				m_nHeight = m_nWidth/m_dScale;

				m_pos_.X = m_posrd_.X - m_nWidth;
				m_pos_.Y = m_posrd_.Y - m_nHeight;

				m_posru_.X = m_posrd_.X;
				m_posru_.Y = m_posrd_.Y - m_nHeight;

				m_posld_.X = m_posrd_.X - m_nWidth;
				m_posld_.Y = m_posrd_.Y;
			}
		}
		break;
	case Dir_LeftDown:
		{
			if(actPt.X < m_posru_.X && actPt.Y > m_posru_.Y)
			{
				m_nWidth = abs(actPt.X - m_posru_.X);
				m_nHeight = m_nWidth/m_dScale;

				m_pos_.X = m_posru_.X - m_nWidth;
				m_pos_.Y = m_posru_.Y;

				m_posld_.X = m_posru_.X - m_nWidth;
				m_posld_.Y = m_posru_.Y + m_nHeight;

				m_posrd_.X = m_posru_.X;
				m_posrd_.Y = m_posru_.Y + m_nHeight;
			}
		}
		break;
	case Dir_RightUp:
		{
			if(actPt.X > m_posld_.X && actPt.Y < m_posld_.Y)
			{
				m_nWidth = abs(actPt.X - m_posld_.X);
				m_nHeight = m_nWidth/m_dScale;

				m_pos_.X = m_posld_.X;
				m_pos_.Y = m_posld_.Y - m_nHeight;

				m_posru_.X = m_posld_.X + m_nWidth;
				m_posru_.Y = m_posld_.Y - m_nHeight;

				m_posrd_.X = m_posld_.X + m_nWidth;
				m_posrd_.Y = m_posld_.Y;
			}
		}
		break;
	case Dir_RightDown:
		{
			if(actPt.X > m_pos_.X && actPt.Y > m_pos_.Y)
			{
				m_nWidth = abs(actPt.X - m_pos_.X);
				m_nHeight = m_nWidth/m_dScale;

				m_posru_.X = m_pos_.X + m_nWidth;
				m_posru_.Y = m_pos_.Y;

				m_posrd_.X = m_pos_.X + m_nWidth;
				m_posrd_.Y = m_pos_.Y + m_nHeight;

				m_posld_.X = m_pos_.X;
				m_posld_.Y = m_pos_.Y + m_nHeight;
			}
		}
		break;
	default:
		break;
	}

	ResetPos();

	m_bNeedSave = true;
	return true;
}

bool CEqmDrawText::PointInMe(const  Gdiplus::PointF& actPt ) const
{
	if ( actPt.X <= m_pos_.X + GetWidth() &&
		actPt.X >= m_pos_.X &&
		actPt.Y <= m_pos_.Y + GetHeight() &&
		actPt.Y >= m_pos_.Y)
	{
		return true;
	}
	return false;
}

Direction CEqmDrawText::PointIsSide(const Gdiplus::PointF& actPt) 
{
	if ( actPt.X <= m_pos_.X &&
		actPt.X >= m_pos_.X-DEFAULT_SIDE_INTERNAL &&
		actPt.Y <= m_pos_.Y+GetHeight()+DEFAULT_SIDE_INTERNAL/2 &&
		actPt.Y >= m_pos_.Y-DEFAULT_SIDE_INTERNAL/2)
	{
		return Dir_Left;
	}
	else if ( actPt.X <= m_pos_.X+GetWidth()+DEFAULT_SIDE_INTERNAL &&
		actPt.X >= m_pos_.X+GetWidth() &&
		actPt.Y <= m_pos_.Y+GetHeight()+DEFAULT_SIDE_INTERNAL/2 &&
		actPt.Y >= m_pos_.Y-DEFAULT_SIDE_INTERNAL/2)
	{
		return Dir_Right;
	}
	else if ( actPt.X <= m_pos_.X+GetWidth()+DEFAULT_SIDE_INTERNAL/2&&
		actPt.X >= m_pos_.X-DEFAULT_SIDE_INTERNAL/2&&
		actPt.Y <= m_pos_.Y&&
		actPt.Y >= m_pos_.Y-DEFAULT_SIDE_INTERNAL )
	{
		return Dir_Up;
	}
	else if ( actPt.X <= m_pos_.X+GetWidth()+DEFAULT_SIDE_INTERNAL/2&&
		actPt.X >= m_pos_.X-DEFAULT_SIDE_INTERNAL/2&&
		actPt.Y <= m_pos_.Y+GetHeight()+DEFAULT_SIDE_INTERNAL&&
		actPt.Y >= m_pos_.Y+GetHeight() )
	{
		return Dir_Down;
	}
	else if ( actPt.X <= m_pos_.X&&
		actPt.X >= m_pos_.X-DEFAULT_SIDE_INTERNAL&&
		actPt.Y <= m_pos_.Y&&
		actPt.Y >= m_pos_.Y-DEFAULT_SIDE_INTERNAL )
	{
		return Dir_LeftUp;
	}
	else if ( actPt.X <= m_pos_.X&&
		actPt.X >= m_pos_.X-DEFAULT_SIDE_INTERNAL&&
		actPt.Y <= m_pos_.Y+GetHeight()+DEFAULT_SIDE_INTERNAL&&
		actPt.Y >= m_pos_.Y+GetHeight() )
	{
		return Dir_LeftDown;
	}
	else if ( actPt.X <= m_pos_.X+GetWidth()+DEFAULT_SIDE_INTERNAL&&
		actPt.X >= m_pos_.X+GetWidth()&&
		actPt.Y <= m_pos_.Y&&
		actPt.Y >= m_pos_.Y-DEFAULT_SIDE_INTERNAL  )
	{
		return Dir_RightUp;
	}
	else if ( actPt.X <= m_pos_.X+GetWidth()+DEFAULT_SIDE_INTERNAL&&
		actPt.X >= m_pos_.X+GetWidth()&&
		actPt.Y <= m_pos_.Y+GetHeight()+DEFAULT_SIDE_INTERNAL&&
		actPt.Y >= m_pos_.Y+GetHeight() )
	{
		return Dir_RightDown;
	}
	else
		return Dir_None;
}

void CEqmDrawText::DrawSelectEdge( Gdiplus::Graphics *pGraphics )
{
	Gdiplus::Pen pen(SELECTPENCOLOR, 1);

	pGraphics->DrawRectangle(&pen, Gdiplus::RectF(m_pos_.X-RECTSIZE,
		m_pos_.Y-RECTSIZE,RECTSIZE,RECTSIZE));
	pGraphics->DrawRectangle(&pen, Gdiplus::RectF(m_pos_.X-RECTSIZE,
		m_pos_.Y+GetHeight(),RECTSIZE,RECTSIZE));
	pGraphics->DrawRectangle(&pen, Gdiplus::RectF(m_pos_.X+GetWidth(),
		m_pos_.Y+GetHeight(),RECTSIZE,RECTSIZE));
	pGraphics->DrawRectangle(&pen, Gdiplus::RectF(m_pos_.X+GetWidth(),
		m_pos_.Y-RECTSIZE,RECTSIZE,RECTSIZE));
	pGraphics->DrawRectangle(&pen, Gdiplus::RectF(m_pos_.X-RECTSIZE,
		m_pos_.Y+GetHeight()/2-RECTSIZE/2,RECTSIZE,RECTSIZE));
	pGraphics->DrawRectangle(&pen, Gdiplus::RectF(m_pos_.X+GetWidth()/2-RECTSIZE/2,
		m_pos_.Y+GetHeight(),RECTSIZE,RECTSIZE));
	pGraphics->DrawRectangle(&pen, Gdiplus::RectF(m_pos_.X+GetWidth(),
		m_pos_.Y+GetHeight()/2-RECTSIZE/2,RECTSIZE,RECTSIZE));
	pGraphics->DrawRectangle(&pen, Gdiplus::RectF(m_pos_.X+GetWidth()/2-RECTSIZE/2,
		m_pos_.Y-RECTSIZE,RECTSIZE,RECTSIZE));
	if ( m_bIsMainAlign )
	{     
		Gdiplus::HatchBrush hatchBrush(Gdiplus::HatchStyleCross, MAINSELECT_BRUSHCOLOR, MAINSELECT_BRUSHCOLOR);

		pGraphics->FillRectangle(&hatchBrush, Gdiplus::RectF(m_pos_.X-RECTSIZE,
			m_pos_.Y-RECTSIZE,RECTSIZE,RECTSIZE));
		pGraphics->FillRectangle(&hatchBrush, Gdiplus::RectF(m_pos_.X-RECTSIZE,
			m_pos_.Y+GetHeight(),RECTSIZE,RECTSIZE));
		pGraphics->FillRectangle(&hatchBrush, Gdiplus::RectF(m_pos_.X+GetWidth(),
			m_pos_.Y+GetHeight(),RECTSIZE,RECTSIZE));
		pGraphics->FillRectangle(&hatchBrush, Gdiplus::RectF(m_pos_.X+GetWidth(),
			m_pos_.Y-RECTSIZE,RECTSIZE,RECTSIZE));
		pGraphics->FillRectangle(&hatchBrush, Gdiplus::RectF(m_pos_.X-RECTSIZE,
			m_pos_.Y+GetHeight()/2-RECTSIZE/2,RECTSIZE,RECTSIZE));
		pGraphics->FillRectangle(&hatchBrush, Gdiplus::RectF(m_pos_.X+GetWidth()/2-RECTSIZE/2,
			m_pos_.Y+GetHeight(),RECTSIZE,RECTSIZE));
		pGraphics->FillRectangle(&hatchBrush, Gdiplus::RectF(m_pos_.X+GetWidth(),
			m_pos_.Y+GetHeight()/2-RECTSIZE/2,RECTSIZE,RECTSIZE));
		pGraphics->FillRectangle(&hatchBrush, Gdiplus::RectF(m_pos_.X+GetWidth()/2-RECTSIZE/2,
			m_pos_.Y-RECTSIZE,RECTSIZE,RECTSIZE));
	}
}

bool CEqmDrawText::CopyProperty( CEqmDrawBase *pBase )
{
	CEqmDrawText *pText = dynamic_cast<CEqmDrawText*>(pBase);
	if ( pText )
	{
		m_fontsize = pText->GetFontSize();
		m_color    = pText->GetFontColor();
		m_dTransparence = pText->GetTransparence();
		m_fontname = pText->GetFontName();
		m_bordertype = pText->GetBorderType();
		m_borderColor = pText->GetBorderColor();
		m_dTransparence = pText->GetTransparence();
		m_nTextBKType = pText->GetTextBKType();
		m_bNeedSave = true;
		return true;
	}

	return false;
}	

bool CEqmDrawText::ShiftEqmtsUp()
{
	SetPos(Gdiplus::PointF(GetPos().X, GetPos().Y - MOVEDISTANCEBYKEY));
	UpdatePos();
	return true;
}

bool CEqmDrawText::ShiftEqmtsDown()
{
	SetPos(Gdiplus::PointF(GetPos().X, GetPos().Y + MOVEDISTANCEBYKEY));
	UpdatePos();
	return true;
}

bool CEqmDrawText::ShiftEqmtsLeft()
{
	SetPos(Gdiplus::PointF(GetPos().X - MOVEDISTANCEBYKEY, GetPos().Y));
	UpdatePos();
	return true;
}

bool CEqmDrawText::ShiftEqmtsRight()
{
	SetPos(Gdiplus::PointF(GetPos().X + MOVEDISTANCEBYKEY, GetPos().Y));
	UpdatePos();
	return true;
}

bool CEqmDrawText::DoMoveEqmt( Gdiplus::PointF prePt, Gdiplus::PointF actPt )
{
	float x_min = GetPos().X+actPt.X - prePt.X;
	float x_max = GetPos().X+actPt.X - prePt.X+m_nWidth;
	float y_min = GetPos().Y+actPt.Y - prePt.Y;
	float y_max = GetPos().Y+actPt.Y - prePt.Y+m_nHeight;
	//if ( x_min<0 || x_max>1920 || y_min<0 || y_max>955 )
	//{
	//	return false;
	//}
	if (x_min < 0)
	{
		x_min = 0;
		x_max = x_min + m_nWidth;
	}
	if (x_max > 1920)
	{
		x_max = 1920;
		x_min = 1920 - m_nWidth;
	}
	if (y_min < 0)
	{
		y_min = 0;
		y_max = y_min + m_nHeight;
	}
	if (y_max > 955)
	{
		y_max = 955;
		y_min = y_max - m_nHeight;
	}
	//SetPos(Gdiplus::PointF(GetPos().X+actPt.X - prePt.X, GetPos().Y+actPt.Y - prePt.Y));
	SetPos(Gdiplus::PointF(x_min, y_min));
	UpdatePos();
	return true;
}


bool CEqmDrawText::IsSplashing() const
{

	return false;
}

void CEqmDrawText::SetSplashing(bool bSplash)
{

}

bool CEqmDrawText::IsValueMode() const
{
	return m_showmode == MODE_VALUE;
}

CEqmDrawText* CEqmDrawText::Copy()
{
	CEqmDrawText *pNewText =  new CEqmDrawText(*this);
	pNewText->m_pCommonBorderPen = NULL;
	pNewText->m_pFont = NULL;
	pNewText->m_pHatchBrush = NULL;
	pNewText->m_pSolidBrush = NULL;
	pNewText->m_hPen3DSunkenTopLeft = NULL;
	pNewText->m_hPen3DSunkenBottomRight= NULL;
	pNewText->m_hPen3DHumpTopLeft= NULL;
	pNewText->m_hPen3DHumpBottomRight= NULL;
	m_bNeedSave = true;
	return pNewText;
}
//////////////////////////////////////////////////////////////////////////

int     CEqmDrawText::GetAlign() const
{
    return m_align;
}
void    CEqmDrawText::SetAlign(int align)
{
    m_align = align;
	m_bNeedSave = true;
}
//////////////////////////////////////////////////////////////////////////

int CEqmDrawText::GetBorderColor() const
{
	return m_borderColor;
}
void CEqmDrawText::SetBorderColor( int bordercolor )
{
	m_borderColor = bordercolor;
	m_bNeedSave = true;
}
int CEqmDrawText::GetBorderType()
{
	return m_bordertype;
}
void CEqmDrawText::SetBorderType(int  borderis3d)
{
	m_bordertype = borderis3d;
	m_bNeedSave = true;
}

void CEqmDrawText::DrawTextBkImage(Gdiplus::Graphics* pGraphics, const CRect& boundrect)
{
	int x = boundrect.left;
	int y = boundrect.top;
	int nWidth = boundrect.Width();
	int nHeight = boundrect.Height();
	/*获取运行地址*/
	CString m_strSysPath;
	Project::Tools::GetSysPath(m_strSysPath);
	/*获取运行地址*/
	CString strImagePath;
	if (m_nTextBKType == 0)
		strImagePath = L"";
	else if (m_nTextBKType == 1)
		strImagePath =  m_strSysPath + L"\\Images\\TextBackgroudImg\\backgroudImg_1.png" ;
	else if (m_nTextBKType == 2)
		strImagePath =  m_strSysPath + L"\\Images\\TextBackgroudImg\\backgroudImg_2.png" ;
	else if (m_nTextBKType == 3)
		strImagePath =  m_strSysPath + L"\\Images\\TextBackgroudImg\\backgroudImg_3.png" ;
	else if (m_nTextBKType == 4)
		strImagePath =  m_strSysPath + L"\\Images\\TextBackgroudImg\\backgroudImg_4.png" ;
	else if (m_nTextBKType == 5)
		strImagePath =  m_strSysPath + L"\\Images\\TextBackgroudImg\\backgroudImg_5.png" ;
	else if (m_nTextBKType == 6)
		strImagePath =  m_strSysPath + L"\\Images\\TextBackgroudImg\\backgroudImg_6.png" ;
	else if (m_nTextBKType == 7)
		strImagePath =  m_strSysPath + L"\\Images\\TextBackgroudImg\\backgroudImg_7.png" ;
	else if (m_nTextBKType == 8)
		strImagePath =  m_strSysPath + L"\\Images\\TextBackgroudImg\\backgroudImg_8.png" ;
	else if (m_nTextBKType == 9)
		strImagePath =  m_strSysPath + L"\\Images\\TextBackgroudImg\\backgroudImg_9.png" ;
	else if (m_nTextBKType == 10)
		strImagePath =  m_strSysPath + L"\\Images\\TextBackgroudImg\\backgroudImg_10.png";
	/*加载图片*/
	Gdiplus::Image image(strImagePath);
	/*绘制图片*/
	pGraphics->DrawImage(&image,x,y,nWidth,nHeight);
}

void CEqmDrawText::DrawTextBorder( Gdiplus::Graphics* pGraphics, const CRect& boundrect )
{
	//画3D边框
	m_bordertype = 1;
	switch(m_bordertype)
	{
	case 0:   //无
		{
		}
		break;
	case 1:    //普通边框
		{
			int x = boundrect.left;
			int y = boundrect.top;
			int nWidth = boundrect.Width();
			int nHeight = boundrect.Height();
			pGraphics->DrawRectangle(m_pCommonBorderPen,x,y,nWidth,nHeight);
		}
		break;
	case 2:    //黑框白底
		{
			int x = boundrect.left-4 ;
			int y = boundrect.top -3;
			int nWidth = boundrect.Width()+4;
			int nHeight = boundrect.Height()+4;
			pGraphics->DrawRectangle(m_pCommonBorderPen,x,y,nWidth,nHeight);
			pGraphics->FillRectangle(m_pHatchBrush,x,y,nWidth,nHeight);
		}
		break;
	case 3:           //3D凹边框
		{
			int x = boundrect.left -3 ;
			int y = boundrect.top -2;
			int nWidth = boundrect.Width()+4;
			int nHeight = boundrect.Height()+4;
			Gdiplus::PointF pt1,pt2,pt3,pt4,pt_1,pt_2,pt_3,pt_4;
			pt1.X = x;
			pt1.Y = y;
			pt2.X = x+nWidth;
			pt2.Y = y;
			pt3.X = x;
			pt3.Y = y+nHeight;
			pt4.X = x+nWidth;
			pt4.Y = y+nHeight;
			//
			pt_1.X = x-2;
			pt_1.Y = y;

			pt_2.X = x+nWidth+2;
			pt_2.Y = y;

			pt_3.X = x;
			pt_3.Y = y+nHeight+2;

			pt_4.X = x+nWidth+2;
			pt_4.Y = y+nHeight;

			HDC hdc = pGraphics->GetHDC();
			FastDrawLine(hdc, m_hPen3DSunkenTopLeft, PointFToCPoint(pt1), PointFToCPoint(pt2));
			FastDrawLine(hdc, m_hPen3DSunkenTopLeft, PointFToCPoint(pt1), PointFToCPoint(pt3));
			FastDrawLine(hdc, m_hPen3DSunkenBottomRight, PointFToCPoint(pt4), PointFToCPoint(pt2));
			FastDrawLine(hdc, m_hPen3DSunkenBottomRight, PointFToCPoint(pt3), PointFToCPoint(pt4));
			pGraphics->ReleaseHDC(hdc);

			pGraphics->FillRectangle(m_pHatchBrush,x,y,nWidth,nHeight);
		}
		break;
	case 4: //3D凸边框
		{
			int x = boundrect.left -4 ;
			int y = boundrect.top -3;
			int nWidth = boundrect.Width()+4;
			int nHeight = boundrect.Height()+4;

			Gdiplus::PointF pt1,pt2,pt3,pt4,pt_3,pt_4;
			pt1.X = x;
			pt1.Y = y;
			pt2.X = x+nWidth;
			pt2.Y = y;
			pt3.X = x;
			pt3.Y = y+nHeight;
			pt4.X = x+nWidth;
			pt4.Y = y+nHeight;
			//
			pt_3.X = x;
			pt_3.Y = y+nHeight;
			pt_4.X = x+nWidth+2;
			pt_4.Y = y+nHeight;

			HDC hdc = pGraphics->GetHDC();
			FastDrawLine(hdc, m_hPen3DHumpTopLeft, PointFToCPoint(pt1), PointFToCPoint(pt2));
			FastDrawLine(hdc, m_hPen3DHumpTopLeft, PointFToCPoint(pt1), PointFToCPoint(pt3));
			FastDrawLine(hdc, m_hPen3DHumpBottomRight, PointFToCPoint(pt2), PointFToCPoint(pt4));
			FastDrawLine(hdc, m_hPen3DHumpBottomRight, PointFToCPoint(pt3), PointFToCPoint(pt4));
			pGraphics->ReleaseHDC(hdc);

			pGraphics->FillRectangle(m_pHatchBrush,x,y,nWidth,nHeight);
		}
		break;
	default:
		break;

	}
}

void CEqmDrawText::DrawString( Gdiplus::Graphics *pGraphics )
{
	ASSERT(pGraphics);

	//centerpos.x 是最左边, 
	//let text rect auto size
	Gdiplus::RectF outRect;
	Gdiplus::StringFormat strFomat;
	strFomat.SetAlignment(Gdiplus::StringAlignmentCenter);
	strFomat.SetLineAlignment(Gdiplus::StringAlignmentCenter);
	
	//判断文本的宽度，高度.
	pGraphics->MeasureString(m_strShowValue.c_str(), (int)m_strShowValue.size(), m_pFont, Gdiplus::RectF(0,0,0,0), &strFomat, &outRect);
	m_nWordWidth = outRect.Width;
	m_nWordHeight = outRect.Height;
	//m_nWidth  = outRect.Width+1; 
	//m_nHeight = /*outRect.Height*/m_pFont->GetHeight(pGraphics);


	UpdatePos();
	//const float centerX = GetPos().X;
	//const float centerY = GetPos().Y;
	float rectX = m_pos_.X;
	float rectY = m_pos_.Y;// - m_nHeight/(double)2;
	float rectW = m_nWidth ; //总是显示不全
	float rectH = m_nHeight;

	Gdiplus::RectF rect0(rectX, rectY, rectW, rectH);

	CRect rect(rect0.GetLeft(), rect0.GetTop(), rect0.GetRight(), rect0.GetBottom());
	DrawTextBorder(pGraphics, rect);
	DrawTextBkImage(pGraphics, rect);
	switch (m_align)
	{
	case 0:
		{	// 左中
			rect0.X = rectX;
			rect0.Y += (rectH - m_nWordHeight)/2;
		}
		break;
	case 1:
		{	// 右中
			if (rectW < m_nWordWidth)
			{
				rect0.X = rectX;
			} 
			else
			{
				rect0.X += rectW - m_nWordWidth;
			}
			rect0.Y += (rectH - m_nWordHeight)/2;
		}
		break;
	case 2:
		{	// 中中
			if (rectW < m_nWordWidth)
			{
				rect0.X = rectX;
			}
			else
			{
				rect0.X += (rectW - m_nWordWidth)/2;
			}			
			rect0.Y += (rectH - m_nWordHeight)/2;
		}
		break;
	case 3:
		{	// 左上
			rect0.X = rectX;
			rect0.Y = rectY;
		}
		break;
	case 4:
		{	// 右上
			if (rectW < m_nWordWidth)
			{
				rect0.X = rectX;
			} 
			else
			{
				rect0.X += rectW - m_nWordWidth;
			}
			rect0.Y = rectY;
		}
		break;
	case 5:
		{	// 中上
			if (rectW < m_nWordWidth)
			{
				rect0.X = rectX;
			}
			else
			{
				rect0.X += (rectW - m_nWordWidth)/2;
			}			
			rect0.Y = rectY;
		}
		break;
	case 6:
		{	// 左下
			rect0.X = rectX;
			rect0.Y += rectH - m_nWordHeight;
		}
		break;
	case 7:
		{	// 右下
			if (rectW < m_nWordWidth)
			{
				rect0.X = rectX;
			} 
			else
			{
				rect0.X += rectW - m_nWordWidth;
			}
			rect0.Y += rectH - m_nWordHeight;
		}
		break;
	case 8:
		{	// 中下
			if (rectW < m_nWordWidth)
			{
				rect0.X = rectX;
			}
			else
			{
				rect0.X += (rectW - m_nWordWidth)/2;
			}			
			rect0.Y += rectH - m_nWordHeight;
		}
		break;
	default:
		{
			rect0.X = rectX;
			rect0.Y = rectY;
		}
		break;
	}
	rect0.Width += 4;
	pGraphics->DrawString(m_strShowValue.c_str(), -1, m_pFont, rect0, NULL, m_pSolidBrush);
}

void CEqmDrawText::DrawNumberWithUnit( Gdiplus::Graphics *pGraphics )
{
	ASSERT(pGraphics);

	//centerpos.x 是最左边, 
	//let text rect auto size
	Gdiplus::StringFormat strFomat;
	strFomat.SetAlignment(Gdiplus::StringAlignmentCenter);
	strFomat.SetLineAlignment(Gdiplus::StringAlignmentCenter);
	
	Point numberimagearea;
	wstring wstrNumber, wstrUnit;
	UtilDrawNumber::GetInstance()->SplitString(m_strShowValue, wstrNumber, wstrUnit, numberimagearea);
	// get the rect of the unit string.
	Gdiplus::RectF rectunit;
	pGraphics->MeasureString(wstrUnit.c_str(), (int)wstrUnit.size(), m_pFont, Gdiplus::RectF(0,0,0,0), &strFomat, &rectunit);
	
	// calculate the border width and height;
	int borderwidth = numberimagearea.X < MIN3DBORDERWIDTH ? MIN3DBORDERWIDTH : numberimagearea.X+5;
	int borderheight = numberimagearea.Y;
	
	// calculate the topleft point pos.
	const float centerX = GetPos().X;
	const float centerY = GetPos().Y;
	const float topleft_Xpos = centerX;
	const float topleft_Ypos = centerY - m_nHeight/2;
	
	//generate the rect of the border.
	CRect borderrect(topleft_Xpos, topleft_Ypos, topleft_Xpos+borderwidth, topleft_Ypos+borderheight);
	//如果液晶字体不为空
	if (!wstrNumber.empty())
	{
		DrawTextBorder(pGraphics, borderrect);
	}

	if(true)//背景不为0，1-10
	{
		//绘制背景图
		DrawTextBkImage(pGraphics, borderrect);
	}
	
	// calculate the total width and height .
	m_nWidth  = rectunit.Width + borderwidth +1; 
	m_nHeight = rectunit.Height>= numberimagearea.Y ?rectunit.Height:numberimagearea.Y ;
	
	//////////////////////////////////////////////////////////////////////////
	const float rectW = m_nWidth ; //总是显示不全
	const float rectH = m_nHeight;
	
	//////////////////////////////////////////////////////////////////////////
	wstring wstrLeft;
	const float c_number_y_blank = 2;
	const int numberIntervalX = 0;
	
	//画液晶数字
	const int numberWidth = UtilDrawNumber::GetInstance()->DrawGreenNumber(pGraphics, m_strShowValue, topleft_Xpos+3, topleft_Ypos- c_number_y_blank,  wstrLeft);
	
	if(wstrLeft.length() )
	{
		Gdiplus::RectF rect1(topleft_Xpos + borderwidth + 3, topleft_Ypos , rectunit.Width, (Gdiplus::REAL)m_nHeight);
		pGraphics->DrawString(wstrUnit.c_str(), -1, m_pFont, rect1, NULL, m_pSolidBrush);
	}
}

bool CEqmDrawText::IsNoBorder()
{
	return m_bordertype == 0;
}

void CEqmDrawText::CreateFont()
{
	if (m_pFont == NULL)
	{
		const wstring str_font = m_fontname;
		Gdiplus::FontFamily fontFamily(str_font.c_str());
		if(!fontFamily.IsAvailable())
		{
			m_fontname = _T("宋体");
		}
		m_pFont = new Gdiplus::Font(m_fontname, m_fontsize, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel,NULL);
		ASSERT(m_pFont);
	}
}

void CEqmDrawText::CreateSolidBrush()
{
	if (m_pSolidBrush == NULL)
	{
		Gdiplus::Color color(255, GetRValue(m_color), GetGValue(m_color), GetBValue(m_color));
		m_pSolidBrush = new Gdiplus::SolidBrush(color);
		ASSERT(m_pSolidBrush);
	}
}

void CEqmDrawText::DeleteFont()
{
	if (m_pFont)
	{
		delete m_pFont;
		m_pFont = NULL;
	}
}

void CEqmDrawText::DeleteSolidBrush()
{
	if (m_pSolidBrush)
	{
		delete m_pSolidBrush;
		m_pSolidBrush = NULL;
	}
}

CEqmDrawText::~CEqmDrawText()
{
	DeleteFont();
	DeleteSolidBrush();
	DeleteHatchBrush();
	DeleteCommonBorderPen();
	Delete3DPenSunken();
	Delete3DPenHump();
}

void CEqmDrawText::CreateHatchBrush()
{
	if(m_borderColor!=-1)
	{
		if (m_pHatchBrush == NULL)
		{
			Gdiplus::Color fillColor(GetRValue(m_borderColor), GetGValue(m_borderColor), GetBValue(m_borderColor));
			m_pHatchBrush = new Gdiplus::HatchBrush(Gdiplus::HatchStyleCross,fillColor, fillColor); 
			ASSERT(m_pHatchBrush);
		}
	}	
}

void CEqmDrawText::DeleteHatchBrush()
{
	if(m_pHatchBrush)
	{
		delete m_pHatchBrush;
		m_pHatchBrush = NULL;
	}
}

void CEqmDrawText::CreateCommonBorderPen()
{
	if (m_pCommonBorderPen == NULL)
	{
		Gdiplus::Color color_topleft(128, 128, 128, 128);		
		m_pCommonBorderPen = new Gdiplus::Pen(color_topleft, 0);
		ASSERT(m_pCommonBorderPen);
		m_pCommonBorderPen->SetDashStyle(DashStyleDot);
		m_pCommonBorderPen->SetAlignment(Gdiplus::PenAlignmentInset);
	}
}

void CEqmDrawText::DeleteCommonBorderPen()
{
	if (m_pCommonBorderPen)
	{
		delete m_pCommonBorderPen;
		m_pCommonBorderPen = NULL;
	}
}

void CEqmDrawText::Create3DPenSunken()
{
	if (m_hPen3DSunkenTopLeft == NULL)
	{
		m_hPen3DSunkenTopLeft = ::CreatePen(PS_SOLID, 3, RGB(130,130,130));
		ASSERT(m_hPen3DSunkenTopLeft);
	}
	if (m_hPen3DSunkenBottomRight)
	{
		m_hPen3DSunkenBottomRight = ::CreatePen(PS_SOLID, 5, RGB(247,247,247));
		ASSERT(m_hPen3DSunkenBottomRight);
	}
}

void CEqmDrawText::Delete3DPenSunken()
{
	if (m_hPen3DSunkenTopLeft)
	{
		DeleteObject(m_hPen3DSunkenTopLeft);
		m_hPen3DSunkenTopLeft = NULL;
	}
	if (m_hPen3DSunkenBottomRight)
	{
		DeleteObject(m_hPen3DSunkenBottomRight);
		m_hPen3DSunkenBottomRight = NULL;
	}
}

void CEqmDrawText::Create3DPenHump()
{
	if (m_hPen3DHumpTopLeft == NULL)
	{
		m_hPen3DHumpTopLeft = ::CreatePen(PS_SOLID, 3, RGB(6,6,6));
		ASSERT(m_hPen3DHumpTopLeft);
	}
	if (m_hPen3DHumpBottomRight)
	{
		m_hPen3DHumpBottomRight = ::CreatePen(PS_SOLID, 5, RGB(247,247,247));
		ASSERT(m_hPen3DHumpBottomRight);
	}
}

void CEqmDrawText::Delete3DPenHump()
{
	if (m_hPen3DHumpTopLeft)
	{
		DeleteObject(m_hPen3DHumpTopLeft);
		m_hPen3DHumpTopLeft = NULL;
	}
	if (m_hPen3DHumpBottomRight)
	{
		DeleteObject(m_hPen3DHumpBottomRight);
		m_hPen3DHumpBottomRight = NULL;
	}
}

wstring CEqmDrawText::GetPhysicalID()
{
	return m_strPhysicalIDBind;
}

void CEqmDrawText::SetPhysicalID( wstring strPhysicalID )
{
	m_strPhysicalIDBind = strPhysicalID;
	m_bNeedSave = true;
}

void CEqmDrawText::SetTextModeShowString( const CString strText )
{
	m_strTextModeString = strText;
	m_bNeedSave = true;
}

const CString CEqmDrawText::GetTextModeShowString()
{
	return m_strTextModeString;
}

void CEqmDrawText::SetPointNum( const int nNum )
{
	m_nPointNum = nNum;
	m_bNeedSave = true;
}

const int CEqmDrawText::GetPointNum()
{
	return m_nPointNum;
}

void CEqmDrawText::SetDateTime( const CString str )
{
	m_strDateTime = str;
	m_bNeedSave = true;
}

const CString CEqmDrawText::GetDateTime()
{
	return m_strDateTime;
}

void CEqmDrawText::SetDateTimePoint( const CString str )
{
	m_strDateTimePoint = str;
	m_bNeedSave = true;
}

const CString CEqmDrawText::GetDateTimePoint()
{
	return m_strDateTimePoint;
}

int CEqmDrawText::GetTextBKType() const
{
	return m_nTextBKType;
}

void CEqmDrawText::SetTextBKType( int nTextBKType )
{
	m_nTextBKType = nTextBKType;
	m_bNeedSave = true;
}

bool CEqmDrawText::ShiftEqmtsCtrlUp()
{
	SetPos(Gdiplus::PointF(GetPos().X, GetPos().Y - MOVEDISTANCEBYKEYCTRL));
	UpdatePos();
	return true;
}

bool CEqmDrawText::ShiftEqmtsCtrlDown()
{
	SetPos(Gdiplus::PointF(GetPos().X, GetPos().Y + MOVEDISTANCEBYKEYCTRL));
	UpdatePos();
	return true;
}

bool CEqmDrawText::ShiftEqmtsCtrlLeft()
{
	SetPos(Gdiplus::PointF(GetPos().X - MOVEDISTANCEBYKEYCTRL, GetPos().Y));
	UpdatePos();
	return true;
}

bool CEqmDrawText::ShiftEqmtsCtrlRight()
{
	SetPos(Gdiplus::PointF(GetPos().X + MOVEDISTANCEBYKEYCTRL, GetPos().Y));
	UpdatePos();
	return true;
}

void CEqmDrawText::UpdatePos(void)
{
	m_pos_.X = m_pos.X;
	m_pos_.Y = m_pos.Y - m_nHeight/2;
	m_posru_.X = m_pos_.X + m_nWidth;
	m_posru_.Y = m_pos_.Y;
	m_posld_.X = m_pos_.X;
	m_posld_.Y = m_pos_.Y + m_nHeight;
	m_posrd_.X = m_pos_.X + m_nWidth;
	m_posrd_.Y = m_pos_.Y + m_nHeight;
}

void CEqmDrawText::ResetPos(void)
{
	m_pos.X = m_pos_.X;
	m_pos.Y = m_pos_.Y + m_nHeight/2;
}

bool CEqmDrawText::ClonePropertyByDest( CEqmDrawBase *pBase )
{
	if ( pBase && m_eEqmType == pBase->GetEqmType() )
	{
		CopyBaseProperty(pBase);
		//特有属性

		CEqmDrawText* pDest = (CEqmDrawText*)pBase;
		if(pDest)
		{
			SetFontSize(pDest->GetFontSize());
			SetFontName(pDest->GetFontName());
			SetFontColor(pDest->GetFontColor());
			SetInitialString(pDest->GetInitialstring());
			SetShowString(pDest->GetShowString(),SHOWTYPE_DRAW);

			SetTextShowMode(pDest->GetTextShowMode());
			SetTransparence(pDest->GetTransparence());
			SetBorderColor(pDest->GetBorderColor());
			SetBorderType(pDest->GetBorderType());

			SetAlign(pDest->GetAlign());
			SetSplashing(pDest->IsSplashing());
			SetPhysicalID(pDest->GetPhysicalID());

			SetTextModeShowString(pDest->GetTextModeShowString());
			SetPointNum(pDest->GetPointNum());
			SetDateTime(pDest->GetDateTime());
			SetDateTimePoint(pDest->GetDateTimePoint());

			SetTextBKType(pDest->GetTextBKType());
		}
		return true;
	}
	return false;
}

bool CEqmDrawText::CopyInFormatBrush(CEqmDrawBase *pBase)
{
	CEqmDrawText* pText = dynamic_cast<CEqmDrawText*>(pBase);
	if (pText != NULL && pText->GetEqmType() == m_eEqmType)
	{
		SetHeight(pText->GetHeight());
		SetWidth(pText->GetWidth());
		SetLayer(pText->GetLayer());

		DeleteFont();
		DeleteSolidBrush();
		DeleteHatchBrush();
		DeleteCommonBorderPen();
		Delete3DPenSunken();
		Delete3DPenHump();

		SetFontSize(pText->GetFontSize());
		SetFontColor(pText->GetFontColor());
		SetTransparence(pText->GetTransparence());
		SetFontName(pText->GetFontName());
		SetBorderType(pText->GetBorderType());
		SetBorderColor(pText->GetBorderColor());
		SetAlign(pText->GetAlign());
		SetPointNum(pText->GetPointNum());

		m_bNeedSave = true;
		return true;
	}

	return false;
}

TextAttri CEqmDrawText::GetTextAttribute(void) const
{
	return m_textAttri;
}

void CEqmDrawText::SetTextAttribute(const TextAttri& txtAttr)
{
	m_textAttri = txtAttr;
	m_bNeedSave = true;
}

CString CEqmDrawText::GetDeviceCfg(void) const
{
	return m_strDeviceCfg;
}

void CEqmDrawText::SetDeviceCfg(const CString strDevCfg)
{
	m_strDeviceCfg = strDevCfg;
	m_bNeedSave = true;
}

CString CEqmDrawText::GetTemperCfg(void) const
{
	return m_strTemperCfg;
}

void CEqmDrawText::SetTemperCfg(const CString strTemperCfg)
{
	m_strTemperCfg = strTemperCfg;
	m_bNeedSave = true;
}

bool CEqmDrawText::GetFaultDiagnosis(void) const
{
	return m_bFaultDiagnosis;
}

void CEqmDrawText::SetFaultDiagnosis(const bool bFaultDiag)
{
	m_bFaultDiagnosis = bFaultDiag;
	m_bNeedSave = true;
}


CString CEqmDrawText::GetBindScript()
{
	return m_strBindScript;
}
void CEqmDrawText::SetBindScript(CString strBindScript)
{

	m_strBindScript = strBindScript;
}

CString CEqmDrawText::GetUnit()
{
	return m_cstrUnit;
}

void CEqmDrawText::SetUnit(CString cstrUnit)
{
	m_cstrUnit = cstrUnit;
}
