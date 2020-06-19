#pragma once
#include <vector>

// CGeomTreeCtrl
class CGeomTreeCtrl : public CTreeCtrl
{
	DECLARE_DYNAMIC(CGeomTreeCtrl)

public:
	enum TrackState
	{
		TrackNothing,
		TrackReady,
		TrackContinue,
		TrackCanceled
	};

public:
	CGeomTreeCtrl();
	virtual ~CGeomTreeCtrl();

public:
	HTREEITEM SelectTreeItem(CPoint point, bool bSelect);
	void SelectTreeItem(HTREEITEM hItem, bool bSelect);

	void DrawTree(CDC* pDC);
	void DrawRubberBandTrack(CDC* pDC);

	HTREEITEM GetFirstSelecedItem() const;
	HTREEITEM GetNextSelectedItem(HTREEITEM hItem) const;
	HTREEITEM GetLastSelectedItem() const;

	void ClearSelection(HTREEITEM hExceptItem = NULL);

private:
	CPoint m_StartPoint;
	CPoint m_PreviousPoint;
	CPoint m_CurrentPoint;
	TrackState m_TrackState;
	std::vector<HTREEITEM> m_SelectedItemList;

protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnTvnItemChanged(NMHDR *pNMHDR, LRESULT *pResult);
};


