
// bzMagView.cpp : bzMagView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
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
	// ǥ�� �μ� ����Դϴ�.
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

// bzMagView ����/�Ҹ�
bzMagView::bzMagView()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
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
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}

// bzMagView �׸���
void bzMagView::OnDraw(CDC* pDC)
{
	bzMagDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
	HDC hDC = pDC->GetSafeHdc();
	view_.on_draw((int)(hDC));
}


// bzMagView �μ�
void bzMagView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL bzMagView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void bzMagView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void bzMagView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}


void bzMagView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// bzMagView ����

#ifdef _DEBUG
void bzMagView::AssertValid() const
{
	CView::AssertValid();
}

void bzMagView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

bzMagDoc* bzMagView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(bzMagDoc)));
	return (bzMagDoc*)m_pDocument;
}
#endif //_DEBUG


// bzMagView �޽��� ó����


void bzMagView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	view_.on_size(cx, cy);
}


void bzMagView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
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
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	return TRUE;
}

BOOL bzMagView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	int zoomMode = view_.getZoomMode();
	if (nHitTest != HTCLIENT || zoomMode == ModelerView::ZoomOff) {
		return CView::OnSetCursor(pWnd, nHitTest, message);
	}

	SetCursor();
	return TRUE;
}


void bzMagView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
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
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
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
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	view_.on_mouse_move(point.x, point.y, nFlags);
	SetCursor();
	Invalidate(FALSE);

	CView::OnMouseMove(nFlags, point);
}

void bzMagView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	view_.on_key_down(0, 0, nChar, nFlags);
	SetCursor();
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void bzMagView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	view_.on_key_up(0, 0, nChar, nFlags);
	SetCursor();
	CView::OnKeyUp(nChar, nRepCnt, nFlags);
}


void bzMagView::OnToolsMovewindow()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	int zoomMode = view_.getZoomMode();
	if(zoomMode == ModelerView::Drag)
		view_.setZoomMode(ModelerView::ZoomOff);
	else
		view_.setZoomMode(ModelerView::Drag);
}


void bzMagView::OnUpdateToolsMovewindow(CCmdUI *pCmdUI)
{
	// TODO: ���⿡ ��� ������Ʈ UI ó���� �ڵ带 �߰��մϴ�.
	int zoomMode = view_.getZoomMode();
	pCmdUI->SetCheck(zoomMode == ModelerView::Drag);
}


void bzMagView::OnToolsZoomin()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	int zoomMode = view_.getZoomMode();
	if(zoomMode == ModelerView::ZoomIn)
		view_.setZoomMode(ModelerView::ZoomOff);
	else
		view_.setZoomMode(ModelerView::ZoomIn);
}


void bzMagView::OnUpdateToolsZoomin(CCmdUI *pCmdUI)
{
	// TODO: ���⿡ ��� ������Ʈ UI ó���� �ڵ带 �߰��մϴ�.
	int zoomMode = view_.getZoomMode();
	pCmdUI->SetCheck(zoomMode == ModelerView::ZoomIn);
}


void bzMagView::OnToolsZoomout()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	int zoomMode = view_.getZoomMode();
	if(zoomMode == ModelerView::ZoomIn)
		view_.setZoomMode(ModelerView::ZoomOff);
	else
		view_.setZoomMode(ModelerView::ZoomOut);
}


void bzMagView::OnUpdateToolsZoomout(CCmdUI *pCmdUI)
{
	// TODO: ���⿡ ��� ������Ʈ UI ó���� �ڵ带 �߰��մϴ�.
	int zoomMode = view_.getZoomMode();
	pCmdUI->SetCheck(zoomMode == ModelerView::ZoomOut);
}

void bzMagView::OnToolsFitwindow()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
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