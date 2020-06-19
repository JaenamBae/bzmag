
// bzmagModelerView.h : CbzmagModelerView 클래스의 인터페이스
//

#pragma once

#include "bzmag/core/kernel.h"
#include "bzmag/core/module.h"
#include "bzmag/engine/engine.h"

using namespace bzmag;
using namespace bzmag::engine;

INCLUDE_MODULE(Engine);

class CbzmagModelerDoc;

class CbzmagModelerView : public CView
{
protected: // serialization에서만 만들어집니다.
	CbzmagModelerView();
	DECLARE_DYNCREATE(CbzmagModelerView)

// 특성입니다.
public:
	CbzmagModelerDoc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// 구현입니다.
public:
	virtual ~CbzmagModelerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	ModelerView view_;
	
// 생성된 메시지 맵 함수
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual void OnInitialUpdate();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // bzmagModelerView.cpp의 디버그 버전
inline CbzmagModelerDoc* CbzmagModelerView::GetDocument() const
   { return reinterpret_cast<CbzmagModelerDoc*>(m_pDocument); }
#endif

