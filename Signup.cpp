// Signup.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "SimpleMessangerClient.h"
#include "Signup.h"
#include "afxdialogex.h"
#include "Common.h"
#include "MainFrm.h"


// CSignup ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CSignup, CDialogEx)

CSignup::CSignup(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SIGNUP, pParent)
	, m_Edit_Id(_T(""))
	, m_Edit_Pw(_T(""))
	, m_Edit_Pw2(_T(""))
{

}

CSignup::~CSignup()
{
}

void CSignup::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SIGNUP_ID, m_Edit_Id);
	DDV_MaxChars(pDX, m_Edit_Id, 14);
	DDX_Text(pDX, IDC_EDIT_SIGNUP_PW, m_Edit_Pw);
	DDV_MaxChars(pDX, m_Edit_Pw, 14);
	DDX_Text(pDX, IDC_EDIT_SIGNUP_PW2, m_Edit_Pw2);
	DDV_MaxChars(pDX, m_Edit_Pw2, 14);
}


BEGIN_MESSAGE_MAP(CSignup, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_SUBMIT, &CSignup::OnBnClickedButtonSubmit)
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CSignup �޽��� ó�����Դϴ�.

BOOL CSignup::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void CSignup::OnBnClickedButtonSubmit()
{
	UpdateData(TRUE);

	if (m_Edit_Pw.Compare(m_Edit_Pw2))
	{
		AfxMessageBox(_T("��й�ȣ�� ��ġ���� �ʽ��ϴ�"));
		return;
	}
	if (m_Edit_Pw.IsEmpty() || m_Edit_Id.IsEmpty())
	{
		AfxMessageBox(_T("������ �Է����ּ���"));
		return;
	}
	CMember Member;
	strcpy(Member.Member_ID, (LPCTSTR)m_Edit_Id);
	strcpy(Member.Member_PW, (LPCTSTR)m_Edit_Pw);

	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	pMainFrame->sendPacket(&Member, sizeof(Member), MSGProtocol::CLNT_MSG_MEMBERJOINREQUEST);
	OnOK();
}



