#pragma once

#include "SignupView.h"

// CSignupFrame 프레임입니다.

class CSignupFrame : public CFrameWnd
{
	DECLARE_DYNCREATE(CSignupFrame)
protected:
	CSignupFrame();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CSignupFrame();

	CSignupView* pSignupView;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


