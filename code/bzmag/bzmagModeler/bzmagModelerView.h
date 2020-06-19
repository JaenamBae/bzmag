
// bzmagModelerView.h : CbzmagModelerView Ŭ������ �������̽�
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
protected: // serialization������ ��������ϴ�.
	CbzmagModelerView();
	DECLARE_DYNCREATE(CbzmagModelerView)

// Ư���Դϴ�.
public:
	CbzmagModelerDoc* GetDocument() const;

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// �����Դϴ�.
public:
	virtual ~CbzmagModelerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	ModelerView view_;
	
// ������ �޽��� �� �Լ�
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

#ifndef _DEBUG  // bzmagModelerView.cpp�� ����� ����
inline CbzmagModelerDoc* CbzmagModelerView::GetDocument() const
   { return reinterpret_cast<CbzmagModelerDoc*>(m_pDocument); }
#endif

