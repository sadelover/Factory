#pragma once
#include "eqmdrawbase.h"


//typedef enum tagFaceType
//{
//	east	= 0,
//	west,
//	south,
//	north
//}FaceType;
//
//class DLLEXPORT CCoorInfo
//{
//public:
//	CCoorInfo(void);
//	~CCoorInfo(void);
//	CString		GetCoordinateName(void) const;
//	FaceType	GetFaceType(void) const;
//	double		GetValue(void) const;
//
//	void		SetCoordinate(const FaceType& factType, const double dValue);
//
//private:
//	FaceType	m_enumFaceType;
//	double		m_dValue;
//};

class DLLEXPORT CMapPageInfo
{
public:
	CMapPageInfo(void);
	~CMapPageInfo(void);

	//CCoorInfo GetLongitude(const int nNum) const;
	//CCoorInfo GetLatitude(const int nNum) const;
	int		GetPageId(void) const;
	//CString GetLongitudeName(const int nNum) const;
	//CString GetLatitudeName(const int nNum) const;
	double	GetLongitudeValue(const int nNum) const;
	double	GetLatitudeValue(const int nNum) const;

	//void	SetLongitude(const int nNum, const CCoorInfo corVal);
	//void	SetLatitude(const int nNum, const CCoorInfo corVal);
	void	SetPageId(const int nPageId);
	//void	SetLongitudeFromName(const int nNum, const CString strLongitudeName);
	//void	SetLatitudeFromName(const int nNum, const CString strLatitudeName);
	void	SetLongitudeValue(const int nNum, const double dVal);
	void	SetLatitudeValue(const int nNum, const double dVal);

private:
	//CCoorInfo	m_longitude1;
	//CCoorInfo	m_longitude2;
	//CCoorInfo	m_latitude1;
	//CCoorInfo	m_latitude2;

	int			m_nPageId;
	double		m_dLongitude1;
	double		m_dLongitude2;
	double		m_dLatitude1;
	double		m_dLatitude2;
public:
    CString     m_strEquipName;
	CString     m_strRoomName;
	CString     m_strEquipType;
	//int         m_nEquipNo;
	

};


class DLLEXPORT CEqmDrawMapNavigate : public CEqmDrawBase
{
public:
	CEqmDrawMapNavigate(void);
	~CEqmDrawMapNavigate(void);

public:
	virtual CEqmDrawMapNavigate*	Copy(void);
	virtual void					Draw(Gdiplus::Graphics *pGraphics, Image *pBitmap = NULL);
	virtual bool					PointInMe(const Gdiplus::PointF& actPt) const;	//check if the mouse point at the equipment
	virtual Direction				PointIsSide(const Gdiplus::PointF& actPt);		//check if the mouse point at the side of equipment
	virtual void					DrawSelectEdge(Gdiplus::Graphics *pGraphics);	//Draw the select edge when the mouse selects the equipment
	virtual bool					ChangeSize(const Gdiplus::PointF& actPt, Direction dir);//change the equipment size
	virtual bool					CopyProperty(CEqmDrawBase *pBase);
	virtual bool					ClonePropertyByDest(CEqmDrawBase *pBase);
	void							SetDefaultBitmap(Image* pBitmap);
	Image*							GetDefaultBitmap();
	virtual bool					ShiftEqmtsUp(void);
	virtual bool					ShiftEqmtsDown(void);
	virtual bool					ShiftEqmtsLeft(void);
	virtual bool					ShiftEqmtsRight(void);
	virtual bool					ShiftEqmtsCtrlUp(void);
	virtual bool					ShiftEqmtsCtrlDown(void);
	virtual bool					ShiftEqmtsCtrlLeft(void);
	virtual bool					ShiftEqmtsCtrlRight(void);

	void							SetScaleRuler(const int nScaleRuler);
	void							SetMapPageInfo(const vector<CMapPageInfo>& vecMapPageInfo);

	int								GetPicId(void) const;
	int								GetScaleRuler(void) const;
	vector<CMapPageInfo>			GetMapPageInfo(void) const;

	void                            SetEquipName(const CString strEquipName);
	CString                         GetEquipName(void)const;

	void                            SetRoomName(const CString strRoomName);
	CString                         GetRoomName(void)const;

	void                            SetEquipType(const int nEquipType);
	int                             GetEquipType(void)const;

	void                            SetEquipNo(const int nEquipNo);
	int                             GetEquipNo(void)const;

	void                            SetEquipStyle(const int nEquipStyle);
	int                             GetEquipStyle(void)const;

	void                            SetEquipTemplatePageId(const int nID);
	int                             GetEquipTemplatePageId(void)const;

	void                            SetEquipTemplatePageName(CString strName);
	CString                         GetEquipTemplatePageName(void)const;

	void                            SetEquipParamList(vector<CString> paramList);
	vector<CString>                 GetEquipParamList(void)const;

	void                            SetEquipTemplateGroupId(const int nID);
	int                             GetEquipTemplateGroupId(void)const;

	void                            SetTemplateName(const CString cstrTemplateName);
	CString                         GetTemplateName(void)const;

	void							SetBanOnDisplay(const bool bEnable);
	bool							GetBanOnDisplay(void)const;

	CString  GetParamListAllString();


	void SetParamListAllString(CString strAll);


	void RotetaBitMap( Image *pBitMap /*= NULL*/ );

	void ResetRotateBmp(Image *pBitMap );

	int GetRotateAngle();

	void SetRotateAngle( int angle );

	virtual bool				LeftRotate90();//左旋转90度
	virtual bool				RightRotate90();//右旋转90度

private:
	Image*							m_pBitmap;
	int								m_nBaseHeight;
	int								m_nBaseWidth;
	float							m_fscale;
	int								m_nScaleRuler;
    vector<CMapPageInfo>			m_vecMapPageInfo;
    CString     m_strEquipName;
	CString     m_strRoomName;
	int         m_nEquiptype;
	int         m_nEquipNo;
	int         m_nEquipStyle;

	int      m_nTemplateGroupId;
	int      m_nTemplatePageId;
	CString  m_strTemplatePageName;
	vector<CString> m_strParamList;

	int							m_fRotateAngel;
	CString		m_cstrTemplateName;
	bool m_bBanOnDispay;
};

