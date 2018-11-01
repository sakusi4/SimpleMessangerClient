#include "stdafx.h"
#include "ChatManager.h"

vector<CChatBoxFrame*> CChatManager::RoomsVector;

CChatManager::CChatManager()
{
}


CChatManager::~CChatManager()
{
}

void CChatManager::CreateRoom(CMessageBox * pMessageBox)
{
	CString FriendId = pMessageBox->Id;
	CString ShowMessage = pMessageBox->Message;

	for (int i = 0; RoomsVector.size(); i++)
	{
		if (!(RoomsVector[i]->FriendId.Compare(FriendId)))
		{
			RoomsVector[i]->SetFocus();
			RoomsVector[i]->pChatBoxView->insertMessage(pMessageBox);
			return;
		}
	}

	CRuntimeClass* pRuntimeClass = RUNTIME_CLASS(CChatBoxFrame);
	CChatBoxFrame* pMessageBoxFrame = (CChatBoxFrame*)pRuntimeClass->CreateObject();
	pMessageBoxFrame->SettingMessage(pMessageBox);

	pMessageBoxFrame->ShowWindow(SW_SHOW);
	
	if (ShowMessage != _T(""))
	{
		pMessageBoxFrame->pChatBoxView->insertMessage(pMessageBox);
	}
	
	RoomsVector.push_back(pMessageBoxFrame);
}

void CChatManager::DeleteRoom(CString id)
{
	for (auto begin = RoomsVector.begin(); begin != RoomsVector.end(); begin++)
	{
		if (!((*begin)->FriendId.Compare(id)))
		{
			RoomsVector.erase(begin);
			break;
		}
	}
}
