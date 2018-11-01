
// MainFrm.h : CMainFrame Ŭ������ �������̽�
//

#pragma once
#include "SimpleMessangerClientView.h"
#include "FriendListView.h"
#include "Common.h"
#include "ChatManager.h"

class CMainFrame : public CFrameWnd
{
	
protected: // serialization������ ��������ϴ�.
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Ư���Դϴ�.
public:

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// �����Դϴ�.
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // ��Ʈ�� ������ ���Ե� ����Դϴ�.
	CToolBar          m_wndToolBar;
	CStatusBar        m_wndStatusBar;

// ������ �޽��� �� �Լ�
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

public:
	CString MyId;

	WSADATA Wsa;
	SOCKADDR_IN Serveraddr;
	SOCKET Sock;

	CFriendListView* pFriendListView;
	CSimpleMessangerClientView* pLoginView;
	CChatManager* pChatManager;

	CString SoundPath;

	void Connect();

	void sendPacket(void* buf, int size, int flag);
	afx_msg LRESULT OnReceivemsg(WPARAM wParam, LPARAM lParam);
	afx_msg void OnClose();
};


