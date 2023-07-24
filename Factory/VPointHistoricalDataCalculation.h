#pragma once

#include "gridctrl/GridCtrl.h"
#include "afxdtctl.h"
#include "afxwin.h"
#include "HttpOperation.h"
#include "DataPoint/DataPointEntry.h"
#include "json/json.h"
#include "DataPoint\DataPointManager.h"

// CVPointHistoricalDataCalculation �Ի���
//��ʷ���ݲ��㴰��
const CString period_m1 = L"m1";
const CString period_m5 = L"m5";
const CString period_h1 = L"h1";
const CString period_d1 = L"d1";

class CVPointHistoricalDataCalculation : public CDialogEx
{
	DECLARE_DYNAMIC(CVPointHistoricalDataCalculation)

public:
	CVPointHistoricalDataCalculation(CWnd* pParent = NULL);   // ��׼���캯��
	CVPointHistoricalDataCalculation(vector<DataPointEntry> &vec, CDataPointManager *pointManager, CWnd* pParent = NULL);   // ��׼���캯��
	CVPointHistoricalDataCalculation(CString cstrPointName, CString cstrPointScript, CDataPointManager *pointManager, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CVPointHistoricalDataCalculation();

	//�����б���������Ϣ��ӡ
	void ShowMsg(CString& cstrMsg);
	void ShowMsg(const wchar_t* cstrMsg);
// �Ի�������
	enum { IDD = IDD_DIALOG_VPOINT_HSITORICAL_DATA_CALCULATION };
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonCalculation();
	afx_msg void OnBnClickedButtonInsert();
	afx_msg LRESULT OnUpdateMyData(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedButtonPointListCtl();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	//��Ҫ����ĵ�
	//���㹫ʽ
	struct PointStruct
	{
		CString cstrName;
		CString cstrScript;
		PointStruct(CString name,CString script):cstrName(name),cstrScript(script){}
	};
	vector<PointStruct> m_pointList;//����㼯��

	CDataPointManager* m_pointManager;
	//���ݱ����
	CGridCtrl m_gridInput;
	//��ֹʱ�����
	CDateTimeCtrl m_dateTime_from;
	CDateTimeCtrl m_dateTime_time_from;
	CDateTimeCtrl m_dateTime_to;
	CDateTimeCtrl m_dateTime_time_to;
	// ʱ��ָ��б�ؼ�
	CComboBox m_combo_period;
	// static�ؼ�
	CStatic m_static_Msg;
	// ListBox���б���ʾ�ؼ�
	CListBox m_list_point;
	//http ����
	CString m_CalcIp;
	int m_nCalcPort;

private:
	/*
	 *	@brief: �ӽ����ȡ���������Ϊjson������pysite�ӿڻ�ȡ���ݲ��������
	*/
	bool CalculationAndDisplayPoints(void);
	/*
	 *	@brief: ֻ���𷢵�����úͲ���
	*/
	bool CalculationAndDisplayOnePoint(Json::Value& jsonSendPacket, int nIndex);
	/*
	 * @brief: �������ݴ������
	*/
	void InsertDatabase(void);
	/*
	 *	@brief: �����б������������к�
	 *	@param[in]: nRow �����һ���������Ѿ�����
	*/
	void GridSetRow(int nRow);
	/*
	 * @brief: ���ؼ���շ���
	*/
	void GridClear(void);
	/*
	 * @brief: �������һ�������ֵ��ʾ��
	 * @Param[in]:cstrPointName ����һ��ʱ����Ҫһ��ʱ���һ�е�ֵ����ֵ��ͷ��Ҫ��ʾ������
	*/
	void GridAddGroup(CString cstrPointName, int nRow);
	/*
	 * @brief: �б�ؼ�ˢ��,�����������Ҳ��ˢ�½�ȥ
	*/
	void RefreshListCtrl(void);
	/*
	 * @brief: ����Ͳ����߳�
	*/
	static UINT Thread_Calculation(LPVOID pParam);
	static UINT Thread_Insert(LPVOID pParam);
};

