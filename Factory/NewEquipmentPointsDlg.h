#pragma once


// CNewEquipmentPointsDlg dialog

class CNewEquipmentPointsDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CNewEquipmentPointsDlg)

public:
	CNewEquipmentPointsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CNewEquipmentPointsDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_NEW_EQUIPMENT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEditNumChiller();
	afx_msg void OnBnClickedOk();
	int m_nChillerCount;//���̨��
	int m_nEquipmentStartNum;//�豸��ʼ���
	int m_nChTowerCount;//��ȴ��̨��
	int m_nCWPCount;//�����̨��
	int m_nPriChCount;//һ���䶳��
	int m_nSecCWPCount;//����
	int m_nPriHWPCount;//һ���ȱ�
	int m_nSecHCWCount;//����
	CString m_strNewRoomName;//������
	BOOL m_bAddControlPoints;//
	BOOL m_bChillerHasValve;//
	int m_nPriChWPCount;//
	int m_nFanBlowerCount;//ÿ̨���̨��
	CString m_PlantDescription;
};
