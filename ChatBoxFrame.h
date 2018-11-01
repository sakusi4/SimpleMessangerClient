#pragma once
#include "ChatBoxView.h"
#include "Common.h"


// CChatBoxFrame �������Դϴ�.

class CChatBoxFrame : public CFrameWnd
{
	DECLARE_DYNCREATE(CChatBoxFrame)
protected:
	CChatBoxFrame();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CChatBoxFrame();
	
	
protected:
	DECLARE_MESSAGE_MAP()

public:
	CChatBoxView* pChatBoxView;
	CString FriendId;
	void SettingMessage(CMessageBox* pMessageBox);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
};


