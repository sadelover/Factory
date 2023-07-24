//bird add 20110926

#pragma once

//#include "Tools/USingletonTemplate.h"
#include "tools/CustomTools/CustomTools.h"
//////////////////////////////////////////////////////////////////////////
#include "Tools/DllDefine.h"
//class Image;

class DLLEXPORT UtilDrawNumber{
public:
    UtilDrawNumber();
    virtual ~UtilDrawNumber();

    static UtilDrawNumber *GetInstance(void);
    static void            DestroyInstance();

    enum _e_number{
        E_NUM_0 = 0, 
        E_NUM_1,
        E_NUM_2,
        E_NUM_3,
        E_NUM_4,
        E_NUM_5,
        E_NUM_6,
        E_NUM_7,
        E_NUM_8,
        E_NUM_9,
        E_NUM_POINT,  //10
        E_NUM_MINUS,    //11

        E_NUM_MAX,
    };

    void    InitVecFileName();
    bool    LoadPicture();
    void    GetResourcePath();

    bool    SplitString(const wstring& wstrSrc, wstring& wstrNumber, wstring& wstrLeft, Point& NumberWH);

    int     DrawGreenNumber(Graphics* pGc, const wstring& wstr,  int posX, int posY, wstring& wstrLeft);

protected:
	Image*				m_vecImage[E_NUM_MAX];
    //vector<Image*>      m_vecImage;
    //vector<wstring>     m_vecStrFileName;
    wstring				m_vecStrFileName[E_NUM_MAX];
	wstring             m_strResPath;

    static UtilDrawNumber*    m_pDrawNumber ;
    static Mutex		m_mutex; //bird  3.4
};

//use like this
//USingletonTemplate<UtilDrawNumber>::GetInstance();
