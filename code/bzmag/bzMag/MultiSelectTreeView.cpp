// CMultiSelectTreeView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MultiSelectTreeView.h"
#include "BufferDC.h"

// MultiSelectTreeView
MultiSelectTreeView::MultiSelectTreeView()
{

}

MultiSelectTreeView::~MultiSelectTreeView()
{

}

BEGIN_MESSAGE_MAP(MultiSelectTreeView, CView)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

void MultiSelectTreeView::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
		return;

	CRect rectClient;
	GetClientRect(&rectClient);
	m_treeCtrl.SetWindowPos(NULL, rectClient.left + 1, rectClient.top + 1, rectClient.Width() - 2, rectClient.Height() - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}

int MultiSelectTreeView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// 뷰를 만듭니다.
	const DWORD dwViewStyle = TVS_TRACKSELECT | WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | TVS_SHOWSELALWAYS ;
	if (!m_treeCtrl.Create(dwViewStyle, rectDummy, this, 1))
	{
		TRACE0("트리 컨트롤을 만들지 못했습니다.\n");
		return -1;
	}
	m_treeCtrl.ShowWindow(TRUE);
	m_treeCtrl.UpdateWindow();

	return 0;
}

void MultiSelectTreeView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	AdjustLayout();
}

void MultiSelectTreeView::OnSetFocus(CWnd* pOldWnd)
{
	CView::OnSetFocus(pOldWnd);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_treeCtrl.SetFocus();
}
