#pragma once

#include "MultiSelectTreeCtrl.h"


// MultiSelectTreeView 뷰입니다.
class MultiSelectTreeView : public CView
{
	friend class MultiSelectTreeCtrl;

protected:
	MultiSelectTreeView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~MultiSelectTreeView();

protected:
	// 트리 화면에 맞춤
	void AdjustLayout();


protected:
	MultiSelectTreeCtrl m_treeCtrl;
	HTREEITEM m_hSheetsRoot;
	HTREEITEM m_hLinesRoot;
	HTREEITEM m_hCSsRoot;

protected:
	DECLARE_MESSAGE_MAP()

public:
	virtual void OnTreeItemChanged(NMHDR *pNMHDR, LRESULT *pResult) = 0;

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
};

