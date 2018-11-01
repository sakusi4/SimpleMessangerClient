// ChatBoxView.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "SimpleMessangerClient.h"
#include "ChatBoxView.h"

#include "MainFrm.h"

// CChatBoxView

IMPLEMENT_DYNCREATE(CChatBoxView, CFormView)

CChatBoxView::CChatBoxView()
	: CFormView(IDD_CHATBOXVIEW)
	, m_Edit_InputMsg(_T(""))
{

}

CChatBoxView::~CChatBoxView()
{
}

void CChatBoxView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_CHAT_INPUTMSG, m_Edit_InputMsg);
	DDV_MaxChars(pDX, m_Edit_InputMsg, 400);
	DDX_Control(pDX, IDC_EDIT_CHAT_OUTPUTMSG, m_Edit_OutputMsg);
}

BEGIN_MESSAGE_MAP(CChatBoxView, CFormView)
	ON_BN_CLICKED(IDC_BUTTON_CHAT_SEND, &CChatBoxView::OnBnClickedButtonChatSend)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CChatBoxView �����Դϴ�.

#ifdef _DEBUG
void CChatBoxView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CChatBoxView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CChatBoxView �޽��� ó�����Դϴ�.
BOOL CChatBoxView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	return CFormView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

void CChatBoxView::OnBnClickedButtonChatSend()
{
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();

	CString msg = _T("");
	GetDlgItemText(IDC_EDIT_CHAT_INPUTMSG, msg);

	CEdit* pEditOutput = (CEdit*)GetDlgItem(IDC_EDIT_CHAT_OUTPUTMSG);

	CMessageBox messagebox;
	strcpy(messagebox.Message, (LPCTSTR)msg);
	strcpy(messagebox.Id, (LPCTSTR)pMainFrame->MyId);
	
	insertMessage(&messagebox);

	strcpy(messagebox.Id, (LPCTSTR)FriendId);
	
	pMainFrame->sendPacket(&messagebox, sizeof(messagebox), MSGProtocol::CLNT_MSG_SENDCHATMSG);

	msg = _T("");
	SetDlgItemText(IDC_EDIT_CHAT_INPUTMSG, msg);

	CEdit* pEditInput = (CEdit*)GetDlgItem(IDC_EDIT_CHAT_INPUTMSG);
	pEditInput->SetFocus();
}

void CChatBoxView::insertMessage(CMessageBox * pMessageBox)
{
	CEdit* pEditOutput = (CEdit*)GetDlgItem(IDC_EDIT_CHAT_OUTPUTMSG);
	int Len = pEditOutput->GetWindowTextLength();
	pEditOutput->SetSel(Len, Len);

	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CString idAndMessage = _T("");
	CString ShowMessage = _T("");
	
	BOOL bMode = FALSE;
	HTREEITEM hItem;
	hItem = pMainFrame->pFriendListView->m_Tree_Friends.GetRootItem();
	HTREEITEM hChildItem = pMainFrame->pFriendListView->m_Tree_Friends.GetChildItem(hItem);
	while (hChildItem != NULL)
	{
		if (pMainFrame->pFriendListView->m_Tree_Friends.GetItemText(hChildItem).Find(pMessageBox->Id) != -1)
		{
			idAndMessage = pMainFrame->pFriendListView->m_Tree_Friends.GetItemText(hChildItem);
			bMode = TRUE;
			break;
		}
		hChildItem = pMainFrame->pFriendListView->m_Tree_Friends.GetNextSiblingItem(hChildItem);
	}

	if (bMode)
		ShowMessage = idAndMessage;
	else
		ShowMessage = (CString)pMessageBox->Id + _T(" (") + pMainFrame->pFriendListView->StateMessage + _T(" )");
	
	ShowMessage += _T(" : \r\n");
	ShowMessage += pMessageBox->Message;
	ShowMessage += _T("\r\n");

	pEditOutput->ReplaceSel(ShowMessage);
}

BOOL CChatBoxView::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
//	return CFormView::OnEraseBkgnd(pDC);
}

void CChatBoxView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
					   // �׸��� �޽����� ���ؼ��� CFormView::OnPaint()��(��) ȣ������ ���ʽÿ�.
	CRect ClientRect;
	GetClientRect(&ClientRect);
	dc.FillSolidRect(&ClientRect, RGB(255, 255, 255));
}
