// FileTransferFrame.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SimpleMessangerClient.h"
#include "FileTransferFrame.h"
#include "MainFrm.h"
#include <mmsystem.h>


// CFileTransferFrame

IMPLEMENT_DYNCREATE(CFileTransferFrame, CFrameWnd)

CFileTransferFrame::CFileTransferFrame()
{
	Create(NULL, _T("파일전송"));

	m_hSock = INVALID_SOCKET;
	m_isWork = FALSE;

	m_recvFilePath = _T("");
	m_recvFileOpenFlag = 0;
}

CFileTransferFrame::~CFileTransferFrame()
{
	closeServer();
}

void CFileTransferFrame::startFileTransfer()
{
	// 파일 보내기 모드일 때
	if (m_mode == CFileTransferFrame::MODE_SEND)
	{
		// 파일을 선택했는지 검사
		if (m_file.m_hFile == (HANDLE)-1)
		{
			MessageBox(_T("파일을 선택해 주세요."), _T("오류"), MB_ICONWARNING);

			return;
		}

		// 파일전송 서버 오픈
		if (openServer() == FALSE)
		{
			AfxMessageBox(_T(("파일전송 에러!(서버)")));
			CloseWindow();
			return;
		}

		// 파일전송 정보를 파일 수신자에게 전송
		// 파일전송 정보수집
		CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();

		CFileTransferInfo fileInfo;

		strcpy(fileInfo.fileName, m_file.GetFileName());	// 파일이름	
		fileInfo.sendAddr = getServerAddr();				// 서버 주소
		strcpy(fileInfo.sendId, (LPCTSTR)pMainFrame->MyId);	// 보낸 유저아이디
		strcpy(fileInfo.recvId, m_transUser);				// 받는 유저아이디
		fileInfo.fileSize = m_file.GetLength();				// 파일 크기

															// 서버 전송
		
		pMainFrame->sendPacket(&fileInfo, sizeof(CFileTransferInfo), MSGProtocol::CLNT_MSG_FILETRANSFERREQ);
	}
	else	// 파일 수신 모드 일 때
	{
		if (m_recvFilePath == _T(""))
		{
			MessageBox(_T("저장 경로를 선택해주세요."), _T("실패"), MB_ICONWARNING);
			return;
		}

		// 서버에 접속
		if (connectServer() == FALSE)
		{
			AfxMessageBox(_T("파일수신 에러!(서버접속)"));
			disconnectServer();

			return;
		}

		// 파일 열기
		if (m_file.Open(m_recvFilePath, m_recvFileOpenFlag) == FALSE)
		{
			MessageBox(_T("파일에러!"), _T("파일에러"), MB_ICONWARNING);
			disconnectServer();

			return;
		}

		// 전송받을 파일의 위치 지정
		m_file.Seek(m_fStartByte, CFile::begin);

		// 파일을 받을 스레드 생성
		AfxBeginThread(fileRecvThread, this);
	}

	// 파일 송수신 작업중으로 변경
	m_isWork = TRUE;

	// 각버튼 활성화 변경
	m_view->GetDlgItem(IDC_BUTTON_FILEPATH)->EnableWindow(FALSE);
	m_view->GetDlgItem(IDC_BUTTON_TRANSFERSTART)->EnableWindow(FALSE);
}

void CFileTransferFrame::selectFilePath()
{
	// 파일 보내기 모드일 때
	if (m_mode == CFileTransferFrame::MODE_SEND)
	{
		// 전송파일 선택
		CFileDialog fileDlg(TRUE, NULL, NULL, OFN_EXPLORER | OFN_HIDEREADONLY,
			_T("모든 파일(*.*) | *.*||"), this);

		if (fileDlg.DoModal() != IDOK)
			return;

		//	AfxMessageBox(fileDlg.GetPathName());

		// 파일 닫기(이미 열려져 있다면)
		if (m_file.m_hFile != (HANDLE)-1)
			m_file.Close();

		// 파일 열기
		if (m_file.Open(fileDlg.GetPathName(), CFile::modeRead | CFile::typeBinary) == FALSE)
		{
			MessageBox(_T("파일 에러"), _T("파일 에러"), MB_ICONWARNING);

			return;
		}

		// 선택한 파일 정보 보이기
		m_view->GetDlgItem(IDC_EDIT_FILEPATH)->SetWindowText(m_file.GetFilePath());

		CString fileSize = _T("");
		fileSize.Format(_T("%0.3f%s"), m_file.GetLength() / 1000000.0f, _T(" MByte"));
		m_view->GetDlgItem(IDC_EDIT_FILESIZE)->SetWindowText(fileSize);
	}
	else	// 파일 수신 모드 일 때
	{
		// 저장될 파일 위치 선정
		CFileDialog fileDlg(FALSE, NULL, m_fileTransInfo.fileName,
			OFN_EXPLORER | OFN_OVERWRITEPROMPT, _T("모든 파일(*.*) | *.*||"), this);

		if (fileDlg.DoModal() != IDOK)
			return;

		m_recvFilePath = fileDlg.GetPathName();
		m_fStartByte = 0;		// 수신할 파일의 시작 바이트

								// 동일 이름의 파일이 있을 경우 이어 받기 여부 결정
		m_recvFileOpenFlag = CFile::modeWrite | CFile::typeBinary | CFile::modeCreate;	// 기본 플래그

		CFileStatus statusFile;
		if (CFile::GetStatus(m_recvFilePath, statusFile) == TRUE)
		{
			int result = MessageBox(_T("같은 파일이 존재 합니다. 이어받으시겠습니까?"), _T("파일전송"), MB_ICONWARNING | MB_YESNO);
			if (result == IDYES)	// 이어받기 선택
			{
				// 파일 사이즈 계산
				if (statusFile.m_size >= m_fileTransInfo.fileSize)
				{	// 기존의 파일이 전송할 파일크기보다 클경우
					MessageBox(_T("이어받기 에러!"), _T("이어받기 실패"), MB_ICONWARNING);
					return;
				}

				// 전송 파일의 시작위치 지정
				CFile tempFile;
				tempFile.Open(m_recvFilePath, CFile::modeRead);
				m_fStartByte = tempFile.GetLength();	// CFileStatus의 사이즈 멤버 변수는
				tempFile.Close();						// long이기 때문에 큰파일의 용량을 제대로 얻을 수 없다.
														// 그래서 CFile로 용량을 알아온다


				m_recvFileOpenFlag |= CFile::modeNoTruncate;	// 이어받기를 위한 플래그추가
			}
		}
	}
}

void CFileTransferFrame::initializeFileTransfer(int mode, const CString& user, CFileTransferInfo* filetransinfo)
{
	m_mode = mode;		// 파일전송모드 지정
	m_transUser = user;	// 받는 유저

	CString strStatic, strPath, strTrans;

	if (m_mode == CFileTransferFrame::MODE_SEND)	// 파일을 전송하려 할때 
	{
		strStatic = _T("받는이");
		strPath = _T("파일선택");
		strTrans = _T("파일전송");

		// 받는이 이름 컨트롤에 표시
		m_view->GetDlgItem(IDC_EDIT_FILERECEIVEID)->SetWindowText(m_transUser);
		SetWindowText(_T("파일전송"));
	}
	else	// 파일을 받으려 할 때
	{
		strStatic = _T("보낸이");
		strPath = _T("저장위치");
		strTrans = _T("파일수신");

		// 보낸이, 파일명, 파일크기 정보 컨트롤에 세팅
		m_view->GetDlgItem(IDC_EDIT_FILERECEIVEID)->SetWindowText(filetransinfo->sendId);
		m_view->GetDlgItem(IDC_EDIT_FILEPATH)->SetWindowText(filetransinfo->fileName);

		CString fileSize = _T("");
		fileSize.Format(_T("%0.3f %s"), filetransinfo->fileSize / 1000000.0f, _T("MByte"));
		m_view->GetDlgItem(IDC_EDIT_FILESIZE)->SetWindowText(fileSize);

		// 전송파일 정보 보관
		m_fileTransInfo = *filetransinfo;

		SetWindowText(_T("파일수신"));
	}

	// static 컨트롤 텍스트 초기화
	m_view->GetDlgItem(IDC_STATIC_IDTITLE)->SetWindowText(strStatic);

	// 버튼 컨트롤 텍스트 초기화
	m_view->GetDlgItem(IDC_BUTTON_FILEPATH)->SetWindowText(strPath);
	m_view->GetDlgItem(IDC_BUTTON_TRANSFERSTART)->SetWindowText(strTrans);
}

void CFileTransferFrame::finalizeFileTransfer()
{
	// 파일 닫기
	if (m_file.m_hFile != (HANDLE)-1)
		m_file.Close();

	// 파일 송수신 작업 여부 변경
	m_isWork = FALSE;

	// 파일 보내기 모드일 때
	if (m_mode == CFileTransferFrame::MODE_SEND)
		closeServer();	// 서버 닫기
	else	// 파일 수신 모드일 때
		disconnectServer();	// 서버 접속 끊기
}

SOCKADDR_IN CFileTransferFrame::getServerAddr()
{
	return m_sAddr;
}

unsigned short CFileTransferFrame::getServerPort()
{
	return m_sAddr.sin_port;
}

BOOL CFileTransferFrame::connectServer()
{
	if (WSAStartup(MAKEWORD(2, 2), &m_wsaData) != 0)
	{
		AfxMessageBox(_T("WSAStartup() Error!"));
		return FALSE;
	}

	m_hSock = socket(PF_INET, SOCK_STREAM, 0);

	if (m_hSock == INVALID_SOCKET)
	{
		AfxMessageBox(_T("socket() Error!"));
		WSACleanup();
		return FALSE;
	}

	m_sAddr = m_fileTransInfo.sendAddr;	// 서버의 주소

	if (connect(m_hSock, (SOCKADDR*)&m_sAddr, sizeof(m_sAddr))
		== SOCKET_ERROR)
	{
		AfxMessageBox(_T("접속실패"));
		closesocket(m_hSock);
		WSACleanup();
		return FALSE;
	}

	return TRUE;
}

void CFileTransferFrame::closeServer()
{
	if (m_hSock != INVALID_SOCKET)
	{
		closesocket(m_hSock);
		WSACleanup();

		m_hSock = INVALID_SOCKET;
	}
}

BOOL CFileTransferFrame::openServer()
{
	if (WSAStartup(MAKEWORD(2, 2), &m_wsaData) != 0)
	{
		AfxMessageBox(_T("WSAStartup error!"));
		return FALSE;
	}

	m_hSock = WSASocket(PF_INET, SOCK_STREAM, 0, NULL, NULL, WSA_FLAG_OVERLAPPED);

	if (m_hSock == INVALID_SOCKET)
	{
		AfxMessageBox(_T("socket error!"));
		return FALSE;
	}

	CString strAddr; // 이 변수에 IP주소가 저장된다.
	HOSTENT *p_host_info;
	IN_ADDR in;
	char host_name[128] = { 0 };

	gethostname(host_name, 128);
	p_host_info = gethostbyname(host_name);

	if (p_host_info != NULL)
	{
		for (int i = 0; p_host_info->h_addr_list[i]; i++)
		{
			memcpy(&in, p_host_info->h_addr_list[i], 4);
			strAddr = inet_ntoa(in);
		}
	}

	memset(&m_sAddr, 0, sizeof(SOCKADDR));
	m_sAddr.sin_family = AF_INET;
	m_sAddr.sin_addr.s_addr = inet_addr(strAddr);
	m_sAddr.sin_port = htons(22000);

	if (bind(m_hSock, (SOCKADDR*)&m_sAddr, sizeof(m_sAddr)) == SOCKET_ERROR)
	{
		AfxMessageBox(_T("bind error!"));
		return FALSE;
	}

	// 자동할당된 포트를 얻기
	int nLen = sizeof(SOCKADDR_IN);
	getsockname(m_hSock, (SOCKADDR*)&m_sAddr, &nLen);

	if (listen(m_hSock, 5) == SOCKET_ERROR)
	{
		AfxMessageBox(_T("listen error!"));
		return FALSE;
	}

	// 소켓을 비동기로 전환
	if (WSAAsyncSelect(m_hSock, GetSafeHwnd(),
		CFileTransferFrame::WM_SERVER_ACCEPT, FD_ACCEPT)
		== SOCKET_ERROR)
	{
		AfxMessageBox(_T("WSAAsyncSelect error!"));
		return FALSE;
	}

	return TRUE;
}

UINT CFileTransferFrame::fileRecvThread(LPVOID arg)
{
	CFileTransferFrame* ftFrame = (CFileTransferFrame*)arg;

	// 계산된 파일 시작 위치 전송
	int sendByte = 0;
	while (sendByte < sizeof(long))
	{
		sendByte = send(ftFrame->m_hSock, (char*)&(ftFrame->m_fStartByte), sizeof(long), 0);

		if (sendByte == -1)	// 소켓 에러
		{
			int wsaErr = WSAGetLastError();
		
			if (wsaErr == WSAEWOULDBLOCK)	// 소켓 지연 다시 시도
				continue;
			else							// 소켓 에러로 종료함
				return 0;
		}
		else
			sendByte += sendByte;
	}

	// 파일 수신
	char buf[CFileTransferFrame::BUF_SIZE];
	int recvSize;

	DWORD startTime = timeGetTime(); // 전송속도를 계산하기 위한 변수
	DWORD lastTime = startTime;		 // 전송속도를 계산하기 위한 변수
	DWORD secRecvSize = 0;

	while (ftFrame->m_isWork)
	{
		recvSize = recv(ftFrame->m_hSock, buf, CFileTransferFrame::BUF_SIZE, 0);

		// 전송속도 계산				
		DWORD currTime = timeGetTime();
		secRecvSize += recvSize;

		if ((currTime - lastTime) >= 1000)	// 1초당 출력
		{
			ftFrame->setRecvSpeed(secRecvSize / ((currTime - startTime) / 1000));
			lastTime = currTime;
		}

		if (recvSize == 0)	// 서버 소켓 종료
			break;

		if (recvSize == -1)	// 소켓 에러
		{
			int wsaErr = WSAGetLastError();

			if (wsaErr == WSAEWOULDBLOCK)	// 소켓 지연 다시 시도
				continue;
			else							// 소켓 에러로 종료함
				break;
		}

		if (recvSize > 0)
		{
			ftFrame->m_file.Write(buf, recvSize);	// 파일에 기록
													// 진행상황(프로그래스바) 이동
			ftFrame->setProgressPos((float)ftFrame->m_file.GetLength() / (float)ftFrame->m_fileTransInfo.fileSize * 100);
		}
	}

	closesocket(ftFrame->m_hSock);

	// 파일 송수신 작업 마무리
	ftFrame->finalizeFileTransfer();

	return 0;
}

void CFileTransferFrame::disconnectServer()
{
	if (m_hSock != INVALID_SOCKET)
	{
		closesocket(m_hSock);
		WSACleanup();

		m_hSock = INVALID_SOCKET;
	}
}

void CFileTransferFrame::setRecvSpeed(DWORD speed)
{
	// 속도 단위 KByte
	speed = speed / 1000;

	// 문자열로 변환
	CString spdStr = _T("");
	spdStr.Format(_T("%d"), speed);

	// 1000단위로 콤마를 찍어준다.
	CString commStr = _T("");

	for (int i = spdStr.GetLength(), k = 1; i >= 1; i--, k++)
	{
		commStr.Format(_T("%c%s"), spdStr.GetAt(i - 1), CString(commStr));

		if ((k % 3 == 0) && (i > 1))
			commStr.Format(_T("%s%s"), _T(","), CString(commStr));
	}

	commStr.Format(_T("%s %s"), CString(commStr), _T("KByte"));

	m_view->GetDlgItem(IDC_EDIT_TRANSSPEED)->SetWindowText(commStr);
}

void CFileTransferFrame::setProgressPos(int pos)
{
	// 프로그레스바 초기화
	((CProgressCtrl*)m_view->GetDlgItem(IDC_PROGRESS_FILE))->SetPos(pos);
}

UINT CFileTransferFrame::fileSendThread(LPVOID arg)
{
	CFileTransferFrame* ftFrame = (CFileTransferFrame*)arg;

	// 전송파일 시작 위치 수신
	long fStartByte = 0;
	int recvByte = 0;
	while (recvByte < sizeof(long))
	{
		recvByte = recv(ftFrame->m_hClntSock, (char*)&fStartByte, sizeof(long), 0);

		if (recvByte == -1)	// 소켓 에러
		{
			int wsaErr = WSAGetLastError();

			if (wsaErr == WSAEWOULDBLOCK)	// 소켓 지연 다시 시도
				continue;
			else							// 소켓 에러로 종료함
				return 0;
		}
		else
			recvByte += recvByte;
	}

	// 파일 전송 위치 이동
	ftFrame->m_file.Seek(fStartByte, CFile::begin);

	// 파일 송신
	char buf[CFileTransferFrame::BUF_SIZE];
	int readSize = 0;

	DWORD startTime = timeGetTime();	// 전송속도를 계산하기 위한 변수
	DWORD lastTime = startTime;			// 전송속도를 계산하기 위한 변수
	DWORD secRecvSize = 0;

	while (ftFrame->m_isWork && ftFrame->m_file.GetPosition() < ftFrame->m_file.GetLength())
	{
		readSize = ftFrame->m_file.Read(buf, CFileTransferFrame::BUF_SIZE);	// 파일읽기

																			// 전송속도 계산		
		DWORD currTime = timeGetTime();
		secRecvSize += readSize;

		if ((currTime - lastTime) >= 1000)	// 1초당 출력
		{
			// 출력
			ftFrame->setRecvSpeed(secRecvSize / ((currTime - startTime) / 1000));

			lastTime = currTime;
		}

		if (readSize > 0)	// 파일에서 읽은 데이터 전송
		{
			while (send(ftFrame->m_hClntSock, buf, readSize, 0) == -1)
			{
				int wsaErr = WSAGetLastError();

				if (wsaErr == WSAEWOULDBLOCK)	// 소켓 지연이기 때문에 다시 시도
					continue;
				else							// 소켓에러
				{
					closesocket(ftFrame->m_hClntSock);	// 수신자 소켓 닫기
					ftFrame->m_isWork = FALSE;			// 반복문을 빠져나가게

					break;
				}
			}

			// 진행사항(프로그래스바) 이동
			ftFrame->setProgressPos((float)ftFrame->m_file.GetPosition() / (float)ftFrame->m_file.GetLength() * 100);
		}
	}

	shutdown(ftFrame->m_hClntSock, SD_SEND);
//	closesocket(ftFrame->m_hClntSock);	// 수신자 소켓 닫기

										// 파일 송수신 작업 마무리
	ftFrame->finalizeFileTransfer();

	return 0;
}


BEGIN_MESSAGE_MAP(CFileTransferFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_MESSAGE(WM_SERVER_ACCEPT, OnServerAccept)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CFileTransferFrame 메시지 처리기입니다.


int CFileTransferFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// 파일전송 formView 동적생성
	CRuntimeClass *pRuntimeClass = RUNTIME_CLASS(CFileTransferView);
	m_view = (CFileTransferView*)pRuntimeClass->CreateObject();

	// formView 윈도우 생성
	if (!m_view->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW, CRect(0, 0, 100, 100), this,
		AFX_IDW_PANE_FIRST, NULL))
	{
		return -1;
	}

	// FormView창 크기로 Frame 크기를 맞춤
	RecalcLayout();
	m_view->ResizeParentToFit(FALSE);

	return 0;
}

LRESULT CFileTransferFrame::OnServerAccept(WPARAM wParam, LPARAM lParam)
{

	// 접속한 클라이언트 담당 소켓
	SOCKADDR_IN cAddr;
	int cAddrLen = sizeof(cAddr);

	m_hClntSock = accept(m_hSock, (SOCKADDR*)&cAddr, &cAddrLen);

	// 파일을 전송할 스레드 생성
	AfxBeginThread(fileSendThread, this);

	return 0;
}


void CFileTransferFrame::OnClose()
{
	// 강제 종료 여부
	if (m_isWork)
		if (MessageBox(_T("파일 전송작업 중 입니다. 종료하시겠습니까?"), _T("종료확인"), MB_ICONWARNING | MB_YESNO)
			!= IDYES)
			return;

	// 종료전 마무리 작업
	finalizeFileTransfer();

	CFrameWnd::OnClose();
}


BOOL CFileTransferFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style &= ~WS_MAXIMIZEBOX;
	cs.style &= ~WS_THICKFRAME;

	return CFrameWnd::PreCreateWindow(cs);
}
