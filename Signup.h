#pragma once
#include "afxwin.h"


// CSignup 대화 상자입니다.

class CSignup : public CDialogEx
{
	DECLARE_DYNAMIC(CSignup)

public:
	CSignup(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSignup();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SIGNUP };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CString m_Edit_Id;
	CString m_Edit_Pw;
	CString m_Edit_Pw2;
	afx_msg void OnBnClickedButtonSubmit();
	virtual BOOL OnInitDialog();
};
