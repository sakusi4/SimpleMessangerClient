#pragma once
#include "ChatBoxView.h"
#include "Common.h"


// CChatBoxFrame 프레임입니다.

class CChatBoxFrame : public CFrameWnd
{
	DECLARE_DYNCREATE(CChatBoxFrame)
protected:
	CChatBoxFrame();           // 동적 만들기에 사용되는 protected 생성자입니다.
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


