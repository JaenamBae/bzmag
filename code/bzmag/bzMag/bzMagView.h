#pragma once

/*
Description : bzMagView Ŭ������ �������̽�
              bzmag::engine::ModelerView class���� �������̽� --> �޽��� �ڵ鷯 ����
			  ����� ���Ǹ� ���� ��庰 ���콺 ������ ��ȭ��� �߰�
Last Update : 2014.07.25
Author : Jaenam Bae (jnbae@katech.re.kr)
*/

#include "bzmag/core/kernel.h"
#include "bzmag/core/module.h"
#include "bzmag/engine/engine.h"
#include "bzmag/winGraphics/modelerview.h"

using namespace bzmag;
using namespace bzmag::engine;

INCLUDE_MODULE(Engine);
INCLUDE_MODULE(WinGraphics);

class bzMagDoc;

class bzMagView : public CView
{
protected: // serialization������ ��������ϴ�.
	bzMagView();
	DECLARE_DYNCREATE(bzMagView)

protected:
	enum CursorType
	{
		CursorNormal,
		CursorLoupe,
		CursorLoupePlus,
		CursorLoupeMinus,
		CursorGripOpen,
		CursorGripClosed
	};


// Ư���Դϴ�.
public:
	bzMagDoc* GetDocument() const;

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// �����Դϴ�.
public:
	virtual ~bzMagView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	ModelerView view_;
	static double m_DefaultPresets[];	// ����Ʈ ������
	static HCURSOR m_Cursors[6];		// ���콺 Ŀ��


public:
	// Get the number of available presets.
	static BOOL LoadCursor(CursorType type, UINT nResourceID, HINSTANCE hInst = NULL);
	static BOOL LoadCursor(int type, UINT nResourceID, HINSTANCE hInst = NULL)
	{ return LoadCursor((CursorType) type, nResourceID, hInst) ; }
	// Load the cursors QZoomView shows. All instances of QZoomView share the same
	// cursors, so loading is only needed once.

	void SetCursor(void);
	void SelectNode();

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
	afx_msg void OnToolsFitwindow();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnToolsMovewindow();
	afx_msg void OnUpdateToolsMovewindow(CCmdUI *pCmdUI);
	afx_msg void OnToolsZoomin();
	afx_msg void OnUpdateToolsZoomin(CCmdUI *pCmdUI);
	afx_msg void OnToolsZoomout();
	afx_msg void OnUpdateToolsZoomout(CCmdUI *pCmdUI);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg LRESULT OnGeomPart(WPARAM wParam, LPARAM lParam);
};

#ifndef _DEBUG  // bzMagView.cpp�� ����� ����
inline bzMagDoc* bzMagView::GetDocument() const
   { return reinterpret_cast<bzMagDoc*>(m_pDocument); }
#endif

