#pragma once
#include "afxcmn.h"
#include "Common.h"
#include <vector>
#include "ChatManager.h"
#include "BtnST.h"
#include "afxwin.h"
#include "MyTree.h"


// CFriendListView 폼 뷰입니다.

class CFriendListView : public CFormView
{
	DECLARE_DYNCREATE(CFriendListView)

protected:
	CFriendListView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CFriendListView();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FRIENDLISTVIEW };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	static vector<CString> FriendVector;

	LOGFONT Font;
	CMyTree m_Tree_Friends;
	CChatManager* pChatManager;

	CButtonST m_Button_Search;

	CString StateMessage;
	CString m_Edit_Search;
	CString ImagePath;

	int OnlineFriendCount;

	void FriendAddTree(FriendState* pFriendState);
	void FriendSetState(FriendState* pFriendState);
	void FirnedSetStateMessage(FriendStateMessage* friendstatemessage);
	CString FriendGetName(HTREEITEM hChild);
	BOOL IsLogin(CString Id);

	afx_msg void OnMenuFrdelete();
	afx_msg void OnSendmessage();
	afx_msg void OnNMRClickTreeFriends(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMenuChat();
	afx_msg void OnMenuFradd();
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnNMDblclkTreeFriends(NMHDR *pNMHDR, LRESULT *pResult);
	
	afx_msg void OnBnClickedButtonMainSearch();

	afx_msg void OnEnSetfocusEditSearch();
	afx_msg void OnStatemessage();
	afx_msg void OnFilesend();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	virtual void OnInitialUpdate();
	afx_msg void OnImage();
	CStatic m_Static_Image;
};


