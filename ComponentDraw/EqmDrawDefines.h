
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
#include <comdef.h>//初始化一下com口
#include <GdiPlus.h>
using namespace Gdiplus;

class CEqmDrawBase;
class CEqmDrawProject;
class CEqmDrawPage;

class CEqmDrawDiagnoseGraphicPlus;
// equipment types for the draw architecture

#define RECTSIZE 8              //selectside size
#define DEFAULT_SIDE_INTERNAL  (RECTSIZE+4)   // 绘制选中框的默认间距

#define DEFAULT_PIPE_WIDTH	  14	

#define WM_UPDATEDLLCOUNT   WM_USER+11

#define INSERT_META_FILE_ID_START	 100000
#define IMPORT_PAGE_ELEMENT_ID_START 10000000

#define SELECTPENCOLOR Gdiplus::Color(0, 0, 255)
#define BRUSHCOLOR Gdiplus::Color(255, 230, 230, 230)

#define MAINSELECT_BRUSHCOLOR Gdiplus::Color(255, 50, 50, 255)

const int MAX_STRING_LENGTH					= 256;//字符串的最大长度

#define _Rotate90FlipNone   90
#define _Rotate180FlipNone  180
#define _Rotate270FlipNone  270
#define _Rotate90FlipXY    -90
#define _Rotate180FlipXY   -180
#define _Rotate270FlipXY   -270

#define PORTOFFSET 7  //the area of how to choose the port point

#define MOVEDISTANCEBYKEY 1 //按下上下左右键设备移动的距离
#define MOVEDISTANCEBYKEYCTRL 10

enum EQMDRAWTYPE
{
	EQMDRAW_UNKNOWN =0,		//unknown
	EQMDRAW_CHILLER,		//冷机- 1 
	EQMDRAW_PUMP,			//水泵- 2 
	EQMDRAW_COOLINGTOWER,	//冷却塔- 3
	EQMDRAW_VALVE,			//阀门- 4
	EQMDRAW_TEMPUP,         //温度上-5
	EQMDRAW_TEMPDOWN,       //温度下-6
	EQMDRAW_FLOW,           //流量-7
	EQMDRAW_PRESS,          //压差	-8
	EQMDRAW_REDARROW,       //红箭头-9
	EQMDRAW_BLUEARROW,      //蓝箭头-10
	EQMDRAW_REDLIGHT,       //红色灯-11
	EQMDRAW_GREENLIGHT,     //绿色灯-12
	EQMDRAW_YELLOWLIGHT,    //黄色灯-13
	EQMDRAW_BOOLBUTTON,     //bool button-14
	EQMDRAW_INCREBUTTON,    //increment button-15
	EQMDRAW_DECBUTTON,      //dec button-16
	EQMDRAW_UNSERIAL,       //未知串口设备-17
	EQMDRAW_UNPLC,			//未知PLC设备-18
	EQMDRAW_POWERMETER,		//电表。-19
	EQMDRAW_AIRBOX,			//空调箱-20	

	EQMDRAW_TEXT,    		//文字-21
	EQMDRAW_STRAIGHTPIPE,	//直管道-22
	EQMDRAW_FOLDPIPE,		//折管道-23
	EQMDRAW_MOVE,			//-24
	EQMDRAW_NEWLINKER,      //创建连接点-25
	GraphicType_LINE,		//直线-26
	GraphicType_RECTANGLE,	//矩形-27
	GraphicType_TRIANGLE,	//三角形-28
	GraphicType_ELLIPSE,	//椭圆-29
	GraphicType_POLYGON,	//多边形-30
	GraphicType_ARROW,		//箭头	-31	
	
	EQMDRAW_PRESSURE,		//	压力传感器 - 32, add fow xichai.
	EQMDRAW_MV_DUCT,         //电动风阀执行器 -33，
	EQMDRAW_TH,              //温湿度传感器 -34
	EQMDRAW_RETURNFAN,       //回风机   -35
	EQMDRAW_SUPPLYFAN,       //送风机   -36
	EQMDRAW_AHU,             //组合式空气处理机组 -37	
	//9.5 新增设备 hun
	EQMDRAW_YORKCENTRIFUGAL,    //约克离心 -38
	EQMDRAW_YORKSCREW,          //约克螺杆  -39
	EQMDRAW_TRANCECENTRIFUGAL,  //特灵离心  -40
	EQMDRAW_TRANCESCREW,        //特灵螺杆  -41
	EQMDRAW_CARRIERCENTRIFUGAL, //开利离心  -42
	EQMDRAW_CARRIERSCREW,        //开利螺杆  -43
	EQMDRAW_COOLINGPUMP,         //冷却泵    -44
	EQMDRAW_PRICHWPUMP,          //一次冷冻泵 -45
	EQMDRAW_SECCHWPUMP,          //二次冷冻泵 -46
	EQMDRAW_COOLINGTOWER1,	     //冷却塔一- 47
	EQMDRAW_COOLINGTOWER2,	     //冷却塔二- 48
	EQMDRAW_METAFILE,            //自定义图元
	EQMDRAW_METABUTTON,			 //按钮图元
	EQMDRAW_BEND,				 //管道弯头
	EQMDRAW_LINE_CHART,			 //折线图
	EQMDRAW_BAR_CHART,			 //柱状图
	EQMDRAW_PIE_CHART,			 //饼图
	EQMDRAW_DASHBOARD,			 //仪表
	EQMDRAW_TIME_SELECT,		 //时间选择
	EQMDRAW_TIME_BAR,			 //时间选择
	EQMDRAW_PROGRESS_BAR,		 //进度条柱子
	EQMDRAW_DIAGNOSE_CHART,		 //诊断图
	EQMDRAW_POLYGON,			//多边形
	EQMDRAW_AREA_CHANGE,		 //区域变化图元
	EQMDRAW_SHELL_EXECUTE,		 //打开一个文件
	EQMDRAW_EMBEDED_PAGE,		 //嵌入式页面
	EQMDRAW_LIQUID_LEVEL,		 //液位
	EQMDRAW_CHKBTN_PLUS,		 //单选/多选按钮
	EQMDRAW_NOTICE_BOARD,		 //公告栏
	EQMDRAW_HISTORY_TREND,		//历史趋势分析
	EQMDRAW_ENERGY_SAVE_ROI,	//节能措施ROI分析
	EQMDRAW_MAP_NAVIGATE,		//地图导航
	EQMDRAW_FORMAT_BRUSH,		//格式刷
	EQMDRAW_NAVIGATE_WINDOW,	//导航悬浮窗
	EQMDRAW_LEVEL_RULER,		//水平仪标尺
	EQMDRAW_DATA_ANALYSE,		//数据分析
	EQMDRAW_TEMPER_DISTRI,		//温度分布
	EQMDRAW_DATA_REPORT,		//数据报表
	EQMDRAW_PLANE_TEMPE_DISTR,	//平面温度分布
	EQMDRAW_DOTLINE_FRAME,		//虚线框
	EQMDRAW_GENERAL_COMPONENT,	//通用组件
	EQMDRAWTYPE_MAX,
	EQMDRAW_LINE,   //直线
	EQMDRAW_RECTANGLE, //矩形
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
	//sany add 添加新类型：一次热水泵，二次热水泵
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
//水管默认颜色
#define DEFAULTINNERCOLOR	Gdiplus::Color(0,171,228)	//默认内颜色
#define	DEFAULTOUTERCOLOR	Gdiplus::Color(0,100,154)	//默认外颜色

#define NONEFILLCOLOR -1	//填充颜色为空

#define DEFAULTBACKGROUNDCOLOR RGB(219,219,219) //背景颜色

#define CELLINTERVAL 10	//网络的距离间隔


// 文本的显示模式
// 包含显示开关状态
// 包含显示值
enum TEXTSHOWMODE
{
	MODE_ENUM=0,	//枚举模式
	MODE_VALUE,		//值模式
	MODE_TIME,		//时间模式
	MODE_STRING,	//字符串模式
	MODE_MAX, 
};

#define MODEONTEXT _T("开启")
#define MODEOFFTEXT _T("关闭")
#define MODEERRORTEXT _T("故障")
#define MODEACTTEXT _T("动作中..")
#define MODELOADTEXT _T("加载中...")
#define MODEUNLOADTEXT _T("卸载中...")

#define MODE_AUTO_TEXT                  _T("自动")
#define MODE_MANUAL_TEXT              _T("手动")

#define MODE_BEOP_TEXT	_T("BEOP")
#define MODE_BA_TEXT		_T("BA")

#define MODE_REMOTE_TEXT	_T("远程")
#define MODE_LOCAL_TEXT _T("就地")

#define MODEOACTEXT _T("OAC模式")
#define MODENORMALTEXT _T("普通模式")

#define MODE_YES_TEXT _T("是")
#define MODE_NO_TEXT _T("否")

#define MODE_NORMAL_TEXT  _T("正常")
#define MODE_ABNORMAL_TEXT _T("异常")

#define MODEEMPTYTEXT _T("")

//cooling tower fan speed
#define C_STR_MODE_FAN_SPEED_HIGH		_T("高")
#define C_STR_MODE_FAN_SPEED_LOW		_T("低")
#define C_STR_MODE_FAN_SPEED_ERROR		_T("未知")

#define C_STR_LAYER_BG  _T("底层")
#define C_STR_LAYER_TOP  _T("上层")

#define C_STR_ALIGN_LEFT _T("左对齐")
#define C_STR_ALIGN_RIGHT _T("右对齐")
#define C_STR_ALIGN_HCENTER _T("居中")

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
	NonePump_Type = -1,     // 初始化值
	CoolingPump_Type,       // 冷却泵
	PriPump_Type,           // 一次泵
	SecPump_Type,           // 二次泵
	SerialPump_Type,        // 高级泵

    Pump_max, 
};

enum LineStyles
{
	Line_Solid = 0 ,		/* 实线 */
	Line_Dash ,				/* -------  */
	Line_Dot  ,				/* .......  */
	Line_DashDot ,			/* _._._._  */
	Line_DashDotDot ,		/* _.._.._  */

	Line_NULL ,
};


enum Direction             //设备改变的方向
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

//数据库参数
struct DataBaseSetting
{// 数据库信息参数结构体
	DataBaseSetting()
	{
		nPort = 3306;
	};
	string strDBIP;					//  host地址
	string strDBIP2;				//  备用IP
	string strDBName;				//  数据库名字
	string strRealTimeDBName;       //  实时数据库名字
	string strUserName;				//  用户名
	int    nAutoLogin;				//	是否自动登录
	int    DTUPortNumber;			//	DTU端口号
	int    DTUBaudrate;				//	DTU波特率
	int    nPort;					//  端口
	string strLastEditSoftVersion;  //  上次编辑过的软件版本

	bool bEnableMenuGroup;   //允许使用二级菜单
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
	CString strNodeName;//节点名称
	int nLinkPageID;//链接页面ID
	int nID;//节点自身ID,根节点的ID为0
	int nParentID;//父节点ID
	vector<CEqmDrawDiagnoseGraphicPlus*> vecDiagnoseGraphicItem;//代表叶子节点关联的诊断图表项
};

/************************************图标横轴单位*********************************************/
enum enum_graph_unit
{
	E_NULL,//无
	E_AUTO,//自动
	E_YEAR,//年
	E_MONTH,//月
	E_YEAR_MONTH,//年月
	E_MONTH_DAY,//月日
	E_MONTH_DAY_HOUR,//月日时
	E_HOUR_MINUTE,//时分
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
	CString				strPrjName;				//项目名称
	bool				bLoadPoint;				//是否已加载过点表
	int					nPointBaseOffset;		//点表偏移量
	bool				bLoadImage;				//是否已加载过图片资源
	int					nImageBaseOffset;		//图片资源偏移量
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
		strPointName	= _T("点名");
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
	int nErrType;	//0 事件图片  1事件绑定 2按钮  3page_contain_elements背景  4bend 5dashboard 6 chart 7 unit10 8 unit12
	wstring strPointName;
	wstring strPageName;
	wstring strErrInfo;		//出错信息
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
	TEXT_ATTR_TEMPERA,		// 温度
	TEXT_ATTR_HUMIDITY,		// 湿度
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


// 管道弯头类型
typedef enum
{
	bendUnknown	= 0,
	bendThreeUp,		// 三通向上
	bendThreeRight,		// 三通向右
	bendThreeDown,		// 三通向下
	bendThreeLeft,		// 三通向左
	bendCross,			// 四通
	bendTwoLeftUp,		// 二通左上
	bendTwoRightUp,		// 二通右上
	bendTwoRightDown,	// 二通右下
	bendTwoLeftDown,	// 二通左下
}BendType;

// 管道弯头信息
typedef struct tagBendInfo
{
	CPoint		point;
	BendType	type;

	tagBendInfo(void)
	{
		type = bendUnknown;
	}
}BendInfo;

// 版本信息
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
//const CString g_strPointAttrSystem[g_nLenPointAttrSystem] = {_T("无"), _T("暖通"), _T("动力"), _T("照明插座"), _T("精密空调")};
//const CString g_strPointAttrDevice[g_nLen PointAttrDevice] = {_T("无"), _T("冷机"), _T("水泵"), _T("冷却塔"), _T("AHU"), _T("VAV"), _T("系统")};
//const CString g_strPointAttrType[g_nLenPointAttrType] = {_T("无"), _T("电量表"), _T("冷量表"), _T("温度"), _T("流量"), _T("压力"), _T("电流"), _T("功率"), _T("频率"), _T("启停"), _T("报警")};
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
	CEqmDrawBase *pFormatEqmt; //格式刷设备
	vector<CEqmDrawBase*> vecSelectEqmt;
	vector<CEqmDrawBase*> mVecSelectEqmtCopied;
	DRAWTYPE type;
	int nShowLocation;
	bool bIsCreatePipe;
	bool bIsCreatePolygon;//true已创建即进入绘制过程
	Gdiplus::PointF prePoint;
	Gdiplus::PointF curPoint;
	Gdiplus::PointF actPt;//界面鼠标移动位置
	Gdiplus::PointF PrePt;//移动前坐标
	EQMDRAWTYPE m_nDrawMode; //1:chiller,2:Pump,3:coolingpump,4:coolingtower,5:ahu,6:valve,7:pipe,
	SHOWTYPE    m_showType; //显示模式，绘图模式和运行模式
	LineStyles  m_lineStyle;
	bool m_bDrawingLine;
	bool m_bDragging;    //是否拖拽
	bool m_bIsChangeSize;    //是否需要改变大小
	bool m_bIsDrawCell;      //是否画网格
	bool m_bStartChangeSize; //是否正在改变大小
	int  m_ChangeDirection;  //记录改变size的方向
	bool m_bIsCreate2DGraph; //是否画2D图形
	bool m_ifSelectBox; //use mouse select box to select multiple items;
	bool m_bIsChangeBackgroundColor;
	DWORD m_backGroudColor;
	double m_fScreenScale;
	int GetDefaultPipeWidth() const;	//默认管道宽度。
	void SetDefaultPipeWidth(int pipewidth);

	//严观微添加
	void SetLineWidth(const int width);
	const int GetLineWidth() const;
	void SetLineColor(const DWORD color);
	const DWORD GetLineColor() const;
	CEqmDrawProject* m_project;
private:
	PipeColor m_pipeColor; //管道默认颜色		
	int m_defaultpipewidth ;
	//严观微添加
	int				m_iLineWidth;
	DWORD           m_dwLineColor; 
};

#endif
