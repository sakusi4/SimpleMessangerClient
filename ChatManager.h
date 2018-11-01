#pragma once

#include "ChatBoxFrame.h"
#include "ChatBoxView.h"
#include "Common.h"
#include <vector>

class CChatManager
{
public:
	CChatManager();
	~CChatManager();

	static vector<CChatBoxFrame*> RoomsVector;
	void CreateRoom(CMessageBox* pMessageBox);
	static void DeleteRoom(CString id);
};

