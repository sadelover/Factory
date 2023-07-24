
/*
Author:		Juneay
Time:		2010-11-14
Comments:	�����齨��ͼ��
*/

#ifndef COMPONENT_H
#define COMPONENT_H

#pragma once
#include "MemDC.h"
#include <GdiPlus.h>
#include "Tools/DllDefine.h"

//ʹ��gdi+��Ҫ������ͷ�ļ�
#ifndef ULONG_PTR
//#define ULONG_PTR unsigned long*
using namespace Gdiplus;
#endif

class DrawContext;

class DLLEXPORT ComponentDraw
{
public:
	ComponentDraw();
	virtual ~ComponentDraw();

    //static ComponentDraw*   GetInstance();//bird add
public:

	///< ��ʼ����ͼ�����ġ�
	void init(CWnd* pWnd, CRect* pRect = NULL);	
	
	///< ��ͼ�λ�����Ŀ�괰����
	void draw(DrawContext* pFrame = NULL);
	
	///< ���û�ͼ����С
	void setRect(CRect rect);

	void SetRectChanged(bool bRectChange);

	void SetChooseGrid(bool bChooseGrid);

	bool GetChooseGrid();

    CDC*    GetCdc(); //bird add
    
private:
	
	///<�ͷ�mem dc��Դ
	void releaseMemDC();
	
	/*///< �����ڴ�dc
	void createMemDC();*/
	
	///< �ͷ�gc��Դ
	void releaseGC();		
	
	void	CreateStaticMemDC();
	void	DeleteStaticMemDC();
	
	void	CreateDynamicMemDC();
	void	DeleteDynamicMemDC();

	///< ���»�ͼ������
	
	 void  updateStaticContex();

	///< ���õ�ǰ��ͼ������
	void setDrawObject(CDC* pDC);
	
protected:

	///< 
	virtual void prepareDraw();		

	///��̬�����ӿ�
	virtual void drawStaticScene();	
	
	///���ƶ�̬�����ӿ�
	virtual void drawDynamicScene();
	
	CWnd*	GetWnd();

	bool isRectChanged();	///< ��ͼ�����Ƿ����˸ı�

protected:
	///< ��ͼ����
	CRect		m_rect;
	
	///< gdi��ͼdc����ָ��
	CDC			*m_pDC;

	///< gdiplus��ͼGraphics����ָ��
	Gdiplus::Graphics	*m_pGC;
	
	///< ��ͼ����
	DrawContext	*m_pDrawContext;

	///�Ƿ�ѡ������
	bool        m_bChooseGrid;

	//������ɫ
	DWORD       m_backGroudColor;

private:

	///< ��ͼ������
	CWnd		*m_pWnd;
	
	///< ���ڻ�ͼ�豸�����ָ��
	CDC			*m_pWndDC;
	
	///< ����̬������dc
	CMemDrawDC		*m_pStaticDC;
	
	///< ��������ʹ�ø�dc��ͼ
	CMemDrawDC		*m_pMemDC;

	///< ��ͼ�����Ƿ����˸ı�
	bool		m_bRectChanged;
	
	///< ��һ������ʱ������ƾ�̬����
	bool		m_bStartup;
};

#endif