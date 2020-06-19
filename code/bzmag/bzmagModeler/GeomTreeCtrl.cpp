// GeomTreeCtrl.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "GeomTreeCtrl.h"
#include "BufferDC.h"


// CGeomTreeCtrl
CGeomTreeCtrl::CGeomTreeCtrl() : m_TrackState(TrackNothing)
{
	
}

CGeomTreeCtrl::~CGeomTreeCtrl()
{
	
}

// CGeomTreeCtrl 메시지 처리기입니다.
BEGIN_MESSAGE_MAP(CGeomTreeCtrl, CTreeCtrl)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_KILLFOCUS()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_KEYDOWN()
	ON_NOTIFY_REFLECT_EX(TVN_ITEMCHANGED, &CGeomTreeCtrl::OnTvnItemChanged)
END_MESSAGE_MAP()

HTREEITEM CGeomTreeCtrl::SelectTreeItem(CPoint point, bool bSelect)
{
	UINT uFlags;
	HTREEITEM hItem = HitTest(point, &uFlags);
	if( hItem != NULL && (uFlags & TVHT_ONITEM) )
	{
		SelectTreeItem(hItem, bSelect);
		return hItem;
	}
	else
		return NULL;
}

void CGeomTreeCtrl::SelectTreeItem(HTREEITEM hItem, bool bSelect)
{
	if( hItem == NULL ) return ;

	bool bCtrl = (::GetKeyState(VK_CONTROL) & 0x8000) != 0;
	UINT nState = GetItemState(hItem, TVIS_SELECTED) & TVIS_SELECTED;

	if( bSelect ) {
		if( bCtrl ) {
			if( nState == TVIS_SELECTED ) {
				SetItemState(hItem, ~TVIS_SELECTED, TVIS_SELECTED);
			}
			else {
				SetItemState(hItem, TVIS_SELECTED, TVIS_SELECTED);
			}
		}
		else {
			ClearSelection();
			SetItemState(hItem, TVIS_SELECTED, TVIS_SELECTED);
		}
	}
	else {
		SetItemState(hItem, ~TVIS_SELECTED, TVIS_SELECTED);
	}
}

void CGeomTreeCtrl::DrawTree(CDC* pDC)
{
	CTreeCtrl::DefWindowProc( WM_PAINT, (WPARAM)pDC->GetSafeHdc(), 0 );
}

void CGeomTreeCtrl::DrawRubberBandTrack(CDC* pDC)
{
	if( m_TrackState == TrackContinue)
	{
		pDC->MoveTo( m_StartPoint );
		pDC->LineTo( m_StartPoint.x, m_CurrentPoint.y );
		pDC->LineTo( m_CurrentPoint.x, m_CurrentPoint.y );
		pDC->LineTo( m_CurrentPoint.x, m_StartPoint.y );
		pDC->LineTo( m_StartPoint );
	}
}


HTREEITEM CGeomTreeCtrl::GetFirstSelecedItem() const
{
	if( m_SelectedItemList.size() > 0 )
		return *(m_SelectedItemList.begin());
	else
		return NULL;
}

HTREEITEM CGeomTreeCtrl::GetNextSelectedItem(HTREEITEM hItem) const
{
	std::vector<HTREEITEM>::const_iterator it;
	it = std::find(m_SelectedItemList.begin(), m_SelectedItemList.end(), hItem);
	if( it != m_SelectedItemList.end() && ++it != m_SelectedItemList.end() )
		return *it;
	else
		return NULL;
}

HTREEITEM CGeomTreeCtrl::GetLastSelectedItem() const
{
	if( m_SelectedItemList.size() > 0 )
		return *(m_SelectedItemList.rbegin());
	else
		return NULL;
}

void CGeomTreeCtrl::ClearSelection(HTREEITEM hExceptItem/* = NULL*/)
{
	std::vector<HTREEITEM>::iterator it;
	for( it = m_SelectedItemList.begin(); it != m_SelectedItemList.end(); )
	{
		HTREEITEM hItem = *it;
		if( hItem != hExceptItem )
		{
			it = m_SelectedItemList.erase(it);
			SetItemState( hItem, ~TVIS_SELECTED, TVIS_SELECTED );
		}
		else ++it;
	}
}


BOOL CGeomTreeCtrl::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	return TRUE;
}


void CGeomTreeCtrl::OnPaint()
{
	PAINTSTRUCT ps;
	CDC* pDC = BeginPaint(&ps);

	CDC* pMemDC = new CBufferDC(pDC);

	DrawTree(pMemDC);
	DrawRubberBandTrack(pMemDC);

	delete pMemDC;
	EndPaint(&ps);
}


void CGeomTreeCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	HTREEITEM hItem = SelectTreeItem( point, true );
	if( hItem == NULL )
	{
		ClearSelection();

		//ASSERT(m_TrackState == TrackNothing);
		if( m_TrackState == TrackNothing )
		{
			SetCapture();
			VERIFY( ::GetCapture() );

			m_StartPoint    = point;
			m_PreviousPoint = point;

			m_TrackState = TrackReady;
			return;
		}
	}

	//CTreeCtrl::OnLButtonDown(nFlags, point);
}


void CGeomTreeCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if( m_TrackState == TrackContinue || m_TrackState == TrackReady )
	{
		m_TrackState = TrackNothing;
		VERIFY( ::ReleaseCapture() );

		m_StartPoint.SetPoint(0, 0);
		m_PreviousPoint.SetPoint(0, 0);
		m_CurrentPoint.SetPoint(0, 0);

		Invalidate(FALSE);
		return;
	}

	//CTreeCtrl::OnLButtonUp(nFlags, point);
}


void CGeomTreeCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if( m_TrackState == TrackReady || m_TrackState == TrackContinue )
	{
		m_TrackState = TrackContinue;
		m_CurrentPoint = point;

		if( point.y == m_PreviousPoint.y ) {
			m_PreviousPoint = point;
			return;
		}

		//Rubber Band에 Item에 들어오고 있음.
		if( abs(point.y - m_StartPoint.y) > abs(m_PreviousPoint.y - m_StartPoint.y) )
		{
			CRect TrackRect( m_StartPoint.x, m_StartPoint.y, point.x, point.y );
			TrackRect.NormalizeRect();

			for( HTREEITEM hItem = GetFirstVisibleItem(); hItem != NULL; hItem = GetNextVisibleItem(hItem) )
			{
				CRect ItemRect;
				GetItemRect( hItem, &ItemRect, FALSE );

				if( ((point.y > m_StartPoint.y) && (ItemRect.top < TrackRect.bottom) && (ItemRect.bottom > TrackRect.top)) ||
					((point.y < m_StartPoint.y) && (ItemRect.bottom > TrackRect.top) && (ItemRect.top < TrackRect.bottom)) )
				{
					SetItemState( hItem, TVIS_SELECTED, TVIS_SELECTED );
				}
				else {
					SetItemState( hItem, ~TVIS_SELECTED, TVIS_SELECTED );
				}
			}
		}

		//Rubber Band에 Item에 나가고 있음.
		else
		{
			CRect DeltaRect( m_PreviousPoint.x, m_PreviousPoint.y, point.x, point.y );
			DeltaRect.NormalizeRect();

			for( HTREEITEM hItem = GetFirstVisibleItem(); hItem != NULL; hItem = GetNextVisibleItem(hItem) )
			{
				CRect ItemRect;
				GetItemRect( hItem, &ItemRect, FALSE );
				if( ((point.y > m_PreviousPoint.y) && (ItemRect.bottom < DeltaRect.top)) ||
					((point.y < m_PreviousPoint.y) && (ItemRect.top > DeltaRect.bottom)) )
				{
					SetItemState( hItem, ~TVIS_SELECTED, TVIS_SELECTED );
				}
			}
		}

		m_PreviousPoint = point;
		Invalidate(FALSE);

		return;
	}

	//CTreeCtrl::OnMouseMove(nFlags, point);
}

void CGeomTreeCtrl::OnKillFocus(CWnd* pNewWnd)
{
	CTreeCtrl::OnKillFocus(pNewWnd);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	Invalidate(FALSE);
}


BOOL CGeomTreeCtrl::OnTvnItemChanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMTVITEMCHANGE *pNMTVItemChange = reinterpret_cast<NMTVITEMCHANGE*>(pNMHDR);
	HTREEITEM hItem = pNMTVItemChange->hItem;

	UINT nSelect = GetItemState( hItem, TVIS_SELECTED ) & TVIS_SELECTED;
	std::vector<HTREEITEM>::iterator it = std::find( m_SelectedItemList.begin(), m_SelectedItemList.end(), hItem );
	if( nSelect == TVIS_SELECTED )
	{
		if( it == m_SelectedItemList.end() )
			m_SelectedItemList.push_back( hItem );
	}
	else
	{
		if( it != m_SelectedItemList.end() )
			m_SelectedItemList.erase( it );
	}

	*pResult = 0;

	return FALSE;
}


void CGeomTreeCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if( nChar == VK_DELETE )
	{
		for( HTREEITEM hItem = GetFirstSelecedItem(); hItem != NULL; hItem = GetNextSelectedItem(hItem) )
		{
		//	CGeomTreeView* pView = (CGeomTreeView*)GetParent();
		//	pView->DeleteGeomPartFromManager( hItem );
		}
	}

	CTreeCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
}
