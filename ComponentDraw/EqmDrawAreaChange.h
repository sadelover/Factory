
#ifndef EQMDRAWAREACHANGE_H__
#define EQMDRAWAREACHANGE_H__

#include "EqmDrawBase.h"
#include <vector>
#include <string>
#include <set>

using namespace std;

#pragma warning(disable:4305)
#pragma warning(disable:4018)
#pragma warning(disable:4715)									

typedef struct _tagAreaInfo
{
	_tagAreaInfo()
	{
		BnmID = 0;
		nPageID = 0;
	}
	CString	strPointList;
	int		BnmID;
	Image*	pBmpName;
	int		nPageID;
}AreaInfo;

class DLLEXPORT CEqmDrawAreaChange : public CEqmDrawBase
{
public:
	virtual ~CEqmDrawAreaChange();
	CEqmDrawAreaChange();
	CEqmDrawAreaChange(EQMDRAWTYPE type, const Gdiplus::PointF& centerpos, int pageid, 
		int id, int width = 0, int height = 0);
public:
	//
	int							GetRotateAngle() const; //bird int 3.1
	void						SetRotateAngle(int angle);

	virtual void				SetEqmType(EQMDRAWTYPE type);
	
	virtual void				Draw(Gdiplus::Graphics *pGraphics, Image *pBitmap = NULL);             //draw the equipment
	virtual bool				PointInMe(const Gdiplus::PointF& actPt) const;        //check if the mouse point at the equipment
	virtual Direction			PointIsSide(const Gdiplus::PointF& actPt) ;      //check if the mouse point at the side of equipment
	virtual void				DrawSelectEdge(Gdiplus::Graphics *pGraphics);	 //Draw the select edge when the mouse selects the equipment
	virtual bool				ChangeSize(const Gdiplus::PointF& actPt, Direction dir);//change the equipment size

	virtual bool				LeftRotate90();//����ת90��
	virtual bool				RightRotate90();//����ת90��
	virtual bool				ShiftEqmtsUp();        //�������ϼ�
	virtual bool				ShiftEqmtsDown();      //�������¼�
	virtual bool				ShiftEqmtsLeft();      //���������
	virtual bool				ShiftEqmtsRight();     //�������Ҽ�
	virtual bool						ShiftEqmtsCtrlUp();        
	virtual bool						ShiftEqmtsCtrlDown();      
	virtual bool						ShiftEqmtsCtrlLeft();      
	virtual bool						ShiftEqmtsCtrlRight();   
	virtual bool				CopyProperty(CEqmDrawBase *pBase);
	virtual CEqmDrawAreaChange*		Copy();
	virtual bool				ClonePropertyByDest(CEqmDrawBase *pBase);		//��¡Ŀ�����������
	void						RotetaBitMap(Image *pBitMap = NULL);  //must call reset rotate
	void						ResetRotateBmp(Image *pBitMap = NULL);	


	void						SetBitmap(Image* pBitmap);
	Image*						GetBitmap();
	void						SetPicID(const int nPicID);
	int							GetPicID();

	void						SetAreaInfo(vector<_tagAreaInfo> vecAreaInfo);
	vector<_tagAreaInfo>		GetAreaInfo();

	void						AddAreaInfo(const CString strPointList, const int nAreaImageID, const int nLinkPageID);

public:
	
protected:
	int							m_fRotateAngel;
	wstring						m_strEqmName;
	EQUIPMENTSUBTYPE			m_subtype;  //�豸������
	int							m_nStateCount; //the state count of the equipment
	vector<Gdiplus::PointF>		m_vecPortPoints; // the port points
	double						m_dStepInterval;
	bool						m_bIsClicked;
	double						m_dUpperLimit;//����ֵ
	double						m_dDownLimit;//����ֵ
	int							m_base_height;
	int							m_base_width ;
	float						m_nscale;
	int							m_nValue;
	int							m_nTimeTotal;
	wstring						m_bindString;
	Image*						m_pBmpName;			//Ĭ�ϱ���ͼ
	int							m_nPicID;

	int							m_nLinkPageID;
	vector<_tagAreaInfo>		m_vecAreaInfo;
};
#endif