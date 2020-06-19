#pragma once

#include "MultiSelectTreeView.h"


class CbzmagModelerDoc;

// CbzmagTreeView ���Դϴ�.
class CbzmagTreeView : public MultiSelectTreeView
{
	DECLARE_DYNCREATE(CbzmagTreeView)

protected:
	CbzmagTreeView();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CbzmagTreeView();

public:
	virtual void OnDraw(CDC* pDC);      // �� �並 �׸��� ���� �����ǵǾ����ϴ�.

public:
	CbzmagModelerDoc* GetDocument() const;

public:
	virtual void OnTreeItemChanged(NMHDR *pNMHDR, LRESULT *pResult);

protected:
	void InitGeomTree();

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
};

#ifndef _DEBUG  // CbzmagTreeView.cpp�� ����� ����
inline CbzmagModelerDoc* CbzmagTreeView::GetDocument() const
{ return reinterpret_cast<CbzmagModelerDoc*>(m_pDocument); }
#endif

