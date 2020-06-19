#pragma once

/*
Description : bzMagTreeView Ŭ������ �������̽�
              ���� �𵨸��� �����丮�� Ʈ�����·� �����ִ� Ŭ����
			  Ansys Maxwell �𵨷��� ��ġ��ŷ�Ͽ� �䳻������

Last Update : 2014.07.25
Author : Jaenam Bae (jnbae@katech.re.kr)
*/

#include "MultiSelectTreeView.h"
#include "core/kernel.h"
#include "core/module.h"
#include "engine/engine.h"

using namespace bzmag;
using namespace bzmag::engine;

INCLUDE_MODULE(Engine);

class bzMagDoc;

// bzMagTreeView ���Դϴ�.
class bzMagTreeView : public MultiSelectTreeView
{
	DECLARE_DYNCREATE(bzMagTreeView)

protected:
	bzMagTreeView();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~bzMagTreeView();

public:
	virtual void OnDraw(CDC* pDC);      // �� �並 �׸��� ���� �����ǵǾ����ϴ�.

public:
	bzMagDoc* GetDocument() const;

public:
	void UpdateStatus();
	void SelectTreeItem(GeomBaseNode* node, bool bSelect);
	void ClearSelection();

	void Binding();
	virtual void OnTreeItemChanged(HTREEITEM hItem);

protected:
	void InitGeomTree();
	void MakeHeadTree(GeomHeadNode* head, HTREEITEM hParent, Node* last = 0);
	void MakeCSTree(Node* node, HTREEITEM hParent);
	void BindProperty();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();


public:
	afx_msg LRESULT OnGeomPart(WPARAM wParam, LPARAM lParam);
};

#ifndef _DEBUG  // bzMagTreeView.cpp�� ����� ����
inline bzMagDoc* bzMagTreeView::GetDocument() const
{ return reinterpret_cast<bzMagDoc*>(m_pDocument); }
#endif

