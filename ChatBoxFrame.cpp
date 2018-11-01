// ChatBoxFrame.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SimpleMessangerClient.h"
#include "ChatBoxFrame.h"
#include "ChatManager.h"


// CChatBoxFrame

IMPLEMENT_DYNCREATE(CChatBoxFrame, CFrameWnd)

CChatBoxFrame::CChatBoxFrame()
{
	Create(NULL, _T("대화창"));
}

CChatBoxFrame::~CChatBoxFrame()
{
}


BEGIN_MESSAGE_MAP(CChatBoxFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CChatBoxFrame 메시지 처리기입니다.


void CChatBoxFrame::SettingMessage(CMessageBox* pMessageBox)
{
	this->FriendId = pMessageBox->Id;
	pChatBoxView->FriendId = pMessageBox->Id;

	CString title = _T("님과의 대화");
	title = pMessageBox->Id + title;
	SetWindowText(title);
}

int CChatBoxFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRuntimeClass* pRuntimeClass = RUNTIME_CLASS(CChatBoxView);
	pChatBoxView = (CChatBoxView*)pRuntimeClass->CreateObject();

	pChatBoxView->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW, CRect(0, 0, 150, 150), this, AFX_IDW_PANE_FIRST);

	RecalcLayout();
	pChatBoxView->ResizeParentToFit(FALSE);

	return 0;
}

void CChatBoxFrame::OnClose()
{
	CChatManager::DeleteRoom(FriendId);
	CFrameWnd::OnClose();
}
