#pragma once
#include "FileTransferView.h"
#include "Common.h"

// CFileTransferFrame 프레임입니다.

class CFileTransferFrame : public CFrameWnd
{
	DECLARE_DYNCREATE(CFileTransferFrame)
protected:
	CFileTransferFrame();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CFileTransferFrame();

public:
	void startFileTransfer();
	void selectFilePath();
	void initializeFileTransfer(int, const CString&, CFileTransferInfo* = NULL);
	void finalizeFileTransfer();
	SOCKADDR_IN getServerAddr();
	unsigned short getServerPort();
	BOOL connectServer();
	void closeServer();
	BOOL openServer();

protected:
	DECLARE_MESSAGE_MAP()

public:
	static UINT fileRecvThread(LPVOID arg);

	enum
	{
		WM_SERVER_ACCEPT = WM_USER + 101,	// 서버 accept 메시지
		MODE_SEND = 1,	// 파일전송창 모드
		MODE_RECEIVE,
		BUF_SIZE = 4096		// 파일 전송버퍼 크기
	};

private:
	void disconnectServer();

	// 공통 멤버
	CFileTransferView* m_view;		    // 자식 FormView
	int m_mode;							// 전송창 모드

public:
	void setRecvSpeed(DWORD);
	void setProgressPos(int);
	static UINT fileSendThread(LPVOID);

	CFile m_file;			// 처리파일
	SOCKET m_hSock;
	WSADATA m_wsaData;
	SOCKADDR_IN m_sAddr;
	BOOL m_isWork;			// 파일전송 작업중 여부

	SOCKET m_hClntSock;		// 클라이언트 소켓
	CString m_transUser;	// 전송받는 유저ID	

							// 파일 수신자가 사용하는 멤버
	CFileTransferInfo m_fileTransInfo;	// 파일정보
	CString m_recvFilePath;				// 전송받을 파일 경로
	long m_fStartByte;					// 전송받을 파일의 시작 위치(Byte)
	UINT m_recvFileOpenFlag;			// 수신할 파일의 오픈 플래그

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnServerAccept(WPARAM wParam, LPARAM lParam);
	afx_msg void OnClose();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
};


