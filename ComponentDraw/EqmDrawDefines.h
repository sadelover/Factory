
/*
 *	This header defines the common enums and structs for this 
 *	module.
 *
 *
 *
 *author : guibicheng
 *date: 2010.12.1
*/

#ifndef EQMDRAWDEFINES_H__
#define EQMDRAWDEFINES_H__
#include <math.h>
#include <string>
#include <vector>

using  std::vector;
using  std::string;
using namespace std;

#include "Tools/DllDefine.h"
#include <comdef.h>//��ʼ��һ��com��
#include <GdiPlus.h>
using namespace Gdiplus;

class CEqmDrawBase;
class CEqmDrawProject;
class CEqmDrawPage;

class CEqmDrawDiagnoseGraphicPlus;
// equipment types for the draw architecture

#define RECTSIZE 8              //selectside size
#define DEFAULT_SIDE_INTERNAL  (RECTSIZE+4)   // ����ѡ�п��Ĭ�ϼ��

#define DEFAULT_PIPE_WIDTH	  14	

#define WM_UPDATEDLLCOUNT   WM_USER+11

#define INSERT_META_FILE_ID_START	 100000
#define IMPORT_PAGE_ELEMENT_ID_START 10000000

#define SELECTPENCOLOR Gdiplus::Color(0, 0, 255)
#define BRUSHCOLOR Gdiplus::Color(255, 230, 230, 230)

#define MAINSELECT_BRUSHCOLOR Gdiplus::Color(255, 50, 50, 255)

const int MAX_STRING_LENGTH					= 256;//�ַ�������󳤶�

#define _Rotate90FlipNone   90
#define _Rotate180FlipNone  180
#define _Rotate270FlipNone  270
#define _Rotate90FlipXY    -90
#define _Rotate180FlipXY   -180
#define _Rotate270FlipXY   -270

#define PORTOFFSET 7  //the area of how to choose the port point

#define MOVEDISTANCEBYKEY 1 //�����������Ҽ��豸�ƶ��ľ���
#define MOVEDISTANCEBYKEYCTRL 10

enum EQMDRAWTYPE
{
	EQMDRAW_UNKNOWN =0,		//unknown
	EQMDRAW_CHILLER,		//���- 1 
	EQMDRAW_PUMP,			//ˮ��- 2 
	EQMDRAW_COOLINGTOWER,	//��ȴ��- 3
	EQMDRAW_VALVE,			//����- 4
	EQMDRAW_TEMPUP,         //�¶���-5
	EQMDRAW_TEMPDOWN,       //�¶���-6
	EQMDRAW_FLOW,           //����-7
	EQMDRAW_PRESS,          //ѹ��	-8
	EQMDRAW_REDARROW,       //���ͷ-9
	EQMDRAW_BLUEARROW,      //����ͷ-10
	EQMDRAW_REDLIGHT,       //��ɫ��-11
	EQMDRAW_GREENLIGHT,     //��ɫ��-12
	EQMDRAW_YELLOWLIGHT,    //��ɫ��-13
	EQMDRAW_BOOLBUTTON,     //bool button-14
	EQMDRAW_INCREBUTTON,    //increment button-15
	EQMDRAW_DECBUTTON,      //dec button-16
	EQMDRAW_UNSERIAL,       //δ֪�����豸-17
	EQMDRAW_UNPLC,			//δ֪PLC�豸-18
	EQMDRAW_POWERMETER,		//���-19
	EQMDRAW_AIRBOX,			//�յ���-20	

	EQMDRAW_TEXT,    		//����-21
	EQMDRAW_STRAIGHTPIPE,	//ֱ�ܵ�-22
	EQMDRAW_FOLDPIPE,		//�۹ܵ�-23
	EQMDRAW_MOVE,			//-24
	EQMDRAW_NEWLINKER,      //�������ӵ�-25
	GraphicType_LINE,		//ֱ��-26
	GraphicType_RECTANGLE,	//����-27
	GraphicType_TRIANGLE,	//������-28
	GraphicType_ELLIPSE,	//��Բ-29
	GraphicType_POLYGON,	//�����-30
	GraphicType_ARROW,		//��ͷ	-31	
	
	EQMDRAW_PRESSURE,		//	ѹ�������� - 32, add fow xichai.
	EQMDRAW_MV_DUCT,         //�綯�緧ִ���� -33��
	EQMDRAW_TH,              //��ʪ�ȴ����� -34
	EQMDRAW_RETURNFAN,       //�ط��   -35
	EQMDRAW_SUPPLYFAN,       //�ͷ��   -36
	EQMDRAW_AHU,             //���ʽ����������� -37	
	//9.5 �����豸 hun
	EQMDRAW_YORKCENTRIFUGAL,    //Լ������ -38
	EQMDRAW_YORKSCREW,          //Լ���ݸ�  -39
	EQMDRAW_TRANCECENTRIFUGAL,  //��������  -40
	EQMDRAW_TRANCESCREW,        //�����ݸ�  -41
	EQMDRAW_CARRIERCENTRIFUGAL, //��������  -42
	EQMDRAW_CARRIERSCREW,        //�����ݸ�  -43
	EQMDRAW_COOLINGPUMP,         //��ȴ��    -44
	EQMDRAW_PRICHWPUMP,          //һ���䶳�� -45
	EQMDRAW_SECCHWPUMP,          //�����䶳�� -46
	EQMDRAW_COOLINGTOWER1,	     //��ȴ��һ- 47
	EQMDRAW_COOLINGTOWER2,	     //��ȴ����- 48
	EQMDRAW_METAFILE,            //�Զ���ͼԪ
	EQMDRAW_METABUTTON,			 //��ťͼԪ
	EQMDRAW_BEND,				 //�ܵ���ͷ
	EQMDRAW_LINE_CHART,			 //����ͼ
	EQMDRAW_BAR_CHART,			 //��״ͼ
	EQMDRAW_PIE_CHART,			 //��ͼ
	EQMDRAW_DASHBOARD,			 //�Ǳ�
	EQMDRAW_TIME_SELECT,		 //ʱ��ѡ��
	EQMDRAW_TIME_BAR,			 //ʱ��ѡ��
	EQMDRAW_PROGRESS_BAR,		 //����������
	EQMDRAW_DIAGNOSE_CHART,		 //���ͼ
	EQMDRAW_POLYGON,			//�����
	EQMDRAW_AREA_CHANGE,		 //����仯ͼԪ
	EQMDRAW_SHELL_EXECUTE,		 //��һ���ļ�
	EQMDRAW_EMBEDED_PAGE,		 //Ƕ��ʽҳ��
	EQMDRAW_LIQUID_LEVEL,		 //Һλ
	EQMDRAW_CHKBTN_PLUS,		 //��ѡ/��ѡ��ť
	EQMDRAW_NOTICE_BOARD,		 //������
	EQMDRAW_HISTORY_TREND,		//��ʷ���Ʒ���
	EQMDRAW_ENERGY_SAVE_ROI,	//���ܴ�ʩROI����
	EQMDRAW_MAP_NAVIGATE,		//��ͼ����
	EQMDRAW_FORMAT_BRUSH,		//��ʽˢ
	EQMDRAW_NAVIGATE_WINDOW,	//����������
	EQMDRAW_LEVEL_RULER,		//ˮƽ�Ǳ��
	EQMDRAW_DATA_ANALYSE,		//���ݷ���
	EQMDRAW_TEMPER_DISTRI,		//�¶ȷֲ�
	EQMDRAW_DATA_REPORT,		//���ݱ���
	EQMDRAW_PLANE_TEMPE_DISTR,	//ƽ���¶ȷֲ�
	EQMDRAW_DOTLINE_FRAME,		//���߿�
	EQMDRAW_GENERAL_COMPONENT,	//ͨ�����
	EQMDRAWTYPE_MAX,
	EQMDRAW_LINE,   //ֱ��
	EQMDRAW_RECTANGLE, //����
};

enum EQUIPMENTSUBTYPE
{
	SUBTYPE_UNKNOWN = 0,
	EQUIPMENTSUBTYPE_CHILLER_VALVE,			// chiller valve
	EQUIPMENTSUBTYPE_COOLINGTOWER_VALVE,		// cooling tower valve

	EQUIPMENTSUBTYPE_COOLING_PUMP,		// cooling water pump
	EQUIPMENTSUBTYPE_PRICHILLING_PUMP,		// primary chilled pump
	EQUIPMENTSUBTYPE_SECCHILLING_PUMP,		// secondary chilled pump

    EQUIPMENTSUBTYPE_MV_VALVE,
	//EQUIPMENTSUBTYPE_HOT_PUMP,		// hot water pump
	//sany add ��������ͣ�һ����ˮ�ã�������ˮ��
	EQUIPMENTSUBTYPE_PRIHOT_PUMP,		// hot water pump
	EQUIPMENTSUBTYPE_SECHOT_PUMP,		// hot water pump

	EQUIPMENTSUBTYPE_MAX, 	
};

typedef struct _tagPageIDAndName
{
	_tagPageIDAndName()
	{
		nPageID = 0;
	}
	int nPageID;
	CString strPageName;
}PageIDName;
//ˮ��Ĭ����ɫ
#define DEFAULTINNERCOLOR	Gdiplus::Color(0,171,228)	//Ĭ������ɫ
#define	DEFAULTOUTERCOLOR	Gdiplus::Color(0,100,154)	//Ĭ������ɫ

#define NONEFILLCOLOR -1	//�����ɫΪ��

#define DEFAULTBACKGROUNDCOLOR RGB(219,219,219) //������ɫ

#define CELLINTERVAL 10	//����ľ�����


// �ı�����ʾģʽ
// ������ʾ����״̬
// ������ʾֵ
enum TEXTSHOWMODE
{
	MODE_ENUM=0,	//ö��ģʽ
	MODE_VALUE,		//ֵģʽ
	MODE_TIME,		//ʱ��ģʽ
	MODE_STRING,	//�ַ���ģʽ
	MODE_MAX, 
};

#define MODEONTEXT _T("����")
#define MODEOFFTEXT _T("�ر�")
#define MODEERRORTEXT _T("����")
#define MODEACTTEXT _T("������..")
#define MODELOADTEXT _T("������...")
#define MODEUNLOADTEXT _T("ж����...")

#define MODE_AUTO_TEXT                  _T("�Զ�")
#define MODE_MANUAL_TEXT              _T("�ֶ�")

#define MODE_BEOP_TEXT	_T("BEOP")
#define MODE_BA_TEXT		_T("BA")

#define MODE_REMOTE_TEXT	_T("Զ��")
#define MODE_LOCAL_TEXT _T("�͵�")

#define MODEOACTEXT _T("OACģʽ")
#define MODENORMALTEXT _T("��ͨģʽ")

#define MODE_YES_TEXT _T("��")
#define MODE_NO_TEXT _T("��")

#define MODE_NORMAL_TEXT  _T("����")
#define MODE_ABNORMAL_TEXT _T("�쳣")

#define MODEEMPTYTEXT _T("")

//cooling tower fan speed
#define C_STR_MODE_FAN_SPEED_HIGH		_T("��")
#define C_STR_MODE_FAN_SPEED_LOW		_T("��")
#define C_STR_MODE_FAN_SPEED_ERROR		_T("δ֪")

#define C_STR_LAYER_BG  _T("�ײ�")
#define C_STR_LAYER_TOP  _T("�ϲ�")

#define C_STR_ALIGN_LEFT _T("�����")
#define C_STR_ALIGN_RIGHT _T("�Ҷ���")
#define C_STR_ALIGN_HCENTER _T("����")

enum _e_text_align{
    E_TEXT_ALIGN_LEFT = 0,
    E_TEXT_ALIGN_RIGHT, 
    E_TEXT_ALIGN_HCENTER, 

    E_TEXT_ALIGN_MAX,
};
//////////////////////////////////////////////////////////////////////////

struct DLLEXPORT PipeColor
{
	int innercolor;
	int outercolor;

	PipeColor(int inner = 12615935, int outer = 4194432):innercolor(inner), outercolor(outer)
	{}
};


enum EPumpType
{
	NonePump_Type = -1,     // ��ʼ��ֵ
	CoolingPump_Type,       // ��ȴ��
	PriPump_Type,           // һ�α�
	SecPump_Type,           // ���α�
	SerialPump_Type,        // �߼���

    Pump_max, 
};

enum LineStyles
{
	Line_Solid = 0 ,		/* ʵ�� */
	Line_Dash ,				/* -------  */
	Line_Dot  ,				/* .......  */
	Line_DashDot ,			/* _._._._  */
	Line_DashDotDot ,		/* _.._.._  */

	Line_NULL ,
};


enum Direction             //�豸�ı�ķ���
{
	Dir_None,
	Dir_Left,
	Dir_Right,
	Dir_Up,
	Dir_Down,
	Dir_LeftUp,
	Dir_LeftDown,
	Dir_RightUp,
	Dir_RightDown,

    Dir_max, //bird add
};

enum E_BAR_TYPE
{
	eBarNormal = 0,
	eBarCompare,
};

struct PictureInfo
{
	PictureInfo()
	{
		nSize = 0;
		pBlock = NULL;
		ID = 0;
		issystemimage = 0;
		pBitmap = NULL;
		memset(szName, 0, sizeof(szName));
		etype_equip = L"1";
		nBtnId = 0;
		nBtnSubId = -1;
		etype_p = L"";
		remark = L"";
	};
	wchar_t szName[MAX_PATH];
	int nSize;
	char* pBlock;
	wstring etype_p;
	wstring etype_equip;
	int   ID;
	wstring remark;
	int issystemimage;
	Image* pBitmap;
	int	nBtnId;
	int	nBtnSubId;
};

struct AnimationInfo
{
	int   ID;
	wstring szName;
	std::vector<PictureInfo> vecPicture;
	std::vector<Image*>  vecpBitmap;

	std::vector<int>   vecPicIDList;
};

//���ݿ����
struct DataBaseSetting
{// ���ݿ���Ϣ�����ṹ��
	DataBaseSetting()
	{
		nPort = 3306;
	};
	string strDBIP;					//  host��ַ
	string strDBIP2;				//  ����IP
	string strDBName;				//  ���ݿ�����
	string strRealTimeDBName;       //  ʵʱ���ݿ�����
	string strUserName;				//  �û���
	int    nAutoLogin;				//	�Ƿ��Զ���¼
	int    DTUPortNumber;			//	DTU�˿ں�
	int    DTUBaudrate;				//	DTU������
	int    nPort;					//  �˿�
	string strLastEditSoftVersion;  //  �ϴα༭��������汾

	bool bEnableMenuGroup;   //����ʹ�ö����˵�
};

typedef struct tag_GroupPage
{
	int		GroupID;
	wstring	GroupName;
	int		nGroupShowOrder;

	tag_GroupPage()
	{
		GroupID = 0;
		GroupName = L"";
		nGroupShowOrder = 0;
	}
}GroupPage;

enum DRAWTYPE
{
	REDRAW,
	DRAWEQMT,
	FLASHSCREEN,

	DRAW_COUNT
};


struct DiagnoseTreeNodeInfo
{
	CString strNodeName;//�ڵ�����
	int nLinkPageID;//����ҳ��ID
	int nID;//�ڵ�����ID,���ڵ��IDΪ0
	int nParentID;//���ڵ�ID
	vector<CEqmDrawDiagnoseGraphicPlus*> vecDiagnoseGraphicItem;//����Ҷ�ӽڵ���������ͼ����
};

/************************************ͼ����ᵥλ*********************************************/
enum enum_graph_unit
{
	E_NULL,//��
	E_AUTO,//�Զ�
	E_YEAR,//��
	E_MONTH,//��
	E_YEAR_MONTH,//����
	E_MONTH_DAY,//����
	E_MONTH_DAY_HOUR,//����ʱ
	E_HOUR_MINUTE,//ʱ��
};

struct _Graph_Item_Property
{
	CString								strItemName;//description
	COLORREF							color;
	CString								strPointName;
	int									nStoreType;
};

struct _Import_Setting
{
	int					nID;					//ID
	CString				strPrjName;				//��Ŀ����
	bool				bLoadPoint;				//�Ƿ��Ѽ��ع����
	int					nPointBaseOffset;		//���ƫ����
	bool				bLoadImage;				//�Ƿ��Ѽ��ع�ͼƬ��Դ
	int					nImageBaseOffset;		//ͼƬ��Դƫ����
};

enum SHOWTYPE
{
	SHOWTYPE_DRAW,
	SHOWTYPE_RUN
};

enum _e_layer_
{
    E_LAYER_1 = 0,
	E_LAYER_2,
	E_LAYER_3,
	E_LAYER_4,
	E_LAYER_5,
	E_LAYER_6,
	E_LAYER_7,
	E_LAYER_8,
	E_LAYER_9,
	E_LAYER_10,
	E_LAYER_MAX,
};

enum E_OWNER
{
	eFactory = 0,
	eObserver,
};    

// notice board
typedef enum tagListType
{
	scrawlCurve	= 0,
	scrawlColumn
}ScrawlListType;

typedef struct tagCurveColumn
{
	CString		strPointName;
	COLORREF	color;
	int			nMax;
	int			nMin;
	CString		strRemarks;
	ScrawlListType	scrawlType;

	tagCurveColumn(void)
	{
		strPointName	= _T("����");
		color			= RGB(0, 0, 0);
		nMax			= 100;
		nMin			= 0;
		strRemarks		= _T("");
		scrawlType		= scrawlCurve;
	}
}CurveColumn;

struct ProjectCheckInfo
{
	int nPageID;
	int nElementID;
	int	nImageID;
	int nErrType;	//0 �¼�ͼƬ  1�¼��� 2��ť  3page_contain_elements����  4bend 5dashboard 6 chart 7 unit10 8 unit12
	wstring strPointName;
	wstring strPageName;
	wstring strErrInfo;		//������Ϣ
};

struct NewPicIDInfo
{
	int nOldPicID;
	int nNewPicID;
};

typedef struct tagImageReplaceInfo
{
	int	nPageId;
	wstring	strPageName;
	int	nElementId;
	int	nPicId;

	tagImageReplaceInfo(void)
	{
		Reset();
	}

	void Reset(void)
	{
		nPageId = 0;
		strPageName.clear();
		nElementId = 0;
		nPicId = 0;
	}
}ImgReplaceInfo;

typedef struct tagCustomImageInfo
{
	int nPageId;
	EQMDRAWTYPE nImgType;
	int	nImgId;
	int nElementId;
	int nWidth;
	int nHeight;
	bool bAnimation;

	tagCustomImageInfo(void)
	{
		nPageId		= 0;
		nElementId  = 0;
		nImgType	= EQMDRAW_UNKNOWN;
		nImgId		= 0;
		nWidth		= 0;
		nHeight     = 0;
		bAnimation  = false;
	}
}CustomImgInfo;

typedef enum tagTextAttri
{
	TEXT_ATTR_NO_USE	= 0,
	TEXT_ATTR_TEMPERA,		// �¶�
	TEXT_ATTR_HUMIDITY,		// ʪ��
	TEXT_ATTR_SP
}TextAttri;

// device/area
typedef struct tagTextDeviceInfo
{
	CString	strRegion;
	int		nBindPageId;
	CString	strBindPageName;
	int		nBindTextId;

	tagTextDeviceInfo(void)
	{
		nBindPageId = 0;
		nBindTextId = 0;
	}
}TextDeviceInfo;

typedef struct tagTextAreaInfo
{
	int		nPageId;
	CString	strPageName;
	CString	strRegion;

	tagTextAreaInfo(void)
	{
		nPageId = 0;
	}
}TextAreaInfo;

// calc tree item
typedef struct tagCalcModuleItem
{
	CString	strName;
	CString	strContent;
	CString	strParentName;
}CalcModuleItem;

typedef struct tagCalcModuleInfo
{
	CString	strName;
	int		nPeriod;
	int		nRunState;
	vector<CalcModuleItem>	vecItem;

	tagCalcModuleInfo(void)
	{
		nPeriod		= 0;
		nRunState	= -1;
		vecItem.clear();
	}
}CalcModuleInfo;


// �ܵ���ͷ����
typedef enum
{
	bendUnknown	= 0,
	bendThreeUp,		// ��ͨ����
	bendThreeRight,		// ��ͨ����
	bendThreeDown,		// ��ͨ����
	bendThreeLeft,		// ��ͨ����
	bendCross,			// ��ͨ
	bendTwoLeftUp,		// ��ͨ����
	bendTwoRightUp,		// ��ͨ����
	bendTwoRightDown,	// ��ͨ����
	bendTwoLeftDown,	// ��ͨ����
}BendType;

// �ܵ���ͷ��Ϣ
typedef struct tagBendInfo
{
	CPoint		point;
	BendType	type;

	tagBendInfo(void)
	{
		type = bendUnknown;
	}
}BendInfo;

// �汾��Ϣ
typedef struct tagVerInfo
{
	int		nVersion;
	CString strUser;
	CString	strTime;
	CString	strLog;

	tagVerInfo(void)
	{
		nVersion = 0;
	}
}VersionInfo;


const int g_nLenPointAttrSystem_Del	= 5;
const int g_nLenPointAttrDevice_Del	= 7;
const int g_nLenPointAttrType_Del	= 11;
//const CString g_strPointAttrSystem[g_nLenPointAttrSystem] = {_T("��"), _T("ůͨ"), _T("����"), _T("��������"), _T("���ܿյ�")};
//const CString g_strPointAttrDevice[g_nLen PointAttrDevice] = {_T("��"), _T("���"), _T("ˮ��"), _T("��ȴ��"), _T("AHU"), _T("VAV"), _T("ϵͳ")};
//const CString g_strPointAttrType[g_nLenPointAttrType] = {_T("��"), _T("������"), _T("������"), _T("�¶�"), _T("����"), _T("ѹ��"), _T("����"), _T("����"), _T("Ƶ��"), _T("��ͣ"), _T("����")};
const CString g_strPointAttrSystem_Del[g_nLenPointAttrSystem_Del] = {_T("None"), _T("HVAC"), _T("Power"), _T("Lighting"), _T("CRAC")};
const CString g_strPointAttrDevice_Del[g_nLenPointAttrDevice_Del] = {_T("None"), _T("Chiller"), _T("Pump"), _T("CT"), _T("AHU"), _T("VAV"), _T("System")};
const CString g_strPointAttrType_Del[g_nLenPointAttrType_Del] = {_T("None"), _T("Power meter"), _T("Thermal meter"), _T("Tmperature"), _T("Flow rate"), _T("Pressure"), _T("Amps"), _T("Power"), _T("Frequency"), _T("OnOff"), _T("Alarm")};


const int g_nPicIndexBend			= 18700187;
const int g_nPicIndexDashBoard1		= 20600206;
const int g_nPicIndexPointer1		= 20700207;
const int g_nPicIndexDashBoard2		= 20800208;
const int g_nPicIndexPointer2		= 20900209;
const int g_nPicIndexPointer3		= 20500205;
const int g_nPicIndexChartLine		= 17500175; 
const int g_nPicIndexChartBar		= 17700177;
const int g_nPicIndexChartPie		= 17400174;
const int g_nPicIndexTimeSelect		= 21000210;
const int g_nPicIndexTimeBar		= 17700177;
const int g_nPicIndexProgressBar	= 17800178;
const int g_nPicIndexDiagnoseChart	= 17600176;
const int g_nPicIndexLiquidLevel	= 7700077;
const int g_nPicIndexCheckButton1	= 13400134;
const int g_nPicIndexCheckButton2	= 13500135;
const int g_nPicIndexScrawl			= 17500175;
const int g_nPicIndexHistoryTrend	= 21200212;
const int g_nPicIndexEnergySaveRoi	= 21300213;
const int g_nPicIndexMapNavigate	= 24300243;


//DLLEXPORT
class DLLEXPORT DrawContext
{
public:
    DrawContext(CEqmDrawProject* project);
    ~DrawContext();

    void ClearSelect();

	void UpdateCopiedList(const vector<CEqmDrawBase*>& deletelist);
	
	void ResetContext();
	
	void SetProject(CEqmDrawProject* project);
	CEqmDrawProject* GetProject() ;

	PipeColor GetDefaultPipeColor() const;
	void SetPipeColor(const PipeColor& pipecolor);

	DWORD GetScreenWidth() const ;
	DWORD GetScreenHeight() const;
	
	void SetScreenWidth(DWORD width);
	void SetScreenHeight(DWORD height);

	void AddViewedPage(int nTabIndex);

	DWORD	GetScaledWidth();
	DWORD   GetScaledheight();
	CEqmDrawPage *pPage;
	vector<int> m_pViewedPageTabIndexList;
	int m_nCurViewPageIndex;
	CEqmDrawBase *pNewEqmt;
	CEqmDrawBase *pFormatEqmt; //��ʽˢ�豸
	vector<CEqmDrawBase*> vecSelectEqmt;
	vector<CEqmDrawBase*> mVecSelectEqmtCopied;
	DRAWTYPE type;
	int nShowLocation;
	bool bIsCreatePipe;
	bool bIsCreatePolygon;//true�Ѵ�����������ƹ���
	Gdiplus::PointF prePoint;
	Gdiplus::PointF curPoint;
	Gdiplus::PointF actPt;//��������ƶ�λ��
	Gdiplus::PointF PrePt;//�ƶ�ǰ����
	EQMDRAWTYPE m_nDrawMode; //1:chiller,2:Pump,3:coolingpump,4:coolingtower,5:ahu,6:valve,7:pipe,
	SHOWTYPE    m_showType; //��ʾģʽ����ͼģʽ������ģʽ
	LineStyles  m_lineStyle;
	bool m_bDrawingLine;
	bool m_bDragging;    //�Ƿ���ק
	bool m_bIsChangeSize;    //�Ƿ���Ҫ�ı��С
	bool m_bIsDrawCell;      //�Ƿ�����
	bool m_bStartChangeSize; //�Ƿ����ڸı��С
	int  m_ChangeDirection;  //��¼�ı�size�ķ���
	bool m_bIsCreate2DGraph; //�Ƿ�2Dͼ��
	bool m_ifSelectBox; //use mouse select box to select multiple items;
	bool m_bIsChangeBackgroundColor;
	DWORD m_backGroudColor;
	double m_fScreenScale;
	int GetDefaultPipeWidth() const;	//Ĭ�Ϲܵ���ȡ�
	void SetDefaultPipeWidth(int pipewidth);

	//�Ϲ�΢���
	void SetLineWidth(const int width);
	const int GetLineWidth() const;
	void SetLineColor(const DWORD color);
	const DWORD GetLineColor() const;
	CEqmDrawProject* m_project;
private:
	PipeColor m_pipeColor; //�ܵ�Ĭ����ɫ		
	int m_defaultpipewidth ;
	//�Ϲ�΢���
	int				m_iLineWidth;
	DWORD           m_dwLineColor; 
};

#endif
