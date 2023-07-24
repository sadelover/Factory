
/*
Author:		Juneay
Time:		2010-11-14
Comments:	建立组建绘图类
*/

#ifndef COMPONENT_H
#define COMPONENT_H

#pragma once
#include "MemDC.h"
#include <GdiPlus.h>
#include "Tools/DllDefine.h"

//使用gdi+需要包含的头文件
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

	///< 初始化绘图上下文。
	void init(CWnd* pWnd, CRect* pRect = NULL);	
	
	///< 将图形绘制在目标窗口上
	void draw(DrawContext* pFrame = NULL);
	
	///< 设置绘图区大小
	void setRect(CRect rect);

	void SetRectChanged(bool bRectChange);

	void SetChooseGrid(bool bChooseGrid);

	bool GetChooseGrid();

    CDC*    GetCdc(); //bird add
    
private:
	
	///<释放mem dc资源
	void releaseMemDC();
	
	/*///< 创建内存dc
	void createMemDC();*/
	
	///< 释放gc资源
	void releaseGC();		
	
	void	CreateStaticMemDC();
	void	DeleteStaticMemDC();
	
	void	CreateDynamicMemDC();
	void	DeleteDynamicMemDC();

	///< 更新画图上下文
	
	 void  updateStaticContex();

	///< 设置当前画图上下文
	void setDrawObject(CDC* pDC);
	
protected:

	///< 
	virtual void prepareDraw();		

	///静态场景接口
	virtual void drawStaticScene();	
	
	///绘制动态场景接口
	virtual void drawDynamicScene();
	
	CWnd*	GetWnd();

	bool isRectChanged();	///< 绘图区域是否发生了改变

protected:
	///< 绘图区域
	CRect		m_rect;
	
	///< gdi绘图dc对象指针
	CDC			*m_pDC;

	///< gdiplus绘图Graphics对象指针
	Gdiplus::Graphics	*m_pGC;
	
	///< 绘图参数
	DrawContext	*m_pDrawContext;

	///是否选择网格
	bool        m_bChooseGrid;

	//背景颜色
	DWORD       m_backGroudColor;

private:

	///< 绘图窗口类
	CWnd		*m_pWnd;
	
	///< 窗口画图设备类对象指针
	CDC			*m_pWndDC;
	
	///< 画静态场景的dc
	CMemDrawDC		*m_pStaticDC;
	
	///< 在子类中使用该dc绘图
	CMemDrawDC		*m_pMemDC;

	///< 绘图区域是否发生了改变
	bool		m_bRectChanged;
	
	///< 第一次启动时必须绘制静态场景
	bool		m_bStartup;
};

#endif