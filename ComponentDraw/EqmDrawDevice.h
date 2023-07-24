/*
* Copyright (c) 2001,上海雁文智能科技有限公司
* All rights reserved.
* 
* 文件名称：EqmDrawDevice.h
* 文件标识：
* 摘    要：自定义图元绘图服务函数
* 
* 取代版本：1.0 
* 原作者  ： ****
* 完成日期：0000年0月0日
*/
#ifndef EQMDRAWDEVICE_H__
#define EQMDRAWDEVICE_H__

#include "EqmDrawBase.h"
#include <vector>
#include <string>
#include <set>

using namespace std;

#pragma warning(disable:4305)
#pragma warning(disable:4018)
#pragma warning(disable:4715)										//默认阀门----关

enum DeviceTypeForMeta
{
	enumChiller = 0,//1
	enumPump,//6
	enumCoolingPump,
	enumCoolingPump1,
	enumCoolingPump2,
	enumWarmingPump1,
	enumWarmingPump2,
	enumChillerValve,
	enumCTValve,
	enumGroundValve,
	enumCoolingTower,//1
	enumNull,
	enumCommonValve,
};

enum MouseEventTypeForMeta
{
	enumDevicePropertyWnd = 0,
	enumSettingWnd,
	enumChangeValueWnd,
	enumNullWnd,
	enumJumpPage,
	enumJumpOutsideLink
};

typedef struct _tagEventInfo
{
	_tagEventInfo()
	{
		strSettingPointName = L"";
		deviceType = enumNull;
		mouseEventType = enumNullWnd;
		nDeviceID = 0;
		nPageID = -1;
		nValue = -99;
		strShowName = L"";
		strOutLink = L"";
	}
	CString strSettingPointName;
	DeviceTypeForMeta deviceType;
	MouseEventTypeForMeta mouseEventType;
	int nDeviceID;
	int nPageID;
	int nValue;
	CString strShowName;
	CString strOutLink;
}EventInfo;

typedef struct _tagBindInfo
{
	_tagBindInfo()
	{
		nPicID = 0;
		nValue = 0;
		nFrame = 0;
		nInterval = 0;
	}
	int nPicID;
	int nValue;
	int nFrame;
	int nInterval;
}BindInfo;

typedef struct _tagPlayInfo
{
	_tagPlayInfo()
	{
		nValue = 0;
		nFrame = 0;
		bPlay = false;
		nInterval = 0;
	}
	bool bPlay;
	int nValue;
	int nFrame;
	int nInterval;
	vector<Image*> vecBmp;
}PlayInfo;

typedef struct _tagNameValue
{
	_tagNameValue()
	{
		nLastValue = 0;
		bChanged = false;
	}
	wstring strPointName;
	int nLastValue;
	bool bChanged;
}NameValue;

class DLLEXPORT CEqmDrawDevice : public CEqmDrawBase
{
public:
	CEqmDrawDevice();
	CEqmDrawDevice(	EQMDRAWTYPE type, const Gdiplus::PointF& centerpos, int pageid,int id, int datacomid, int rotateangle, wstring strEqmName, EQUIPMENTSUBTYPE nPumpType, int width, string strPointList,int height, double dStepInterval, double dUpperLimit, double dDownLimit, int nBindType);
	~CEqmDrawDevice();
public:
	int							GetRotateAngle() const; //bird int 3.1
	void						SetRotateAngle(int angle);
	wstring						GetEqmName() const;
	void						SetEqmName(const wstring& strEqmName);
    virtual void				SetEqmType(EQMDRAWTYPE type);
	EQUIPMENTSUBTYPE			GetEquipmentSubType() const;
	void						SetEquipmentSubType(EQUIPMENTSUBTYPE subtype);
	double						GetStepInterval();
	void						SetStepInterval(double stepInterval);
	bool						GetIsClicked();
	void						SetIsClicked(bool bIsClicked);
	void						SetUpperLimit(double dUpperLimit);
	double						GetUpperLimit();
	void						SetDownLimit(double dDownLimit);
	double						GetDownLimit();
	virtual void				Draw(Gdiplus::Graphics *pGraphics, Image *pBitmap = NULL);             //draw the equipment
	virtual bool				PointInMe(const Gdiplus::PointF& actPt) const;        //check if the mouse point at the equipment
	virtual Direction			PointIsSide(const Gdiplus::PointF& actPt) ;      //check if the mouse point at the side of equipment
	virtual void				DrawSelectEdge(Gdiplus::Graphics *pGraphics);	 //Draw the select edge when the mouse selects the equipment
	virtual bool				ChangeSize(const Gdiplus::PointF& actPt, Direction dir);//change the equipment size

	virtual bool				LeftRotate90();//左旋转90度
	virtual bool				RightRotate90();//右旋转90度
	virtual bool				ShiftEqmtsUp();        //按下向上键
	virtual bool				ShiftEqmtsDown();      //按下向下键
	virtual bool				ShiftEqmtsLeft();      //按下向左键
	virtual bool				ShiftEqmtsRight();     //按下向右键
	virtual bool						ShiftEqmtsCtrlUp();        
	virtual bool						ShiftEqmtsCtrlDown();      
	virtual bool						ShiftEqmtsCtrlLeft();      
	virtual bool						ShiftEqmtsCtrlRight();  
	virtual bool				CopyProperty(CEqmDrawBase *pBase);
	virtual bool				ClonePropertyByDest(CEqmDrawBase *pBase);		//克隆目标的所有属性
	virtual bool				CopyInFormatBrush(CEqmDrawBase *pBase);
	virtual CEqmDrawDevice*		Copy();

	void						RotetaBitMap(Image *pBitMap = NULL);  //must call reset rotate
    void						ResetRotateBmp(Image *pBitMap = NULL);	

	void						SetDeviceValue(const int nValue);
	const int					GetDeviceValue();
	void						SetEventInfo(const CString& strName, const DeviceTypeForMeta deviceType, const MouseEventTypeForMeta mouseEventType, const int nDeviceID, const int nPageID, const int nValue, const CString strShowName, const CString strOutLink);
	EventInfo&					GetEventInfo();

    int                         GetBindType();
	void                        SetBindType(int nBindType);

	void						ParseEvent(const CString& strEvent);
	void						SetDefaultValue();
	void						AddBindInfo(const int nValue, const int nFrame, const int nInterval, const int nPicID);
	void						MakePlayInfo();
	void						ParseParam10(const CString& strParam10);
	void						ParseParam10(const CString& strParam10,const int nOffSet);			//导入偏移
	PlayInfo&					GetPlayInfo(const int nValue, bool& bOK);

	void						SetBindString(const wstring strBind);
	wstring						GetBindString();
	void						SetBitmap(Image* pBitmap);
	Image*						GetBitmap();
	void						SetLinkPageID(const int nPageID);
	int							GetLinkPageID();
	wstring                     GetOutsideLink();
	void                        SetOutsideLink(wstring wstrLink);
	void						SetPicID(const int nPicID);
	int							GetPicID();
	vector<BindInfo>			GetBindInfoVec();
	void						SetBindInfoVec(vector<BindInfo> vec);



	void						ClearDiagnoseVec();
	void						InsertDiagnoseVec(const int nPageID, const CString strPageName);
	const vector<PageIDName>&	GetDiagnosePageVec();
	void						SetDiagnosePageVec(const vector<PageIDName>& vec);

	void						InitDiagnoseVec(const int nItemCount);
	void						InsertDiagnosePageItemName(const int nItem, const NameValue st);
	void						ClearDiagnosePageItemVec();
	vector<vector<NameValue>>&  GetDiagnosePageItemVec();
	void						SetDiagnosePageItemVec(vector<vector<NameValue>>& vec);
	const bool					IsDiagnoseAnimation();
	const int					GetDiagnosePageIndex(const int nPageID);

	void						SetImageMetaType(const bool bIsPic);
	bool						GetImageMetaType(void) const;


public:
	MouseEventTypeForMeta		GetEventType() const;
	bool						IsEventEmpty() const;
	const bool					IsForDiagnose() const;
	vector<PlayInfo>			m_vecPlayInfo;
	vector<BindInfo>			m_vecBindInfo;
	EventInfo					m_eventInfo;
	vector<bool>				m_bChangedVec;
protected:
	int							m_fRotateAngel;
	wstring						m_strEqmName;
	EQUIPMENTSUBTYPE			m_subtype;  //设备子类型
	int							m_nStateCount; //the state count of the equipment
	vector<Gdiplus::PointF>		m_vecPortPoints; // the port points
	double						m_dStepInterval;
	bool						m_bIsClicked;
	double						m_dUpperLimit;//上限值
	double						m_dDownLimit;//下限值
	int							m_base_height;
	int							m_base_width ;
	float						m_nscale;
	int							m_nValue;
	int							m_nTimeTotal;
	wstring						m_bindString;
	Image*						m_pBmpName;
	int							m_nPicID;
	vector<PageIDName>			m_vecDiagnosePage;
	int							m_nLinkPageID;
	vector<vector<NameValue>>	m_strDiagnoseItemPointNameVec;
	bool						m_bIsPicture;
	wstring						m_strOutsideLink;  //外链地址


    int                     m_nBindType;
};
#endif