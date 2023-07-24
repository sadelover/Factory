#pragma once
#include "eqmdrawbase.h"

struct _tagDiagnoseItemPlus 
{
	_tagDiagnoseItemPlus()
	{
		color = RGB(255,255,255);
	};
	CString								strItemName;
	CString								strPointName;
	COLORREF							color;
};

struct _tagBoolBindInfo
{
	CString								strBoolPointName;
	CString								strBoolPointDescription;
	vector<_tagDiagnoseItemPlus>		vecDiagnoseItemPlus;
	CString								strResultPointName;
	CString								strResultPointName2;
	CString								strResultPointName3;
	CString								strTreatmentStatePointName;
};

class DLLEXPORT CEqmDrawDiagnoseGraphicPlus : public CEqmDrawBase
{
public:
	CEqmDrawDiagnoseGraphicPlus(void);
	~CEqmDrawDiagnoseGraphicPlus(void);
	virtual void Draw(Gdiplus::Graphics *pGraphics, Image *pBitmap = NULL);
	virtual CEqmDrawDiagnoseGraphicPlus*		Copy();
	virtual bool						PointInMe(const Gdiplus::PointF& actPt) const;        //check if the mouse point at the equipment
	virtual Direction					PointIsSide(const Gdiplus::PointF& actPt);      //check if the mouse point at the side of equipment
	virtual void						DrawSelectEdge(Gdiplus::Graphics *pGraphics);	 //Draw the select edge when the mouse selects the equipment
	virtual bool						ChangeSize(const Gdiplus::PointF& actPt, Direction dir);//change the equipment size
	virtual bool						CopyProperty(CEqmDrawBase *pBase);
	virtual bool						ClonePropertyByDest(CEqmDrawBase *pBase);		//克隆目标的所有属性
	void								SetDefaultBitmap(Image* pBitmap);
	Image*								GetDefaultBitmap();
	virtual bool						ShiftEqmtsUp();       
	virtual bool						ShiftEqmtsDown();     
	virtual bool						ShiftEqmtsLeft();     
	virtual bool						ShiftEqmtsRight();  
	virtual bool						ShiftEqmtsCtrlUp();        
	virtual bool						ShiftEqmtsCtrlDown();      
	virtual bool						ShiftEqmtsCtrlLeft();      
	virtual bool						ShiftEqmtsCtrlRight(); 

	void								SetTimeScope(const int scope);
	const int							GetTimeScope();

	void								SetTimeInterval(const int interval);
	const int							 GetTimeInterval();

	void								SetExtendTimeSpan(const int span);
	const int							GetExtendTimeSpan();

	void								SetNormalPicID(const int nID);
	const int							GetNormalPicID();

	void								SetAnimationOrPicID(const int nID);
	const int							GetAnimationOrPicID();

	void								SetLinkPageID(const int nID);
	const int							GetLinkPageID();

	void								SetIsAnimation(const int nIsAnimation);
	const int							GetIsAnimation();

	void								SetShowMode(const int nShowMode);//0为缩略,1为详细
	const int							GetShowMode();

	void								DeleteAllVecInfo();
	void								InsertOneBoolBindInfo(const _tagBoolBindInfo boolBindInfo);
	void								DeleteOneBoolBindInfo(const CString strBoolPointName);
	void								InsertOneSubItem(const CString strBoolPointName, const _tagDiagnoseItemPlus item);
	void								DeleteOneSubItem(const CString strBoolPointName, const CString strItemName);
	vector<_tagBoolBindInfo>&			GetBoolBindInfoVec();
	void								SetBoolBindInfoVec(vector<_tagBoolBindInfo>& vec);
	vector<_tagDiagnoseItemPlus>&		GetSubItemVec(const CString strBindBoolPointName);

private:
	int									m_nUpdateInterval;
	Image*								m_pBitmap;
	int									m_base_height;
	int									m_base_width ;
	float								m_nscale;
	//属性变量
	int									m_nScope;//单位：天
	int									m_nInterval;//单位：分钟
	int									m_nTimeExtendSpan;//单位：天
	int									m_nNormalPicID;
	int									m_nAnimationOrPicID;
	int									m_nLinkPageID;
	int									m_nIsAnimation;
	int									m_nShowMode;
	vector<_tagBoolBindInfo>			m_vecBoolBind;
};

