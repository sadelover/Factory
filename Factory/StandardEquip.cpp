// StandardEquip.cpp : implementation file
//

#include "stdafx.h"
#include "StandardEquip.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "../ComponentDraw/EqmDrawDefines.h"
#include "MapNavigateSettingDlg.h"
#include "MapNavigateItemSetDlg.h"
#include "../ComponentDraw/ProjectSqlite.h"
#include "CanvasDlg.h"
#include "json/json.h"
#include "json/reader.h"
#include "InputParamDlg.h"
#include <io.h>
// CStandardEquip dialog

IMPLEMENT_DYNAMIC(CStandardEquip, CDialogEx)

CStandardEquip::CStandardEquip(CWnd* pParent /*=NULL*/)
	: CDialogEx(CStandardEquip::IDD, pParent)
	,m_nLayer(4)
	,m_bCreate(true)
	//,m_nEquiptype(0)
	//,m_pProjectSqlite(NULL)
	//,m_bIsCreate(bIsCreate)
	, m_bBanOnDisplay(FALSE)
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	//m_pProjectSqlite = pFrame->m_pPrjSqlite;
}

CStandardEquip::~CStandardEquip()
{
}

void CStandardEquip::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_EQUIP_TEMPLATE_GROUP,  m_combTemplateGroup);
	DDX_Control(pDX, IDC_COMBO_LAYER,  m_comboLayer);
	DDX_CBIndex(pDX, IDC_COMBO_LAYER, m_nLayer);
	DDX_Control(pDX, IDC_COMBO_EQUIP_TEMPLATE, m_combTemplate);
	DDX_Control(pDX, IDC_LIST_CONFIG, m_listSection);
	DDX_Control(pDX, IDC_COMBO_TEMPLATESORT, m_comboxTemplateSort);
	DDX_Check(pDX, IDC_CHECK_BAN_ON_DISPLAY, m_bBanOnDisplay);
}


BEGIN_MESSAGE_MAP(CStandardEquip, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO_LAYER, &CStandardEquip::OnCbnSelchangeComboLayer)
	ON_BN_CLICKED(IDOK, &CStandardEquip::OnBnClickedOk)
	//ON_CBN_SELCHANGE(IDC_COMBO_EQUIPSTYLE, &CStandardEquip::OnCbnSelchangeEquipType)
	//ON_EN_CHANGE(IDC_EDIT_RoomName, &CStandardEquip::OnEnChangeEditRoomname)
	//ON_EN_CHANGE(IDC_EDIT_EQU_TYPE, &CStandardEquip::OnEnChangeEditEquType)
	//ON_EN_CHANGE(IDC_EDIT_ERR_INTERVAL, &CStandardEquip::OnEnChangeEditErrInterval)
	//ON_EN_CHANGE(IDC_EDIT_EquipNum, &CStandardEquip::OnEnChangeEditErrInterval)
	//ON_CBN_SELCHANGE(IDC_COMBO_EQUIPTYPE3, &CStandardEquip::OnCbnSelchangeComboEquiptype3)
	/*ON_CBN_SELCHANGE(IDC_COMBO_EQUIPTYPE, &CStandardEquip::OnCbnSelchangeComboEquiptype)*/
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_CONFIG, &CStandardEquip::OnLvnItemchangedListConfig)
	ON_CBN_SELCHANGE(IDC_COMBO_EQUIP_TEMPLATE, &CStandardEquip::OnCbnSelchangeComboEquipTemplate)
	ON_CBN_SELCHANGE(IDC_COMBO_EQUIP_TEMPLATE_GROUP, &CStandardEquip::OnCbnSelchangeComboEquipTemplateGroup)
	//ON_NOTIFY(HDN_ITEMDBLCLICK,0, &CStandardEquip::OnHdnItemdblclickListConfig)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_CONFIG, &CStandardEquip::OnNMDblclkListConfig)
	ON_CBN_SELCHANGE(IDC_COMBO_TEMPLATESORT, &CStandardEquip::OnCbnSelchangeComboTemplatesort)
END_MESSAGE_MAP()


// CStandardEquip message handlers


void CStandardEquip::OnCbnSelchangeComboLayer()
{
	// TODO: Add your control notification handler code here
}



BOOL CStandardEquip::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	bool bFindTemplate = false;

	pMainFrame = (CMainFrame*)AfxGetApp()->GetMainWnd();
	//获取安装位置下的模板目录
	GetInstallTemplateList(m_listTemplateList);
	//遍历目录查找 SYSTEAM_TEMPLATE
	if(m_cstrTemplateName == L"")//并向下兼容
	{
		m_cstrTemplateName	= SYSTEAM_TEMPLATE;
		bFindTemplate		= true;
	}
	else
	{
		for(int i =0;i<m_listTemplateList.size();i++)
		{
			if(m_cstrTemplateName == m_listTemplateList[i])
			{
				bFindTemplate = true;
				break;
			}
		}
	}
	//判断获取到的模板名是否真实在目录下
	if(bFindTemplate == false)
	{
		CString cstrAfx;
		cstrAfx.Format(L"没有在template目录下找到%s。",m_cstrTemplateName);
		AfxMessageBox(cstrAfx);
	}
	else
	{
		//获取模板列表
		pMainFrame->m_pPrjSqlite->ReadTemplatePageGroupList(m_cstrTemplateName, m_vecPageGroupNameList);//获取模板分类
		//删掉系统模板的其他分组
		if(m_cstrTemplateName == SYSTEAM_TEMPLATE)
		{
			for(vector<GroupPage>::iterator iter = m_vecPageGroupNameList.begin() ;iter!=m_vecPageGroupNameList.end();iter++)
			{
				if(iter->GroupName == L"其他")
				{
					m_vecPageGroupNameList.erase(iter);
					break;
				}
			}
		}
		
		if(m_vecPageGroupNameList.size()>0)
		{
			int nDefaultGroupId = m_vecPageGroupNameList[0].GroupID;
			pMainFrame->m_pPrjSqlite->ReadTemplatePageList(m_cstrTemplateName, nDefaultGroupId, m_vecPageTemplateInfoList);
		}

		// combo box
		m_comboLayer.AddString(_T("1层"));
		m_comboLayer.AddString(_T("2层"));
		m_comboLayer.AddString(_T("3层"));
		m_comboLayer.AddString(_T("4层"));
		m_comboLayer.AddString(_T("5层"));
		m_comboLayer.AddString(_T("6层"));
		m_comboLayer.AddString(_T("7层"));
		m_comboLayer.AddString(_T("8层"));
		m_comboLayer.AddString(_T("9层"));
		m_comboLayer.AddString(_T("10层"));

		m_listSection.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES); // 整行选择、网格线
		m_listSection.InsertColumn(0, _T("参数代号"), LVCFMT_LEFT, 100);
		m_listSection.InsertColumn(1, _T("参数名"), LVCFMT_LEFT, 100); // 插入第2列的列名
		m_listSection.InsertColumn(2, _T("注释"), LVCFMT_LEFT, 200); 
		m_listSection.InsertColumn(3, _T("参数配置"), LVCFMT_LEFT, 100);
		InitCombox_TemplateSort();

		InitControls();
		InitData();
		m_nTemplateGroupLastChoice	= m_combTemplateGroup.GetCurSel();
		m_nTemplateLastChoice		= m_combTemplate.GetCurSel();
	}
	return TRUE;
}
void CStandardEquip::InitControls(void)
{
	//模版页面目录组合框初始化
	int nMySelIndex = -1;

	m_combTemplateGroup.ResetContent();
	m_combTemplate.ResetContent();

	//遍历模板分类并添加至combox，并找到分类ID
	for(int i=0;i<m_vecPageGroupNameList.size();i++)
	{
		int ncur = m_combTemplateGroup.AddString(m_vecPageGroupNameList[i].GroupName.c_str());
		m_combTemplateGroup.SetItemData(ncur, m_vecPageGroupNameList[i].GroupID);

		if(m_vecPageGroupNameList[i].GroupID ==m_nTempateGroupId)//判断ID模板分类
		{
			nMySelIndex = ncur;
		}
	}
	//遍历模板插入combox并找到选择的模板，最后插入其参数
	if(nMySelIndex>=0)
	{
		m_combTemplateGroup.SetCurSel(nMySelIndex);
		m_bFindTemplate = false;//用于判断是否找到对应的模板
		/*
			初始化模板页面清单
		*/
		int nDefaultGroupId = m_vecPageGroupNameList[nMySelIndex].GroupID;
		m_vecPageTemplateInfoList.clear();
		pMainFrame->m_pPrjSqlite->ReadTemplatePageList(m_cstrTemplateName, nDefaultGroupId, m_vecPageTemplateInfoList);
		for(int i=0;i<m_vecPageTemplateInfoList.size();i++)
		{
			int ncur = m_combTemplate.AddString(m_vecPageTemplateInfoList[i].m_strName);
			m_combTemplate.SetItemData(ncur, m_vecPageTemplateInfoList[i].m_nID);
		

			if(m_strTemplatePageName.GetLength()>0 && m_vecPageTemplateInfoList[i].m_strName==m_strTemplatePageName)
			{//这里进入修改流程
				m_bFindTemplate = true;
				m_combTemplate.SetCurSel(ncur);
				SetListSection();
			}
		}

		/*
			页面模板选择有误自动纠正
		*/
		if (m_bCreate == false)
		{
			if(m_bFindTemplate == false)
			{
				bool bFind = false;
				//遍历所有的模板目录
				for(int nIter = 0; nIter < m_vecPageGroupNameList.size(); nIter++)
				{
					//遍历目录下的模板
					m_vecPageTemplateInfoList.clear();
					pMainFrame->m_pPrjSqlite->ReadTemplatePageList(m_cstrTemplateName,m_vecPageGroupNameList[nIter].GroupID, m_vecPageTemplateInfoList);
					for(int i=0;i<m_vecPageTemplateInfoList.size();i++)
					{
						int ncur = m_combTemplate.AddString(m_vecPageTemplateInfoList[i].m_strName);
						m_combTemplate.SetItemData(ncur, m_vecPageTemplateInfoList[i].m_nID);
						if(m_strTemplatePageName.GetLength()>0 && m_vecPageTemplateInfoList[i].m_strName==m_strTemplatePageName)
						{
							m_bFindTemplate = true;
							bFind = true;
							m_combTemplate.SetCurSel(ncur);
							m_combTemplateGroup.SetCurSel(nIter);
							SetListSection();
							AfxMessageBox(L"自动纠正模板分类选择错误，请点击确定。");
							break;
						}
					}
					if(bFind == true)
						break;
				}

				if(m_bFindTemplate == false)
				{
					CString cstrAfx;
					cstrAfx.Format(L"未发现标准设备：\"%s\",请在template.4db中核查确认。",m_strTemplatePageName);
					AfxMessageBox(cstrAfx);
				}
			}
		}
	}else
	{
		for(int i=0;i<m_vecPageTemplateInfoList.size();i++){
			int ncur = m_combTemplate.AddString(m_vecPageTemplateInfoList[i].m_strName);
			m_combTemplate.SetItemData(ncur, m_vecPageTemplateInfoList[i].m_nID);
		}
	}
}

/*
 * 函数描述：模板选择Combox初始化函数
*/
void CStandardEquip::InitCombox_TemplateSort()
{
	//添加至列表
	for(int nListCount=0;nListCount < m_listTemplateList.size(); nListCount++)
	{
		
		if (m_listTemplateList[nListCount]== "template.4db")
			m_comboxTemplateSort.InsertString(nListCount, L"system");
		else//去掉.4db插入
		{
			CString cstrTemp;
			cstrTemp = m_listTemplateList[nListCount].Left(m_listTemplateList[nListCount].Find(L"."));
			m_comboxTemplateSort.InsertString(nListCount, cstrTemp);
		}
		//设置显示
		if(m_listTemplateList[nListCount]== m_cstrTemplateName)
			m_comboxTemplateSort.SetCurSel(nListCount);
	}
}


void CStandardEquip::SetListSection(void)
{
	int nSelId = m_combTemplate.GetCurSel();//获取选择的模板在combox中的编号

	CString wstrTemplateName;
	m_combTemplate.GetLBText(nSelId, wstrTemplateName);
	string strTemplateName;
	//string strConfig;
	strTemplateName = Project::Tools::WideCharToAnsi(wstrTemplateName.GetString());
				
	//获取标准设备配置
	CMainFrame *pMainFrame = static_cast<CMainFrame*>(AfxGetMainWnd());
	wstring globalConfig = _T("globalconfig");
	wstring wstr = pMainFrame->m_pPrjSqlite->GetGlobalConfig(m_cstrTemplateName,globalConfig);

	string strConfig;
	strConfig = Project::Tools::WideCharToAnsi(wstr.c_str());
	Json::Value myRoot;
	Json::Reader jsReader;
	if(!jsReader.parse(strConfig, myRoot))
	{
		AfxMessageBox(_T("模板中未定义参数"));
		return;
	}

	m_listSection.DeleteAllItems();


	Json::Value templateConfigJson = myRoot[strTemplateName];
	Json::Value strJson;
	string str;
	str = Project::Tools::WideCharToAnsi(m_strTemplateParam.GetString());
	jsReader.parse(str,strJson);
	/*vector<CString> m_strParamList;
	m_strParamList.clear();
	Project::Tools::SplitStringByChar(m_strTemplateParam, ',', m_strParamList);*/
	for(int i=0;i<100;i++)
	{
		CString wstrKey;
		wstrKey.Format(_T("%d"), i+1);
		string strKey;
		Project::Tools::WideCharToUtf8(wstrKey.GetString(),strKey);

		Json::Value jsonOneKey = templateConfigJson[strKey];
		if(jsonOneKey.isNull())
		{
			break;
		}
		string name = strJson[i][strKey].toStyledString();
		CString cStrKey =Project::Tools::AnsiToWideChar(name.data()).c_str();
		string strName = jsonOneKey["name"].toStyledString();
		CString cstrName;
		//cstrName.Format(_T("%s"), strName.c_str());
		cstrName = Project::Tools::AnsiToWideChar(strName.data()).c_str();

		string strDescription = jsonOneKey["description"].toStyledString();
		CString cstrDescription;
		//cstrDescription.Format(_T("%s"), strDescription.c_str());
		cstrDescription = Project::Tools::AnsiToWideChar(strDescription.data()).c_str();
		// 插入第3列的列名
		cstrName.Replace(_T("\n"),_T(""));
		wstrKey.Replace(_T("\n"),_T(""));
		cstrDescription.Replace(_T("\n"),_T(""));
		cStrKey.Replace(_T("\n"),_T(""));

		cstrName.Replace(_T("\""),_T(""));
		wstrKey.Replace(_T("\""),_T(""));
		cstrDescription.Replace(_T("\""),_T(""));
		cStrKey.Replace(_T("\""),_T(""));

		int ncur = m_listSection.InsertItem(i, cstrName);
		m_listSection.SetItemText(ncur,0, wstrKey);
		m_listSection.SetItemText(ncur,1, cstrName);
		m_listSection.SetItemText(ncur,2, cstrDescription);
		m_listSection.SetItemText(ncur,3, cStrKey);
	}
}
void CStandardEquip::InitData()
{
	if (m_nLayer==0)
	{
		m_comboLayer.SetCurSel(0);
	}
	if (m_nLayer==1)
	{
		m_comboLayer.SetCurSel(1);
	}
	if (m_nLayer==2)
	{
		m_comboLayer.SetCurSel(2);
	}
	if (m_nLayer==3)
	{
		m_comboLayer.SetCurSel(3);
	}
	if (m_nLayer==4)
	{
		m_comboLayer.SetCurSel(4);
	}
	if (m_nLayer==5)
	{
		m_comboLayer.SetCurSel(5);
	}
	if (m_nLayer==6)
	{
		m_comboLayer.SetCurSel(6);
	}
	if (m_nLayer==7)
	{
		m_comboLayer.SetCurSel(7);
	}
	if (m_nLayer==8)
	{
		m_comboLayer.SetCurSel(8);
	}
	if (m_nLayer==9)
	{
		m_comboLayer.SetCurSel(9);
	}

	/*if (m_nEquiptype==0)
	{
		m_combEquiptype.SetCurSel(0);
	}
	if (m_nEquiptype==1)
	{
		m_combEquiptype.SetCurSel(1);
	}
	if (m_nEquiptype==2)
	{
		m_combEquiptype.SetCurSel(2);
	}
	if (m_nEquiptype==3)
	{
		m_combEquiptype.SetCurSel(3);
	}
	if (m_nEquiptype==4)
	{
		m_combEquiptype.SetCurSel(4);
	}
	if (m_nEquiptype==5)
	{
		m_combEquiptype.SetCurSel(5);
	}
	if (m_nEquiptype==6)
	{
		m_combEquiptype.SetCurSel(6);
	}
	if (m_nEquiptype==7)
	{
		m_combEquiptype.SetCurSel(7);
	}
	if (m_nEquipStyle==0)
	{
		m_combEquipstyle.SetCurSel(0);
	}
	if (m_nEquipStyle==1)
	{
		m_combEquipstyle.SetCurSel(1);
	}*/
}

void CStandardEquip::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	m_nTempateGroupId = m_combTemplateGroup.GetItemData(m_combTemplateGroup.GetCurSel());
	m_nTemplatePageId = m_combTemplate.GetItemData(m_combTemplate.GetCurSel());
	m_combTemplate.GetLBText( m_combTemplate.GetCurSel(), m_strTemplatePageName);
	m_strTemplateParam = _T("");

	Json::Value myRoot;

	//CString cstr;
	for(int i=0;i<m_listSection.GetItemCount();i++)
	{
		//cstr = m_strTemplateParam;
		CString cstrValue = m_listSection.GetItemText(i,3);
		//m_strTemplateParam.Format(_T("%s\"%d\":\"%s\""),cstr,(i+1),str);
		string strValue;
		CString cstrKey;
		cstrKey.Format(_T("%d"), i+1);
		string strKey;
		Project::Tools::WideCharToUtf8(cstrKey.GetString(), strKey);
		Project::Tools::WideCharToUtf8(cstrValue.GetString(),strValue);
		Json::Value oneItem;
		oneItem[strKey] = strValue;
		myRoot.append(oneItem);
		
	}
	string str = myRoot.toStyledString();
	//m_strTemplateParam = str.c_str();//这样赋值是把一个窄字符的字符串给了宽字符的字符串，那么在解析的时候会造成乱码。
	m_strTemplateParam = Project::Tools::UTF8ToWideChar(str.c_str()).c_str();
	m_strTemplateParam.Replace(_T("\n"),_T(""));
	
	CDialogEx::OnOK();
}

int CStandardEquip::GetLayer(void) const
{
	return m_nLayer;
}

void CStandardEquip::SetLayer(const int nLayer)
{
	m_nLayer = nLayer;
}
int CStandardEquip:: GetEquipNo(void) const
{
	return 0;//m_nEquipNo;
}

void CStandardEquip::SetEquipNo(const int nEquipNo)
{
	//m_nEquipNo = nEquipNo;
}

void CStandardEquip::SetMapPageInfo(const vector<CMapPageInfo>& vecMapPageInfo)
{
	m_vecMapPageInfo = vecMapPageInfo;
}


vector<CMapPageInfo> CStandardEquip::GetMapPageInfo(void) const
{
	return m_vecMapPageInfo;
}

int CStandardEquip::GetEquipType(void)const
{
	return 0;//m_nEquiptype;
}
void    CStandardEquip::SetEquipType(const int nEquipType)
{
	//m_nEquiptype = nEquipType;
}
int CStandardEquip::GetEquipStyle(void)const
{
	return 0;//m_nEquipStyle;
}
void    CStandardEquip::SetEquipStyle(const int nEquipStyle)
{
	//m_nEquipStyle = nEquipStyle;
}

//void CStandardEquip::OnCbnSelchangeEquipType()
//{
//	int nCurSel = m_combEquipstyle.GetCurSel();
//	if (CB_ERR == nCurSel)
//	{
//		return;
//	}
//	SetShowPicCombination(m_nEquiptype,nCurSel);
//}
//void CStandardEquip::SetShowPicCombination(const int nType，const int nstyle)//type是冷机设备等类型，style是冷机图片类型
//{
//	vector<int> vecId;
//	if(0 == nType)
//	{//冷机
//		if (0 == nstyle)
//		{	// 类型一
//			InsertButtonGroupIntoOpenS3db(m_vecEquipCh, vecId);
//		}
//
//	}
//	if(1 == nType)
//	{//一次冷却泵
//		if (0 == nstyle)
//		{	//
//			InsertButtonGroupIntoOpenS3db(m_vecEquipPriCWP, vecId);
//		}
//		else if (1 == nType)
//		{	// yellow button
//			InsertButtonGroupIntoOpenS3db(m_vecEquipPriCWP, vecId);
//		}
//	}
//	if (0 == nType)
//	{	// red button
//		InsertButtonGroupIntoOpenS3db(m_vecEquipCh, vecId);
//	}
//	else if (1 == nType)
//	{	// yellow button
//		InsertButtonGroupIntoOpenS3db(m_vecEquipCh, vecId);
//	}
//	else if (2 == nType)
//	{	// green button
//		InsertButtonGroupIntoOpenS3db(m_vecBtnGreen, vecId);
//	}
//	else
//	{
//		return;
//	}
//		Invalidate();
//}
//void CStandardEquip::LoadPreButtonImage(void)
//{
//	CString	strSysPath;
//	GetSysPath(strSysPath);
//	strSysPath += _T("\\config\\");
//
//	CString	arrNameCh[5][2];
//	CString arrPathCh[5][2];
//	arrNameCh[0][0] = _T("Ch_00.png");
//	arrNameCT[1][1] = _T("CT_00.png");
//	arrNameCT[1][2] = _T("CT_01.png");
//	CString	arrNameCWP[2];
//	CString arrPathCWP[2];
//	arrNameCWP[0] = _T("CWP_00.png");
//	arrNameCWP[1] = _T("CWP_01.png");
//	CString	arrNamePriCWP[2];
//	CString arrPathPriCWP[2];
//	arrNamePriCWP[0] = _T("PriCWP_01.png");
//	arrNamePriCWP[1] = _T("PriCWP_01.png");
//	CString	arrNameSecCWP[2];
//	CString arrPathSecCWP[2];
//	arrNameSecCWP[0] = _T("SecCWP_00.png");
//	arrNameSecCWP[1] = _T("SecCWP_01.png");
//	SetDeviceStruct(arrNameCh[1], arrPathCh[1], m_vecBtnRed);
//	SetDeviceStruct(arrNameCT[2], arrNameCT[2], m_vecBtnYellow);
//	SetDeviceStruct(arrNameGreen, arrPathGreen, m_vecBtnGreen);
//}

void CStandardEquip::InsertButtonGroupIntoOpenS3db(const vector<PictureInfo>& vecPicInf, vector<int>& vecId)
{
	//if (NULL == m_pProjectSqlite)
	//{
	//	return;
	//}

	//USES_CONVERSION;
	//vector<PictureInfo>::const_iterator iter;
	//int i = 0;
	//for (iter=vecPicInf.begin(); iter!=vecPicInf.end(); iter++,i++)
	//{
	//	PictureInfo* pSrcInfo = const_cast<PictureInfo*>(&(*iter));
	//	if (NULL == pSrcInfo)
	//	{
	//		continue;;
	//	}

	//	PictureInfo* pDstInfo = m_pProjectSqlite->GetPictureInfo(pSrcInfo->szName, pSrcInfo->nSize, pSrcInfo->remark);
	//	if (NULL == pDstInfo)
	//	{	// 打开的s3db没有该图，直接插入数据库
	//		int nNewId = m_pProjectSqlite->GetMaxPictureNum(true) + 1;
	//		m_pProjectSqlite->InsertPictureData(nNewId, T2A(pSrcInfo->szName), Project::Tools::WideCharToAnsi(pSrcInfo->etype_equip.c_str()).c_str(), pSrcInfo->pBlock, pSrcInfo->nSize, true, 0, 0, T2A((pSrcInfo->etype_p).c_str()));

	//		// 插入内存结构
	//		m_pProjectSqlite->AddIntoPictureList(nNewId, pSrcInfo);

	//		vecId.push_back(nNewId);
	//	}
	//	else
	//	{	// 打开的s3db有该图
	//		vecId.push_back(pDstInfo->ID);
	//	}
	//}
}

//void CStandardEquip::SetButtonGroupStruct(const CString arrName[2], const CString arrPath[2], vector<PictureInfo>& vecBtn)
//{
	/*vector<PictureInfo>::const_iterator	iter;
	assert(m_pProjectSqlite != NULL);
	if (NULL == m_pProjectSqlite)
	{
		return;
	}

	vecBtn.clear();
	for (int i=0; i<5 ;i++)
	{
		FILE* pFile = NULL;
		const errno_t rs = _wfopen_s(&pFile, arrPath[i].GetString(), L"rb");
		assert(0 == rs);
		assert(pFile);
		fseek(pFile, 0, SEEK_END);
		const int file_size = ftell(pFile);
		assert(file_size > 0);
		char* pBuf = new char[file_size];
		memset(pBuf, 0, file_size);
		fseek(pFile, 0, SEEK_SET);
		fread(pBuf, sizeof(char), file_size, pFile);
		const int nPicID = m_pProjectSqlite->GetMaxLibImageNum(false)+1+INSERT_META_FILE_ID_START;
		char szFileName[MAX_PATH] = {0};
		size_t converted = 0;
		wcstombs_s(&converted, szFileName, sizeof(szFileName), arrName[i].GetString(), sizeof(szFileName));
		assert(converted > 0);
		fclose(pFile);

		{
			CMemFile   memfile;  
			memfile.Attach((BYTE*)(pBuf),file_size,1024);  
			memfile.SetLength(file_size); 
			HGLOBAL hMemBmp = GlobalAlloc(GMEM_FIXED, file_size);
			if (hMemBmp)
			{
				IStream* pStmBmp = NULL;
				if(CreateStreamOnHGlobal(hMemBmp, FALSE, &pStmBmp) == S_OK)
				{
					BYTE* pbyBmp = (BYTE *)GlobalLock(hMemBmp);
					memfile.SeekToBegin();
					memfile.Read(pbyBmp, file_size);
					Image* pBitmap = Image::FromStream(pStmBmp);
					PictureInfo info;
					info.ID = nPicID;
					info.pBitmap = pBitmap;
					info.etype_equip = L"7";
					wcsncpy_s(info.szName, sizeof(info.szName)/sizeof(wchar_t), arrName[i].GetString(), sizeof(info.szName)/sizeof(wchar_t)-1);
					info.nSize = file_size;
					info.pBlock = new char[file_size];
					if (info.pBlock != NULL)
					{
						memset(info.pBlock, 0, file_size);
						memcpy_s(info.pBlock, file_size, pBuf, file_size);
					}

					info.nBtnId = 0;
					info.nBtnSubId = i;
					vecBtn.push_back(info);

					GlobalUnlock(hMemBmp);
				}
				GlobalFree(hMemBmp);
				hMemBmp = NULL;
			}
		}
		SAFE_DELETE_ARRAY(pBuf);
	}*/
//}

void CStandardEquip::OnLvnItemchangedListConfig(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}

/*
 * 函数描述：页面模板选择事件服务函数
*/
void CStandardEquip::OnCbnSelchangeComboEquipTemplate()
{
	// TODO: Add your control notification handler code here
	int nSelId = m_combTemplate.GetCurSel();
	if(m_nTemplateLastChoice == nSelId)//选择没有变化则退出
		return;
	m_nTemplateLastChoice = nSelId;

	int nTemplateId = m_combTemplate.GetItemData(nSelId);

	CString wstrTemplateName;
	m_combTemplate.GetLBText(nSelId, wstrTemplateName);
	string strTemplateName;
	//string strConfig;
	strTemplateName = Project::Tools::WideCharToAnsi(wstrTemplateName.GetString());
	
	CMainFrame *pMainFrame = static_cast<CMainFrame*>(AfxGetMainWnd());
	wstring globalConfig = _T("globalconfig");
	wstring wstr = pMainFrame->m_pPrjSqlite->GetGlobalConfig(m_cstrTemplateName,globalConfig);

	string strConfig;
	strConfig = Project::Tools::WideCharToAnsi(wstr.c_str());
	Json::Value myRoot;
	Json::Reader jsReader;
	if(!jsReader.parse(strConfig, myRoot))
	{
		AfxMessageBox(_T("模板中未定义参数"));
		return;
	}

	m_listSection.DeleteAllItems();


	Json::Value templateConfigJson = myRoot[strTemplateName];
	for(int i=0;i<100;i++)
	{
		CString wstrKey;
		wstrKey.Format(_T("%d"), i+1);
		string strKey;
		Project::Tools::WideCharToUtf8(wstrKey.GetString(),strKey);

		Json::Value jsonOneKey = templateConfigJson[strKey];
		if(jsonOneKey.isNull())
		{
			break;
		}
		
		string strName = jsonOneKey["name"].toStyledString();
		CString cstrName;
		//cstrName.Format(_T("%s"), strName.c_str());
		cstrName = Project::Tools::AnsiToWideChar(strName.data()).c_str();

		string strDescription = jsonOneKey["description"].toStyledString();
		CString cstrDescription;
		//cstrDescription.Format(_T("%s"), strDescription.c_str());
		cstrDescription = Project::Tools::AnsiToWideChar(strDescription.data()).c_str();
		// 插入第3列的列名
		cstrName.Replace(_T("\n"),_T(""));
		wstrKey.Replace(_T("\n"),_T(""));
		cstrDescription.Replace(_T("\n"),_T(""));
		

		cstrName.Replace(_T("\""),_T(""));
		wstrKey.Replace(_T("\""),_T(""));
		cstrDescription.Replace(_T("\""),_T(""));
		
		int ncur = m_listSection.InsertItem(i, cstrName);
		m_listSection.SetItemText(ncur,0, wstrKey);
		m_listSection.SetItemText(ncur,1, cstrName);
		m_listSection.SetItemText(ncur,2, cstrDescription);
		m_listSection.SetItemText(ncur,3, wstrKey);
		
	}
	
}
/*
 * 函数描述：页面模板Combo选择后初始化参数列表
*/
void CStandardEquip::SelchangeComboEquipTemplate()
{
	// TODO: Add your control notification handler code here
	int nSelId = m_combTemplate.GetCurSel();
	m_nTemplateLastChoice = nSelId;
	int nTemplateId = m_combTemplate.GetItemData(nSelId);

	CString wstrTemplateName;
	m_combTemplate.GetLBText(nSelId, wstrTemplateName);
	string strTemplateName;
	//string strConfig;
	strTemplateName = Project::Tools::WideCharToAnsi(wstrTemplateName.GetString());
	
	CMainFrame *pMainFrame = static_cast<CMainFrame*>(AfxGetMainWnd());
	wstring globalConfig = _T("globalconfig");
	wstring wstr = pMainFrame->m_pPrjSqlite->GetGlobalConfig(m_cstrTemplateName,globalConfig);

	string strConfig;
	strConfig = Project::Tools::WideCharToAnsi(wstr.c_str());
	Json::Value myRoot;
	Json::Reader jsReader;
	if(!jsReader.parse(strConfig, myRoot))
	{
		AfxMessageBox(_T("模板中未定义参数"));
		return;
	}

	m_listSection.DeleteAllItems();


	Json::Value templateConfigJson = myRoot[strTemplateName];
	for(int i=0;i<100;i++)
	{
		CString wstrKey;
		wstrKey.Format(_T("%d"), i+1);
		string strKey;
		Project::Tools::WideCharToUtf8(wstrKey.GetString(),strKey);

		Json::Value jsonOneKey = templateConfigJson[strKey];
		if(jsonOneKey.isNull())
		{
			break;
		}
		
		string strName = jsonOneKey["name"].toStyledString();
		CString cstrName;
		//cstrName.Format(_T("%s"), strName.c_str());
		cstrName = Project::Tools::AnsiToWideChar(strName.data()).c_str();

		string strDescription = jsonOneKey["description"].toStyledString();
		CString cstrDescription;
		//cstrDescription.Format(_T("%s"), strDescription.c_str());
		cstrDescription = Project::Tools::AnsiToWideChar(strDescription.data()).c_str();
		// 插入第3列的列名
		cstrName.Replace(_T("\n"),_T(""));
		wstrKey.Replace(_T("\n"),_T(""));
		cstrDescription.Replace(_T("\n"),_T(""));
		

		cstrName.Replace(_T("\""),_T(""));
		wstrKey.Replace(_T("\""),_T(""));
		cstrDescription.Replace(_T("\""),_T(""));
		
		int ncur = m_listSection.InsertItem(i, cstrName);
		m_listSection.SetItemText(ncur,0, wstrKey);
		m_listSection.SetItemText(ncur,1, cstrName);
		m_listSection.SetItemText(ncur,2, cstrDescription);
		m_listSection.SetItemText(ncur,3, wstrKey);
		
	}
	
}


/*
 * 函数描述：“模板分类”选择事件服务函数
*/

void CStandardEquip::OnCbnSelchangeComboEquipTemplateGroup()
{
	// TODO: Add your control notification handler code here
	//int m_nID = m_combTemplateGroup.GetItemData("m_nID");
	//return m_nID;
	
	int m_nSelId = m_combTemplateGroup.GetCurSel();
	if(m_nTemplateGroupLastChoice == m_nSelId)//选择没变化退出
		return;
	m_nTemplateGroupLastChoice = m_nSelId;

	m_listSection.DeleteAllItems();
	int m_nGroupId = m_combTemplateGroup.GetItemData(m_nSelId);
	pMainFrame->m_pPrjSqlite->ReadTemplatePageList(m_cstrTemplateName, m_nGroupId, m_vecPageTemplateInfoList);
	m_combTemplate.ResetContent();
	for(int i=0;i<m_vecPageTemplateInfoList.size();i++)
	{
		int ncur = m_combTemplate.AddString(m_vecPageTemplateInfoList[i].m_strName);
		m_combTemplate.SetItemData(ncur, m_vecPageTemplateInfoList[i].m_nID);
	}
	m_combTemplate.SetCurSel(0);
	SelchangeComboEquipTemplate();
}





void CStandardEquip::OnNMDblclkListConfig(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int col = 0;
	int row = 0;
	row = pNMListView->iItem;
	col = pNMListView->iSubItem;
	CString cstrParam(m_listSection.GetItemText(row, 3));//获取第三列第row行的参数
	CInputParamDlg dlg;
	dlg.m_strParamValue = cstrParam;
	//注释原因：表达不清晰或者以前有特殊需求逻辑
	//if(row>=m_strParamValueList.size())
	//{
	//	m_strParamValueList.push_back(L"");
	//}
	//else
	//{
	//	dlg.m_strParamValue = m_strParamValueList[row];
	//}

	if(dlg.DoModal()==IDOK)
	{

		//m_strParamValueList[row] = dlg.m_strParamValue;
		m_listSection.SetItemText(row,3, dlg.m_strParamValue);
	}
	*pResult = 0;
}


/*----------------------------
* 功能 : 遍历目录下的所有文件，并返回
*----------------------------
* 函数 : GetFileListForDir
* 访问 : public
*
* 参数 : fileList [Out] 以文件名称的形式存储遍历后的文件
*/
void CStandardEquip::GetInstallTemplateList(vector<CString> &fileList)
{
	fileList.clear();

	CString cstrRunningPath;
	Project::Tools::GetSysPath(cstrRunningPath);
	cstrRunningPath += L".\\template\\*";

	string strPath;
	Project::Tools::WideCharToUtf8(cstrRunningPath.GetString(), strPath);
	intptr_t handle;                                                //用于查找的句柄
	_finddata_t fileinfo;                          //文件信息的结构体
	handle = _findfirst(strPath.c_str(), &fileinfo);         //第一次查找
	if (-1 == handle)
		return;
	
	if((strcmp(fileinfo.name,".") != 0) && (strcmp(fileinfo.name,"..") != 0) && strstr(fileinfo.name,".4db"))
	{
		wstring wstrTemp;
		Project::Tools::UTF8ToWideChar(string(fileinfo.name),wstrTemp);
		fileList.push_back(wstrTemp.c_str());   //打印出找到的文件的文件名
	}
	while (!_findnext(handle, &fileinfo))               //循环查找其他符合的文件，知道找不到其他的为止
	{
		if((strcmp(fileinfo.name,".") != 0) && (strcmp(fileinfo.name,"..") != 0) && strstr(fileinfo.name,".4db"))
		{
			wstring wstrTemp;
			Project::Tools::UTF8ToWideChar(string(fileinfo.name),wstrTemp);
			fileList.push_back(wstrTemp.c_str());
		}
	}
	_findclose(handle);

	//将template置换到第一位
	for(int i=0;i<fileList.size();i++)
	{
		if(fileList.at(i) == SYSTEAM_TEMPLATE)
		{
			if(i != 0)//
			{
				fileList[i] = fileList[0];
				fileList[0] = SYSTEAM_TEMPLATE;
				break;
			}
			break;
		}
	}
}

/*
	函数说明：切换模板 组合框的选择事件
*/
void CStandardEquip::OnCbnSelchangeComboTemplatesort()
{
	// TODO: 在此添加控件通知处理程序代码
	CString cstrLastChoice = m_cstrTemplateName;
	m_comboxTemplateSort.GetLBText(m_comboxTemplateSort.GetCurSel(),m_cstrTemplateName);
	if(m_cstrTemplateName == "system")//选择不变退出
		if(cstrLastChoice == SYSTEAM_TEMPLATE)
			return;
	else if(cstrLastChoice == m_cstrTemplateName)
		return;
	cstrLastChoice.Empty();

	if (m_cstrTemplateName == "system")
		m_cstrTemplateName = SYSTEAM_TEMPLATE;
	else
		m_cstrTemplateName += L".4db";
	 
	m_vecPageGroupNameList.clear();
	m_vecPageTemplateInfoList.clear();
	m_combTemplateGroup.ResetContent();
	m_combTemplate.ResetContent();

	//读模板分类
	pMainFrame->m_pPrjSqlite->ReadTemplatePageGroupList(m_cstrTemplateName, m_vecPageGroupNameList);
	//删除“系统模板”中的“其他分组”
	if(m_cstrTemplateName == SYSTEAM_TEMPLATE)
	{
		for(vector<GroupPage>::iterator iter = m_vecPageGroupNameList.begin() ;iter!=m_vecPageGroupNameList.end();iter++)
		{
			if(iter->GroupName == L"其他")
			{
				m_vecPageGroupNameList.erase(iter);
				break;
			}
		}
	}
	//初始化模版分类combox
	for(int i=0;i<m_vecPageGroupNameList.size();i++)
	{
		int ncur = m_combTemplateGroup.AddString(m_vecPageGroupNameList[i].GroupName.c_str());
		m_combTemplateGroup.SetItemData(ncur, m_vecPageGroupNameList[i].GroupID);
	}
	m_combTemplateGroup.SetCurSel(0);

	//初始化模板页面清单
	int nDefaultGroupId = m_vecPageGroupNameList[0].GroupID;
	pMainFrame->m_pPrjSqlite->ReadTemplatePageList(m_cstrTemplateName, nDefaultGroupId, m_vecPageTemplateInfoList);
	for(int i=0;i<m_vecPageTemplateInfoList.size();i++)
	{
		int ncur = m_combTemplate.AddString(m_vecPageTemplateInfoList[i].m_strName);
		m_combTemplate.SetItemData(ncur, m_vecPageTemplateInfoList[i].m_nID);
		
	}
	m_combTemplate.SetCurSel(0);

	//初始化参数
	SelchangeComboEquipTemplate();

}
