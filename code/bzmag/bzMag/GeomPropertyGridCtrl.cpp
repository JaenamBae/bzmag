// GeomPropertyGridCtrl.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "bzMag.h"
#include "GeomPropertyGridCtrl.h"
#include "MainFrm.h"
#include "bzMagView.h"
#include "PropGridSlider.h"


// CGeomPropertyGridCtrl

IMPLEMENT_DYNAMIC(CGeomPropertyGridCtrl, CMFCPropertyGridCtrl)

	CGeomPropertyGridCtrl::CGeomPropertyGridCtrl()
{
	m_pCurrGeom = 0;
	m_bChanged = false;
	m_bReady = false;
}

CGeomPropertyGridCtrl::~CGeomPropertyGridCtrl()
{
}


BEGIN_MESSAGE_MAP(CGeomPropertyGridCtrl, CMFCPropertyGridCtrl)

//	ON_WM_KEYDOWN()
END_MESSAGE_MAP()


void CGeomPropertyGridCtrl::PropertyChanged(GeomHeadNode* node) const
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (node == 0) return;

	CMFCPropertyGridProperty* pProp1 = FindItemByData(1);	// 이름
	CMFCPropertyGridProperty* pProp2 = FindItemByData(2);	// 재질명
	CMFCPropertyGridColorProperty* pProp31 = (CMFCPropertyGridColorProperty*)FindItemByData(31);	// 색상
	CPropGridSlider* pProp32 = (CPropGridSlider*)FindItemByData(32);	// 투명도
	CMFCPropertyGridProperty* pProp4 = FindItemByData(4);	// 숨김
	//CMFCPropertyGridProperty* pProp5 = FindItemByData(5);	// 좌표계

	String name     = V_BSTRT( &pProp1->GetValue() );
	String material = V_BSTRT( &pProp2->GetValue() );

	COLORREF rgb    = pProp31->GetColor();
	int r = ((BYTE)(rgb));
	int g = ((BYTE)(rgb>>8));
	int b = ((BYTE)(rgb>>16));
	int alpha = (int)(pProp32->GetValue().fltVal) * 250;
	node->setColor(Color(r,g,b, alpha));

	COleVariant oleSelect = pProp4->GetValue();
	BOOL bHide = (BOOL)(oleSelect.Detach().boolVal);
	node->setName(name);
	node->setHideStatus(bHide);
}

void CGeomPropertyGridCtrl::PropertyChanged(GeomCircleNode* node) const
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (node == 0) return;

	CMFCPropertyGridProperty* pProp1 = FindItemByData(2);
	CMFCPropertyGridProperty* pProp2 = FindItemByData(4);
	CMFCPropertyGridProperty* pProp3 = FindItemByData(5);

	// 메시지 처리
	String radius, cx, cy;
	radius = V_BSTRT(&pProp1->GetValue());
	cx = V_BSTRT(&pProp2->GetValue());
	cy = V_BSTRT(&pProp3->GetValue());

	node->setParameters(cx, cy, radius);
}

void CGeomPropertyGridCtrl::PropertyChanged(GeomRectNode* node) const
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (node == 0) return;

	CMFCPropertyGridProperty* pProp1 = FindItemByData(3);
	CMFCPropertyGridProperty* pProp2 = FindItemByData(4);
	CMFCPropertyGridProperty* pProp3 = FindItemByData(6);
	CMFCPropertyGridProperty* pProp4 = FindItemByData(7);

	// 메시지 처리
	String sx, sy, ex, ey;
	sx = V_BSTRT(&pProp1->GetValue());
	sy = V_BSTRT(&pProp2->GetValue());
	ex = V_BSTRT(&pProp3->GetValue());
	ey = V_BSTRT(&pProp4->GetValue());

	node->setParameters(sx, sy, ex, ey);
}

void CGeomPropertyGridCtrl::PropertyChanged(GeomRotateNode* node) const
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (node == 0) return;

	CMFCPropertyGridProperty* pProp1 = FindItemByData(2);

	// 메시지 처리
	String sangle = V_BSTRT(&pProp1->GetValue());
	node->setParameters(sangle);
}

void CGeomPropertyGridCtrl::PropertyChanged(GeomMoveNode* node) const
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (node == 0) return;

	CMFCPropertyGridProperty* pProp1 = FindItemByData(3);
	CMFCPropertyGridProperty* pProp2 = FindItemByData(4);

	// 메시지 처리
	String sdx = V_BSTRT(&pProp1->GetValue());
	String sdy = V_BSTRT(&pProp1->GetValue());
	node->setParameters(sdx, sdy);
}


// CGeomPropertyGridCtrl 메시지 처리기입니다.
void CGeomPropertyGridCtrl::BindGeometryNode(GeomBaseNode* pGeom)
{
	// 속성 변경시 개체 참조를 위해 포인터를 저장해 둔다
	m_pCurrGeom = pGeom;

	if( m_pCurrGeom == 0 )
		return;

	// 타입에 따라 바인딩 화면이 달라져야 한다
	GeomHeadNode* headnode = dynamic_cast<GeomHeadNode*>(pGeom);
	if (headnode) {
		BindGeometryNode(headnode);
	}

	GeomCircleNode* circlenode = dynamic_cast<GeomCircleNode*>(pGeom);
	if (circlenode) {
		BindGeometryNode(circlenode);
	}

	GeomRectNode* rectenode = dynamic_cast<GeomRectNode*>(pGeom);
	if (rectenode) {
		BindGeometryNode(rectenode);
	}

	GeomRotateNode* rotatenode = dynamic_cast<GeomRotateNode*>(pGeom);
	if (rotatenode) {
		BindGeometryNode(rotatenode);
	}

	GeomMoveNode* movenode = dynamic_cast<GeomMoveNode*>(pGeom);
	if (movenode) {
		BindGeometryNode(movenode);
	}


	ExpandAll();
}


void CGeomPropertyGridCtrl::BindGeometryNode(GeomHeadNode* pGeom)
{
	CMFCPropertyGridProperty* name = new CMFCPropertyGridProperty( _T("이름"), CString(pGeom->getName().c_str()), NULL );
	name->SetData(1);
	AddProperty( name );

	CMFCPropertyGridProperty* material = new CMFCPropertyGridProperty( _T("재질"), CString(pGeom->getMaterial().c_str()), NULL ); 
	material->SetData(2);
	AddProperty( material );

	CMFCPropertyGridProperty* RGBA = new CMFCPropertyGridProperty( _T("컬러설정") ); 
	RGBA->SetData(3);
	Color rgb = pGeom->getColor();
	COLORREF color;// (rgb.r_, rgb.g_, rgb.b_);
	CMFCPropertyGridColorProperty* rrggbb = new CMFCPropertyGridColorProperty( _T("색상"), color, NULL );
	rrggbb->EnableOtherButton(_T("기타"));
	rrggbb->AllowEdit(TRUE);
	rrggbb->SetData(31);
	RGBA->AddSubItem( rrggbb );

	
	CPropGridSlider* alpha = new CPropGridSlider(_T("투명도"), (float)rgb.a_, _T("0~1(투명:1, 불투명:0)"), 0.0f, 1.0f, 100 );
	alpha->SetData(32);
	RGBA->AddSubItem( alpha );
	AddProperty( RGBA );

	CMFCPropertyGridProperty* hide = new CMFCPropertyGridProperty( _T("숨김"), (_variant_t)(true), _T("화면에서 숨김") );
	hide->SetValue((_variant_t)(pGeom->isHide()));
	hide->SetData(4);
	AddProperty( hide );
}

void CGeomPropertyGridCtrl::BindGeometryNode(GeomCircleNode* pGeom)
{
	if( pGeom == 0 ) return;

	_TCHAR sBuff[512];

	CString command(pGeom->description().c_str());
	CMFCPropertyGridProperty* pCommand = new CMFCPropertyGridProperty( _T("명령"), command, command);
	pCommand->AllowEdit( FALSE );
	pCommand->SetData(1);
	AddProperty( pCommand );

	_stprintf_s( sBuff, _T("%f"), pGeom->getRadius() );
	CString radius(pGeom->getRadiusasString().c_str());
	CMFCPropertyGridProperty* pRadius = new CMFCPropertyGridProperty( _T("반경"), radius, sBuff );
	pRadius->SetData(2);
	AddProperty( pRadius );


	Vector2 cc = pGeom->getCenter();
	_stprintf_s( sBuff, _T("%f, %f"), cc.x_, cc.y_ );
	CMFCPropertyGridProperty* pCenter = new CMFCPropertyGridProperty( _T("중심"), 0, TRUE );
	pCenter->SetData(3);
	AddProperty( pCenter );

	_stprintf_s( sBuff, _T("%f"), cc.x_ );
	CString cx(pGeom->getCenterXasString().c_str());
	CMFCPropertyGridProperty* pSubProp = new CMFCPropertyGridProperty( _T("X"), cx, sBuff );
	pSubProp->SetData(4);
	pCenter->AddSubItem( pSubProp );

	_stprintf_s( sBuff, _T("%f"), cc.y_ );
	CString cy(pGeom->getCenterYasString().c_str());
	pSubProp = new CMFCPropertyGridProperty( _T("Y"), cy, sBuff );
	pSubProp->SetData(5);
	pCenter->AddSubItem( pSubProp );

	// 참조 좌표계
	bzmag::Kernel* kernel = bzmag::Kernel::instance();
	Node* cs_root = kernel->lookup("/coord");

	///
//	_stprintf_s(sBuff, _T("%f"), pGeom->get);
//	CString radius(pGeom->getRadiusasString().c_str());
//	CMFCPropertyGridProperty* pCS = new CMFCPropertyGridProperty(_T("기준좌표계"), radius, sBuff);
//	pCS->SetData(6);
//	AddProperty(pCS);
}

void CGeomPropertyGridCtrl::BindGeometryNode(GeomRectNode* pGeom)
{
	if (pGeom == 0) return;

	_TCHAR sBuff[512];

	CString command(pGeom->description().c_str());
	CMFCPropertyGridProperty* pCommand = new CMFCPropertyGridProperty(_T("명령"), command, command);
	pCommand->AllowEdit(FALSE);
	pCommand->SetData(1);
	AddProperty(pCommand);

	Vector2 lefttop(pGeom->getLeft(), pGeom->getTop());
	_stprintf_s(sBuff, _T("%f, %f"), lefttop.x_, lefttop.y_);
	CMFCPropertyGridProperty* pStartPt = new CMFCPropertyGridProperty(_T("시작점"), 0, TRUE);
	pStartPt->SetData(2);
	AddProperty(pStartPt);

	_stprintf_s(sBuff, _T("%f"), lefttop.x_);
	CString sx(pGeom->getLeftasString().c_str());
	CMFCPropertyGridProperty* pSubProp = new CMFCPropertyGridProperty(_T("X"), sx, sBuff);
	pSubProp->SetData(3);
	pStartPt->AddSubItem(pSubProp);

	_stprintf_s(sBuff, _T("%f"), lefttop.y_);
	CString sy(pGeom->getTopasString().c_str());
	pSubProp = new CMFCPropertyGridProperty(_T("Y"), sy, sBuff);
	pSubProp->SetData(4);
	pStartPt->AddSubItem(pSubProp);


	Vector2 rightbottom(pGeom->getRight(), pGeom->getBottom());
	_stprintf_s(sBuff, _T("%f, %f"), rightbottom.x_, rightbottom.y_);
	CMFCPropertyGridProperty* pEndPt = new CMFCPropertyGridProperty(_T("끝점"), 0, TRUE);
	pEndPt->SetData(5);
	AddProperty(pEndPt);

	_stprintf_s(sBuff, _T("%f"), rightbottom.x_);
	CString ex(pGeom->getRightasString().c_str());
	pSubProp = new CMFCPropertyGridProperty(_T("X"), ex, sBuff);
	pSubProp->SetData(6);
	pEndPt->AddSubItem(pSubProp);

	_stprintf_s(sBuff, _T("%f"), rightbottom.y_);
	CString ey(pGeom->getBottomasString().c_str());
	pSubProp = new CMFCPropertyGridProperty(_T("Y"), ey, sBuff);
	pSubProp->SetData(7);
	pEndPt->AddSubItem(pSubProp);
}

void CGeomPropertyGridCtrl::BindGeometryNode(GeomRotateNode* pGeom)
{
	if (pGeom == 0) return;

	_TCHAR sBuff[512];

	CString command(pGeom->description().c_str());
	CMFCPropertyGridProperty* pCommand = new CMFCPropertyGridProperty(_T("명령"), command, command);
	pCommand->AllowEdit(FALSE);
	pCommand->SetData(1);
	AddProperty(pCommand);

	_stprintf_s(sBuff, _T("%f"), pGeom->getAngle());
	CString angle(pGeom->getAngleAsString().c_str());
	CMFCPropertyGridProperty* pAngle = new CMFCPropertyGridProperty(_T("회전각"), angle, sBuff);
	pAngle->SetData(2);
	AddProperty(pAngle);
}

void CGeomPropertyGridCtrl::BindGeometryNode(GeomMoveNode* pGeom)
{
	if (pGeom == 0) return;

	_TCHAR sBuff[512];

	CString command(pGeom->description().c_str());
	CMFCPropertyGridProperty* pCommand = new CMFCPropertyGridProperty(_T("명령"), command, command);
	pCommand->AllowEdit(FALSE);
	pCommand->SetData(1);
	AddProperty(pCommand);

	_stprintf_s(sBuff, _T("%f, %f"), pGeom->get_dx(), pGeom->get_dy());
	CMFCPropertyGridProperty* pCenter = new CMFCPropertyGridProperty(_T("변위"), 0, TRUE);
	pCenter->SetData(2);
	AddProperty(pCenter);

	_stprintf_s(sBuff, _T("%f"), pGeom->get_dx());
	CString dx(pGeom->get_dx_asString().c_str());
	CMFCPropertyGridProperty* pSubProp = new CMFCPropertyGridProperty(_T("dx"), dx, sBuff);
	pSubProp->SetData(3);
	pCenter->AddSubItem(pSubProp);

	_stprintf_s(sBuff, _T("%f"), pGeom->get_dy());
	CString dy(pGeom->get_dy_asString().c_str());
	pSubProp = new CMFCPropertyGridProperty(_T("dy"), dy, sBuff);
	pSubProp->SetData(4);
	pCenter->AddSubItem(pSubProp);
}


void CGeomPropertyGridCtrl::BindGeometryNode(GeomCoverLineNode* pGeom)
{

}

void CGeomPropertyGridCtrl::BindGeometryNode(GeomBooleanNode* pGeom)
{

}


void CGeomPropertyGridCtrl::BindGeometryNode(GeomCloneToNode* pGeom)
{

}


void CGeomPropertyGridCtrl::BindGeometryNode(GeomCloneFromNode* pGeom)
{

}





void CGeomPropertyGridCtrl::ApplyChangedItems()
{
	if (!m_bChanged) return;

	GeomHeadNode* headnode = dynamic_cast<GeomHeadNode*>(m_pCurrGeom);
	if (headnode) PropertyChanged(headnode);

	GeomCircleNode* circle = dynamic_cast<GeomCircleNode*>(m_pCurrGeom);
	if (circle) PropertyChanged(circle);

	GeomRectNode* rect = dynamic_cast<GeomRectNode*>(m_pCurrGeom);
	if (rect) PropertyChanged(rect);

	GeomRotateNode* rotate = dynamic_cast<GeomRotateNode*>(m_pCurrGeom);
	if (rotate) PropertyChanged(rotate);

	GeomMoveNode* move = dynamic_cast<GeomMoveNode*>(m_pCurrGeom);
	if (move) PropertyChanged(move);


	MainFrame* pFrame = (MainFrame*)AfxGetMainWnd();
	CView* pView = 0;
	if (pFrame) pView = pFrame->GetActiveView();
	if (pView) 
	{
		bzMagView* pModelerView = (bzMagView*)(pView->GetParentSplitter(pView, FALSE)->GetPane(0, 1));
		if (pModelerView) pModelerView->Invalidate();
	}
}


void CGeomPropertyGridCtrl::OnPropertyChanged(CMFCPropertyGridProperty* pProp) const
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	CGeomPropertyGridCtrl *ptr = const_cast<CGeomPropertyGridCtrl*>(this);
	ptr->m_bChanged = true;
	ptr->ApplyChangedItems();

	return CMFCPropertyGridCtrl::OnPropertyChanged(pProp);
}

// 속성변경값의 적용은 OnPropertyChanged 함수에서 처리할 수 있지만, 
// 프로퍼티가 변할 때 마다 엄청나게 많은 연산이 일어나기 때문에 
// Enter Key가 눌러질때 적용한다
// WM_KEYDOWN 이벤트에서 Enter Key는 무시하는 것으로 파악된다
// 그래서 PreTranslateMessage에서 구현한다
// Last Updated : 2014.09.25

//BOOL CGeomPropertyGridCtrl::PreTranslateMessage(MSG* pMsg)
//{
//	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
//	int bEnter = false;
//	if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_RETURN)) {
//		bEnter = true;
//	}  
//
//	BOOL ret = CMFCPropertyGridCtrl::PreTranslateMessage(pMsg);
//	if (bEnter) {
//		ApplyChangedItems();
//		RemoveAll();
//		BindGeometryNode(m_pCurrGeom);
//	}
//
//	return ret;
//}


//void CGeomPropertyGridCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
//{
//	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
//	if (nChar == VK_RETURN) {
//		ApplyChangedItems();
//		RemoveAll();
//		BindGeometryNode(m_pCurrGeom);
//	}
//
//	CMFCPropertyGridCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
//}
