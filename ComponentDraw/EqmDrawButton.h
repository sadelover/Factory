#pragma once
#include "eqmdrawbase.h"

struct _tagRelationExpress
{
	_tagRelationExpress()
	{
		strPointName = L"";
		nRelationExpress = 0;
		fResult = 0.0;
	};
	CString strPointName;
	int     nRelationExpress;//0:等于,1:大于,2:小于,3:大于等于,4:小于等于,5:不等于
	double		fResult;
};

struct _tagButtonStatus
{
	_tagButtonStatus()
	{
		nRelationStatus = 0;
	};
	vector<_tagRelationExpress> vecExpress;
	int nRelationStatus;//0:与,1:或
};

class DLLEXPORT CEqmDrawButton :
	public CEqmDrawBase
{
public:
	CEqmDrawButton(void);
	~CEqmDrawButton(void);
	virtual void Draw(Gdiplus::Graphics *pGraphics, Image *pBitmap = NULL);
	virtual CEqmDrawButton*			Copy();
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
	virtual bool						ShiftEqmtsCtrlUp();        
	virtual bool						ShiftEqmtsCtrlDown();      
	virtual bool						ShiftEqmtsCtrlLeft();      
	virtual bool						ShiftEqmtsCtrlRight();  

	void							Set5PicID(const int nIDPicComm, const int nIDPicDisable, const int nIDPicDown, const int nIDPicOver, const int nIDPicChecked);
	void							SetLinkPageID(const int nLinkPageID);

	void							Get5PicID(int& nIDPicComm, int& nIDPicDisable, int& nIDPicDown, int& nIDPicOver, int& nIDPicChecked);
	const int 						GetLinkPageID();

	void							SetBindSetPointName(const CString strName);
	const CString					GetBindSetPointName();

	//void							SetSettingValue(const int nValue);
	/*const int CEqmDrawButton::GetSettingValue()*/
	void							SetSettingValue(const CString strValue);
	const CString					GetSettingValue();

	void							SetTextContent(const CString strText);
	const CString					GetTextContent();

	void							SetTextColor(const COLORREF nColor);
	const COLORREF					GetTextColor();

	void							SetTextSize(const int nSize);
	const int						GetTextSize();

	void							SetOperation(const CString strOperation);
	const CString					GetOperation();

	void							InsertExpress(const _tagRelationExpress express);
	void							DeleteExpress(const CString strPointName);
	void							SetRelation(const int nRelation);
	const int						GetRelation();
	vector<_tagRelationExpress>&		GetRelationVec();
	void							SetRelationVec(vector<_tagRelationExpress>& vec);
	void							DeleteAllRelation();

	void							SetDownloadEnable(const CString strVal);
	CString							GetDownloadEnable(void) const;

	void							SetDownloadName(const CString strVal);
	CString							GetDownloadName(void) const;

private:
	Image*							m_pBitmap;
	int								m_base_height;
	int								m_base_width ;
	float							m_nscale;

	int								m_nIDPicComm;
	int								m_nIDPicOver;
	int								m_nIDPicDown;
	int								m_nIDPicDisable;
	int								m_nIDPicChecked;
	int								m_nLinkPageID;
	CString							m_strBindSetPointName;
	/*=================2019-9-11========================*/
	/*int								m_nSettingValue;*/
	CString							m_strSettingValue;
	CString							m_strTextContent;
	COLORREF						m_nTextColor;
	int								m_nTextSize;

	CString							m_strOperationContent;

	_tagButtonStatus				m_vecRelation;

	CString							m_strDownloadEnable;
	CString							m_strDownloadName;
};

