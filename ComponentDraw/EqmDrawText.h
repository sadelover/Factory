#ifndef EQMDrawTEXT_H__
#define EQMDrawTEXT_H__

#include "Tools/StringDef.h"
#include "EqmDrawBase.h"
#include "EqmDrawDefines.h"
#include "Tools/CustomTools/CustomTools.h"


class DLLEXPORT CEqmDrawText : public CEqmDrawBase
{
public:
	CEqmDrawText();
	CEqmDrawText(const Gdiplus::PointF& centerpos, int pageid, int id, int width,
		int height, int fontsize, CString fontstyle,  wstring strPhysicalID,
		const wstring& value, int color, TEXTSHOWMODE showmode, double dTransparent,
		int ifborder, int bordercolor, const CString strDevCfg, const bool bFaultDiag, const CString strBindScript);
	~CEqmDrawText();
public:

	int		GetFontSize()const;
	void	SetFontSize(int fontsize);

	CString		GetFontName() const;
	void	SetFontName(const CString& fontstyle);

	int		GetFontColor() const;
	void	SetFontColor(int color);

	wstring	GetInitialstring()const ;
	void	SetInitialString(const wstring& initialvalue);

	void    SetShowString(const wstring strShow, SHOWTYPE showtype );
	wstring	GetShowString()const ;

	TEXTSHOWMODE GetTextShowMode() const;
	void		 SetTextShowMode(TEXTSHOWMODE showmode);


	void	SetTransparence(double tranparence);
	double	GetTransparence() const;

	//////////////////////////////////////////////////////////////////////////
	int		GetBorderColor() const;
	void	SetBorderColor(int bordercolor);
	int    GetBorderType();
	void    SetBorderType(int borderis3d);

	bool	IsNoBorder();

	int     GetAlign() const;
	void    SetAlign(int align);

	//是否是值模式。
	bool IsValueMode() const;

public:
	virtual void Draw(Gdiplus::Graphics *pGraphics, Image *pBitmap = NULL);             //draw the equipment

	virtual bool PointInMe(const Gdiplus::PointF& actPt) const;        //check if the mouse point at the equipment

	virtual Direction PointIsSide(const Gdiplus::PointF& actPt) ;      //check if the mouse point at the side of equipment

	virtual void DrawSelectEdge(Gdiplus::Graphics *pGraphics);	 //Draw the select edge when the mouse selects the equipment

	virtual bool ChangeSize(const Gdiplus::PointF& actPt, Direction dir);      //change the equipment size

	virtual bool CopyProperty(CEqmDrawBase *pBase);
	virtual bool DoMoveEqmt(Gdiplus::PointF prePt, Gdiplus::PointF actPt); //移动设备
	virtual bool ClonePropertyByDest(CEqmDrawBase *pBase);
	virtual bool CopyInFormatBrush(CEqmDrawBase *pBase);
	virtual CEqmDrawText* Copy();

	bool IsSplashing() const;
	void SetSplashing(bool bSplash);

	bool ShiftEqmtsUp();        //按下向上键
	bool ShiftEqmtsDown();      //按下向下键
	bool ShiftEqmtsLeft();      //按下向左键
	bool ShiftEqmtsRight();     //按下向右键
	virtual bool						ShiftEqmtsCtrlUp();        
	virtual bool						ShiftEqmtsCtrlDown();      
	virtual bool						ShiftEqmtsCtrlLeft();      
	virtual bool						ShiftEqmtsCtrlRight(); 
	void	DrawString(Gdiplus::Graphics *pGraphics);
	void	DrawNumberWithUnit(Gdiplus::Graphics *pGraphics);

	void	DrawTextBorder(Gdiplus::Graphics* pGraphics, const CRect& boundrect);
	void	DrawTextBkImage(Gdiplus::Graphics* pGraphics, const CRect& boundrect);
	void			CreateFont();
	void			DeleteFont();
	void			CreateSolidBrush();
	void			DeleteSolidBrush();
	void			CreateHatchBrush();
	void			DeleteHatchBrush();
	void			CreateCommonBorderPen();
	void			DeleteCommonBorderPen();
	void			Create3DPenSunken();
	void			Delete3DPenSunken();
	void			Create3DPenHump();
	void			Delete3DPenHump();
	wstring			GetPhysicalID();
	void			SetPhysicalID(wstring strPhysicalID);

	void			SetTextModeShowString(const CString strText);
	const CString	GetTextModeShowString();

	void			SetPointNum(const int nNum);
	const int		GetPointNum();

	void			SetDateTime(const CString str);
	const CString   GetDateTime();

	void			SetDateTimePoint(const CString str);
	const CString   GetDateTimePoint();

	//bk type
	int		GetTextBKType() const;
	void	SetTextBKType(int nTextBKType);

	void	UpdatePos(void);
	void	ResetPos(void);

	TextAttri	GetTextAttribute(void) const;
	void		SetTextAttribute(const TextAttri& txtAttr);

	CString		GetDeviceCfg(void) const;
	void		SetDeviceCfg(const CString strDevCfg);

	CString		GetTemperCfg(void) const;
	void		SetTemperCfg(const CString strTemperCfg);

	bool		GetFaultDiagnosis(void) const;
	void		SetFaultDiagnosis(const bool bFaultDiag);

	CString        GetBindScript();
	void        SetBindScript(CString strBindScript);

	CString		GetUnit();
	void		SetUnit(CString cstrUnit);

private:
	int						m_fontsize;		//字体大小
	CString					m_fontname;	//字体风格
	wstring                 m_strPhysicalIDBind;
	wstring					m_strInitialValue;	//固定文本
	wstring					m_strShowValue;		//固定文本
	int						m_color;			//字体颜色
	TEXTSHOWMODE			m_showmode;		//显示模式	
	double					m_dTransparence;    //透明度
	int						m_bordertype;  //边框
	int						m_borderColor;  //边框背景色
	int						m_nTextBKType;	//背景框样式
	int						m_align; //left = 0, right 1 , hcenter = 2
	Gdiplus::Font*			m_pFont;
	Gdiplus::SolidBrush*	m_pSolidBrush;
	Gdiplus::HatchBrush*	m_pHatchBrush;
	Gdiplus::Pen*           m_pCommonBorderPen; 
	HPEN					m_hPen3DSunkenTopLeft;
	HPEN					m_hPen3DSunkenBottomRight;
	HPEN					m_hPen3DHumpTopLeft;
	HPEN					m_hPen3DHumpBottomRight;
	CString					m_strTextModeString;
	int						m_nPointNum;
	CString					m_strDateTime;
	CString					m_strDateTimePoint;
	int						m_nWordWidth;
	int						m_nWordHeight;
	TextAttri				m_textAttri;
	CString					m_strDeviceCfg;
	CString					m_strTemperCfg;
	bool					m_bFaultDiagnosis;
	CString                 m_strBindScript;                    
	CString					m_cstrUnit;

	Gdiplus::PointF	m_pos_;
	Gdiplus::PointF	m_posru_;		//右上
	Gdiplus::PointF	m_posld_;		//左下
	Gdiplus::PointF	m_posrd_;		//右下
};

#endif