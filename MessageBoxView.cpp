// MessageBox.cpp : 구현 파일입니다.
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


// CMessageBox 진단입니다.

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


// CMessageBox 메시지 처리기입니다.




BOOL CMessageBoxView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

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
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CFormView::OnPaint()을(를) 호출하지 마십시오.
	CRect ClientRect;
	GetClientRect(&ClientRect);
	dc.FillSolidRect(&ClientRect, RGB(255, 255, 255));
}
