#pragma once


// CStateMessage 대화 상자입니다.

class CStateMessage : public CDialogEx
{
	DECLARE_DYNAMIC(CStateMessage)

public:
	CStateMessage(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CStateMessage();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CString m_Edit_StateMessage;
	afx_msg void OnBnClickedButtonSubmit();
};
