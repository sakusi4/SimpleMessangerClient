// MyTree.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SimpleMessangerClient.h"
#include "MyTree.h"


// CMyTree

IMPLEMENT_DYNAMIC(CMyTree, CTreeCtrl)

CMyTree::CMyTree()
{

}

CMyTree::~CMyTree()
{
}


BEGIN_MESSAGE_MAP(CMyTree, CTreeCtrl)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CMyTree 메시지 처리기입니다.

void CMyTree::SetItemFont(HTREEITEM hItem, LOGFONT& logfont)
{
	Color_Font cf;
	if (!m_mapColorFont.Lookup(hItem, cf))
		cf.color = (COLORREF)-1;
	cf.logfont = logfont;
	m_mapColorFont[hItem] = cf;
}

void CMyTree::SetItemBold(HTREEITEM hItem, BOOL bBold)
{
	SetItemState(hItem, bBold ? TVIS_BOLD : 0, TVIS_BOLD);
}

void CMyTree::SetItemColor(HTREEITEM hItem, COLORREF color)
{
	Color_Font cf;
	if (!m_mapColorFont.Lookup(hItem, cf))
		cf.logfont.lfFaceName[0] = '\0';

	cf.color = color;
	m_mapColorFont[hItem] = cf;
}

BOOL CMyTree::GetItemFont(HTREEITEM hItem, LOGFONT * plogfont)
{
	Color_Font cf;
	if (!m_mapColorFont.Lookup(hItem, cf))
		return FALSE;
	if (cf.logfont.lfFaceName[0] == '\0')
		return FALSE;
	*plogfont = cf.logfont;
	return TRUE;

}

BOOL CMyTree::GetItemBold(HTREEITEM hItem)
{
	return GetItemState(hItem, TVIS_BOLD) & TVIS_BOLD;
}

COLORREF CMyTree::GetItemColor(HTREEITEM hItem)
{
	// Returns (COLORREF)-1 if color was not set
	Color_Font cf;
	if (!m_mapColorFont.Lookup(hItem, cf))
		return (COLORREF)-1;
	return cf.color;

}


void CMyTree::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CTreeCtrl::OnPaint()을(를) 호출하지 마십시오.

					   // Create a memory DC compatible with the paint DC
	CDC memDC;
	memDC.CreateCompatibleDC(&dc);

	CRect rcClip, rcClient;
	dc.GetClipBox(&rcClip);
	GetClientRect(&rcClient);

	// Select a compatible bitmap into the memory DC
	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height());
	memDC.SelectObject(&bitmap);

	// Set clip region to be same as that in paint DC
	CRgn rgn;
	rgn.CreateRectRgnIndirect(&rcClip);
	memDC.SelectClipRgn(&rgn);
	rgn.DeleteObject();


	
		// First let the control do its default drawing.
		CWnd::DefWindowProc(WM_PAINT, (WPARAM)memDC.m_hDC, 0);


	HTREEITEM hItem = GetFirstVisibleItem();

	int n = GetVisibleCount() + 1;
	while (hItem && n--)
	{
		CRect rect;

		// Do not meddle with selected items or drop highlighted items
		UINT selflag = TVIS_DROPHILITED | TVIS_SELECTED;
		Color_Font cf;
		
			if (!(GetItemState(hItem, selflag) & selflag)
				&& m_mapColorFont.Lookup(hItem, cf))
			{
				CFont *pFontDC;
				CFont fontDC;
				LOGFONT logfont;

				if (cf.logfont.lfFaceName[0] != '\0')
				{
					logfont = cf.logfont;
				}
				else
				{
					// No font specified, so use window font
					CFont *pFont = GetFont();
					pFont->GetLogFont(&logfont);
				}

				if (GetItemBold(hItem))
					logfont.lfWeight = 700;
				fontDC.CreateFontIndirect(&logfont);
				pFontDC = memDC.SelectObject(&fontDC);

				if (cf.color != (COLORREF)-1)
					memDC.SetTextColor(cf.color);

				CString sItem = GetItemText(hItem);

				GetItemRect(hItem, &rect, TRUE);
				memDC.SetBkColor(GetSysColor(COLOR_WINDOW));
				memDC.TextOut(rect.left + 2, rect.top + 1, sItem);

				memDC.SelectObject(pFontDC);
			}
		hItem = GetNextVisibleItem(hItem);
	}


	dc.BitBlt(rcClip.left, rcClip.top, rcClip.Width(), rcClip.Height(), &memDC,
		rcClip.left, rcClip.top, SRCCOPY);

}
