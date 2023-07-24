// BEOP-DesignerDoc.h : interface of the CBEOPDesignerDoc class
#pragma once
class CBEOPDesignerDoc : public CDocument
{
protected: // create from serialization only
	CBEOPDesignerDoc();
	DECLARE_DYNCREATE(CBEOPDesignerDoc)
// Attributes
public:
// Operations
public:
// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
// Implementation
public:
	virtual ~CBEOPDesignerDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
protected:
// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void SetTitle(LPCTSTR lpszTitle);
};


