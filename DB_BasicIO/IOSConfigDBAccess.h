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
	int nID;							//��ĿID
	int nPort;							//��Ŀ���ݿ�˿�
	wstring strPrjName;					//��Ŀ����
	wstring strDBIP;					//��Ŀ���ݿ�IP
	wstring strDBUser;					//��Ŀ���ݿ��û�
	wstring strPsw;						//��Ŀ���ݿ�����
	wstring strDBDataBase;				//��Ŀ���ݿ�
	wstring strPrjDescription;			//��Ŀ����
	wstring strCountry;					//��Ŀ����
	wstring strLocation;				//��Ŀ��ַ
	wstring strPicPath;					//��ĿͼƬ
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
	int nID;							//ҳ��ID
	int nType;							//ҳ������  0:���� 1:ϵͳ���� 2:�ܺķ���
	int nProjectID;						//��ĿID
	int nOrder;							//ҳ��˳��
	wstring strPageName;				//ҳ������
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
	int nID;							//����ID
	int nParamID;						//���������ڵ�ID
	int nOrder;							//˳��
	int nPointType;						//������
	wstring strName;					//��������
	wstring strPointName;				//��������
	wstring strUnit;					//������λ
	double dHigh;						//����
	double dLow;						//����
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
	int nID;							//����ID
	int nPageID;						//����ҳ��ID
	int nOrder;							//˳��
	wstring strTitle;					//�ڵ��������
	vector<IOSParamItemInfo> vecItem;	//�ڵ㺬�Ĳ���
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
	int nID;							//ͼ����ID
	int nChartID;						//����ͼ��ID
	int nColor;							//ͼ������ɫ
	int nPointType;						//ͼ����������
	int	nProjectID;						//��ĿID
	wstring strName;					//ͼ��������
	wstring strPointName;				//ͼ������
	wstring strDescription;				//ͼ��������
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
	int nID;							//ͼ��ID
	int nType;						    //ͼ������
	int nYInterval;						//ͼ��Y����
	int nPageID;						//ҳ��ID
	int nGroupID;						//�û���ID
	wstring strName;					//ͼ������
	wstring strXName;					//ͼ��X������
	wstring strXUnit;					//ͼ��X�ᵥλ
	wstring strYName;					//ͼ��X������
	wstring strYUnit;					//ͼ��Y�ᵥλ
	wstring strDescription;				//ͼ������
	vector<IOSChartItemInfo> vecItem;	//ͼ�����
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
	int		nID;					//�û�ID
	wstring strName;				//�û�����
	wstring strPsw;					//�û�����
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
	int		nID;					//����ID
	int		nUserID;				//�û�ID
	int		nPush;					//�Ƿ�����
	int		nPushAll;				//����ȫ��
	int		nPushMine;				//�������
	int		nWarningRefresh;		//����ˢ��ʱ��
	int		nPageRefresh;			//ҳ��ˢ��ʱ��
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
	int nID;							//����ID
	int nPageID;						//����ҳ��
	int nImageID;						//����ͼ��
	wstring strName;					//��Ŀ����
	wstring strUserName;				//����������
	wstring strPointName;				//�������
	wstring strFilter;					//����ɸѡ
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
	int nID;							//�豸ID	
	wstring strUserName;				//�豸�û�
	wstring strToken;					//�豸����
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

	bool	LoginIOSConfigOK(const wstring strUser, const wstring strPsw);						//��¼IOS��̨�������ݿ�ɹ�
		
	bool	ReadAllProjecs(vector<IOSProjectInfo>& vecProject);									//��������Ŀ��Ϣ
	bool	ReadAllPagesByProjectID(const int nPrjID, vector<IOSPageInfo>& vecPage);			//������ĿID������Ŀ����ҳ��
	bool	ReadAllParamByPageID(const int nPage, vector<IOSParamInfo>& vecParam);				//����ҳ��ID����ҳ�����в����ڵ�
	bool	ReadAllParamItemByParamID(const int nParamID, vector<IOSParamItemInfo>& vecItem);	//���ݲ���ID����ҳ�����в����ӽڵ�
	bool	ReadAllChartByPageID(const int nPage, vector<IOSChartInfo>& vecChart);				//����ҳ��ID����ҳ������ͼ��
	bool	ReadAllChartItemByChartID(const int nChartID, vector<IOSChartItemInfo>& vecItem);	//����ͼ��ID����ͼ������ͼ����
	bool	ReadAllUser(vector<IOSUserInfo>& vecUser);											//�������û���Ϣ
	bool	ReadAllWarningByPageID(const int nPageID, vector<IOSWarningInfo>& vecWarning);		//����ҳ��ID����ҳ�����б���
	bool	ReadAllCompares(vector<IOSCompareInfo>& vecCompare);								//��������Ŀ�Ƚ���Ϣ
	bool	ReadAllUserGroup(vector<IOSUserGroupInfo>& vecUserGroup);							//�������û���
	bool	ReadAllCompareChartByGroupID(const int nGroupID, vector<IOSChartInfo>& vecChart);			//�����û���ID��������ͼ��
	bool	ReadAllCompareChartItemByChartID(const int nChartID, vector<IOSChartItemInfo>& vecItem);	//����ͼ��ID�����Ƚ�ͼ������ͼ����
	
	bool	ReadOneProjecByID(const int nID, IOSProjectInfo& sProjectInfo);						//��ָ��ID��Ŀ��Ϣ
	bool	ReadOnePageByID(const int nID, IOSPageInfo& sPageInfo);								//��ָ��IDҳ����Ϣ
	bool	ReadOneParamByID(const int nID, IOSParamInfo& sParamInfo);							//��ָ��ID������Ϣ
	bool	ReadOneParamItemByID(const int nID, IOSParamItemInfo& sParamItemInfo);				//��ָ��ID�����ӽڵ���Ϣ
	bool	ReadOneChartByID(const int nID, IOSChartInfo& sChartInfo);							//��ָ��IDͼ����Ϣ
	bool	ReadOneChartItemByID(const int nID, IOSChartItemInfo& sChartItemInfo);				//��ָ��IDͼ������Ϣ
	bool	ReadOneUserByID(const int nID, IOSUserInfo& sUserInfo);								//��ָ��ID�û���Ϣ
	bool	ReadOneWarningByID(const int nID, IOSWarningInfo& sWarningInfo);					//��ָ��ID������Ϣ
	bool	ReadOneCompareByID(const int nID, IOSCompareInfo& sCompareInfo);					//��ָ��ID��Ŀ�Ƚ���Ϣ
	bool	ReadOneUserGroupByID(const int nID, IOSUserGroupInfo& sUserGroupInfo);				//��ָ��ID�û�����Ϣ
	bool	ReadOneCompareChartByChartID(const int nID, IOSChartInfo& sChartInfo);				//��ָ��IDͼ����Ϣ
	bool	ReadOneCompareChartItemByID(const int nID, IOSChartItemInfo& sChartItemInfo);		//��ָ��IDͼ������Ϣ

	bool	InsertOneProject(const IOSProjectInfo sProjectInfo);								//����һ������Ŀ
	bool	InsertOnePage(const IOSPageInfo sPageInfo);											//����һ����ҳ��
	bool	InsertOneParam(const IOSParamInfo sParamInfo);										//����һ���²���
	bool	InsertOneParamItem(const IOSParamItemInfo sParamItemInfo);							//����һ���²����ӽڵ�
	bool	InsertOneChart(const IOSChartInfo sChartInfo);										//����һ����ͼ��
	bool	InsertOneChartItem(const IOSChartItemInfo sChartItemInfo);							//����һ����ͼ����
	bool	InsertOneUser(const IOSUserInfo sUserInfo);											//����һ�����û�
	bool	InsertOneWarning(const IOSWarningInfo sWarningInfo);								//����һ���±���
	bool	InsertOneCompare(const IOSCompareInfo sCompareInfo);								//����һ������Ŀ�Ƚ�
	bool	InsertOneUserGroup(const IOSUserGroupInfo sUserGroupInfo);							//����һ�����û���
	bool	InsertOneCompareChart(const IOSChartInfo sChartInfo);								//����һ����ͼ��
	bool	InsertOneCompareChartItem(const IOSChartItemInfo sChartItemInfo);					//����һ����ͼ����

	bool	DeleteOneProjectByID(const int nID);												//ɾ��һ����Ŀ
	bool	DeleteOnePageByID(const int nID);													//ɾ��һ��ҳ��
	bool	DeleteOneParamByID(const int nID);													//ɾ��һ������
	bool	DeleteOneParamItemByID(const int nID);												//ɾ��һ�������ӽڵ�
	bool	DeleteOneChartByID(const int nID);													//ɾ��һ��ͼ��
	bool	DeleteOneChartItemByID(const int nID);												//ɾ��һ��ͼ����
	bool	DeleteOneUserByID(const int nID);													//ɾ��һ���û�
	bool	DeleteOneWarningByID(const int nID);												//ɾ��һ������
	bool	DeleteOneCompareByID(const int nID);												//ɾ��һ����Ŀ�Ƚ�
	bool	DeleteOneUserGroupByID(const int nID);												//ɾ��һ���û���
	bool	DeleteOneCompareChartByID(const int nID);											//ɾ��һ��ͼ��
	bool	DeleteOneCompareChartItemByID(const int nID);										//ɾ��һ��ͼ����

	bool	UpdateOneProjectByID(const int nID, const IOSProjectInfo sProjectInfo);				//�޸�һ����Ŀ
	bool	UpdateOnePageByID(const int nID, const IOSPageInfo sPageInfo);						//�޸�һ��ҳ��
	bool	UpdateOneParamByID(const int nID, const IOSParamInfo sParamInfo);					//�޸�һ������
	bool	UpdateOneParamItemByID(const int nID, const IOSParamItemInfo sParamItemInfo);		//�޸�һ�������ӽڵ�
	bool	UpdateOneChartByID(const int nID, const IOSChartInfo sChartInfo);					//�޸�һ��ͼ��
	bool	UpdateOneChartItemByID(const int nID, const IOSChartItemInfo sChartItemInfo);		//�޸�һ��ͼ����
	bool	UpdateOneUserByID(const int nID, const IOSUserInfo sUserInfo);						//�޸�һ���û�
	bool	UpdateOneWarningByID(const int nID, const IOSWarningInfo sWarningInfo);				//�޸�һ������
	bool	UpdateOneCompareByID(const int nID, const IOSCompareInfo sCompareInfo);				//�޸�һ����Ŀ�Ƚ�
	bool	UpdateOneUserGroupByID(const int nID, const IOSUserGroupInfo sUserGroupInfo);			//�޸�һ���û���
	bool	UpdateOneCompareChartByID(const int nID, const IOSChartInfo sChartInfo);			//�޸�һ��ͼ��
	bool	UpdateOneCompareChartItemByID(const int nID, const IOSChartItemInfo sChartItemInfo);//�޸�һ��ͼ����

	bool	ReadRelationByUserName(const wstring strUserName, vector<wstring>& vecProject);		//�����û����ƶ��û�����Ŀ��ϵ
	bool	DeleteRelationByProjectName(const wstring strUserName, const wstring strProjectName); //ɾ��һ���û�����Ŀ��ϵ
	bool	InsertRelation(const int nUserID, const int nProjectID);							//�½�һ���û�����Ŀ��ϵ

	bool	ReadAllUsersByUserGroupID(const int nGroupID, vector<IOSUserInfo>& vecUser);
	bool	DeleteGroupRelationByUserID(const int nGroupID, const int nUserID); 
	bool	InsertGroupRelation(const int nGroupID, const int nUserID);							

	bool	ReadProjectConfigByUserID(const int nID, IOSConfigInfo& sProjectConfigInfo);		//�����û�ID������
	bool	InsertProjectConfig(const IOSConfigInfo sProjectConfigInfo);						//������Ŀ���ò���
	bool	DeleteProjcetConfigByUserID(const int nID);										//ɾ����Ŀ���ò���
	bool	UpdateProjectConfigByUserID(const int nID, const IOSConfigInfo sProjectConfigInfo);//�޸���Ŀ���ò���
	bool	IsNameExist(const wstring strUserName);

	bool	ReadAllDeviceToken(vector<IOSDeviceTokenInfo>& vecDeviceToken);						//�������豸����				

private:
	Project::Tools::Mutex	m_lock;
};

