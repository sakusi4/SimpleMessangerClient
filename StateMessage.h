#pragma once


// CStateMessage ��ȭ �����Դϴ�.

class CStateMessage : public CDialogEx
{
	DECLARE_DYNAMIC(CStateMessage)

public:
	CStateMessage(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CStateMessage();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CString m_Edit_StateMessage;
	afx_msg void OnBnClickedButtonSubmit();
};
