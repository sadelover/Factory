
/*
  This class is the base class of the objects to be drawed and displayed in the 
  monitor application

  Author: Guibicheng
  Dateb:  2010.10.28
*/

#ifndef EQMDRAWBASE_H__
#define EQMDRAWBASE_H__
#pragma once

#include "Tools/DllDefine.h"
#include "EqmDrawDefines.h"

#include "Tools/EquipmentStatusDefine.h"


#define DEFAULTCOMID -1

#define SCALE_MAX	5
#define SCALE_MIN	0.1

#pragma warning(disable:4305) 
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
class ComponentDraw;

class DLLEXPORT CEqmDrawBase
{
public:
	//Default Constructor
	CEqmDrawBase();
	CEqmDrawBase(EQMDRAWTYPE type, const Gdiplus::PointF& centerpos, int pageid, int ident, int width = 0, int height = 0, UINT layer = E_LAYER_10);
	virtual ~CEqmDrawBase();

public:
	// center pos set/get
	virtual const Gdiplus::PointF&	    GetPos() const;
	virtual void	                    SetPos(const Gdiplus::PointF& pos);
	// type set/get
	virtual EQMDRAWTYPE					GetEqmType() const;
	virtual void						SetEqmType(EQMDRAWTYPE type);
	//page id set/get
	virtual int							GetPageID() const;
	virtual void						SetPageID(int pageid);
	// id	set/get
	virtual int							GetID() const;
	virtual void						SetID(int ID);
	// main align get/set
	virtual bool						IsMainAlign();
	virtual void						SetMainAlign(bool bIsMainAlign);
	// selected get/set
	virtual bool						IsSelected();
	virtual void						SetSelected(bool bSelected);
	//width get/set
	virtual int							GetWidth() const;
	virtual void						SetWidth(int width);
	// height get/set
	virtual int							GetHeight() const;
	virtual void						SetHeight(int height);
	//is need to show --add by juneay 2010-12-13
	virtual bool						GetShow();
	virtual void						SetShow(bool bShow);
    UINT								GetLayer() const;       //bird add 20111008
    void								SetLayer(UINT layer);

	UINT								    GetStyle() const;       //bird add 20111008
	void								SetStyle(UINT nStyle);
	// true Ϊtop layer, false ��ʾbottom layer.
	bool								IsTopLayer() const;
    virtual int							GetDataComID() const;
    virtual void						SetDataComID(int id);
	virtual wstring						GetPhysicalID() const;
	void								SetPhysicalID(const wstring strID);
	virtual CEqmDrawBase*				Copy() = 0;

public:
	//action function
	virtual void						Draw(Gdiplus::Graphics *pGraphics, Image *pBitmap = NULL) = 0;             //draw the equipment
	virtual bool						PointInMe(const Gdiplus::PointF& actPt) const = 0;        //check if the mouse point at the equipment
	virtual Direction					PointIsSide(const Gdiplus::PointF& actPt)  = 0;      //check if the mouse point at the side of equipment
	virtual void						DrawSelectEdge(Gdiplus::Graphics *pGraphics) = 0;	 //Draw the select edge when the mouse selects the equipment
	virtual bool						ChangeSize(const Gdiplus::PointF& actPt, Direction dir) = 0;//change the equipment size
	virtual bool						ChangeSizeByOriginal(const Gdiplus::PointF& actPt, Direction dir,const Gdiplus::PointF& prePt,int nWidth, int nHeight);//change the equipment size
	virtual bool						CopyProperty(CEqmDrawBase *pBase) = 0;
	virtual bool						ClonePropertyByDest(CEqmDrawBase *pBase);		//��¡Ŀ�����������
	virtual bool						CopyInFormatBrush(CEqmDrawBase *pBase);		//��ʽˢ��
	virtual bool						LeftRotate90();//����ת90��
	virtual bool						RightRotate90();//����ת90��
	virtual bool						ShiftEqmtsUp();        //�������ϼ�
	virtual bool						ShiftEqmtsDown();      //�������¼�
	virtual bool						ShiftEqmtsLeft();      //���������
	virtual bool						ShiftEqmtsRight();     //�������Ҽ�
	virtual bool						ShiftEqmtsCtrlUp();        
	virtual bool						ShiftEqmtsCtrlDown();      
	virtual bool						ShiftEqmtsCtrlLeft();      
	virtual bool						ShiftEqmtsCtrlRight();     
	virtual bool						DoMoveEqmt(Gdiplus::PointF prePt, Gdiplus::PointF actPt); //�ƶ��豸
	virtual bool						IfMeInRect(Gdiplus::RectF rect0, Gdiplus::RectF rectItem);
	virtual bool						IfMeContainRect(Gdiplus::RectF rect0, Gdiplus::RectF rectItem);
	virtual Gdiplus::PointF				GetPortPoint(Gdiplus::PointF actPt); //Get the equipment port
	virtual void						DrawPort(Gdiplus::Graphics *pGraphics);  //draw port
	double								GetDimension(int x1, int y1, int x2, int y2);
	void								ParsePointListFromString(const string& str, vector<Gdiplus::PointF> &vecPoint) const; //bird change  3.3
	void								ParsePointListToString(const vector<Gdiplus::PointF>& vecPoint, string & str) const;
	Gdiplus::PointF						Rotate(int x, int y, int x0, int y0, double angle);  //rotate the point
    virtual void						SetCurrentFrame(int cur);   //add 10.9
    virtual int							GetCurrentFrame();
    virtual void						SetNextFrame();
	void								FastDrawLines(HDC hdc, HPEN hpen, const vector<CPoint>& pointlist);
	void								FastDrawLine(HDC hdc, HPEN hpen, CPoint pointfrom, CPoint pointto);
	CPoint								PointFToCPoint(Gdiplus::PointF pointf);
	void								SetNeedSave(const bool bNeed);
	const bool							GetNeedSave();
	bool								CopyBaseProperty(CEqmDrawBase *pBase);				//����һЩ��������
	void								SetDeleteFlag(const bool bDeleteFlag);
	bool								GetDeleteFlag(void);
protected:

	//���Ͻ�λ��
	Gdiplus::PointF	m_pos;
	Gdiplus::PointF	m_posru;		//����
	Gdiplus::PointF	m_posld;		//����
	Gdiplus::PointF	m_posrd;		//����
	double			m_dScale;		//�����

	//����
	EQMDRAWTYPE		m_eEqmType;
	
	//����ID
	int				m_iPageID;

	//�����ڻ����е�ID
	int				m_iID;

    int				m_iDataComID;
	
	// �Ƿ��������豸
	bool            m_bIsMainAlign; 
	
	// �Ƿ�ѡ��
	bool            m_bIsSelect;    

	//�豸���
	int             m_nWidth;

	//�豸�߶�
	int             m_nHeight;

	//�Ƿ���ʾ
	bool            m_bShow;

    UINT            m_layer;   //��, Ĭ��Ϊ1, 0��Ϊ��ײ�
	UINT            m_Style;

    UINT            m_frameCurrrent; //for gif
    UINT            m_frameMax;

    //״̬
    EQUIPMENT_STATUS	m_eState;
	DWORD				m_advancedstate;

	wstring         m_strPhysicalID;
	bool            m_bNeedSave;
	bool			m_bDeleteFlag;//true:��Ҫ��ɾ��,false������Ҫɾ��
public:
};

#endif
