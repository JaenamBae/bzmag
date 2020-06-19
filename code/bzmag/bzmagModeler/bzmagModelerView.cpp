
// bzmagModelerView.cpp : CbzmagModelerView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
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

// CbzmagModelerView 생성/소멸

CbzmagModelerView::CbzmagModelerView()
{
	// TODO: 여기에 생성 코드를 추가합니다.
}

CbzmagModelerView::~CbzmagModelerView()
{

}

BOOL CbzmagModelerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CbzmagModelerView 그리기

void CbzmagModelerView::OnDraw(CDC* pDC)
{
	CbzmagModelerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
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


// CbzmagModelerView 진단

#ifdef _DEBUG
void CbzmagModelerView::AssertValid() const
{
	CView::AssertValid();
}

void CbzmagModelerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CbzmagModelerDoc* CbzmagModelerView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CbzmagModelerDoc)));
	return (CbzmagModelerDoc*)m_pDocument;
}
#endif //_DEBUG


// CbzmagModelerView 메시지 처리기


void CbzmagModelerView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	view_.on_size(cx, cy);
}


void CbzmagModelerView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	HWND hWnd = GetSafeHwnd();
	view_.init((int)hWnd, -450, -450, 450, 450);

	CRect rc;
	GetClientRect(&rc);
	view_.set_window_size(rc.Width(), rc.Height());
}


BOOL CbzmagModelerView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	return TRUE;
	return CView::OnEraseBkgnd(pDC);
}


void CbzmagModelerView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	view_.zoom_to(4.5, point.x, point.y);
	Invalidate(FALSE);
	CView::OnLButtonDown(nFlags, point);
}
