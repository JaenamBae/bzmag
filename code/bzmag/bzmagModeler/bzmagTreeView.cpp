// CCbzmagTreeView.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "bzmagModeler.h"
#include "bzmagTreeView.h"
#include "bzmagModelerDoc.h"


// CbzmagTreeView

IMPLEMENT_DYNCREATE(CbzmagTreeView, MultiSelectTreeView)

CbzmagTreeView::CbzmagTreeView()
{
	
}

CbzmagTreeView::~CbzmagTreeView()
{

}

BEGIN_MESSAGE_MAP(CbzmagTreeView, MultiSelectTreeView)

END_MESSAGE_MAP()


// CGeomTreeView �׸����Դϴ�.
void CbzmagTreeView::OnDraw(CDC* pDC)
{
	CbzmagModelerDoc* pDoc = GetDocument();
	// TODO: ���⿡ �׸��� �ڵ带 �߰��մϴ�.
}

// CbzmagTreeView �����Դϴ�.
#ifdef _DEBUG
void CbzmagTreeView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CbzmagTreeView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif

CbzmagModelerDoc* CbzmagTreeView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CbzmagModelerDoc)));
	return (CbzmagModelerDoc*)m_pDocument;
}
#endif //_DEBUG

void CbzmagTreeView::InitGeomTree()
{
	m_treeCtrl.DeleteAllItems();
	UINT mask = TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE;
	m_hSheetsRoot = m_treeCtrl.InsertItem(mask, _T("Sheets"), 0, 0, 0, 0, 0, TVI_ROOT, TVI_LAST);
	m_hLinesRoot = m_treeCtrl.InsertItem(mask, _T("Lines"), 0, 0, 0, 0, 0, TVI_ROOT, TVI_LAST);
	m_hCSsRoot = m_treeCtrl.InsertItem(mask, _T("Coordinate Systems"), 0, 0, 0, 0, 0, TVI_ROOT, TVI_LAST);
}

void CbzmagTreeView::OnTreeItemChanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMTVITEMCHANGE *pNMTVItemChange = (NMTVITEMCHANGE*)pNMHDR;
	HTREEITEM hItem = pNMTVItemChange->hItem;
	CWnd* pFrame = AfxGetMainWnd();

	pResult = 0;
}

void CbzmagTreeView::OnInitialUpdate()
{
	MultiSelectTreeView::OnInitialUpdate();

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	InitGeomTree();
}
