// CMultiSelectTreeView.cpp : ���� �����Դϴ�.
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

	// �並 ����ϴ�.
	const DWORD dwViewStyle = TVS_TRACKSELECT | WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | TVS_SHOWSELALWAYS ;
	if (!m_treeCtrl.Create(dwViewStyle, rectDummy, this, 1))
	{
		TRACE0("Ʈ�� ��Ʈ���� ������ ���߽��ϴ�.\n");
		return -1;
	}
	m_treeCtrl.ShowWindow(TRUE);
	m_treeCtrl.UpdateWindow();

	return 0;
}

void MultiSelectTreeView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	AdjustLayout();
}

void MultiSelectTreeView::OnSetFocus(CWnd* pOldWnd)
{
	CView::OnSetFocus(pOldWnd);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	m_treeCtrl.SetFocus();
}
