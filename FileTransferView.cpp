// FileTransferView.cpp : ���� �����Դϴ�.
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


// CFileTransferView �����Դϴ�.

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


// CFileTransferView �޽��� ó�����Դϴ�.


BOOL CFileTransferView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	return CFormView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}


void CFileTransferView::OnBnClickedButtonFilepath()
{
	// �����ӿ��� ó��
	((CFileTransferFrame*)GetParent())->selectFilePath();

	// ���α׷����� �ʱ�ȭ
	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_FILE))->SetRange(0, 100);
	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_FILE))->SetPos(0);
}


void CFileTransferView::OnBnClickedButtonTransferstart()
{
	// �����ӿ��� ó��
	((CFileTransferFrame*)GetParent())->startFileTransfer();
}


void CFileTransferView::OnBnClickedButtonFilecancel()
{
	if (((CFileTransferFrame*)GetParent())->m_isWork == TRUE)
		if (MessageBox(_T("���� ���� ������ �Դϴ�. ����Ͻðڽ��ϱ�?"), _T("�������"), MB_ICONWARNING | MB_YESNO)
			== IDYES)
			((CFileTransferFrame*)GetParent())->finalizeFileTransfer();
}
