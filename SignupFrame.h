#pragma once

#include "SignupView.h"

// CSignupFrame �������Դϴ�.

class CSignupFrame : public CFrameWnd
{
	DECLARE_DYNCREATE(CSignupFrame)
protected:
	CSignupFrame();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CSignupFrame();

	CSignupView* pSignupView;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


