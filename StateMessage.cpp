// StateMessage.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "SimpleMessangerClient.h"
#include "StateMessage.h"
#include "afxdialogex.h"


// CStateMessage ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CStateMessage, CDialogEx)

CStateMessage::CStateMessage(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_STATEMESSAGE, pParent)
	, m_Edit_StateMessage(_T(""))
{

}

CStateMessage::~CStateMessage()
{
}

void CStateMessage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_STATEMESSAGE, m_Edit_StateMessage);
	DDV_MaxChars(pDX, m_Edit_StateMessage, 25);
}


BEGIN_MESSAGE_MAP(CStateMessage, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_SUBMIT, &CStateMessage::OnBnClickedButtonSubmit)
END_MESSAGE_MAP()


// CStateMessage �޽��� ó�����Դϴ�.


void CStateMessage::OnBnClickedButtonSubmit()
{
	UpdateData(TRUE);
	OnOK();
}
