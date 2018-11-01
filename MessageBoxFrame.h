#pragma once
#include "MessageBoxView.h"
#include "Common.h"


// CMessageBoxFrame �������Դϴ�.

class CMessageBoxFrame : public CFrameWnd
{
	DECLARE_DYNCREATE(CMessageBoxFrame)
protected:
	CMessageBoxFrame();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CMessageBoxFrame();

protected:
	DECLARE_MESSAGE_MAP()
public:
	CMessageBoxView* pMessageBoxView;
	CString FriendId;

	void messageSetting(CMessageBox* FriendId, BOOL bReceive = FALSE);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


