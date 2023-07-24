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
	virtual bool					ClonePropertyByDest(CEqmDrawBase *pBase);		//��¡Ŀ�����������
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

	COLORREF						m_nTextColor;//������ɫ
	int								m_nTextSize;//���ִ�С
	CString							m_strTextContent;//button��ʾ������
	eButtonStyle					m_eButtonStyle;//�ؼ�����
	CString							m_strSettingValueForUncheck;//��ѡ�е��趨ֵ
	CString							m_strSettingValueForCheck;//ѡ�е��趨ֵ
	CString							m_strBindPointName;//�󶨵�λ����
	int								m_nGroupID;//������
	CString							m_strEnableExpression;//��Ч״̬���ʽ
	CString							m_strPathUncheckCom;//��ѡ�е�Ĭ��ͼ
	CString							m_strPathCheckCom;//ѡ�е�Ĭ��ͼ
	CString							m_strPathUncheckOver;//��ѡ�е�overͼ
	CString							m_strPathCheckOver;//ѡ�е�overͼ
	CString							m_cstrOptionsDec;
};

