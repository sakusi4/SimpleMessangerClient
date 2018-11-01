// StateMessage.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SimpleMessangerClient.h"
#include "StateMessage.h"
#include "afxdialogex.h"


// CStateMessage 대화 상자입니다.

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


// CStateMessage 메시지 처리기입니다.


void CStateMessage::OnBnClickedButtonSubmit()
{
	UpdateData(TRUE);
	OnOK();
}
