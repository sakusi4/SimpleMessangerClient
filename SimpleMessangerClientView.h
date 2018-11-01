
// SimpleMessangerClientView.h : CSimpleMessangerClientView Ŭ������ �������̽�
//

#pragma once
#include "afxwin.h"
#include "BtnST.h"

class CSimpleMessangerClientDoc;

class CSimpleMessangerClientView : public CFormView
{
protected: // serialization������ ��������ϴ�.
	CSimpleMessangerClientView();
	DECLARE_DYNCREATE(CSimpleMessangerClientView)

public:
#ifdef AFX_DESIGN_TIME
	enum{ IDD = IDD_SIMPLEMESSANGERCLIENT_FORM };
#endif

// Ư���Դϴ�.
public:
	CSimpleMessangerClientDoc* GetDocument() const;

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	virtual void OnInitialUpdate(); // ���� �� ó�� ȣ��Ǿ����ϴ�.

// �����Դϴ�.
public:
	virtual ~CSimpleMessangerClientView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
public:

	afx_msg void OnBnClickedButtonSignup();
	afx_msg void OnBnClickedButtonLogin();
	CString m_Edit_Id;
	CString m_Edit_Pw;
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnBnClickedButtonBlog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedButtonClose();
};

#ifndef _DEBUG  // SimpleMessangerClientView.cpp�� ����� ����
inline CSimpleMessangerClientDoc* CSimpleMessangerClientView::GetDocument() const
   { return reinterpret_cast<CSimpleMessangerClientDoc*>(m_pDocument); }
#endif

