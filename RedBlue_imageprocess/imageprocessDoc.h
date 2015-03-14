// imageprocessDoc.h : interface of the CImageprocessDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMAGEPROCESSDOC_H__1C0F9582_3F8F_4EBE_97FA_06F3E5E96057__INCLUDED_)
#define AFX_IMAGEPROCESSDOC_H__1C0F9582_3F8F_4EBE_97FA_06F3E5E96057__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CImageprocessDoc : public CDocument
{
protected: // create from serialization only
	CImageprocessDoc();
	DECLARE_DYNCREATE(CImageprocessDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImageprocessDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CImageprocessDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CImageprocessDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMAGEPROCESSDOC_H__1C0F9582_3F8F_4EBE_97FA_06F3E5E96057__INCLUDED_)
