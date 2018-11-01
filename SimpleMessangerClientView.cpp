
// SimpleMessangerClientView.cpp : CSimpleMessangerClientView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "SimpleMessangerClient.h"
#endif

#include "SimpleMessangerClientDoc.h"
#include "SimpleMessangerClientView.h"
#include "Signup.h"
#include "Common.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSimpleMessangerClientView

IMPLEMENT_DYNCREATE(CSimpleMessangerClientView, CFormView)

BEGIN_MESSAGE_MAP(CSimpleMessangerClientView, CFormView)
	ON_BN_CLICKED(IDC_BUTTON_SIGNUP, &CSimpleMessangerClientView::OnBnClickedButtonSignup)
	ON_BN_CLICKED(IDC_BUTTON_LOGIN, &CSimpleMessangerClientView::OnBnClickedButtonLogin)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_BLOG, &CSimpleMessangerClientView::OnBnClickedButtonBlog)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

// CSimpleMessangerClientView 생성/소멸

CSimpleMessangerClientView::CSimpleMessangerClientView()
	: CFormView(IDD_SIMPLEMESSANGERCLIENT_FORM)
	, m_Edit_Id(_T(""))
	, m_Edit_Pw(_T(""))
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CSimpleMessangerClientView::~CSimpleMessangerClientView()
{
}

void CSimpleMessangerClientView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_LOGIN_ID, m_Edit_Id);
	DDV_MaxChars(pDX, m_Edit_Id, 14);
	DDX_Text(pDX, IDC_EDIT_LOGIN_PW, m_Edit_Pw);
	DDV_MaxChars(pDX, m_Edit_Pw, 14);
}

BOOL CSimpleMessangerClientView::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style &= ~(WS_BORDER);

	return CFormView::PreCreateWindow(cs);
}

void CSimpleMessangerClientView::OnInitialUpdate()
{
	GetParent()->ModifyStyleEx(WS_EX_CLIENTEDGE, 0);

	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();
}


// CSimpleMessangerClientView 진단

#ifdef _DEBUG
void CSimpleMessangerClientView::AssertValid() const
{
	CFormView::AssertValid();
}

void CSimpleMessangerClientView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CSimpleMessangerClientDoc* CSimpleMessangerClientView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSimpleMessangerClientDoc)));
	return (CSimpleMessangerClientDoc*)m_pDocument;
}
#endif //_DEBUG


// CSimpleMessangerClientView 메시지 처리기

void CSimpleMessangerClientView::OnBnClickedButtonSignup()
{
	
	CSignup Signup;
	Signup.DoModal();
}

void CSimpleMessangerClientView::OnBnClickedButtonLogin()
{
	UpdateData(TRUE);
	CMember Member;
	strcpy(Member.Member_ID, (LPCTSTR)m_Edit_Id);
	strcpy(Member.Member_PW, (LPCTSTR)m_Edit_Pw);


	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	pMainFrame->MyId = m_Edit_Id;
	pMainFrame->sendPacket(&Member, sizeof(Member), MSGProtocol::CLNT_MSG_LOGINREQUEST);

}


BOOL CSimpleMessangerClientView::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
//	return CFormView::OnEraseBkgnd(pDC);
}


void CSimpleMessangerClientView::OnPaint()
{
	CPaintDC dc(this); 
	
	Graphics graphics(dc);

	CRect ClientRect;
	GetClientRect(&ClientRect);

	dc.FillSolidRect(&ClientRect, RGB(255, 255, 255));
	LinearGradientBrush TopBrush(Rect(0, 0, 500, 300), Color(255, 222, 231, 142), Color(0, 222, 231, 142), LinearGradientModeVertical);
	graphics.FillRectangle(&TopBrush, 0, 0, 500, 300);

	CImage LoginLogoImage;
	LoginLogoImage.Load(_T("./res/로그인 로고.png"));
	LoginLogoImage.TransparentBlt(dc.m_hDC, 70, 40, ClientRect.Width() - 100, ClientRect.Height() / 4, RGB(255, 255, 255));
}

void CSimpleMessangerClientView::OnBnClickedButtonBlog()
{
	ShellExecute(m_hWnd, _T("open"), _T("http://blog.naver.com/kizxc4395"), NULL, NULL, SW_SHOW);
}


HBRUSH CSimpleMessangerClientView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);

	UINT nID = pWnd->GetDlgCtrlID();

	if (nCtlColor == CTLCOLOR_STATIC)
	{
		if (nID == IDC_STATIC_LOGIN_ID)
		{
			pDC->SetBkMode(TRANSPARENT);      // 배경투명
			return (HBRUSH)::GetStockObject(NULL_BRUSH);
		}
		if (nID == IDC_STATIC_LOGIN_PW)
		{
			pDC->SetBkMode(TRANSPARENT);      // 배경투명
			return (HBRUSH)::GetStockObject(NULL_BRUSH);
		}
	}
	return hbr;
}
