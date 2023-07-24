#pragma once


// CComboToolbar

class CComboToolbar : public CToolBar
{
	DECLARE_DYNAMIC(CComboToolbar)

public:
	CComboToolbar();
	virtual ~CComboToolbar();
	
	CComboBox m_ComboBox;
protected:
	DECLARE_MESSAGE_MAP()
};


