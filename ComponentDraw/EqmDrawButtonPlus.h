#pragma once
#include "eqmdrawbase.h"


enum eButtonStyle
{
	E_RADIO_BUTTON = 0,
	E_CHECK_BUTTON,
};

class DLLEXPORT CEqmDrawButtonPlus :
	public CEqmDrawBase
{
public:
	CEqmDrawButtonPlus(void);
	~CEqmDrawButtonPlus(void);
	virtual void Draw(Gdiplus::Graphics *pGraphics, Image *pBitmap = NULL);
	virtual CEqmDrawButtonPlus*			Copy();
	virtual bool					PointInMe(const Gdiplus::PointF& actPt) const;        //check if the mouse point at the equipment
	virtual Direction				PointIsSide(const Gdiplus::PointF& actPt);      //check if the mouse point at the side of equipment
	virtual void					DrawSelectEdge(Gdiplus::Graphics *pGraphics);	 //Draw the select edge when the mouse selects the equipment
	virtual bool					ChangeSize(const Gdiplus::PointF& actPt, Direction dir);//change the equipment size
	virtual bool					CopyProperty(CEqmDrawBase *pBase);
	virtual bool					ClonePropertyByDest(CEqmDrawBase *pBase);		//克隆目标的所有属性
	void							SetDefaultBitmap(Image* pBitmap);
	Image*							GetDefaultBitmap();
	virtual bool					ShiftEqmtsUp();       
	virtual bool					ShiftEqmtsDown();     
	virtual bool					ShiftEqmtsLeft();     
	virtual bool					ShiftEqmtsRight(); 
	virtual bool					ShiftEqmtsCtrlUp();        
	virtual bool					ShiftEqmtsCtrlDown();      
	virtual bool					ShiftEqmtsCtrlLeft();      
	virtual bool					ShiftEqmtsCtrlRight();  

	void							SetTextColor(const COLORREF color);
	void							SetTextSize(const int nSize);
	void							SetTextContent(const CString strContent);
	void							SetButtonStyle(const eButtonStyle style);
	void							SetValueForUnCheck(const CString strValue);
	void							SetValueForCheck(const CString strValue);
	void							SetBindPointName(const CString strPointName);
	void							SetGroupID(const int nGroupID);
	void							SetExpression(const CString strExpression);
	void							SetPathUncheckCom(const CString strFullPath);
	void							SetPathUncheckOver(const CString strFullPath);
	void							SetPathCheckCom(const CString strFullPath);
	void							SetPathCheckOver(const CString strFullPath);
	void							SetOptionsDec(const CString cstrOptionsDec);

	const COLORREF					GetTextColor();
	const int						GetTextSize();
	const CString					GetTextContent();
	const eButtonStyle				GetButtonStyle();
	const CString					GetValueForUnCheck();
	const CString					GetValueForCheck();
	const CString					GetBindPointName();
	const int						GetGroupID();
	const CString					GetExpression();
	const CString					GetPathUncheckCom();
	const CString					GetPathUncheckOver();
	const CString					GetPathCheckCom();
	const CString					GetPathCheckOver();
	const CString					GetExpressLeftOp();
	const CString					GetExpressRightOp();
	const CString					GetExpressMidOp();
	const CString					GetOptionsDec();
	const bool						CheckExpression(const CString strExpression);

private:
	Image*							m_pBitmap;
	int								m_base_height;
	int								m_base_width ;
	float							m_nscale;

	COLORREF						m_nTextColor;//文字颜色
	int								m_nTextSize;//文字大小
	CString							m_strTextContent;//button显示的内容
	eButtonStyle					m_eButtonStyle;//控件类型
	CString							m_strSettingValueForUncheck;//非选中的设定值
	CString							m_strSettingValueForCheck;//选中的设定值
	CString							m_strBindPointName;//绑定点位名称
	int								m_nGroupID;//分组编号
	CString							m_strEnableExpression;//有效状态表达式
	CString							m_strPathUncheckCom;//非选中的默认图
	CString							m_strPathCheckCom;//选中的默认图
	CString							m_strPathUncheckOver;//非选中的over图
	CString							m_strPathCheckOver;//选中的over图
	CString							m_cstrOptionsDec;
};

