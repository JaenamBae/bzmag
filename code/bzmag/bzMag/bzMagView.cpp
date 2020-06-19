
// bzMagView.cpp : bzMagView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "bzMag.h"
#endif

#include "bzMagDoc.h"
#include "bzMagView.h"
#include "bzMagTreeView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// bzMagView

IMPLEMENT_DYNCREATE(bzMagView, CView)

BEGIN_MESSAGE_MAP(bzMagView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &bzMagView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONUP()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_SETCURSOR()
	ON_COMMAND(ID_TOOLS_FITWINDOW, &bzMagView::OnToolsFitwindow)
	ON_COMMAND(ID_TOOLS_MOVEWINDOW, &bzMagView::OnToolsMovewindow)
	ON_COMMAND(ID_TOOLS_ZOOMIN, &bzMagView::OnToolsZoomin)
	ON_COMMAND(ID_TOOLS_ZOOMOUT, &bzMagView::OnToolsZoomout)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_MOVEWINDOW, &bzMagView::OnUpdateToolsMovewindow)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_ZOOMIN, &bzMagView::OnUpdateToolsZoomin)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_ZOOMOUT, &bzMagView::OnUpdateToolsZoomout)
END_MESSAGE_MAP()


HCURSOR bzMagView::m_Cursors[6] = { 0 };

// bzMagView 생성/소멸
bzMagView::bzMagView()
{
	// TODO: 여기에 생성 코드를 추가합니다.
//	view_.update_window();

}

bzMagView::~bzMagView()
{
}

/* static */
BOOL bzMagView::LoadCursor(CursorType type, UINT nResourceID, HINSTANCE hInst /* = NULL */)
{
	ASSERT(type >= 0 && type < 6);

	HCURSOR h(0);
	if (nResourceID)
	{

		if (hInst) 
			h = ::LoadCursor(hInst, MAKEINTRESOURCE(nResourceID));
		else 
			h = ::AfxGetApp()->LoadCursor(nResourceID);
	}

	m_Cursors[type] = h;

	return h != 0;
}

void bzMagView::SetCursor(void)
{
	int mode = view_.getZoomMode();
	int iCursor(CursorNormal);
	switch (mode)
	{
	case ModelerView::ZoomIn :
		iCursor = CursorLoupePlus;
		break;
	case ModelerView::ZoomOut :
		iCursor = CursorLoupeMinus;
		break;
	case ModelerView::ZoomRect :
		iCursor = CursorLoupe;
		break;
	case ModelerView::Drag :
		iCursor = view_.is_tracking() ? CursorGripClosed : CursorGripOpen;
		break;
	default:
		ASSERT(mode == ModelerView::ZoomOff);	// No other mode allowed
		break;
	}

	HCURSOR h = NULL;
	if (iCursor >= 0) h = m_Cursors[iCursor];
	if (h) ::SetCursor(h);
}

BOOL bzMagView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// bzMagView 그리기
void bzMagView::OnDraw(CDC* pDC)
{
	bzMagDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
	HDC hDC = pDC->GetSafeHdc();
	view_.on_draw((int)(hDC));
}


// bzMagView 인쇄
void bzMagView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL bzMagView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void bzMagView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void bzMagView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


void bzMagView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// bzMagView 진단

#ifdef _DEBUG
void bzMagView::AssertValid() const
{
	CView::AssertValid();
}

void bzMagView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

bzMagDoc* bzMagView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(bzMagDoc)));
	return (bzMagDoc*)m_pDocument;
}
#endif //_DEBUG


// bzMagView 메시지 처리기


void bzMagView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	view_.on_size(cx, cy);
}


void bzMagView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	HWND hWnd = GetSafeHwnd();
	view_.set_window_handle(int(hWnd));
	view_.set_model_rect(-450, -450, 450, 450);

	CRect rc;
	GetClientRect(&rc);
	view_.set_window_size(rc.Width(), rc.Height());

	// Load the cursors.
	// The cursor loading might as well (or even preferably) be done in the
	// InitInstance() function of the application class, as it should be done only
	// once per program instance. But this doesn't hurt.
	UINT idCursors[] = 
	{
		IDC_LOUPE, IDC_LOUPEPLUS, IDC_LOUPEMINUS, IDC_GRIPOPEN, IDC_GRIPCLOSED
	};
	m_Cursors[0] = ::LoadCursor(NULL, IDC_ARROW);
	for (int i = 1; i <= sizeof(idCursors) / sizeof(UINT); i++) {
		VERIFY(LoadCursor(i, idCursors[i-1]));
	}
}


BOOL bzMagView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	return TRUE;
}

BOOL bzMagView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	int zoomMode = view_.getZoomMode();
	if (nHitTest != HTCLIENT || zoomMode == ModelerView::ZoomOff) {
		return CView::OnSetCursor(pWnd, nHitTest, message);
	}

	SetCursor();
	return TRUE;
}


void bzMagView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	view_.on_mouse_lbutton_down(point.x, point.y, nFlags);
	SetCursor();

	if (view_.getZoomMode() == ModelerView::ZoomOff) {

		bzMagTreeView* pTreeView = (bzMagTreeView*)(GetParentSplitter(this, FALSE)->GetPane(0, 0));
		pTreeView->UpdateStatus();
	}

	Invalidate(FALSE);
	CView::OnLButtonDown(nFlags, point);
}

void bzMagView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	view_.on_mouse_lbutton_up(point.x, point.y, nFlags);
	SetCursor();
	Invalidate(FALSE);

	CView::OnLButtonUp(nFlags, point);
}


void bzMagView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}


void bzMagView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	view_.on_mouse_move(point.x, point.y, nFlags);
	SetCursor();
	Invalidate(FALSE);

	CView::OnMouseMove(nFlags, point);
}

void bzMagView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	view_.on_key_down(0, 0, nChar, nFlags);
	SetCursor();
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void bzMagView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	view_.on_key_up(0, 0, nChar, nFlags);
	SetCursor();
	CView::OnKeyUp(nChar, nRepCnt, nFlags);
}


void bzMagView::OnToolsMovewindow()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	int zoomMode = view_.getZoomMode();
	if(zoomMode == ModelerView::Drag)
		view_.setZoomMode(ModelerView::ZoomOff);
	else
		view_.setZoomMode(ModelerView::Drag);
}


void bzMagView::OnUpdateToolsMovewindow(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	int zoomMode = view_.getZoomMode();
	pCmdUI->SetCheck(zoomMode == ModelerView::Drag);
}


void bzMagView::OnToolsZoomin()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	int zoomMode = view_.getZoomMode();
	if(zoomMode == ModelerView::ZoomIn)
		view_.setZoomMode(ModelerView::ZoomOff);
	else
		view_.setZoomMode(ModelerView::ZoomIn);
}


void bzMagView::OnUpdateToolsZoomin(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	int zoomMode = view_.getZoomMode();
	pCmdUI->SetCheck(zoomMode == ModelerView::ZoomIn);
}


void bzMagView::OnToolsZoomout()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	int zoomMode = view_.getZoomMode();
	if(zoomMode == ModelerView::ZoomIn)
		view_.setZoomMode(ModelerView::ZoomOff);
	else
		view_.setZoomMode(ModelerView::ZoomOut);
}


void bzMagView::OnUpdateToolsZoomout(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	int zoomMode = view_.getZoomMode();
	pCmdUI->SetCheck(zoomMode == ModelerView::ZoomOut);
}

void bzMagView::OnToolsFitwindow()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	view_.fit_to_window();
	Invalidate(FALSE);
}

void bzMagView::SelectNode()
{
	/*
	bzmag::Kernel* kernel = bzmag::Kernel::instance();
	Node* geom_root = kernel->lookup("/geom");
	Node::NodeIterator it;

	if (geom_root == 0) return;

	GeomBaseNode* selected = 0;
	for (it=geom_root->firstChildNode(); it!=geom_root->lastChildNode(); ++it)
	{
		GeomHeadNode* node = dynamic_cast<GeomHeadNode*>((Node*)(*it));
		if (node && node->isSelected() && node->isStandalone() && !node->isHide()) {
			selected = node;
		}
	}
	*/



}

LRESULT bzMagView::OnGeomPart(WPARAM wParam, LPARAM lParam)
{
	GeomBaseNode* pGeomPart = (GeomBaseNode*)lParam; 
	switch( wParam )
	{
	case GEOMPART_ADD :
	case GEOMPART_DELETE :
	case GEOMPART_MODIFY:
		break;

	case GEOM_NEW :
	case GEOM_OPEN :
	case GEOM_CLOSE :
		break;
	}

	return 0;
}