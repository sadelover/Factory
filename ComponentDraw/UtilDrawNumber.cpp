//bird add 20110926

#include <stdafx.h>
#include "UtilDrawNumber.h"
#include "Tools/FileFunction.h"
#include "Tools/CustomTools/CustomTools.h"
//////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//////////////////////////////////////////////////////////////////////////

const TCHAR* c_str_number = _T("number/");

UtilDrawNumber * UtilDrawNumber::m_pDrawNumber = NULL;
Mutex UtilDrawNumber::m_mutex;
//////////////////////////////////////////////////////////////////////////

UtilDrawNumber::UtilDrawNumber()
{
    GetResourcePath();
    InitVecFileName();
    bool ifok = LoadPicture();
}

UtilDrawNumber::~UtilDrawNumber()
{
	for(UINT idx=0; idx< E_NUM_MAX; idx++)
	{
		SAFE_DELETE(m_pDrawNumber->m_vecImage[idx] );
	}
}

UtilDrawNumber * UtilDrawNumber::GetInstance( void )
{
    if ( NULL == m_pDrawNumber )
    {
        Scoped_Lock<Mutex> mut(m_mutex);
        if (!m_pDrawNumber){
            m_pDrawNumber = new UtilDrawNumber();
         //   atexit(DestroyInstance );
        }
    }
    return m_pDrawNumber;
}

void UtilDrawNumber::DestroyInstance()
{
    SAFE_DELETE(m_pDrawNumber);
}

//////////////////////////////////////////////////////////////////////////
void    UtilDrawNumber::InitVecFileName()
{
    m_vecStrFileName[E_NUM_0] = _T("num_0.png");
    m_vecStrFileName[E_NUM_1] = _T("num_1.png");
    m_vecStrFileName[E_NUM_2] = _T("num_2.png");
    m_vecStrFileName[E_NUM_3] = _T("num_3.png");
    m_vecStrFileName[E_NUM_4] = _T("num_4.png");
    m_vecStrFileName[E_NUM_5] = _T("num_5.png");
    m_vecStrFileName[E_NUM_6] = _T("num_6.png");
    m_vecStrFileName[E_NUM_7] = _T("num_7.png");
    m_vecStrFileName[E_NUM_8] = _T("num_8.png");
    m_vecStrFileName[E_NUM_9] = _T("num_9.png");
    m_vecStrFileName[E_NUM_POINT] = _T("num_point.png");
    m_vecStrFileName[E_NUM_MINUS] = _T("num_minus.png");
}

void  UtilDrawNumber::GetResourcePath()
{
    Project::Tools::GetSysPath(m_strResPath);
    m_strResPath += _T("/images/");
    //return m_strResPath;
}

bool UtilDrawNumber::LoadPicture()
{
    bool ifok = true;
    
    Image* img = NULL;
    wstring cstrFileName;

    for(UINT idx=0; idx< E_NUM_MAX; idx++)
	{
        cstrFileName = m_strResPath + c_str_number + m_vecStrFileName[idx];
		if (Tools::FileFunction::FileEixst(cstrFileName.c_str()))
		{
			img = Image::FromFile(cstrFileName.c_str(), FALSE);
			if(!img)
			{
				ASSERT(false);
				ifok = false;
				continue;
			}
			m_vecImage[idx]= img;
		}
		else
		{
			CString strPrompt;
			strPrompt.Format(L"Miss File: %s", cstrFileName.c_str());
			AfxMessageBox(strPrompt);
		}
    }
    return ifok;
}

//分割字符串, 只能识别前面数字, 后面字母的
//bool: if ok, return true
bool    UtilDrawNumber::SplitString(const wstring& wstrSrc, wstring& wstrNumber, wstring& wstrLeft, Point& NumberWH)
{
    CString  strFdata = wstrSrc.c_str();
    const int  strLen = strFdata.GetLength();
    int  numPos = 0;
    int charWidth = 0;
    int charInArrayIdx = 0;
    int drawWidth = 0;
    bool ifFindChar =false;

    for(int idx = 0; idx<strLen; idx++)         //逐个判断是哪个数字
    {
        if(strFdata[idx] == _T('.'))  //point
        {	            
            charInArrayIdx = E_NUM_POINT;
        }
        else if(strFdata[idx] == _T('-')) //负号
        {		
            charInArrayIdx = E_NUM_MINUS;
        }
        else
        {
            charInArrayIdx = strFdata[idx] - _T('0');
            if(charInArrayIdx >= E_NUM_MAX
                || charInArrayIdx < 0)  {
                    ifFindChar = true;
                    //wstrLeft = wstrSrc.substr(idx);
                    //wstrNumber = wstrSrc.substr(0, idx);
					wstrLeft = strFdata.Right(strLen - idx);
					wstrNumber = strFdata.Left(idx);

                    break;
            }
        }
        charWidth = m_vecImage[charInArrayIdx]->GetWidth();
        drawWidth += charWidth;
    }

    if(ifFindChar){}
    else{
        //all string is number
        wstrNumber = wstrSrc;
    }
    NumberWH.X = drawWidth;
    NumberWH.Y = m_vecImage[E_NUM_0]->GetHeight(); //height
    return true;
}

//////////////////////////////////////////////////////////////////////////
/**************************************************************
function:数字已经放在了m_numImage[12]中，根据数字贴出来。
parameter: crColor 数字颜色背景， nIDResource数字位图背景/
remark:数字可以用SetBkMode(TRANSPARENT);做成透明的，但是这样会造成数字在改变的时候产生遮盖。所以
采用了伪透明的方法，就是让数字的背景和背景一样。这样在改变时候看起来是透明，又不会产生遮盖。
validFigure,几位有效的小数，默认为1.
return int: actual draw width
if pGc == null, just mesure draw width
***************************************************************/
int UtilDrawNumber::DrawGreenNumber(Graphics* pGc, const wstring& wstr, int posX, int posY, wstring& wstrLeft)
{

    CString  strFdata;
    CString  str;
    int drawWidth = 0;

	wstring strcopy = wstr;

    //str.Format(_T(".%df"), validFigure);
    //strFdata.Format(_T("%") + str, fData);    //转成字符串
    strFdata = strcopy.c_str();

    const int  strLen = strFdata.GetLength();

    if(!m_vecImage[E_NUM_0]){
        ASSERT(false);
        return 0;
    }

    //const int charIntervalX = 8;
    //const int  bmpW = m_vecImage[E_NUM_0]->GetWidth()*strLen + charIntervalX;
    //const int  bmpH = m_vecImage[E_NUM_0]->GetHeight();
    //CBitmap  memBmp;
    //CDC      memDC;
    //memDC.CreateCompatibleDC(pDC);
    //memBmp.CreateCompatibleBitmap(pDC, bmpW, bmpH);
    //memDC.SelectObject(&memBmp);
    //memDC.FillSolidRect(CRect(0,0, bmpW, bmpH), RGB(255,255,255) ) ;

    PointF bitPos((float)posX, (float)posY);
    int  numPos = 0;
    int charWidth = 0;
    int charInArrayIdx = 0;
    for(int idx = 0; idx<strLen; idx++)         //逐个判断是哪个数字
    {
        if(strFdata[idx] == _T('.'))  //point
        {	            
            charInArrayIdx = E_NUM_POINT;
        }
        else if(strFdata[idx] == _T('-')) //负号
        {		
            charInArrayIdx = E_NUM_MINUS;
        }
        else
        {
            charInArrayIdx = strFdata[idx] - _T('0');
            if(charInArrayIdx >= E_NUM_MAX
                || charInArrayIdx < 0)  {
                    wstrLeft = strcopy.substr(idx);
                    break;
            }
        }
        //offset image 1st  //g.drawimage use top|right  align
        charWidth = m_vecImage[charInArrayIdx]->GetWidth();
        
        pGc->DrawImage(m_vecImage[charInArrayIdx], bitPos);
        bitPos.X += charWidth;  
        drawWidth += charWidth;

        ////test
        //Pen pen1( Color(255,0,0) );
        //Point pt0(bitPos.X, bitPos.Y);
        //Point pt1(bitPos.X, bitPos.Y+ 20);
        //Status st0 = graphics.DrawLine(&pen1, pt1, pt1);
        
    }
    // pDC->TransparentBlt(posX, posY, bmpW, bmpH, &memDC, 0, 0,bmpW, bmpH, RGB(255, 255, 255) );

    //graphics.ReleaseHDC(memDC.m_hDC);
    //memDC.SelectObject(pOldBmp);
    //memDC.DeleteDC();
    //if(nIDResource)
    //{
    //    memBmp.DeleteObject();
    //}

    return drawWidth;
}


