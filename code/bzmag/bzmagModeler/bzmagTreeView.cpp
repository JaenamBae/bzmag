// CCbzmagTreeView.cpp : 구현 파일입니다.
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


// CGeomTreeView 그리기입니다.
void CbzmagTreeView::OnDraw(CDC* pDC)
{
	CbzmagModelerDoc* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.
}

// CbzmagTreeView 진단입니다.
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

CbzmagModelerDoc* CbzmagTreeView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
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

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	InitGeomTree();
}
