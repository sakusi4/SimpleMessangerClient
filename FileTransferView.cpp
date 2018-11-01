// FileTransferView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SimpleMessangerClient.h"
#include "FileTransferView.h"
#include "FileTransferFrame.h"


// CFileTransferView

IMPLEMENT_DYNCREATE(CFileTransferView, CFormView)

CFileTransferView::CFileTransferView()
	: CFormView(IDD_FILETRANSFERVIEW)
{

}

CFileTransferView::~CFileTransferView()
{
}

void CFileTransferView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFileTransferView, CFormView)
	ON_BN_CLICKED(IDC_BUTTON_FILEPATH, &CFileTransferView::OnBnClickedButtonFilepath)
	ON_BN_CLICKED(IDC_BUTTON_TRANSFERSTART, &CFileTransferView::OnBnClickedButtonTransferstart)
	ON_BN_CLICKED(IDC_BUTTON_FILECANCEL, &CFileTransferView::OnBnClickedButtonFilecancel)
END_MESSAGE_MAP()


// CFileTransferView 진단입니다.

#ifdef _DEBUG
void CFileTransferView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CFileTransferView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CFileTransferView 메시지 처리기입니다.


BOOL CFileTransferView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CFormView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}


void CFileTransferView::OnBnClickedButtonFilepath()
{
	// 프래임에서 처리
	((CFileTransferFrame*)GetParent())->selectFilePath();

	// 프로그레스바 초기화
	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_FILE))->SetRange(0, 100);
	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_FILE))->SetPos(0);
}


void CFileTransferView::OnBnClickedButtonTransferstart()
{
	// 프래임에서 처리
	((CFileTransferFrame*)GetParent())->startFileTransfer();
}


void CFileTransferView::OnBnClickedButtonFilecancel()
{
	if (((CFileTransferFrame*)GetParent())->m_isWork == TRUE)
		if (MessageBox(_T("파일 전송 진행중 입니다. 취소하시겠습니까?"), _T("전송취소"), MB_ICONWARNING | MB_YESNO)
			== IDYES)
			((CFileTransferFrame*)GetParent())->finalizeFileTransfer();
}
