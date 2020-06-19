////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "PropGridSlider.h"
////////////////////////////////////////////////////////////////////////////////////


BEGIN_MESSAGE_MAP(CPropSlider, CSliderCtrl)
	ON_NOTIFY_REFLECT(NM_RELEASEDCAPTURE, &CPropSlider::OnNMReleasedcapture)
	ON_WM_SETCURSOR()
END_MESSAGE_MAP()


void CPropSlider::OnNMReleasedcapture(NMHDR *pNMHDR, LRESULT *pResult)
{
	OnUpdateValue();
	*pResult = 0;
}

BOOL CPropSlider::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	switch (pMsg->message) {
		case WM_MOUSEMOVE:
			if (pMsg->wParam == 1) OnUpdateValue();
			break;
		case WM_MOUSEWHEEL:
			OnUpdateValue();
			break;
	}

	return CSliderCtrl::PreTranslateMessage(pMsg);
}

void CPropSlider::OnUpdateValue()
{
	int pos = GetPos();
	if (pos != m_iPrevPos) {
		m_iPrevPos = pos;
		m_pParentProp->OnSliderPosChanged();
	}
}

void CPropGridSlider::OnSliderPosChanged()
{
	int pos = m_pSlider->GetPos();
	float ratio = ((float)pos) / ((float)m_iSliderStep);
	float value = (m_fMax - m_fMin) * ratio + m_fMin;
	SetValue((float)value);
	m_pWndList->OnPropertyChanged(this);
}

void CPropGridSlider::SetSliderPos()
{
	COleVariant value = GetValue();
	float fval = value.fltVal;
	float ratio = (fval - m_fMin) / (m_fMax - m_fMin);
	int pos = (int)(ratio * ((float)m_iSliderStep));

	m_pSlider->SetPos(pos);
}

BOOL CPropGridSlider::OnEndEdit() 
{
	SetSliderPos();
	return CMFCPropertyGridProperty::OnEndEdit();
}

void CPropGridSlider::OnDrawValue(CDC* pDC, CRect rect)
{
	CRect rt = rect;
	int w = rect.Width();

	rect.right = rect.left 
		+ (m_iEditCtrlWidth < 0 ? w / (-m_iEditCtrlWidth) : m_iEditCtrlWidth);
	rt.left = rect.right;

	if (!m_pSlider) {
		m_pSlider = new CPropSlider(this);
		DWORD style = WS_VISIBLE | WS_CHILD | TBS_BOTH | TBS_NOTICKS;
		m_pSlider->Create(style, rt, m_pWndList, 5);
		m_pSlider->SetRange(0, m_iSliderStep);
		m_pSlider->ShowWindow(SW_SHOW);
		SetSliderPos();
	}
	
	m_pSlider->MoveWindow(&rt);
	CMFCPropertyGridProperty::OnDrawValue(pDC, rect);
}

CWnd* CPropGridSlider::CreateInPlaceEdit(CRect rectEdit, BOOL& bDefaultFormat)
{
	int w = rectEdit.Width();
	rectEdit.right = rectEdit.left 
		+ (m_iEditCtrlWidth < 0 ? w / (-m_iEditCtrlWidth) : m_iEditCtrlWidth);

	return CMFCPropertyGridProperty::CreateInPlaceEdit(rectEdit, bDefaultFormat);
}


BOOL CPropSlider::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	::SetCursor(::LoadCursor(NULL, IDC_ARROW));
	return TRUE;
	//return CSliderCtrl::OnSetCursor(pWnd, nHitTest, message);
}


//void CPropSlider::OnPaint()
//{
//	CPaintDC dc(this); // device context for painting
//	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
//	// 그리기 메시지에 대해서는 CSliderCtrl::OnPaint()을(를) 호출하지 마십시오.
//	RedrawWindow();
//}
