// MessageBox.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "SimpleMessangerClient.h"
#include "MessageBoxView.h"
#include "Common.h"
#include "MainFrm.h"
#include "MessageBoxFrame.h"



// CMessageBox

IMPLEMENT_DYNCREATE(CMessageBoxView, CFormView)

CMessageBoxView::CMessageBoxView()
	: CFormView(IDD_MESSAGEBOX)
	, m_Edit_Message(_T(""))
{

}

CMessageBoxView::~CMessageBoxView()
{
}

void CMessageBoxView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_MSG_MESSAGE, m_Edit_Message);
	DDV_MaxChars(pDX, m_Edit_Message, 400);
}

BEGIN_MESSAGE_MAP(CMessageBoxView, CFormView)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_BUTTON_MSG_RESEND, &CMessageBoxView::OnBnClickedButtonMsgResend)
	ON_BN_CLICKED(IDC_BUTTON_MSG_OK, &CMessageBoxView::OnBnClickedButtonMsgOk)
	ON_BN_CLICKED(IDC_BUTTON_MSG_EXIT, &CMessageBoxView::OnBnClickedButtonMsgExit)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CMessageBox �����Դϴ�.

#ifdef _DEBUG
void CMessageBoxView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CMessageBoxView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMessageBox �޽��� ó�����Դϴ�.




BOOL CMessageBoxView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	return CFormView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}


int CMessageBoxView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}


void CMessageBoxView::OnBnClickedButtonMsgResend()
{

	CRuntimeClass* pRuntimeClass = RUNTIME_CLASS(CMessageBoxFrame);
	CMessageBoxFrame* pMessageBoxFrame = (CMessageBoxFrame*)pRuntimeClass->CreateObject();

	CMessageBox* messagebox = new CMessageBox();
	strcpy(messagebox->Id, (LPCTSTR)FriendId);
	pMessageBoxFrame->messageSetting(messagebox);

	pMessageBoxFrame->ShowWindow(SW_SHOW);
	
	GetParentFrame()->DestroyWindow();
}


void CMessageBoxView::OnBnClickedButtonMsgOk()
{
	UpdateData(TRUE);

	CMessageBox messagebox;
	strcpy(messagebox.Id, (LPCTSTR)FriendId);
	strcpy(messagebox.Message, (LPCTSTR)m_Edit_Message);

	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	pMainFrame->sendPacket(&messagebox, sizeof(messagebox), MSGProtocol::CLNT_MSG_SENDPAPER);

	GetParentFrame()->DestroyWindow();
}


void CMessageBoxView::OnBnClickedButtonMsgExit()
{
	GetParentFrame()->DestroyWindow();
}


BOOL CMessageBoxView::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
//	return CFormView::OnEraseBkgnd(pDC);
}


void CMessageBoxView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
					   // �׸��� �޽����� ���ؼ��� CFormView::OnPaint()��(��) ȣ������ ���ʽÿ�.
	CRect ClientRect;
	GetClientRect(&ClientRect);
	dc.FillSolidRect(&ClientRect, RGB(255, 255, 255));
}
