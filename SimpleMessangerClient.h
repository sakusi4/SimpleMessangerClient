
// SimpleMessangerClient.h : SimpleMessangerClient ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.


// CSimpleMessangerClientApp:
// �� Ŭ������ ������ ���ؼ��� SimpleMessangerClient.cpp�� �����Ͻʽÿ�.
//

class CSimpleMessangerClientApp : public CWinApp
{
public:
	CSimpleMessangerClientApp();


// �������Դϴ�.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// �����Դϴ�.
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CSimpleMessangerClientApp theApp;
