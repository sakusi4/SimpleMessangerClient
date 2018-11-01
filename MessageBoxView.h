#pragma once



// CMessageBox �� ���Դϴ�.

class CMessageBoxView : public CFormView
{
	DECLARE_DYNCREATE(CMessageBoxView)

protected:
	CMessageBoxView();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CMessageBoxView();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MESSAGEBOX };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	CString FriendId;
	CString m_Edit_Message;
	afx_msg void OnBnClickedButtonMsgResend();
	afx_msg void OnBnClickedButtonMsgOk();
	afx_msg void OnBnClickedButtonMsgExit();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
};


