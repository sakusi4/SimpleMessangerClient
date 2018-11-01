// MessageBoxFrame.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "SimpleMessangerClient.h"
#include "MessageBoxFrame.h"


// CMessageBoxFrame

IMPLEMENT_DYNCREATE(CMessageBoxFrame, CFrameWnd)

CMessageBoxFrame::CMessageBoxFrame()
{
	Create(NULL, _T("���� ����"));
}

CMessageBoxFrame::~CMessageBoxFrame()
{
}


BEGIN_MESSAGE_MAP(CMessageBoxFrame, CFrameWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CMessageBoxFrame �޽��� ó�����Դϴ�.


void CMessageBoxFrame::messageSetting(CMessageBox* Friend, BOOL bReceive)
{
	pMessageBoxView->FriendId = Friend->Id;
	CEdit* pEdit = (CEdit*)pMessageBoxView->GetDlgItem(IDC_EDIT_MSG_MESSAGE);

	if (bReceive == FALSE)
	{
		pEdit->SetReadOnly(FALSE);
		pMessageBoxView->GetDlgItem(IDC_BUTTON_MSG_RESEND)->ShowWindow(SW_HIDE);
		pMessageBoxView->GetDlgItem(IDC_BUTTON_MSG_EXIT)->ShowWindow(SW_HIDE);
		pMessageBoxView->GetDlgItem(IDC_BUTTON_MSG_OK)->ShowWindow(SW_SHOW);
		CString temp = _T("�Կ��� ����");
		CString title = Friend->Id + temp;
		SetWindowText(title);
	}
	else
	{
		
		pEdit->SetReadOnly(TRUE);
		pMessageBoxView->GetDlgItem(IDC_BUTTON_MSG_RESEND)->ShowWindow(SW_SHOW);
		pMessageBoxView->GetDlgItem(IDC_BUTTON_MSG_EXIT)->ShowWindow(SW_SHOW);
		pMessageBoxView->GetDlgItem(IDC_BUTTON_MSG_OK)->ShowWindow(SW_HIDE);
		CString temp = _T("�Կ��� ����");
		CString title = Friend->Id + temp;
		SetWindowText(title);

		pMessageBoxView->m_Edit_Message = Friend->Message;
		pMessageBoxView->UpdateData(FALSE);
	}

}

int CMessageBoxFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  ���⿡ Ư��ȭ�� �ۼ� �ڵ带 �߰��մϴ�.

	CRuntimeClass* pRuntimeClass = RUNTIME_CLASS(CMessageBoxView);
	pMessageBoxView = (CMessageBoxView*)pRuntimeClass->CreateObject();
	

	pMessageBoxView->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW, CRect(0, 0, 150, 150), this, AFX_IDW_PANE_FIRST);

	
	RecalcLayout();
	pMessageBoxView->ResizeParentToFit(FALSE);

	return 0;
}
