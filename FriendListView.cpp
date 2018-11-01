// FriendListView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SimpleMessangerClient.h"
#include "FriendListView.h"
#include "MainFrm.h"
#include "MessageBoxFrame.h"
#include "ChatBoxFrame.h"
#include "FriendAdd.h"
#include "StateMessage.h"
#include "FileTransferFrame.h"
#include <future>

vector<CString> CFriendListView::FriendVector;
// CFriendListView

IMPLEMENT_DYNCREATE(CFriendListView, CFormView)

CFriendListView::CFriendListView()
	: CFormView(IDD_FRIENDLISTVIEW)
	, m_Edit_Search(_T(""))
{
	OnlineFriendCount = 0;
	pChatManager = new CChatManager();
}

CFriendListView::~CFriendListView()
{
	delete pChatManager;
}

void CFriendListView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_FRIENDS, m_Tree_Friends);
	DDX_Control(pDX, IDC_BUTTON_MAIN_SEARCH, m_Button_Search);
	DDX_Text(pDX, IDC_EDIT_SEARCH, m_Edit_Search);
	DDV_MaxChars(pDX, m_Edit_Search, 30);
	DDX_Control(pDX, IDC_STATIC_IMAGE, m_Static_Image);
}

BEGIN_MESSAGE_MAP(CFriendListView, CFormView)
	ON_WM_CREATE()
	ON_COMMAND(ID_MENU_FRDELETE, &CFriendListView::OnMenuFrdelete)
	ON_COMMAND(ID_SENDMESSAGE, &CFriendListView::OnSendmessage)
	ON_NOTIFY(NM_RCLICK, IDC_TREE_FRIENDS, &CFriendListView::OnNMRClickTreeFriends)
	ON_COMMAND(ID_MENU_CHAT, &CFriendListView::OnMenuChat)
	ON_COMMAND(ID_MENU_FRADD, &CFriendListView::OnMenuFradd)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_NOTIFY(NM_DBLCLK, IDC_TREE_FRIENDS, &CFriendListView::OnNMDblclkTreeFriends)
	ON_BN_CLICKED(IDC_BUTTON_MAIN_SEARCH, &CFriendListView::OnBnClickedButtonMainSearch)

	ON_EN_SETFOCUS(IDC_EDIT_SEARCH, &CFriendListView::OnEnSetfocusEditSearch)
	ON_COMMAND(ID_STATEMESSAGE, &CFriendListView::OnStatemessage)
	ON_COMMAND(ID_FILESEND, &CFriendListView::OnFilesend)
	ON_COMMAND(ID_IMAGE, &CFriendListView::OnImage)
END_MESSAGE_MAP()

// CFriendListView 진단입니다.

#ifdef _DEBUG
void CFriendListView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CFriendListView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CFriendListView 메시지 처리기입니다.
BOOL CFriendListView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	return CFormView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

int CFriendListView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

BOOL CFriendListView::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style &= ~(WS_BORDER);
	return CFormView::PreCreateWindow(cs);
}

void CFriendListView::OnInitialUpdate()
{
	GetParent()->ModifyStyleEx(WS_EX_CLIENTEDGE, 0);

	CFormView::OnInitialUpdate();

	GetParentFrame()->RecalcLayout();
	ResizeParentToFit(FALSE);

	CBitmap Bmp;
	Bmp.LoadBitmapA(IDB_STATE);

	CImageList ImgList;
	ImgList.Create(16, 16, ILC_COLOR24, 4, 0);
	ImgList.Add(&Bmp, RGB(255, 0, 0));

	CFont cFont;
	cFont.CreateFont(20, 10, 0, 0, FW_HEAVY, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, _T("굴림"));

	Font.lfHeight = 20;
	Font.lfWidth = 10;
	Font.lfEscapement = 0;
	Font.lfOrientation = 0;
	Font.lfWeight = FW_HEAVY;
	Font.lfItalic = FALSE;
	Font.lfUnderline = FALSE;
	Font.lfStrikeOut = FALSE;
	Font.lfCharSet = DEFAULT_CHARSET;
	Font.lfOutPrecision = OUT_DEFAULT_PRECIS;
	Font.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	Font.lfQuality = DEFAULT_QUALITY;
	Font.lfPitchAndFamily = DEFAULT_PITCH;
	strcpy(Font.lfFaceName, (LPCSTR)_T("굴림"));

	m_Tree_Friends.SetFont(&cFont);
	m_Tree_Friends.SetImageList(&ImgList, TVSIL_NORMAL);
	ImgList.Detach();

	m_Tree_Friends.DeleteAllItems();

	CString RootText;
	RootText.Format("친구목록 (%d/%d)", 0, FriendVector.size());
	m_Tree_Friends.InsertItem(RootText, 2, 2, TVI_ROOT, TVI_LAST);

	m_Tree_Friends.SetItemFont(m_Tree_Friends.GetRootItem(), Font);
	m_Tree_Friends.SetItemColor(m_Tree_Friends.GetRootItem(), RGB(20, 190, 177));

	m_Button_Search.SetIcon(IDI_SEARCH);
	m_Button_Search.DrawTransparent();

	m_Edit_Search = _T("친구를 검색 해보세요.");
	UpdateData(FALSE);
}


void CFriendListView::FriendAddTree(FriendState* pFriendState)
{
	for (int i = 0; i < FriendVector.size(); i++)
	{
		if (!(FriendVector[i].Compare(pFriendState->id)))
			return;
	}

	if (pFriendState->state == MSGProtocol::USER_STATE_LOGIN)
		OnlineFriendCount++;

	FriendVector.push_back(pFriendState->id);

	CString idAndMessage = (CString)pFriendState->id;
	idAndMessage += _T(" (");
	idAndMessage += (CString)pFriendState->statemessage;
	idAndMessage += _T(")");

	m_Tree_Friends.InsertItem(idAndMessage, pFriendState->state, pFriendState->state, m_Tree_Friends.GetRootItem(), TVI_LAST);
	m_Tree_Friends.Invalidate();
}


void CFriendListView::FriendSetState(FriendState * pFriendState)
{
	HTREEITEM  hChild;
	hChild = m_Tree_Friends.GetChildItem(m_Tree_Friends.GetRootItem());

	while (hChild != NULL)
	{
		if (!FriendGetName(hChild).Compare(pFriendState->id))
		{
			if (pFriendState->state == MSGProtocol::USER_STATE_LOGIN)
			{
				CString SoundPath = _T("./sound/buddy_login.wav");
				PlaySound(SoundPath, AfxGetInstanceHandle(), SND_ASYNC);

				OnlineFriendCount++;
				m_Tree_Friends.SetItemImage(hChild, 1, 1);
				OnPaint();
				return;
			}
			else
			{
				OnlineFriendCount--;
				m_Tree_Friends.SetItemImage(hChild, 0, 0);
				OnPaint();
				return;
			}
		}
		hChild = m_Tree_Friends.GetNextSiblingItem(hChild);
	}
}

void CFriendListView::FirnedSetStateMessage(FriendStateMessage* friendstatemessage)
{
	HTREEITEM  hChild;

	hChild = m_Tree_Friends.GetChildItem(m_Tree_Friends.GetRootItem());
	while (hChild != NULL)
	{
		if (!FriendGetName(hChild).Compare(friendstatemessage->id))
		{
			CString temp = friendstatemessage->id;
			temp += _T(" (");
			temp += friendstatemessage->Message;
			temp += _T(")");
			m_Tree_Friends.SetItemText(hChild, temp);
		}
		hChild = m_Tree_Friends.GetNextSiblingItem(hChild);
	}
}

CString CFriendListView::FriendGetName(HTREEITEM hChild)
{
	CString Name = m_Tree_Friends.GetItemText(hChild);
	if (Name.Find(_T(" ")) != -1)
	{
		int index = Name.Find(_T(" "));
		Name = Name.Left(index);
		return Name;
	}
	return Name;
}

BOOL CFriendListView::IsLogin(CString Id)
{
	HTREEITEM  hChild;
	hChild = m_Tree_Friends.GetChildItem(m_Tree_Friends.GetRootItem());

	int image;

	while (hChild != NULL)
	{
		if (!FriendGetName(hChild).Compare(Id))
		{
			m_Tree_Friends.GetItemImage(hChild, image, image);
			if (image == 0)
				return FALSE;

			else
				return TRUE;
		}
		hChild = m_Tree_Friends.GetNextSiblingItem(hChild);
	}
}

void CFriendListView::OnNMRClickTreeFriends(NMHDR *pNMHDR, LRESULT *pResult)
{
	TV_HITTESTINFO hit_info;

	// 화면상에서 마우스의 위치를 얻는다.
	::GetCursorPos(&hit_info.pt);

	// 얻은 마우스 좌표를 트리컨트롤 기준의 좌표로 변경한다.
	::ScreenToClient(m_Tree_Friends.m_hWnd, &hit_info.pt);

	// 현재 마우스 좌표가 위치한 항목 정보를 얻는다.
	HTREEITEM current_item = m_Tree_Friends.HitTest(&hit_info);

	if (current_item == m_Tree_Friends.GetRootItem())
	{
		return;
	}
	if (current_item != NULL)
	{
		// 마우스가 위치한 항목을 찾았다면 해당 항목을 선택한다.
		m_Tree_Friends.Select(current_item, TVGN_CARET);

		// 오른쪽 버튼 클릭시 팝업메뉴 뜨게
		CPoint p;
		GetCursorPos(&p);
		CMenu menu;
		menu.LoadMenu(IDR_MAINFRAME);
		CMenu* Dmenu = menu.GetSubMenu(2); //맨 앞에 메뉴를 가져옴
		Dmenu->AppendMenu(MF_SEPARATOR, NULL, "");	// 세퍼레이터 메뉴
		Dmenu->AppendMenu(MF_STRING, ID_MENU_FRDELETE, "친구삭제");	// 친구삭제 메뉴도 추가
		Dmenu->DeleteMenu(ID_STATEMESSAGE, MF_BYCOMMAND);
		Dmenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, p.x, p.y, AfxGetMainWnd());
	}
	*pResult = 0;
}

void CFriendListView::OnPaint()
{
	CPaintDC dc(this);

	Graphics graphics(dc);
	graphics.SetSmoothingMode(SmoothingModeHighQuality);
	SetStretchBltMode(dc.m_hDC, HALFTONE);

	CRect ClientRect;
	GetClientRect(&ClientRect);
	dc.FillSolidRect(&ClientRect, RGB(255, 255, 255));

	LinearGradientBrush TopBrush(Rect(0, 0, 500, 80), Color(250, 222, 231, 142), Color(245, 222, 231, 142), LinearGradientModeVertical);
	graphics.FillRectangle(&TopBrush, 0, 0, 500, 80);

	//화면 하단 회색 칠하기
	dc.FillSolidRect(0, 410, 480, 200, RGB(240, 240, 240));

	CImage MarkImage;
	MarkImage.Load(_T("./res/mark.png"));
	MarkImage.TransparentBlt(dc.m_hDC, 0, ClientRect.BottomRight().y - 20, ClientRect.Width() - 20, 20, RGB(255, 255, 255));

	CPen Pen;
	CPen* pOldPen;
	Pen.CreatePen(PS_SOLID, 1, RGB(192, 192, 192));
	pOldPen = dc.SelectObject(&Pen);
	//화면 상단 선 긋기
	dc.MoveTo(0, 80);
	dc.LineTo(500, 80);
	//화면 하단 선 긋기
	dc.MoveTo(0, 410);
	dc.LineTo(500, 410);
	dc.MoveTo(0, 480);
	dc.LineTo(500, 480);
	dc.SelectObject(pOldPen);

	CImage Clover;
	Clover.Load(_T("./res/clover.png"));
	Clover.StretchBlt(dc.m_hDC, ClientRect.BottomRight().x - 35, 415, 25, 25);

	CImage Clovers;
	Clovers.Load(_T("./res/clovers.png"));

	Clovers.StretchBlt(dc.m_hDC, ClientRect.BottomRight().x - 170, 23, 160, 50);

	dc.SetBkMode(TRANSPARENT);
	CRect StateRect(90, 17, 400, 50);
	CRect NameRect(90, 50, 400, 70);

	CFont Font;
	Font.CreateFont(25, 10, 0, 0, FW_EXTRABOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH, _T("Arial"));
	CFont* pOldFont = dc.SelectObject(&Font);
	dc.DrawText(StateMessage, &StateRect, DT_TOP | DT_LEFT);
	dc.SelectObject(pOldFont);

	dc.DrawText(((CMainFrame*)AfxGetMainWnd())->MyId + _T("  [ 온라인 ]"), &NameRect, DT_TOP | DT_LEFT);

	CString RootText;
	RootText.Format("친구목록 (%d/%d)", OnlineFriendCount, FriendVector.size());
	m_Tree_Friends.SetItemColor(m_Tree_Friends.GetRootItem(), RGB(20, 190, 177));
	m_Tree_Friends.SetItemText(m_Tree_Friends.GetRootItem(), RootText);
}

void CFriendListView::OnNMDblclkTreeFriends(NMHDR *pNMHDR, LRESULT *pResult)
{
	TV_HITTESTINFO hit_info;

	// 화면상에서 마우스의 위치를 얻는다.
	::GetCursorPos(&hit_info.pt);

	// 얻은 마우스 좌표를 트리컨트롤 기준의 좌표로 변경한다.
	::ScreenToClient(m_Tree_Friends.m_hWnd, &hit_info.pt);

	// 현재 마우스 좌표가 위치한 항목 정보를 얻는다.
	HTREEITEM current_item = m_Tree_Friends.HitTest(&hit_info);

	if (current_item == m_Tree_Friends.GetRootItem())
	{
		static BOOL bFlag = FALSE;
		if (!bFlag)
		{
			m_Tree_Friends.SetItemImage(m_Tree_Friends.GetRootItem(), 3, 3);
			bFlag = TRUE;
		}
		else
		{
			m_Tree_Friends.SetItemImage(m_Tree_Friends.GetRootItem(), 2, 2);
			bFlag = FALSE;
		}
	}
	*pResult = 0;
}

void CFriendListView::OnMenuFrdelete()
{

	HTREEITEM hItem = m_Tree_Friends.GetSelectedItem();
	if (hItem == NULL)
	{
		AfxMessageBox(_T("삭제할 친구를 선택해주세요"));
	}
	else
	{
		if (hItem == m_Tree_Friends.GetRootItem())
		{
			AfxMessageBox(_T("정상적인 유저가 아닙니다."));
			return;
		}

		CString DeleteId = FriendGetName(hItem);

		for (auto IterBegin = FriendVector.begin(); IterBegin != FriendVector.end(); IterBegin++)
		{
			if (!(IterBegin->Compare(DeleteId)))
			{
				FriendVector.erase(IterBegin);
				break;
			}
		}

		HTREEITEM hRoot = m_Tree_Friends.GetRootItem();
		HTREEITEM hChild = m_Tree_Friends.GetChildItem(hRoot);
		while (hChild != NULL)
		{
			if (!(FriendGetName(hChild).Compare(DeleteId)))
			{
				m_Tree_Friends.DeleteItem(hChild);
				break;
			}
			hChild = m_Tree_Friends.GetNextSiblingItem(hChild);
		}


		char Buf[MSGProtocol::IDLENGTH];
		strcpy(Buf, (LPCTSTR)DeleteId);

		CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
		pMainFrame->sendPacket(Buf, sizeof(Buf), MSGProtocol::CLNT_MSG_FRIENDDELETE);
	}
}


void CFriendListView::OnSendmessage()
{
	HTREEITEM hItem = m_Tree_Friends.GetSelectedItem();

	if (hItem == NULL)
	{
		AfxMessageBox(_T("쪽지를 보낼 친구를 선택해주세요"));
	}
	else
	{
		if (hItem == m_Tree_Friends.GetRootItem())
		{
			AfxMessageBox(_T("정상적인 유저가 아닙니다."));
			return;
		}
		if (!IsLogin(FriendGetName(hItem)))
		{
			AfxMessageBox(_T("해당 유저가 오프라인 입니다."));
			return;
		}
		CString FriendId = FriendGetName(hItem);

		CRuntimeClass* pRuntimeClass = RUNTIME_CLASS(CMessageBoxFrame);
		CMessageBoxFrame* pMessageBoxFrame = (CMessageBoxFrame*)pRuntimeClass->CreateObject();

		CMessageBox messagebox;
		strcpy(messagebox.Id, (LPCTSTR)FriendId);
		pMessageBoxFrame->messageSetting(&messagebox);

		pMessageBoxFrame->ShowWindow(SW_SHOW);
	}
}

void CFriendListView::OnMenuChat()
{
	HTREEITEM hItem = m_Tree_Friends.GetSelectedItem();

	if (hItem == NULL)
	{
		AfxMessageBox(_T("대화를 신청할 친구를 선택해주세요"));
	}
	else
	{
		if (hItem == m_Tree_Friends.GetRootItem())
		{
			AfxMessageBox(_T("정상적인 유저가 아닙니다."));
			return;
		}
		if (!IsLogin(FriendGetName(hItem)))
		{
			AfxMessageBox(_T("해당 유저가 오프라인 입니다."));
			return;
		}
		CMessageBox messagebox;
		strcpy(messagebox.Id, (LPCTSTR)FriendGetName(hItem));
		strcpy(messagebox.Message, (LPCTSTR)_T(""));

		pChatManager->CreateRoom(&messagebox);

	}
}
void CFriendListView::OnFilesend()
{
	HTREEITEM hItem = m_Tree_Friends.GetSelectedItem();

	if (hItem == NULL)
	{
		AfxMessageBox(_T("파일을 전송할 친구를 선택해주세요"));
	}
	else
	{
		if (hItem == m_Tree_Friends.GetRootItem())
		{
			AfxMessageBox(_T("정상적인 유저가 아닙니다."));
			return;
		}
		if (!IsLogin(FriendGetName(hItem)))
		{
			AfxMessageBox(_T("해당 유저가 오프라인 입니다."));
			return;
		}
		CString recvUser = FriendGetName(hItem);

		CRuntimeClass *pRuntimeClass = RUNTIME_CLASS(CFileTransferFrame);
		CFileTransferFrame* fileTransferFrame = (CFileTransferFrame*)pRuntimeClass->CreateObject();

		fileTransferFrame->initializeFileTransfer(CFileTransferFrame::MODE_SEND, recvUser);
		fileTransferFrame->ShowWindow(SW_SHOW);
	}
}
void CFriendListView::OnStatemessage()
{
	CStateMessage statemessage;
	if (statemessage.DoModal() == IDOK)
	{
		FriendStateMessage friendstatemessage;
		strcpy(friendstatemessage.id, (LPCTSTR)((CMainFrame*)AfxGetMainWnd())->MyId);
		strcpy(friendstatemessage.Message, (LPCTSTR)statemessage.m_Edit_StateMessage);

		StateMessage = statemessage.m_Edit_StateMessage;

		((CMainFrame*)AfxGetMainWnd())->sendPacket(&friendstatemessage, sizeof(friendstatemessage), MSGProtocol::CLNT_MSG_STATEMESSAGE);
		Invalidate();
	}
}

void CFriendListView::OnMenuFradd()
{
	CFriendAdd FriendAdd;
	if (FriendAdd.DoModal() == IDOK)
	{
		char Buf[MSGProtocol::IDLENGTH];
		strcpy(Buf, (LPCTSTR)FriendAdd.m_Edit_AddId);

		CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
		pMainFrame->sendPacket(Buf, sizeof(Buf), MSGProtocol::CLNT_MSG_FRIENDINSERT);
	}
}

void CFriendListView::OnBnClickedButtonMainSearch()
{
	UpdateData(TRUE);

	CString temp = m_Edit_Search;
	HTREEITEM  hChild;

	hChild = m_Tree_Friends.GetChildItem(m_Tree_Friends.GetRootItem());
	while (hChild != NULL)
	{
		if (!FriendGetName(hChild).Compare(temp))
		{
			m_Tree_Friends.SelectItem(hChild);
			AfxMessageBox(_T("선택 되었습니다."));
			return;
		}
		hChild = m_Tree_Friends.GetNextSiblingItem(hChild);
	}
	AfxMessageBox(_T("친구목록에 존재하지 않습니다"));
}

void CFriendListView::OnEnSetfocusEditSearch()
{
	m_Edit_Search = _T("");
	UpdateData(FALSE);
}

BOOL CFriendListView::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CFriendListView::OnImage()
{
	CFileDialog Dlg(TRUE, _T("Image"), NULL,
		OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST,
		_T("All Files(*.*)|*.*||"), this);
	if (Dlg.DoModal() == IDOK)
	{
		ImagePath = Dlg.GetPathName();

		if (ImagePath.Find(_T(".jpg")) == -1 && ImagePath.Find(_T(".png")) == -1 &&
			ImagePath.Find(_T(".bmp")) == -1)
		{
			AfxMessageBox(_T("이미지 파일이 아닙니다."));
			return;
		}
	}
	char ImagePathBuf[100];
	strcpy(ImagePathBuf, (LPCTSTR)ImagePath);

	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	pMainFrame->sendPacket(ImagePathBuf, sizeof(ImagePathBuf), MSGProtocol::CLNT_MSG_SETIMAGE);
	Invalidate();
}