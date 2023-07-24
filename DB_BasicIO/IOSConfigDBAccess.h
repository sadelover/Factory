#pragma once

#include "DatabaseSession.h"
#include "../Tools/CustomTools/CustomTools.h"
#include "wtypes.h"
#include <vector>
#include <string>
#include <utility>

struct _declspec(dllexport) IOSProjectInfo
{
	IOSProjectInfo()
	{
		nID = 0;
	}
	int nID;							//项目ID
	int nPort;							//项目数据库端口
	wstring strPrjName;					//项目名称
	wstring strDBIP;					//项目数据库IP
	wstring strDBUser;					//项目数据库用户
	wstring strPsw;						//项目数据库密码
	wstring strDBDataBase;				//项目数据库
	wstring strPrjDescription;			//项目描述
	wstring strCountry;					//项目国籍
	wstring strLocation;				//项目地址
	wstring strPicPath;					//项目图片
	wstring strUnitproperty01;
	wstring strUnitproperty02;
	wstring strUnitproperty03;
	wstring strUnitproperty04;
	wstring strUnitproperty05;
};

struct _declspec(dllexport) IOSPageInfo
{
	IOSPageInfo()
	{
		nID = 0;
	}
	int nID;							//页面ID
	int nType;							//页面类型  0:报警 1:系统参数 2:能耗分析
	int nProjectID;						//项目ID
	int nOrder;							//页面顺序
	wstring strPageName;				//页面名称
	wstring strUnitproperty01;
	wstring strUnitproperty02;
	wstring strUnitproperty03;
	wstring strUnitproperty04;
	wstring strUnitproperty05;
};

struct _declspec(dllexport) IOSParamItemInfo
{
	IOSParamItemInfo()
	{
		nID = 0;
	}
	int nID;							//参数ID
	int nParamID;						//所属参数节点ID
	int nOrder;							//顺序
	int nPointType;						//点类型
	wstring strName;					//参数名称
	wstring strPointName;				//参数点名
	wstring strUnit;					//参数单位
	double dHigh;						//高限
	double dLow;						//低限
	wstring strUnitproperty01;
	wstring strUnitproperty02;
	wstring strUnitproperty03;
	wstring strUnitproperty04;
	wstring strUnitproperty05;
};

struct _declspec(dllexport) IOSParamInfo
{
	IOSParamInfo()
	{
		nID = 0;
	}
	int nID;							//参数ID
	int nPageID;						//所属页面ID
	int nOrder;							//顺序
	wstring strTitle;					//节点参数标题
	vector<IOSParamItemInfo> vecItem;	//节点含的参数
	wstring strUnitproperty01;
	wstring strUnitproperty02;
	wstring strUnitproperty03;
	wstring strUnitproperty04;
	wstring strUnitproperty05;
};

struct _declspec(dllexport) IOSChartItemInfo
{
	IOSChartItemInfo()
	{
		nID = 0;
		nProjectID = 0;
	}
	int nID;							//图表项ID
	int nChartID;						//所属图表ID
	int nColor;							//图表项颜色
	int nPointType;						//图表项绑点类型
	int	nProjectID;						//项目ID
	wstring strName;					//图表项名称
	wstring strPointName;				//图表项绑点
	wstring strDescription;				//图表项描述
	wstring strUnitproperty01;
	wstring strUnitproperty02;
	wstring strUnitproperty03;
	wstring strUnitproperty04;
	wstring strUnitproperty05;
};

struct _declspec(dllexport) IOSChartInfo
{
	IOSChartInfo()
	{
		nID = 0;
	}
	int nID;							//图表ID
	int nType;						    //图表类型
	int nYInterval;						//图表Y轴间隔
	int nPageID;						//页面ID
	int nGroupID;						//用户组ID
	wstring strName;					//图表名称
	wstring strXName;					//图表X轴名称
	wstring strXUnit;					//图表X轴单位
	wstring strYName;					//图表X轴名称
	wstring strYUnit;					//图表Y轴单位
	wstring strDescription;				//图表描述
	vector<IOSChartItemInfo> vecItem;	//图表项集合
	wstring strUnitproperty01;
	wstring strUnitproperty02;
	wstring strUnitproperty03;
	wstring strUnitproperty04;
	wstring strUnitproperty05;
};

struct _declspec(dllexport) IOSUserInfo
{
	IOSUserInfo()
	{
		nID = 0;
	}
	int		nID;					//用户ID
	wstring strName;				//用户名称
	wstring strPsw;					//用户密码
	wstring strUnitproperty01;
	wstring strUnitproperty02;
	wstring strUnitproperty03;
	wstring strUnitproperty04;
	wstring strUnitproperty05;
};

struct _declspec(dllexport) IOSConfigInfo
{
	IOSConfigInfo()
	{
		nID = 0;
	}
	int		nID;					//配置ID
	int		nUserID;				//用户ID
	int		nPush;					//是否推送
	int		nPushAll;				//推送全部
	int		nPushMine;				//推送相关
	int		nWarningRefresh;		//报警刷新时间
	int		nPageRefresh;			//页面刷新时间
	wstring strUnitproperty01;
	wstring strUnitproperty02;
	wstring strUnitproperty03;
	wstring strUnitproperty04;
	wstring strUnitproperty05;
};

struct _declspec(dllexport) IOSWarningInfo
{
	IOSWarningInfo()
	{
		nID = 0;
	}
	int nID;							//报警ID
	int nPageID;						//报警页面
	int nImageID;						//报警图标
	wstring strName;					//项目名称
	wstring strUserName;				//报警责任人
	wstring strPointName;				//报警绑点
	wstring strFilter;					//报警筛选
	wstring strUnitproperty01;
	wstring strUnitproperty02;
	wstring strUnitproperty03;
	wstring strUnitproperty04;
	wstring strUnitproperty05;
};

struct _declspec(dllexport) IOSDeviceTokenInfo
{
	IOSDeviceTokenInfo()
	{
		nID = 0;
	}
	int nID;							//设备ID	
	wstring strUserName;				//设备用户
	wstring strToken;					//设备令牌
	wstring strProject;					
	wstring strUnitproperty01;
	wstring strUnitproperty02;
	wstring strUnitproperty03;
	wstring strUnitproperty04;
	wstring strUnitproperty05;
};

struct _declspec(dllexport) IOSCompareInfo
{
	IOSCompareInfo()
	{
		nID = 0;
	}
	int nID;
	int nChartType;
	wstring strPointName;				
	wstring strName;					
	wstring strProjects;					
	wstring strXUnit;
	wstring strYUnit;
	wstring strUnitproperty01;
	wstring strUnitproperty02;
	wstring strUnitproperty03;
	wstring strUnitproperty04;
	wstring strUnitproperty05;
};

struct _declspec(dllexport) IOSUserGroupInfo
{
	IOSUserGroupInfo()
	{
		nID = 0;
	}
	int nID;
	wstring strName;					
};

class _declspec(dllexport) CIOSConfigDBAccess  : public CDatabaseSeesion
{
public:
	CIOSConfigDBAccess(void);
	~CIOSConfigDBAccess(void);

public:

	bool	LoginIOSConfigOK(const wstring strUser, const wstring strPsw);						//登录IOS后台配置数据库成功
		
	bool	ReadAllProjecs(vector<IOSProjectInfo>& vecProject);									//读所有项目信息
	bool	ReadAllPagesByProjectID(const int nPrjID, vector<IOSPageInfo>& vecPage);			//根据项目ID读出项目所有页面
	bool	ReadAllParamByPageID(const int nPage, vector<IOSParamInfo>& vecParam);				//根据页面ID读出页面所有参数节点
	bool	ReadAllParamItemByParamID(const int nParamID, vector<IOSParamItemInfo>& vecItem);	//根据参数ID读出页面所有参数子节点
	bool	ReadAllChartByPageID(const int nPage, vector<IOSChartInfo>& vecChart);				//根据页面ID读出页面所有图表
	bool	ReadAllChartItemByChartID(const int nChartID, vector<IOSChartItemInfo>& vecItem);	//根据图表ID读出图表所有图表项
	bool	ReadAllUser(vector<IOSUserInfo>& vecUser);											//读所有用户信息
	bool	ReadAllWarningByPageID(const int nPageID, vector<IOSWarningInfo>& vecWarning);		//根据页面ID读出页面所有报警
	bool	ReadAllCompares(vector<IOSCompareInfo>& vecCompare);								//读所有项目比较信息
	bool	ReadAllUserGroup(vector<IOSUserGroupInfo>& vecUserGroup);							//读所有用户组
	bool	ReadAllCompareChartByGroupID(const int nGroupID, vector<IOSChartInfo>& vecChart);			//根据用户组ID读出所有图表
	bool	ReadAllCompareChartItemByChartID(const int nChartID, vector<IOSChartItemInfo>& vecItem);	//根据图表ID读出比较图表所有图表项
	
	bool	ReadOneProjecByID(const int nID, IOSProjectInfo& sProjectInfo);						//读指定ID项目信息
	bool	ReadOnePageByID(const int nID, IOSPageInfo& sPageInfo);								//读指定ID页面信息
	bool	ReadOneParamByID(const int nID, IOSParamInfo& sParamInfo);							//读指定ID参数信息
	bool	ReadOneParamItemByID(const int nID, IOSParamItemInfo& sParamItemInfo);				//读指定ID参数子节点信息
	bool	ReadOneChartByID(const int nID, IOSChartInfo& sChartInfo);							//读指定ID图表信息
	bool	ReadOneChartItemByID(const int nID, IOSChartItemInfo& sChartItemInfo);				//读指定ID图表项信息
	bool	ReadOneUserByID(const int nID, IOSUserInfo& sUserInfo);								//读指定ID用户信息
	bool	ReadOneWarningByID(const int nID, IOSWarningInfo& sWarningInfo);					//读指定ID报警信息
	bool	ReadOneCompareByID(const int nID, IOSCompareInfo& sCompareInfo);					//读指定ID项目比较信息
	bool	ReadOneUserGroupByID(const int nID, IOSUserGroupInfo& sUserGroupInfo);				//读指定ID用户组信息
	bool	ReadOneCompareChartByChartID(const int nID, IOSChartInfo& sChartInfo);				//读指定ID图表信息
	bool	ReadOneCompareChartItemByID(const int nID, IOSChartItemInfo& sChartItemInfo);		//读指定ID图表项信息

	bool	InsertOneProject(const IOSProjectInfo sProjectInfo);								//插入一个新项目
	bool	InsertOnePage(const IOSPageInfo sPageInfo);											//插入一个新页面
	bool	InsertOneParam(const IOSParamInfo sParamInfo);										//插入一个新参数
	bool	InsertOneParamItem(const IOSParamItemInfo sParamItemInfo);							//插入一个新参数子节点
	bool	InsertOneChart(const IOSChartInfo sChartInfo);										//插入一个新图表
	bool	InsertOneChartItem(const IOSChartItemInfo sChartItemInfo);							//插入一个新图表项
	bool	InsertOneUser(const IOSUserInfo sUserInfo);											//插入一个新用户
	bool	InsertOneWarning(const IOSWarningInfo sWarningInfo);								//插入一个新报警
	bool	InsertOneCompare(const IOSCompareInfo sCompareInfo);								//插入一个新项目比较
	bool	InsertOneUserGroup(const IOSUserGroupInfo sUserGroupInfo);							//插入一个新用户组
	bool	InsertOneCompareChart(const IOSChartInfo sChartInfo);								//插入一个新图表
	bool	InsertOneCompareChartItem(const IOSChartItemInfo sChartItemInfo);					//插入一个新图表项

	bool	DeleteOneProjectByID(const int nID);												//删除一个项目
	bool	DeleteOnePageByID(const int nID);													//删除一个页面
	bool	DeleteOneParamByID(const int nID);													//删除一个参数
	bool	DeleteOneParamItemByID(const int nID);												//删除一个参数子节点
	bool	DeleteOneChartByID(const int nID);													//删除一个图表
	bool	DeleteOneChartItemByID(const int nID);												//删除一个图表项
	bool	DeleteOneUserByID(const int nID);													//删除一个用户
	bool	DeleteOneWarningByID(const int nID);												//删除一个报警
	bool	DeleteOneCompareByID(const int nID);												//删除一个项目比较
	bool	DeleteOneUserGroupByID(const int nID);												//删除一个用户组
	bool	DeleteOneCompareChartByID(const int nID);											//删除一个图表
	bool	DeleteOneCompareChartItemByID(const int nID);										//删除一个图表项

	bool	UpdateOneProjectByID(const int nID, const IOSProjectInfo sProjectInfo);				//修改一个项目
	bool	UpdateOnePageByID(const int nID, const IOSPageInfo sPageInfo);						//修改一个页面
	bool	UpdateOneParamByID(const int nID, const IOSParamInfo sParamInfo);					//修改一个参数
	bool	UpdateOneParamItemByID(const int nID, const IOSParamItemInfo sParamItemInfo);		//修改一个参数子节点
	bool	UpdateOneChartByID(const int nID, const IOSChartInfo sChartInfo);					//修改一个图表
	bool	UpdateOneChartItemByID(const int nID, const IOSChartItemInfo sChartItemInfo);		//修改一个图表项
	bool	UpdateOneUserByID(const int nID, const IOSUserInfo sUserInfo);						//修改一个用户
	bool	UpdateOneWarningByID(const int nID, const IOSWarningInfo sWarningInfo);				//修改一个报警
	bool	UpdateOneCompareByID(const int nID, const IOSCompareInfo sCompareInfo);				//修改一个项目比较
	bool	UpdateOneUserGroupByID(const int nID, const IOSUserGroupInfo sUserGroupInfo);			//修改一个用户组
	bool	UpdateOneCompareChartByID(const int nID, const IOSChartInfo sChartInfo);			//修改一个图表
	bool	UpdateOneCompareChartItemByID(const int nID, const IOSChartItemInfo sChartItemInfo);//修改一个图表项

	bool	ReadRelationByUserName(const wstring strUserName, vector<wstring>& vecProject);		//根据用户名称读用户与项目关系
	bool	DeleteRelationByProjectName(const wstring strUserName, const wstring strProjectName); //删除一个用户与项目关系
	bool	InsertRelation(const int nUserID, const int nProjectID);							//新建一个用户与项目关系

	bool	ReadAllUsersByUserGroupID(const int nGroupID, vector<IOSUserInfo>& vecUser);
	bool	DeleteGroupRelationByUserID(const int nGroupID, const int nUserID); 
	bool	InsertGroupRelation(const int nGroupID, const int nUserID);							

	bool	ReadProjectConfigByUserID(const int nID, IOSConfigInfo& sProjectConfigInfo);		//根据用户ID读参数
	bool	InsertProjectConfig(const IOSConfigInfo sProjectConfigInfo);						//创建项目配置参数
	bool	DeleteProjcetConfigByUserID(const int nID);										//删除项目配置参数
	bool	UpdateProjectConfigByUserID(const int nID, const IOSConfigInfo sProjectConfigInfo);//修改项目配置参数
	bool	IsNameExist(const wstring strUserName);

	bool	ReadAllDeviceToken(vector<IOSDeviceTokenInfo>& vecDeviceToken);						//读所有设备令牌				

private:
	Project::Tools::Mutex	m_lock;
};

