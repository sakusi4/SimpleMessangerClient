#pragma once
#include "afxwin.h"


// CSignup ��ȭ �����Դϴ�.

class CSignup : public CDialogEx
{
	DECLARE_DYNAMIC(CSignup)

public:
	CSignup(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CSignup();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SIGNUP };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CString m_Edit_Id;
	CString m_Edit_Pw;
	CString m_Edit_Pw2;
	afx_msg void OnBnClickedButtonSubmit();
	virtual BOOL OnInitDialog();
};
