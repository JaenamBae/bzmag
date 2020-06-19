// GeomPropertyGridCtrl.cpp : ���� �����Դϴ�.
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
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if (node == 0) return;

	CMFCPropertyGridProperty* pProp1 = FindItemByData(1);	// �̸�
	CMFCPropertyGridProperty* pProp2 = FindItemByData(2);	// ������
	CMFCPropertyGridColorProperty* pProp31 = (CMFCPropertyGridColorProperty*)FindItemByData(31);	// ����
	CPropGridSlider* pProp32 = (CPropGridSlider*)FindItemByData(32);	// ����
	CMFCPropertyGridProperty* pProp4 = FindItemByData(4);	// ����
	//CMFCPropertyGridProperty* pProp5 = FindItemByData(5);	// ��ǥ��

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
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if (node == 0) return;

	CMFCPropertyGridProperty* pProp1 = FindItemByData(2);
	CMFCPropertyGridProperty* pProp2 = FindItemByData(4);
	CMFCPropertyGridProperty* pProp3 = FindItemByData(5);

	// �޽��� ó��
	String radius, cx, cy;
	radius = V_BSTRT(&pProp1->GetValue());
	cx = V_BSTRT(&pProp2->GetValue());
	cy = V_BSTRT(&pProp3->GetValue());

	node->setParameters(cx, cy, radius);
}

void CGeomPropertyGridCtrl::PropertyChanged(GeomRectNode* node) const
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if (node == 0) return;

	CMFCPropertyGridProperty* pProp1 = FindItemByData(3);
	CMFCPropertyGridProperty* pProp2 = FindItemByData(4);
	CMFCPropertyGridProperty* pProp3 = FindItemByData(6);
	CMFCPropertyGridProperty* pProp4 = FindItemByData(7);

	// �޽��� ó��
	String sx, sy, ex, ey;
	sx = V_BSTRT(&pProp1->GetValue());
	sy = V_BSTRT(&pProp2->GetValue());
	ex = V_BSTRT(&pProp3->GetValue());
	ey = V_BSTRT(&pProp4->GetValue());

	node->setParameters(sx, sy, ex, ey);
}

void CGeomPropertyGridCtrl::PropertyChanged(GeomRotateNode* node) const
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if (node == 0) return;

	CMFCPropertyGridProperty* pProp1 = FindItemByData(2);

	// �޽��� ó��
	String sangle = V_BSTRT(&pProp1->GetValue());
	node->setParameters(sangle);
}

void CGeomPropertyGridCtrl::PropertyChanged(GeomMoveNode* node) const
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if (node == 0) return;

	CMFCPropertyGridProperty* pProp1 = FindItemByData(3);
	CMFCPropertyGridProperty* pProp2 = FindItemByData(4);

	// �޽��� ó��
	String sdx = V_BSTRT(&pProp1->GetValue());
	String sdy = V_BSTRT(&pProp1->GetValue());
	node->setParameters(sdx, sdy);
}


// CGeomPropertyGridCtrl �޽��� ó�����Դϴ�.
void CGeomPropertyGridCtrl::BindGeometryNode(GeomBaseNode* pGeom)
{
	// �Ӽ� ����� ��ü ������ ���� �����͸� ������ �д�
	m_pCurrGeom = pGeom;

	if( m_pCurrGeom == 0 )
		return;

	// Ÿ�Կ� ���� ���ε� ȭ���� �޶����� �Ѵ�
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
	CMFCPropertyGridProperty* name = new CMFCPropertyGridProperty( _T("�̸�"), CString(pGeom->getName().c_str()), NULL );
	name->SetData(1);
	AddProperty( name );

	CMFCPropertyGridProperty* material = new CMFCPropertyGridProperty( _T("����"), CString(pGeom->getMaterial().c_str()), NULL ); 
	material->SetData(2);
	AddProperty( material );

	CMFCPropertyGridProperty* RGBA = new CMFCPropertyGridProperty( _T("�÷�����") ); 
	RGBA->SetData(3);
	Color rgb = pGeom->getColor();
	COLORREF color;// (rgb.r_, rgb.g_, rgb.b_);
	CMFCPropertyGridColorProperty* rrggbb = new CMFCPropertyGridColorProperty( _T("����"), color, NULL );
	rrggbb->EnableOtherButton(_T("��Ÿ"));
	rrggbb->AllowEdit(TRUE);
	rrggbb->SetData(31);
	RGBA->AddSubItem( rrggbb );

	
	CPropGridSlider* alpha = new CPropGridSlider(_T("����"), (float)rgb.a_, _T("0~1(����:1, ������:0)"), 0.0f, 1.0f, 100 );
	alpha->SetData(32);
	RGBA->AddSubItem( alpha );
	AddProperty( RGBA );

	CMFCPropertyGridProperty* hide = new CMFCPropertyGridProperty( _T("����"), (_variant_t)(true), _T("ȭ�鿡�� ����") );
	hide->SetValue((_variant_t)(pGeom->isHide()));
	hide->SetData(4);
	AddProperty( hide );
}

void CGeomPropertyGridCtrl::BindGeometryNode(GeomCircleNode* pGeom)
{
	if( pGeom == 0 ) return;

	_TCHAR sBuff[512];

	CString command(pGeom->description().c_str());
	CMFCPropertyGridProperty* pCommand = new CMFCPropertyGridProperty( _T("���"), command, command);
	pCommand->AllowEdit( FALSE );
	pCommand->SetData(1);
	AddProperty( pCommand );

	_stprintf_s( sBuff, _T("%f"), pGeom->getRadius() );
	CString radius(pGeom->getRadiusasString().c_str());
	CMFCPropertyGridProperty* pRadius = new CMFCPropertyGridProperty( _T("�ݰ�"), radius, sBuff );
	pRadius->SetData(2);
	AddProperty( pRadius );


	Vector2 cc = pGeom->getCenter();
	_stprintf_s( sBuff, _T("%f, %f"), cc.x_, cc.y_ );
	CMFCPropertyGridProperty* pCenter = new CMFCPropertyGridProperty( _T("�߽�"), 0, TRUE );
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

	// ���� ��ǥ��
	bzmag::Kernel* kernel = bzmag::Kernel::instance();
	Node* cs_root = kernel->lookup("/coord");

	///
//	_stprintf_s(sBuff, _T("%f"), pGeom->get);
//	CString radius(pGeom->getRadiusasString().c_str());
//	CMFCPropertyGridProperty* pCS = new CMFCPropertyGridProperty(_T("������ǥ��"), radius, sBuff);
//	pCS->SetData(6);
//	AddProperty(pCS);
}

void CGeomPropertyGridCtrl::BindGeometryNode(GeomRectNode* pGeom)
{
	if (pGeom == 0) return;

	_TCHAR sBuff[512];

	CString command(pGeom->description().c_str());
	CMFCPropertyGridProperty* pCommand = new CMFCPropertyGridProperty(_T("���"), command, command);
	pCommand->AllowEdit(FALSE);
	pCommand->SetData(1);
	AddProperty(pCommand);

	Vector2 lefttop(pGeom->getLeft(), pGeom->getTop());
	_stprintf_s(sBuff, _T("%f, %f"), lefttop.x_, lefttop.y_);
	CMFCPropertyGridProperty* pStartPt = new CMFCPropertyGridProperty(_T("������"), 0, TRUE);
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
	CMFCPropertyGridProperty* pEndPt = new CMFCPropertyGridProperty(_T("����"), 0, TRUE);
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
	CMFCPropertyGridProperty* pCommand = new CMFCPropertyGridProperty(_T("���"), command, command);
	pCommand->AllowEdit(FALSE);
	pCommand->SetData(1);
	AddProperty(pCommand);

	_stprintf_s(sBuff, _T("%f"), pGeom->getAngle());
	CString angle(pGeom->getAngleAsString().c_str());
	CMFCPropertyGridProperty* pAngle = new CMFCPropertyGridProperty(_T("ȸ����"), angle, sBuff);
	pAngle->SetData(2);
	AddProperty(pAngle);
}

void CGeomPropertyGridCtrl::BindGeometryNode(GeomMoveNode* pGeom)
{
	if (pGeom == 0) return;

	_TCHAR sBuff[512];

	CString command(pGeom->description().c_str());
	CMFCPropertyGridProperty* pCommand = new CMFCPropertyGridProperty(_T("���"), command, command);
	pCommand->AllowEdit(FALSE);
	pCommand->SetData(1);
	AddProperty(pCommand);

	_stprintf_s(sBuff, _T("%f, %f"), pGeom->get_dx(), pGeom->get_dy());
	CMFCPropertyGridProperty* pCenter = new CMFCPropertyGridProperty(_T("����"), 0, TRUE);
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
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	CGeomPropertyGridCtrl *ptr = const_cast<CGeomPropertyGridCtrl*>(this);
	ptr->m_bChanged = true;
	ptr->ApplyChangedItems();

	return CMFCPropertyGridCtrl::OnPropertyChanged(pProp);
}

// �Ӽ����氪�� ������ OnPropertyChanged �Լ����� ó���� �� ������, 
// ������Ƽ�� ���� �� ���� ��û���� ���� ������ �Ͼ�� ������ 
// Enter Key�� �������� �����Ѵ�
// WM_KEYDOWN �̺�Ʈ���� Enter Key�� �����ϴ� ������ �ľǵȴ�
// �׷��� PreTranslateMessage���� �����Ѵ�
// Last Updated : 2014.09.25

//BOOL CGeomPropertyGridCtrl::PreTranslateMessage(MSG* pMsg)
//{
//	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
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
//	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
//	if (nChar == VK_RETURN) {
//		ApplyChangedItems();
//		RemoveAll();
//		BindGeometryNode(m_pCurrGeom);
//	}
//
//	CMFCPropertyGridCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
//}
