// bzMagTreeView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "bzMag.h"
#include "bzmagTreeView.h"
#include "bzMagDoc.h"
#include "bzMagView.h"

// bzMagTreeView

IMPLEMENT_DYNCREATE(bzMagTreeView, MultiSelectTreeView)

bzMagTreeView::bzMagTreeView()
{
	
}

bzMagTreeView::~bzMagTreeView()
{

}

BEGIN_MESSAGE_MAP(bzMagTreeView, MultiSelectTreeView)
	ON_MESSAGE(WM_GEOMPART, OnGeomPart)
END_MESSAGE_MAP()


// CGeomTreeView 그리기입니다.
void bzMagTreeView::OnDraw(CDC* pDC)
{
	bzMagDoc* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.


}

void bzMagTreeView::SelectTreeItem(GeomBaseNode* node, bool bSelect)
{
	m_treeCtrl.SelectTreeItem(LPARAM(node), bSelect);
}

void bzMagTreeView::ClearSelection()
{
	m_treeCtrl.ClearSelection();
}

void bzMagTreeView::UpdateStatus()
{
	m_treeCtrl.Expand(m_hSheetsRoot, TVE_EXPAND);

	for( HTREEITEM hItem = m_treeCtrl.GetFirstVisibleItem(); 
		hItem != NULL; 
		hItem = m_treeCtrl.GetNextVisibleItem(hItem) )
	{
		LPARAM lParam = m_treeCtrl.GetItemData(hItem);
		GeomBaseNode* node = (GeomBaseNode*)lParam;
		GeomHeadNode* head = dynamic_cast<GeomHeadNode*>(node);

		if (head) {
			if (head->isSelected()) m_treeCtrl.SetItemState(hItem, TVIS_SELECTED, TVIS_SELECTED);
			else m_treeCtrl.SetItemState(hItem, ~TVIS_SELECTED, TVIS_SELECTED);
		}
	}
}

void bzMagTreeView::Binding()
{
	InitGeomTree();

	bzmag::Kernel* kernel = bzmag::Kernel::instance();

	// 형상정보 노드의 루트는 "/geom" 이다
	Node* geom_root = kernel->lookup("/geom");
	
	// 지오메트리 노드 트리 작성
	Node::NodeIterator it;
	for (it=geom_root->firstChildNode(); it!=geom_root->lastChildNode(); ++it)
	{
		Node* node = *it;
		GeomHeadNode* head = dynamic_cast<GeomHeadNode*>(node);
		Polygon_set_2* poly = head->getGeometry();
		assert(head != 0);
		HTREEITEM hRoot;
		if (head && poly) {
			if (!poly->is_empty()) hRoot = m_hSheetsRoot;
			else                   hRoot = m_hLinesRoot;

			if (head->isStandalone()) MakeHeadTree(head, hRoot);
		}
	}

	// 좌표계 생성
	Node* cs_root = kernel->lookup("/coord/global");
	MakeCSTree(cs_root, m_hCSsRoot);
}

// bzMagTreeView 진단입니다.
#ifdef _DEBUG
void bzMagTreeView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void bzMagTreeView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif

bzMagDoc* bzMagTreeView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(bzMagDoc)));
	return (bzMagDoc*)m_pDocument;
}
#endif //_DEBUG

void bzMagTreeView::InitGeomTree()
{
	m_treeCtrl.Initialize();
	UINT mask = TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE;
	m_hSheetsRoot = m_treeCtrl.InsertItem(mask, _T("Sheets"), 0, 0, 0, 0, 0, TVI_ROOT, TVI_LAST);
	m_hLinesRoot  = m_treeCtrl.InsertItem(mask, _T("Lines"), 0, 0, 0, 0, 0, TVI_ROOT, TVI_LAST);
	m_hCSsRoot    = m_treeCtrl.InsertItem(mask, _T("Coordinate Systems"), 0, 0, 0, 0, 0, TVI_ROOT, TVI_LAST);
}

void bzMagTreeView::MakeHeadTree(GeomHeadNode* head, HTREEITEM hParent, Node* last)
{
	UINT mask = TVIF_TEXT | TVIF_PARAM;

	if (head)
	{
		String name = head->getName();
		CString cname(name.c_str());
		HTREEITEM hHead = m_treeCtrl.InsertItem(mask, cname, 0, 0, 0, 0, (LPARAM)head, hParent, TVI_LAST);

		Node* child = 0;
		if (head->getNumChildren()>0) child = *(head->firstChildNode());
		GeomBaseNode* geom = dynamic_cast<GeomBaseNode*>(child);
		while (geom != 0)
		{
			CString description(geom->description().c_str());
			HTREEITEM hItem = m_treeCtrl.InsertItem(mask, description, 0, 0, 0, 0, (LPARAM)geom, hHead, TVI_LAST);

			// Boolean 노드인 경우 하부 트리 작성
			GeomBooleanNode* boolNode = dynamic_cast<GeomBooleanNode*>(geom);
			if (boolNode) {
				Node::NodeIterator it;
				for (it=boolNode->firstToolNode(); it!=boolNode->lastToolNode(); ++it)
				{
					Node* subnode = *it;
					GeomHeadNode* sub_head = dynamic_cast<GeomHeadNode*>(subnode);
					if (sub_head) MakeHeadTree(sub_head, hItem);
				}
			}

			// CloneFrom 노드인 경우 하부트리 작성
			GeomCloneFromNode* cloneNode = dynamic_cast<GeomCloneFromNode*>(geom);
			if (cloneNode) {
				GeomCloneToNode* from = cloneNode->getReferenceNode();
				if (from) MakeHeadTree(from->getHeadNode(), hItem, from);
			}

			if (geom == last) break;

			child = 0;
			if (geom->getNumChildren()>0) child = *(geom->firstChildNode());
			geom = dynamic_cast<GeomBaseNode*>(child);
		}
	}

	m_treeCtrl.Expand(m_hSheetsRoot, TVE_EXPAND);
}

void bzMagTreeView::MakeCSTree(Node* node, HTREEITEM hParent)
{
	UINT mask = TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE;
	CString description(node->getName().c_str());
	HTREEITEM hItem = m_treeCtrl.InsertItem(mask, description, 0, 0, 0, 0, (LPARAM)node, hParent, TVI_LAST);

	Node::NodeIterator it;
	for (it=node->firstChildNode(); it!=node->lastChildNode(); ++it)
	{
		Node* child_node = *it;
		MakeCSTree(child_node, hItem);
	}
}

void bzMagTreeView::OnTreeItemChanged(HTREEITEM hItem)
{
	CWnd* pFrame = AfxGetMainWnd();
	if (pFrame == 0) return;

	LPARAM lparam = m_treeCtrl.GetItemData(hItem);
	UINT nSelect = m_treeCtrl.GetItemState( hItem, TVIS_SELECTED ) & TVIS_SELECTED;

	GeomBaseNode* node = (GeomBaseNode*)(lparam);
	GeomHeadNode* head = dynamic_cast<GeomHeadNode*>(node);
	bzMagView*   pView = (bzMagView*)(GetParentSplitter(this, FALSE)->GetPane(0, 1));
	if (nSelect) {
		if (head) {
			head->setSelectedStatus(true);
			pView->Invalidate(FALSE);
		}
	}
	else {
		if (head) {
			head->setSelectedStatus(false);
			pView->Invalidate(FALSE);
		}
	}

	CWnd* pProperty = pFrame->GetDlgItem(ID_VIEW_PROPERTIESWND);
	if( pProperty ) {
		if (m_treeCtrl.GetSelectedCount() == 1) {
			HTREEITEM hSelected = m_treeCtrl.GetFirstSelecedItem();
			if (hSelected) lparam = m_treeCtrl.GetItemData(hSelected);
			pProperty->SendMessage( WM_GEOMPART, GEOMPART_SELECT, lparam );
		}
		else {
			pProperty->SendMessage( WM_GEOMPART, GEOMPART_SELECT, 0 );
		}
	}
}

void bzMagTreeView::OnInitialUpdate()
{
	MultiSelectTreeView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	USING_MODULE(Engine);
	InitGeomTree();

}

LRESULT bzMagTreeView::OnGeomPart(WPARAM wParam, LPARAM lParam)
{
	GeomBaseNode* pGeomPart = (GeomBaseNode*)lParam; 
	switch( wParam )
	{
	case GEOMPART_ADD :
	case GEOMPART_DELETE :
	case GEOM_OPEN :
		Binding();
		break;

	case GEOMPART_MODIFY:
		break;

	case GEOM_NEW :
	case GEOM_CLOSE :
		InitGeomTree();
		break;
	}

	return 0;
}

void bzMagTreeView::BindProperty()
{

}
