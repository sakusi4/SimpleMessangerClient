#pragma once
#include "FileTransferView.h"
#include "Common.h"

// CFileTransferFrame �������Դϴ�.

class CFileTransferFrame : public CFrameWnd
{
	DECLARE_DYNCREATE(CFileTransferFrame)
protected:
	CFileTransferFrame();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
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
		WM_SERVER_ACCEPT = WM_USER + 101,	// ���� accept �޽���
		MODE_SEND = 1,	// ��������â ���
		MODE_RECEIVE,
		BUF_SIZE = 4096		// ���� ���۹��� ũ��
	};

private:
	void disconnectServer();

	// ���� ���
	CFileTransferView* m_view;		    // �ڽ� FormView
	int m_mode;							// ����â ���

public:
	void setRecvSpeed(DWORD);
	void setProgressPos(int);
	static UINT fileSendThread(LPVOID);

	CFile m_file;			// ó������
	SOCKET m_hSock;
	WSADATA m_wsaData;
	SOCKADDR_IN m_sAddr;
	BOOL m_isWork;			// �������� �۾��� ����

	SOCKET m_hClntSock;		// Ŭ���̾�Ʈ ����
	CString m_transUser;	// ���۹޴� ����ID	

							// ���� �����ڰ� ����ϴ� ���
	CFileTransferInfo m_fileTransInfo;	// ��������
	CString m_recvFilePath;				// ���۹��� ���� ���
	long m_fStartByte;					// ���۹��� ������ ���� ��ġ(Byte)
	UINT m_recvFileOpenFlag;			// ������ ������ ���� �÷���

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnServerAccept(WPARAM wParam, LPARAM lParam);
	afx_msg void OnClose();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
};


