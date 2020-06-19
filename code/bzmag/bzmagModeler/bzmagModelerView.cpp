
// bzmagModelerView.cpp : CbzmagModelerView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
#ifndef SHARED_HANDLERS
#include "bzmagModeler.h"
#endif

#include "bzmagModelerDoc.h"
#include "bzmagModelerView.h"
#include "engine/engine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CbzmagModelerView

IMPLEMENT_DYNCREATE(CbzmagModelerView, CView)

BEGIN_MESSAGE_MAP(CbzmagModelerView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// CbzmagModelerView ����/�Ҹ�

CbzmagModelerView::CbzmagModelerView()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
}

CbzmagModelerView::~CbzmagModelerView()
{

}

BOOL CbzmagModelerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}

// CbzmagModelerView �׸���

void CbzmagModelerView::OnDraw(CDC* pDC)
{
	CbzmagModelerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
	HDC hDC = pDC->GetSafeHdc();
	view_.on_draw(hDC);
}

void CbzmagModelerView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CbzmagModelerView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CbzmagModelerView ����

#ifdef _DEBUG
void CbzmagModelerView::AssertValid() const
{
	CView::AssertValid();
}

void CbzmagModelerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CbzmagModelerDoc* CbzmagModelerView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CbzmagModelerDoc)));
	return (CbzmagModelerDoc*)m_pDocument;
}
#endif //_DEBUG


// CbzmagModelerView �޽��� ó����


void CbzmagModelerView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	view_.on_size(cx, cy);
}


void CbzmagModelerView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	HWND hWnd = GetSafeHwnd();
	view_.init((int)hWnd, -450, -450, 450, 450);

	CRect rc;
	GetClientRect(&rc);
	view_.set_window_size(rc.Width(), rc.Height());
}


BOOL CbzmagModelerView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	return TRUE;
	return CView::OnEraseBkgnd(pDC);
}


void CbzmagModelerView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	view_.zoom_to(4.5, point.x, point.y);
	Invalidate(FALSE);
	CView::OnLButtonDown(nFlags, point);
}
