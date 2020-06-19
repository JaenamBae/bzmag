#pragma once

#include "MultiSelectTreeView.h"


class CbzmagModelerDoc;

// CbzmagTreeView 뷰입니다.
class CbzmagTreeView : public MultiSelectTreeView
{
	DECLARE_DYNCREATE(CbzmagTreeView)

protected:
	CbzmagTreeView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CbzmagTreeView();

public:
	virtual void OnDraw(CDC* pDC);      // 이 뷰를 그리기 위해 재정의되었습니다.

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

#ifndef _DEBUG  // CbzmagTreeView.cpp의 디버그 버전
inline CbzmagModelerDoc* CbzmagTreeView::GetDocument() const
{ return reinterpret_cast<CbzmagModelerDoc*>(m_pDocument); }
#endif

