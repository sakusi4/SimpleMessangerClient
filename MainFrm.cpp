
// MainFrm.cpp : CMainFrame 클래스의 구현
//

#include "stdafx.h"
#include "SimpleMessangerClient.h"
#include "MainFrm.h"
#include "Common.h"
#include "FriendAdd.h"
#include "MessageBoxFrame.h"
#include "ChatBoxFrame.h"
#include "FileTransferFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_MESSAGE(WM_RECEIVEMSG, &CMainFrame::OnReceivemsg)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame 생성/소멸

CMainFrame::CMainFrame()
{
	pChatManager = new CChatManager();
}

CMainFrame::~CMainFrame()
{
	delete pChatManager;
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	Connect();
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CFrameWnd::PreCreateWindow(cs))
		return FALSE;

	cs.style &= ~WS_MAXIMIZEBOX;
	cs.style &= ~WS_THICKFRAME;

	cs.style &= ~FWS_ADDTOTITLE;
	this->SetTitle(_T("Messanger"));

	return TRUE;
}

// CMainFrame 진단

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 메시지 처리기

void CMainFrame::Connect()
{
	WSAStartup(MAKEWORD(2, 2), &Wsa);

	Sock = socket(AF_INET, SOCK_STREAM, 0);
	if (Sock == INVALID_SOCKET)
	{
		AfxMessageBox(_T("socket()오류"));
	}

	ZeroMemory(&Serveraddr, sizeof(Serveraddr));

	Serveraddr.sin_family = AF_INET;
	Serveraddr.sin_port = htons(21000);
	Serveraddr.sin_addr.s_addr = inet_addr(_T("127.0.0.1"));	//Server IP , Current : Loopback ip

	int retval = connect(Sock, (SOCKADDR *)&Serveraddr, sizeof(Serveraddr));
	if (retval == SOCKET_ERROR)
	{
		AfxMessageBox(_T("서버 연결에 실패 하였습니다."));
	}
	else
	{
		WSAAsyncSelect(Sock, GetSafeHwnd(), WM_RECEIVEMSG, FD_READ);
	}
}

void CMainFrame::sendPacket(void * buf, int size, int flag)
{
	PACKETHEADER packetheader;
	packetheader.flag = flag;
	packetheader.size = size;

	
	int AllSize = size + sizeof(packetheader);
	char* Packet = new char[AllSize];
	CopyMemory(Packet, &packetheader, sizeof(packetheader));
	CopyMemory(Packet + sizeof(packetheader), buf, size);

	send(Sock, Packet, AllSize, 0);
}
LRESULT CMainFrame::OnReceivemsg(WPARAM wParam, LPARAM lParam)
{
	int Header[2];
	char Buf[MSGProtocol::BUFSIZE];
	ZeroMemory(Buf, MSGProtocol::BUFSIZE);

	int retval = recv(Sock, (char*)Header, sizeof(int) * 2, 0);

	if (retval == 0)
	{
		AfxMessageBox(_T("서버와 연결이 끊겼습니다."));
		return 1;
	}
	retval = recv(Sock, Buf, Header[1], 0);

	if (Header[0] == MSGProtocol::SERV_MSG_MEMBERJOINRESULT)
	{
		int* result = (int*)Buf;

		if (*result == 1)
			AfxMessageBox(_T("회원 가입 성공"));
		else
			AfxMessageBox(_T("이미 사용중인 계정 입니다."));
	}
	else if (Header[0] == MSGProtocol::SERV_MSG_LOGINRESULT)
	{
		int* result = (int*)Buf;

		if (*result == 1)
		{
			//현재 메인 프레임의 뷰 포인터를 가져온다.
			pLoginView = (CSimpleMessangerClientView*)GetActiveView();

			//폼뷰 동적생성
			CRuntimeClass* pRuntimeClass = RUNTIME_CLASS(CFriendListView);
			pFriendListView = (CFriendListView*)pRuntimeClass->CreateObject();
			pFriendListView->Create(NULL, NULL, 0L, CFrameWnd::rectDefault, this, IDD_FRIENDLISTVIEW);

			//생성한 폼뷰를 현재 뷰로 설정
			SetActiveView(pFriendListView);

			//로그인 폼뷰를 숨긴다.
			pLoginView->ShowWindow(SW_HIDE);

			//친구목록 뷰를 보이게한다.
			pFriendListView->ShowWindow(SW_SHOW);
			pFriendListView->SetDlgCtrlID(AFX_IDW_PANE_FIRST);

			pFriendListView->OnInitialUpdate();

			SoundPath = _T("./sound/login.wav");
			PlaySound(SoundPath, AfxGetInstanceHandle(), SND_ASYNC);

		}
		else if (*result == 2)
			AfxMessageBox(_T("이미 로그인 중인 계정입니다."));
		else
			AfxMessageBox(_T("로그인 정보가 정확하지 않습니다."));
	}
	else if (Header[0] == MSGProtocol::SERV_MSG_FRIENDINSERTRST)
	{
		int* result = (int*)Buf;
		if (*result == 1)
		{
			AfxMessageBox(_T("친구 추가 성공"));
			pFriendListView->Invalidate();
			CMainFrame::sendPacket(NULL, 0, MSGProtocol::CLNT_MSG_FLISTREQUEST);
		}
		else
		{
			AfxMessageBox(_T("등록되지 않은 아이디 입니다."));
		}
	}
	else if (Header[0] == MSGProtocol::SERV_MSG_FLIST)
	{
		FriendState* pFriendState = (FriendState*)Buf;
		pFriendListView->FriendAddTree(pFriendState);
	}
	else if (Header[0] == MSGProtocol::SERV_MSG_FRIENDSTATE)
	{
		FriendState* pFriendState = (FriendState*)Buf;
		pFriendListView->FriendSetState(pFriendState);
	}
	else if (Header[0] == MSGProtocol::SERV_MSG_RECVPAPER)
	{
		CMessageBox* pMessageBox = (CMessageBox*)Buf;

		CRuntimeClass* pRuntimeClass = RUNTIME_CLASS(CMessageBoxFrame);
		CMessageBoxFrame* pMessageBoxFrame = (CMessageBoxFrame*)pRuntimeClass->CreateObject();
		pMessageBoxFrame->messageSetting(pMessageBox, TRUE);

		pMessageBoxFrame->ShowWindow(SW_SHOW);

		SoundPath = _T("./sound/recv_message.wav");
		PlaySound(SoundPath, AfxGetInstanceHandle(), SND_ASYNC);
	}
	else if (Header[0] == MSGProtocol::SERV_MSG_CHATMSG)
	{
		CMessageBox* pMessageBox = (CMessageBox*)Buf;

		pChatManager->CreateRoom(pMessageBox);

		SoundPath = _T("./sound/recv_chat.wav");
		PlaySound(SoundPath, AfxGetInstanceHandle(), SND_ASYNC);
	}
	else if (Header[0] == MSGProtocol::SERV_MSG_STATEMESSAGE)
	{
		FriendStateMessage* friendstatemessage = (FriendStateMessage*)Buf;
		pFriendListView->FirnedSetStateMessage(friendstatemessage);
	}
	else if (Header[0] == MSGProtocol::SERV_MSG_MYSTATEMESSAGE)
	{	
		pFriendListView->StateMessage = Buf;
		pFriendListView->Invalidate();
	}
	else if (Header[0] == MSGProtocol::SERV_MSG_FRSTATEMESSAGE)
	{
		FriendStateMessage* friendstatemessage = (FriendStateMessage*)Buf;
		pFriendListView->FirnedSetStateMessage(friendstatemessage);
	}
	else if (Header[0] == MSGProtocol::SERV_MSG_FILETRANSFERREQ)
	{
		CFileTransferInfo* fileinfo = (CFileTransferInfo*)Buf;

		CRuntimeClass *pRuntimeClass = RUNTIME_CLASS(CFileTransferFrame);
		CFileTransferFrame* fileTransferFrame = (CFileTransferFrame*)pRuntimeClass->CreateObject();

		// 파일전송창 초기화 및 윈도우 보이기
		fileTransferFrame->initializeFileTransfer(CFileTransferFrame::MODE_RECEIVE, fileinfo->fileName, fileinfo);
		fileTransferFrame->ShowWindow(SW_SHOW);
	}
	return 0;
}

void CMainFrame::OnClose()
{
	shutdown(Sock, SD_SEND);
	WSACleanup();
	CFrameWnd::OnClose();
}

