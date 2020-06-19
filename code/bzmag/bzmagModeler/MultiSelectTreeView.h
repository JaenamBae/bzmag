#pragma once

#include "MultiSelectTreeCtrl.h"


// MultiSelectTreeView ���Դϴ�.
class MultiSelectTreeView : public CView
{
	friend class MultiSelectTreeCtrl;

protected:
	MultiSelectTreeView();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~MultiSelectTreeView();

protected:
	// Ʈ�� ȭ�鿡 ����
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

