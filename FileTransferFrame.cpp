// FileTransferFrame.cpp : ���� �����Դϴ�.
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
	Create(NULL, _T("��������"));

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
	// ���� ������ ����� ��
	if (m_mode == CFileTransferFrame::MODE_SEND)
	{
		// ������ �����ߴ��� �˻�
		if (m_file.m_hFile == (HANDLE)-1)
		{
			MessageBox(_T("������ ������ �ּ���."), _T("����"), MB_ICONWARNING);

			return;
		}

		// �������� ���� ����
		if (openServer() == FALSE)
		{
			AfxMessageBox(_T(("�������� ����!(����)")));
			CloseWindow();
			return;
		}

		// �������� ������ ���� �����ڿ��� ����
		// �������� ��������
		CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();

		CFileTransferInfo fileInfo;

		strcpy(fileInfo.fileName, m_file.GetFileName());	// �����̸�	
		fileInfo.sendAddr = getServerAddr();				// ���� �ּ�
		strcpy(fileInfo.sendId, (LPCTSTR)pMainFrame->MyId);	// ���� �������̵�
		strcpy(fileInfo.recvId, m_transUser);				// �޴� �������̵�
		fileInfo.fileSize = m_file.GetLength();				// ���� ũ��

															// ���� ����
		
		pMainFrame->sendPacket(&fileInfo, sizeof(CFileTransferInfo), MSGProtocol::CLNT_MSG_FILETRANSFERREQ);
	}
	else	// ���� ���� ��� �� ��
	{
		if (m_recvFilePath == _T(""))
		{
			MessageBox(_T("���� ��θ� �������ּ���."), _T("����"), MB_ICONWARNING);
			return;
		}

		// ������ ����
		if (connectServer() == FALSE)
		{
			AfxMessageBox(_T("���ϼ��� ����!(��������)"));
			disconnectServer();

			return;
		}

		// ���� ����
		if (m_file.Open(m_recvFilePath, m_recvFileOpenFlag) == FALSE)
		{
			MessageBox(_T("���Ͽ���!"), _T("���Ͽ���"), MB_ICONWARNING);
			disconnectServer();

			return;
		}

		// ���۹��� ������ ��ġ ����
		m_file.Seek(m_fStartByte, CFile::begin);

		// ������ ���� ������ ����
		AfxBeginThread(fileRecvThread, this);
	}

	// ���� �ۼ��� �۾������� ����
	m_isWork = TRUE;

	// ����ư Ȱ��ȭ ����
	m_view->GetDlgItem(IDC_BUTTON_FILEPATH)->EnableWindow(FALSE);
	m_view->GetDlgItem(IDC_BUTTON_TRANSFERSTART)->EnableWindow(FALSE);
}

void CFileTransferFrame::selectFilePath()
{
	// ���� ������ ����� ��
	if (m_mode == CFileTransferFrame::MODE_SEND)
	{
		// �������� ����
		CFileDialog fileDlg(TRUE, NULL, NULL, OFN_EXPLORER | OFN_HIDEREADONLY,
			_T("��� ����(*.*) | *.*||"), this);

		if (fileDlg.DoModal() != IDOK)
			return;

		//	AfxMessageBox(fileDlg.GetPathName());

		// ���� �ݱ�(�̹� ������ �ִٸ�)
		if (m_file.m_hFile != (HANDLE)-1)
			m_file.Close();

		// ���� ����
		if (m_file.Open(fileDlg.GetPathName(), CFile::modeRead | CFile::typeBinary) == FALSE)
		{
			MessageBox(_T("���� ����"), _T("���� ����"), MB_ICONWARNING);

			return;
		}

		// ������ ���� ���� ���̱�
		m_view->GetDlgItem(IDC_EDIT_FILEPATH)->SetWindowText(m_file.GetFilePath());

		CString fileSize = _T("");
		fileSize.Format(_T("%0.3f%s"), m_file.GetLength() / 1000000.0f, _T(" MByte"));
		m_view->GetDlgItem(IDC_EDIT_FILESIZE)->SetWindowText(fileSize);
	}
	else	// ���� ���� ��� �� ��
	{
		// ����� ���� ��ġ ����
		CFileDialog fileDlg(FALSE, NULL, m_fileTransInfo.fileName,
			OFN_EXPLORER | OFN_OVERWRITEPROMPT, _T("��� ����(*.*) | *.*||"), this);

		if (fileDlg.DoModal() != IDOK)
			return;

		m_recvFilePath = fileDlg.GetPathName();
		m_fStartByte = 0;		// ������ ������ ���� ����Ʈ

								// ���� �̸��� ������ ���� ��� �̾� �ޱ� ���� ����
		m_recvFileOpenFlag = CFile::modeWrite | CFile::typeBinary | CFile::modeCreate;	// �⺻ �÷���

		CFileStatus statusFile;
		if (CFile::GetStatus(m_recvFilePath, statusFile) == TRUE)
		{
			int result = MessageBox(_T("���� ������ ���� �մϴ�. �̾�����ðڽ��ϱ�?"), _T("��������"), MB_ICONWARNING | MB_YESNO);
			if (result == IDYES)	// �̾�ޱ� ����
			{
				// ���� ������ ���
				if (statusFile.m_size >= m_fileTransInfo.fileSize)
				{	// ������ ������ ������ ����ũ�⺸�� Ŭ���
					MessageBox(_T("�̾�ޱ� ����!"), _T("�̾�ޱ� ����"), MB_ICONWARNING);
					return;
				}

				// ���� ������ ������ġ ����
				CFile tempFile;
				tempFile.Open(m_recvFilePath, CFile::modeRead);
				m_fStartByte = tempFile.GetLength();	// CFileStatus�� ������ ��� ������
				tempFile.Close();						// long�̱� ������ ū������ �뷮�� ����� ���� �� ����.
														// �׷��� CFile�� �뷮�� �˾ƿ´�


				m_recvFileOpenFlag |= CFile::modeNoTruncate;	// �̾�ޱ⸦ ���� �÷����߰�
			}
		}
	}
}

void CFileTransferFrame::initializeFileTransfer(int mode, const CString& user, CFileTransferInfo* filetransinfo)
{
	m_mode = mode;		// �������۸�� ����
	m_transUser = user;	// �޴� ����

	CString strStatic, strPath, strTrans;

	if (m_mode == CFileTransferFrame::MODE_SEND)	// ������ �����Ϸ� �Ҷ� 
	{
		strStatic = _T("�޴���");
		strPath = _T("���ϼ���");
		strTrans = _T("��������");

		// �޴��� �̸� ��Ʈ�ѿ� ǥ��
		m_view->GetDlgItem(IDC_EDIT_FILERECEIVEID)->SetWindowText(m_transUser);
		SetWindowText(_T("��������"));
	}
	else	// ������ ������ �� ��
	{
		strStatic = _T("������");
		strPath = _T("������ġ");
		strTrans = _T("���ϼ���");

		// ������, ���ϸ�, ����ũ�� ���� ��Ʈ�ѿ� ����
		m_view->GetDlgItem(IDC_EDIT_FILERECEIVEID)->SetWindowText(filetransinfo->sendId);
		m_view->GetDlgItem(IDC_EDIT_FILEPATH)->SetWindowText(filetransinfo->fileName);

		CString fileSize = _T("");
		fileSize.Format(_T("%0.3f %s"), filetransinfo->fileSize / 1000000.0f, _T("MByte"));
		m_view->GetDlgItem(IDC_EDIT_FILESIZE)->SetWindowText(fileSize);

		// �������� ���� ����
		m_fileTransInfo = *filetransinfo;

		SetWindowText(_T("���ϼ���"));
	}

	// static ��Ʈ�� �ؽ�Ʈ �ʱ�ȭ
	m_view->GetDlgItem(IDC_STATIC_IDTITLE)->SetWindowText(strStatic);

	// ��ư ��Ʈ�� �ؽ�Ʈ �ʱ�ȭ
	m_view->GetDlgItem(IDC_BUTTON_FILEPATH)->SetWindowText(strPath);
	m_view->GetDlgItem(IDC_BUTTON_TRANSFERSTART)->SetWindowText(strTrans);
}

void CFileTransferFrame::finalizeFileTransfer()
{
	// ���� �ݱ�
	if (m_file.m_hFile != (HANDLE)-1)
		m_file.Close();

	// ���� �ۼ��� �۾� ���� ����
	m_isWork = FALSE;

	// ���� ������ ����� ��
	if (m_mode == CFileTransferFrame::MODE_SEND)
		closeServer();	// ���� �ݱ�
	else	// ���� ���� ����� ��
		disconnectServer();	// ���� ���� ����
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

	m_sAddr = m_fileTransInfo.sendAddr;	// ������ �ּ�

	if (connect(m_hSock, (SOCKADDR*)&m_sAddr, sizeof(m_sAddr))
		== SOCKET_ERROR)
	{
		AfxMessageBox(_T("���ӽ���"));
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

	CString strAddr; // �� ������ IP�ּҰ� ����ȴ�.
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

	// �ڵ��Ҵ�� ��Ʈ�� ���
	int nLen = sizeof(SOCKADDR_IN);
	getsockname(m_hSock, (SOCKADDR*)&m_sAddr, &nLen);

	if (listen(m_hSock, 5) == SOCKET_ERROR)
	{
		AfxMessageBox(_T("listen error!"));
		return FALSE;
	}

	// ������ �񵿱�� ��ȯ
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

	// ���� ���� ���� ��ġ ����
	int sendByte = 0;
	while (sendByte < sizeof(long))
	{
		sendByte = send(ftFrame->m_hSock, (char*)&(ftFrame->m_fStartByte), sizeof(long), 0);

		if (sendByte == -1)	// ���� ����
		{
			int wsaErr = WSAGetLastError();
		
			if (wsaErr == WSAEWOULDBLOCK)	// ���� ���� �ٽ� �õ�
				continue;
			else							// ���� ������ ������
				return 0;
		}
		else
			sendByte += sendByte;
	}

	// ���� ����
	char buf[CFileTransferFrame::BUF_SIZE];
	int recvSize;

	DWORD startTime = timeGetTime(); // ���ۼӵ��� ����ϱ� ���� ����
	DWORD lastTime = startTime;		 // ���ۼӵ��� ����ϱ� ���� ����
	DWORD secRecvSize = 0;

	while (ftFrame->m_isWork)
	{
		recvSize = recv(ftFrame->m_hSock, buf, CFileTransferFrame::BUF_SIZE, 0);

		// ���ۼӵ� ���				
		DWORD currTime = timeGetTime();
		secRecvSize += recvSize;

		if ((currTime - lastTime) >= 1000)	// 1�ʴ� ���
		{
			ftFrame->setRecvSpeed(secRecvSize / ((currTime - startTime) / 1000));
			lastTime = currTime;
		}

		if (recvSize == 0)	// ���� ���� ����
			break;

		if (recvSize == -1)	// ���� ����
		{
			int wsaErr = WSAGetLastError();

			if (wsaErr == WSAEWOULDBLOCK)	// ���� ���� �ٽ� �õ�
				continue;
			else							// ���� ������ ������
				break;
		}

		if (recvSize > 0)
		{
			ftFrame->m_file.Write(buf, recvSize);	// ���Ͽ� ���
													// �����Ȳ(���α׷�����) �̵�
			ftFrame->setProgressPos((float)ftFrame->m_file.GetLength() / (float)ftFrame->m_fileTransInfo.fileSize * 100);
		}
	}

	closesocket(ftFrame->m_hSock);

	// ���� �ۼ��� �۾� ������
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
	// �ӵ� ���� KByte
	speed = speed / 1000;

	// ���ڿ��� ��ȯ
	CString spdStr = _T("");
	spdStr.Format(_T("%d"), speed);

	// 1000������ �޸��� ����ش�.
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
	// ���α׷����� �ʱ�ȭ
	((CProgressCtrl*)m_view->GetDlgItem(IDC_PROGRESS_FILE))->SetPos(pos);
}

UINT CFileTransferFrame::fileSendThread(LPVOID arg)
{
	CFileTransferFrame* ftFrame = (CFileTransferFrame*)arg;

	// �������� ���� ��ġ ����
	long fStartByte = 0;
	int recvByte = 0;
	while (recvByte < sizeof(long))
	{
		recvByte = recv(ftFrame->m_hClntSock, (char*)&fStartByte, sizeof(long), 0);

		if (recvByte == -1)	// ���� ����
		{
			int wsaErr = WSAGetLastError();

			if (wsaErr == WSAEWOULDBLOCK)	// ���� ���� �ٽ� �õ�
				continue;
			else							// ���� ������ ������
				return 0;
		}
		else
			recvByte += recvByte;
	}

	// ���� ���� ��ġ �̵�
	ftFrame->m_file.Seek(fStartByte, CFile::begin);

	// ���� �۽�
	char buf[CFileTransferFrame::BUF_SIZE];
	int readSize = 0;

	DWORD startTime = timeGetTime();	// ���ۼӵ��� ����ϱ� ���� ����
	DWORD lastTime = startTime;			// ���ۼӵ��� ����ϱ� ���� ����
	DWORD secRecvSize = 0;

	while (ftFrame->m_isWork && ftFrame->m_file.GetPosition() < ftFrame->m_file.GetLength())
	{
		readSize = ftFrame->m_file.Read(buf, CFileTransferFrame::BUF_SIZE);	// �����б�

																			// ���ۼӵ� ���		
		DWORD currTime = timeGetTime();
		secRecvSize += readSize;

		if ((currTime - lastTime) >= 1000)	// 1�ʴ� ���
		{
			// ���
			ftFrame->setRecvSpeed(secRecvSize / ((currTime - startTime) / 1000));

			lastTime = currTime;
		}

		if (readSize > 0)	// ���Ͽ��� ���� ������ ����
		{
			while (send(ftFrame->m_hClntSock, buf, readSize, 0) == -1)
			{
				int wsaErr = WSAGetLastError();

				if (wsaErr == WSAEWOULDBLOCK)	// ���� �����̱� ������ �ٽ� �õ�
					continue;
				else							// ���Ͽ���
				{
					closesocket(ftFrame->m_hClntSock);	// ������ ���� �ݱ�
					ftFrame->m_isWork = FALSE;			// �ݺ����� ����������

					break;
				}
			}

			// �������(���α׷�����) �̵�
			ftFrame->setProgressPos((float)ftFrame->m_file.GetPosition() / (float)ftFrame->m_file.GetLength() * 100);
		}
	}

	shutdown(ftFrame->m_hClntSock, SD_SEND);
//	closesocket(ftFrame->m_hClntSock);	// ������ ���� �ݱ�

										// ���� �ۼ��� �۾� ������
	ftFrame->finalizeFileTransfer();

	return 0;
}


BEGIN_MESSAGE_MAP(CFileTransferFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_MESSAGE(WM_SERVER_ACCEPT, OnServerAccept)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CFileTransferFrame �޽��� ó�����Դϴ�.


int CFileTransferFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// �������� formView ��������
	CRuntimeClass *pRuntimeClass = RUNTIME_CLASS(CFileTransferView);
	m_view = (CFileTransferView*)pRuntimeClass->CreateObject();

	// formView ������ ����
	if (!m_view->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW, CRect(0, 0, 100, 100), this,
		AFX_IDW_PANE_FIRST, NULL))
	{
		return -1;
	}

	// FormViewâ ũ��� Frame ũ�⸦ ����
	RecalcLayout();
	m_view->ResizeParentToFit(FALSE);

	return 0;
}

LRESULT CFileTransferFrame::OnServerAccept(WPARAM wParam, LPARAM lParam)
{

	// ������ Ŭ���̾�Ʈ ��� ����
	SOCKADDR_IN cAddr;
	int cAddrLen = sizeof(cAddr);

	m_hClntSock = accept(m_hSock, (SOCKADDR*)&cAddr, &cAddrLen);

	// ������ ������ ������ ����
	AfxBeginThread(fileSendThread, this);

	return 0;
}


void CFileTransferFrame::OnClose()
{
	// ���� ���� ����
	if (m_isWork)
		if (MessageBox(_T("���� �����۾� �� �Դϴ�. �����Ͻðڽ��ϱ�?"), _T("����Ȯ��"), MB_ICONWARNING | MB_YESNO)
			!= IDYES)
			return;

	// ������ ������ �۾�
	finalizeFileTransfer();

	CFrameWnd::OnClose();
}


BOOL CFileTransferFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style &= ~WS_MAXIMIZEBOX;
	cs.style &= ~WS_THICKFRAME;

	return CFrameWnd::PreCreateWindow(cs);
}
