// FriendAdd.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "SimpleMessangerClient.h"
#include "FriendAdd.h"
#include "afxdialogex.h"


// CFriendAdd ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CFriendAdd, CDialogEx)

CFriendAdd::CFriendAdd(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_FRIENDADD, pParent)
	, m_Edit_AddId(_T(""))
{

}

CFriendAdd::~CFriendAdd()
{
}

void CFriendAdd::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_FRADD_ID, m_Edit_AddId);
	DDV_MaxChars(pDX, m_Edit_AddId, 15);
}


BEGIN_MESSAGE_MAP(CFriendAdd, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_FRADD_SUBMIT, &CFriendAdd::OnBnClickedButtonFraddSubmit)
END_MESSAGE_MAP()


// CFriendAdd �޽��� ó�����Դϴ�.


void CFriendAdd::OnBnClickedButtonFraddSubmit()
{
	UpdateData(TRUE);
	OnOK();
}
