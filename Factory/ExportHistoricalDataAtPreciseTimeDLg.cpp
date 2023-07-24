// ExportHistoricalDataAtPreciseTimeDLg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ExportHistoricalDataAtPreciseTimeDLg.h"
#include "afxdialogex.h"
#include "json/json.h"
#include "Tools/CustomTools/CustomTools.h"
#include "HttpOperation.h"
#include "ExcelOperator.h"

// CExportHistoricalDataAtPreciseTimeDLg �Ի���

IMPLEMENT_DYNAMIC(CExportHistoricalDataAtPreciseTimeDLg, CDialogEx)

CExportHistoricalDataAtPreciseTimeDLg::CExportHistoricalDataAtPreciseTimeDLg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CExportHistoricalDataAtPreciseTimeDLg::IDD, pParent)
{

}

CExportHistoricalDataAtPreciseTimeDLg::~CExportHistoricalDataAtPreciseTimeDLg()
{
}

void CExportHistoricalDataAtPreciseTimeDLg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_CDateControl);
	DDX_Control(pDX, IDC_DATETIMEPICKER2, m_CTimeControl);
}


BEGIN_MESSAGE_MAP(CExportHistoricalDataAtPreciseTimeDLg, CDialogEx)
	ON_BN_CLICKED(IDC_Save_As, &CExportHistoricalDataAtPreciseTimeDLg::OnBnClickedSaveAs)
END_MESSAGE_MAP()


// CExportHistoricalDataAtPreciseTimeDLg ��Ϣ�������


void CExportHistoricalDataAtPreciseTimeDLg::OnBnClickedSaveAs()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CTime timeDate;
	CTime timeTime;
	CString  cstrTime;

	//��ȡ���ں�ʱ��
	m_CDateControl.GetTime(timeDate);
	cstrTime = timeDate.Format(_T("%Y-%m-%d"));
	cstrTime+= L" ";
	m_CTimeControl.GetTime(timeTime);
	cstrTime += timeTime.Format(_T("%H:%M:%S"));
	
	//�����ӿ�json��ʽ�Ĳ���
	string strTime;
	string strJsonPack;
	Project::Tools::WideCharToUtf8(cstrTime.GetString(), strTime);
	Json::Value jsonRoot;
	jsonRoot["time"] = strTime;
	jsonRoot["format"] = "m1";
	strJsonPack = jsonRoot.toStyledString();

	//ͨ���ӿڻ�ȡֵ�������
	CString cstrResult;
	CHttpOperation http_log(L"127.0.0.1", 5000, _T("/get_history_data_all_one_moment_padded "));
	bool bRet_log = http_log.HttpPostGetRequestEx(0, strJsonPack.data(), cstrResult,  _T("Content-Type: application/json"));
	if(!bRet_log)
	{
		AfxMessageBox(L"û������dompysite��");
		return;
	}
	string strResult;
	Project::Tools::WideCharToUtf8(cstrResult.GetString(), strResult);
	Json::Reader jsonReader;
	jsonRoot.clear();//����������յ���json������
	if(jsonReader.parse(strResult, jsonRoot) == false)
	{
		AfxMessageBox(L"dompysite����json��ʽ����");
		return;
	}
	if(jsonRoot["err"].asInt() != 0)
	{
		AfxMessageBox(L"dompysite��ȡʧ��");
		return;
	}
	if(jsonRoot["data"].size() == 0)
	{
		AfxMessageBox(L"��ʱ������");
		return;
	}

	//��������ֵ
	vector <CString> cstrPointNameList;
	vector <CString> cstrPointValueList;
	Json::Value jsonPointList;
	jsonPointList = jsonRoot["data"];
	for(Json::Value::iterator jsonIter = jsonPointList.begin(); jsonIter!=jsonPointList.end();jsonIter++)
	{
		CString cstrTempName( (*jsonIter)["name"].asCString() );
		CString cstrTempValue( (*jsonIter)["value"].asCString() );
		cstrPointNameList.push_back(cstrTempName);
		cstrPointValueList.push_back(cstrTempValue);
	}

	//�����ļ�
	CFileDialog dlg(FALSE, _T(""), _T(""), 0, _T("CSV File (*.csv)|*.csv|")_T("All Files (*.*)|*.*||"), NULL);
	if (dlg.DoModal() == IDOK)
	{
		CString strFilePath = dlg.GetPathName();
		CString strName = dlg.GetFileName();
		if (strName.GetLength() == 0)
		{
			AfxMessageBox(_T("�ļ����Ʋ���Ϊ��"));
			return;
		}
		char* old_locale = _strdup( setlocale(LC_CTYPE,NULL) );
		setlocale( LC_ALL, "chs" );

		//��ȡ��ʽ��ʱ��
		cstrTime = timeDate.Format(_T("%Y/%m/%d"));
		cstrTime+= L" ";
		cstrTime += timeTime.Format(_T("%H:%M:%S"));

		CString cstrNoSecend;
		cstrNoSecend = timeDate.Format(_T("%Y/%m/%d"));
		cstrNoSecend+= L" ";
		cstrNoSecend += timeTime.Format(_T("%H:%M"));

		CStdioFile filehandle;
		if(filehandle.Open(strFilePath, CFile::modeCreate|CFile::modeWrite))
		{
			CString strOut;
			wstring strTime,strName,strValue;

			strOut.Format(_T("\"%s\",\"%s\",\"%s\"\n"),cstrNoSecend, L"CurrentTime", cstrTime);
			filehandle.WriteString(strOut);

			for(int i=0; i<cstrPointNameList.size(); ++i)
			{
				strOut.Format(_T("\"%s\",\"%s\",\"%s\"\n"), cstrNoSecend, cstrPointNameList[i], cstrPointValueList[i]);
				filehandle.WriteString(strOut);
			}
			filehandle.Close();
			setlocale( LC_ALL, old_locale );
			AfxMessageBox(_T("����CSV�ļ��ɹ�"));
		}
		else
		{
			setlocale( LC_ALL, old_locale );
			AfxMessageBox(_T("�޷����ѱ�����ļ����µ���CSV�ļ�ʧ�ܡ�"));
		}	
	}
}
	