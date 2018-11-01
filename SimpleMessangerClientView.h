
// SimpleMessangerClientView.h : CSimpleMessangerClientView 클래스의 인터페이스
//

#pragma once
#include "afxwin.h"
#include "BtnST.h"

class CSimpleMessangerClientDoc;

class CSimpleMessangerClientView : public CFormView
{
protected: // serialization에서만 만들어집니다.
	CSimpleMessangerClientView();
	DECLARE_DYNCREATE(CSimpleMessangerClientView)

public:
#ifdef AFX_DESIGN_TIME
	enum{ IDD = IDD_SIMPLEMESSANGERCLIENT_FORM };
#endif

// 특성입니다.
public:
	CSimpleMessangerClientDoc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual void OnInitialUpdate(); // 생성 후 처음 호출되었습니다.

// 구현입니다.
public:
	virtual ~CSimpleMessangerClientView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
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

#ifndef _DEBUG  // SimpleMessangerClientView.cpp의 디버그 버전
inline CSimpleMessangerClientDoc* CSimpleMessangerClientView::GetDocument() const
   { return reinterpret_cast<CSimpleMessangerClientDoc*>(m_pDocument); }
#endif

