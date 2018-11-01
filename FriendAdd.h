#pragma once


// CFriendAdd 대화 상자입니다.

class CFriendAdd : public CDialogEx
{
	DECLARE_DYNAMIC(CFriendAdd)

public:
	CFriendAdd(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CFriendAdd();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FRIENDADD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CString m_Edit_AddId;
	afx_msg void OnBnClickedButtonFraddSubmit();
};
