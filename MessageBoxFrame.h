#pragma once
#include "MessageBoxView.h"
#include "Common.h"


// CMessageBoxFrame 프레임입니다.

class CMessageBoxFrame : public CFrameWnd
{
	DECLARE_DYNCREATE(CMessageBoxFrame)
protected:
	CMessageBoxFrame();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CMessageBoxFrame();

protected:
	DECLARE_MESSAGE_MAP()
public:
	CMessageBoxView* pMessageBoxView;
	CString FriendId;

	void messageSetting(CMessageBox* FriendId, BOOL bReceive = FALSE);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


