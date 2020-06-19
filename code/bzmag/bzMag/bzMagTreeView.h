#pragma once

/*
Description : bzMagTreeView 클래스의 인터페이스
              형상 모델링의 히스토리를 트리형태로 보여주는 클래스
			  Ansys Maxwell 모델러를 벤치마킹하여 흉내내었음

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

// bzMagTreeView 뷰입니다.
class bzMagTreeView : public MultiSelectTreeView
{
	DECLARE_DYNCREATE(bzMagTreeView)

protected:
	bzMagTreeView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~bzMagTreeView();

public:
	virtual void OnDraw(CDC* pDC);      // 이 뷰를 그리기 위해 재정의되었습니다.

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

#ifndef _DEBUG  // bzMagTreeView.cpp의 디버그 버전
inline bzMagDoc* bzMagTreeView::GetDocument() const
{ return reinterpret_cast<bzMagDoc*>(m_pDocument); }
#endif

