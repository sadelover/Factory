#pragma once
#include "afxdtctl.h"


// CExportHistoricalDataAtPreciseTimeDLg �Ի���

class CExportHistoricalDataAtPreciseTimeDLg : public CDialogEx
{
	DECLARE_DYNAMIC(CExportHistoricalDataAtPreciseTimeDLg)

public:
	CExportHistoricalDataAtPreciseTimeDLg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CExportHistoricalDataAtPreciseTimeDLg();

// �Ի�������
	enum { IDD = IDD_Export_HistoricalData_At_PreciseTime };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	// ���ڿ�����
	CDateTimeCtrl m_CDateControl;
	// ʱ�������
	CDateTimeCtrl m_CTimeControl;
	//���水ť������
	afx_msg void OnBnClickedSaveAs();
};
