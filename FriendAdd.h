#pragma once


// CFriendAdd ��ȭ �����Դϴ�.

class CFriendAdd : public CDialogEx
{
	DECLARE_DYNAMIC(CFriendAdd)

public:
	CFriendAdd(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CFriendAdd();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FRIENDADD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CString m_Edit_AddId;
	afx_msg void OnBnClickedButtonFraddSubmit();
};
