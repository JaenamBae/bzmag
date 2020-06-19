
// bzMagDoc.cpp : bzMagDoc Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
#ifndef SHARED_HANDLERS
#include "bzMag.h"
#endif

#include "bzMagDoc.h"
#include "CreateCircleDlg.h"
#include "CreateRectangleDlg.h"
#include "DialogExpressions.h"

#include <propkey.h>
#include <memory>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// bzMagDoc

IMPLEMENT_DYNCREATE(bzMagDoc, CDocument)

BEGIN_MESSAGE_MAP(bzMagDoc, CDocument)
	ON_COMMAND(ID_FILE_NEW, &bzMagDoc::OnFileNew)
	ON_COMMAND(ID_MODELER_CIRCLE, &bzMagDoc::OnModelerCircle)
	ON_COMMAND(ID_MODELER_RECTANGLE, &bzMagDoc::OnModelerRectangle)
	ON_COMMAND(ID_MODELER_EXPR, &bzMagDoc::OnModelerExpr)
END_MESSAGE_MAP()


// bzMagDoc ����/�Ҹ�

bzMagDoc::bzMagDoc() : geom_root_("/geom"), coord_root_("/coord"), globalCS_(0), exprserver_(0)
{
	// TODO: ���⿡ ��ȸ�� ���� �ڵ带 �߰��մϴ�.
	USING_MODULE(Engine);
	USING_MODULE(WinGraphics);

	kernel_ = bzmag::Kernel::instance();
	exprserver_ = bzmag::engine::ExpressionServer::instance();

	globalCS_ = dynamic_cast<GeomCSNode*>(kernel_->create("GeomCSNode", coord_root_ + "/global"));
	globalCS_->setParameters("0", "0", "0");
// 
// 	Node* cs_root = kernel_->lookup("/coord/global");

	// Test
// 	Expression* expr1 = dynamic_cast<Expression*>(kernel_->create("Expression"));
// 	if (expr1->setKey("a")) {
// 		expr1->setExpression("100.5");
// 		expr1->setHidden(false);
// 		expressions_.push_back(expr1);
// 	}
// 
// 	Expression* expr2 = dynamic_cast<Expression*>(kernel_->create("Expression"));;
// 	if (expr2->setKey("b")) {
// 		expr2->setExpression("a");
// 		expr2->setHidden(false);
// 		expressions_.push_back(expr2);
// 	}
// 
// 	Expression* expr3 = dynamic_cast<Expression*>(kernel_->create("Expression"));
// 	if (expr3->setKey("c")) {
// 		expr3->setExpression("a+b");
// 		expr3->setHidden(false);
// 		expressions_.push_back(expr3);
// 	}

	//bool bb = expr3->deleteExpression();
}

bzMagDoc::~bzMagDoc()
{
	// �޸� ���� ���� �ſ� �߿���
	Clear();
	if (globalCS_) {
	//	globalCS_->detach();
	//	delete globalCS_;
	}
}

BOOL bzMagDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: ���⿡ ���ʱ�ȭ �ڵ带 �߰��մϴ�.
	// SDI ������ �� ������ �ٽ� ����մϴ�.

	return TRUE;
}




// bzMagDoc serialization

void bzMagDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	}
	else
	{
		// TODO: ���⿡ �ε� �ڵ带 �߰��մϴ�.
	}
}

#ifdef SHARED_HANDLERS

// ����� �׸��� �����մϴ�.
void bzMagDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// ������ �����͸� �׸����� �� �ڵ带 �����Ͻʽÿ�.
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// �˻� ó���⸦ �����մϴ�.
void bzMagDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// ������ �����Ϳ��� �˻� �������� �����մϴ�.
	// ������ �κ��� ";"�� ���еǾ�� �մϴ�.

	// ��: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void bzMagDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// bzMagDoc ����

#ifdef _DEBUG
void bzMagDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void bzMagDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// bzMagDoc ���
// void bzMagDoc::AddUserExpression(Expression* expr)
// {
// 	expressions_.push_back(expr);
// }
// 
// void bzMagDoc::DeleteUserExpression(Expression* expr)
// {
// 	expressions_.remove(expr);
// }

void bzMagDoc::Clear()
{
	CWnd* pFrame = AfxGetMainWnd();
//	CWnd* pProperty = pFrame->GetDlgItem(ID_VIEW_PROPERTIESWND);
//	pProperty->SendMessage( WM_GEOMPART, GEOMPART_SELECT, 0 );

	if( pFrame ) pFrame->SendMessageToDescendants( WM_GEOMPART, GEOM_NEW, 0 );
	UpdateAllViews(NULL);


	// ���� ���� �ʱ�ȭ
	Node* geom_root = kernel_->lookup(geom_root_);
	if (geom_root) geom_root->clearChildren<GeomBaseNode>();

	Node* cs_root = kernel_->lookup(coord_root_);
	if (cs_root) cs_root->clearChildren<GeomCSNode>();

	// ����� ���� ���� --> ���� �����ؾ� ��
//	expressions_.clear();

	globalCS_ = dynamic_cast<GeomCSNode*>(kernel_->create("GeomCSNode", coord_root_ + "/global"));
	globalCS_->setParameters("0", "0", "0");
}


void bzMagDoc::OnFileNew()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	// ���� ���� �ʱ�ȭ
	Clear();

	bzmag::Kernel* kernel = bzmag::Kernel::instance();
	Node* geom_root = kernel->lookup("/geom");

	// ��ǥ�� ����
	GeomCSNode* cs1 = dynamic_cast<GeomCSNode*>(kernel->create("GeomCSNode","/coord/global/cs1"));
	cs1->setParameters("100", "150", "_pi/8.0");

	GeomCSNode* cs2 = dynamic_cast<GeomCSNode*>(kernel->create("GeomCSNode","/coord/global/cs1/cs2"));
	cs2->setParameters("0", "50", "_pi/8.0");

	GeomCSNode* cs3 = dynamic_cast<GeomCSNode*>(kernel->create("GeomCSNode","/coord/global/cs3"));
	cs3->setParameters("200", "100", "_pi/4.0");


	// ��1 �׸���
	GeomHeadNode* h1 = dynamic_cast<GeomHeadNode*>(kernel->create("GeomHeadNode", "/geom/name1"));
	GeomCircleNode* c1 = dynamic_cast<GeomCircleNode*>(kernel->create("GeomCircleNode", "/geom/name1/circle"));
	c1->setParameters("0", "0", "10");
	kernel->create("GeomCoverLineNode", "/geom/name1/circle/coverline");

/*
	// ��2 �׸���
	GeomHeadNode* h2 = dynamic_cast<GeomHeadNode*>(kernel->create("GeomHeadNode", "/geom/name2"));
	GeomCircleNode* c2 = dynamic_cast<GeomCircleNode*>(kernel->create("GeomCircleNode", "/geom/name2/circle"));
	c2->setParameters("0", "0", "400");
	kernel->create("GeomCoverLineNode", "/geom/name2/circle/coverline");


*/
	// �簢��1 �׸���
	GeomHeadNode* h4 = dynamic_cast<GeomHeadNode*>(kernel->create("GeomHeadNode", "/geom/name4"));
	GeomRectNode* r1 = dynamic_cast<GeomRectNode*>(kernel->create("GeomRectNode", "/geom/name4/rectangle"));
	r1->setParameters("0", "0", "200", "100");
	kernel->create("GeomCoverLineNode", "/geom/name4/rectangle/coverline");
	GeomRotateNode* m1 = dynamic_cast<GeomRotateNode*>(kernel->create("GeomRotateNode", "/geom/name4/rectangle/coverline/rotate"));
	m1->setParameters("_pi/4.0");


/*
	// �Ҹ������� ���� �ڵ�
	GeomSubtractNode* sub = dynamic_cast<GeomSubtractNode*>(kernel->create("GeomSubtractNode", "/geom/name1/circle/coverline/object"));
	GeomBooleanNode::ToolNodes tools;
	tools.push_back(h4);
	sub->setToolNode(tools);

	// A' = A - B�� ���꿡 �־�, ����������� ����� A'�� �Ǹ�, B�� ���ӳ�尡 ��
	// ���� B�� �������� ��� Ȱ���ϰ� ���� ���, B�� ������ ���� ������ �����ϵ��� ��
	// �Ʒ� 4���� B�� ������ ���� �ڵ��̸� �ɼǻ�����
	// �Ʒ� �ڵ�� �Ҹ������ڵ� ���� �Ǵ� ���Ŀ� �� �� ����
	GeomCloneToNode* cs = dynamic_cast<GeomCloneToNode*>(kernel->create("GeomCloneToNode", "/geom/name4/rectangle/coverline/rotate/cloneTo"));
	GeomHeadNode* h3 = dynamic_cast<GeomHeadNode*>(kernel->create("GeomHeadNode", "/geom/name4_1"));
	GeomCloneFromNode* from = dynamic_cast<GeomCloneFromNode*>(kernel->create("GeomCloneFromNode", "/geom/name4_1/clonefrom"));
	from->setReferedNode(cs);

	// ��� �Ӽ� ����� �ڵ����� ������Ʈ�� �Ǵ��� Ȯ���ϱ� ���� �׽�Ʈ �ڵ�
	c2->setParameters("50", "0", "200");



	// ��3 �׸���
	GeomHeadNode* h5 = dynamic_cast<GeomHeadNode*>(kernel->create("GeomHeadNode", "/geom/name5"));
	GeomCircleNode* c3 = dynamic_cast<GeomCircleNode*>(kernel->create("GeomCircleNode", "/geom/name5/circle"));
	c3->setParameters("100", "50", "300");
	kernel->create("GeomCoverLineNode", "/geom/name5/circle/coverline");
	GeomRotateNode* m2 = dynamic_cast<GeomRotateNode*>(kernel->create("GeomRotateNode", "/geom/name5/circle/coverline/rotate"));
	m2->setParameters("_pi/3.0", cs1);

	//	GeomRotateNode* m2 = dynamic_cast<GeomRotateNode*>(kernel->create("GeomRotateNode", "/geom/name1/circle/coverline/object/rotate"));
	//	m2->setParameters("_pi/4.0", cs1);
*/

	CWnd* pFrame = AfxGetMainWnd();
	if( pFrame ) pFrame->SendMessageToDescendants( WM_GEOMPART, GEOM_OPEN, 0 );
	UpdateAllViews(NULL);
}


void bzMagDoc::OnModelerCircle()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CreateCircleDlg dlg;  // CDialog �Ļ� Ŭ������ �ν��Ͻ��� ����
	INT_PTR nResponse = dlg.DoModal();  // CDialog Ŭ������ ��� �Լ��� ȣ��
	
	if (nResponse == IDCANCEL) return;

	// ����̸� �����ڵ� --> String ����ȯ
	String name(dlg.name_);

	// �÷�
	COLORREF rgb = dlg.colorCtrl_.GetColor();
	int nBitmaskRed   = 0x0000ff; //255
	int nBitmaskGreen = 0x00ff00; //65280
	int nBitmaskBlue  = 0xff0000; //16711680

	int r = (( rgb & nBitmaskRed ));
	int g = (( rgb & nBitmaskGreen ) >> 8);
	int b = (( rgb & nBitmaskBlue ) >> 16);
	int a = (int)(1.0 - ((double)(dlg.alpha_))) * 250;

	// ������ǥ�� ���
	GeomCSNode* cs = dynamic_cast<GeomCSNode*>(dlg.getCSNode());

	// ���̾�α� �ڽ����� �Է� ���� ���� �̿��ϴ� ��ƾ
	Node* geom_root = kernel_->lookup(geom_root_);

	String path1 = geom_root_ + "/" + name;
	String path2 = path1 + "/circle";
	String path3 = path2 + "/coverline";

	GeomHeadNode* h1 = dynamic_cast<GeomHeadNode*>(kernel_->create("GeomHeadNode", path1));
	h1->setColor(Color(r,g,b,a));
	GeomCircleNode* c1 = dynamic_cast<GeomCircleNode*>(kernel_->create("GeomCircleNode", path2));
	c1->setParameters(String(dlg.cx_), String(dlg.cy_), String(dlg.radius_));
	kernel_->create("GeomCoverLineNode", path3);

	CWnd* pFrame = AfxGetMainWnd();
	if( pFrame ) pFrame->SendMessageToDescendants( WM_GEOMPART, GEOMPART_ADD, 0 );
	UpdateAllViews(NULL);
}


void bzMagDoc::OnModelerRectangle()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CreateRectangleDlg dlg;  // CDialog �Ļ� Ŭ������ �ν��Ͻ��� ����
	INT_PTR nResponse = dlg.DoModal();  // CDialog Ŭ������ ��� �Լ��� ȣ��

	if (nResponse == IDCANCEL) return;

	double x1 = _tstof(dlg.x1_);
	double y1 = _tstof(dlg.y1_);
	double x2 = _tstof(dlg.x2_);
	double y2 = _tstof(dlg.y2_);

	// ����̸� �����ڵ� --> String ����ȯ
	String name(dlg.name_);

	// �÷�
	COLORREF rgb = dlg.colorCtrl_.GetColor();
	int nBitmaskRed   = 0x0000ff; //255
	int nBitmaskGreen = 0x00ff00; //65280
	int nBitmaskBlue  = 0xff0000; //16711680

	int r = (( rgb & nBitmaskRed ));
	int g = (( rgb & nBitmaskGreen ) >> 8);
	int b = (( rgb & nBitmaskBlue ) >> 16);
	int a = (int)(1.0 - ((double)(dlg.alpha_))) * 255;

	// ������ǥ�� ���
	GeomCSNode* cs = dynamic_cast<GeomCSNode*>(dlg.getCSNode());

	// ���̾�α� �ڽ����� �Է� ���� ���� �̿��ϴ� ��ƾ
	Node* geom_root = kernel_->lookup(geom_root_);

	String path1 = geom_root_ + "/" + name;
	String path2 = path1 + "/rectangle";
	String path3 = path2 + "/coverline";

	GeomHeadNode* h1 = dynamic_cast<GeomHeadNode*>(kernel_->create("GeomHeadNode", path1));
	h1->setColor(Color(r, g, b, a));
	GeomRectNode* c1 = dynamic_cast<GeomRectNode*>(kernel_->create("GeomRectNode", path2));
	c1->setParameters(String(dlg.x1_), String(dlg.y1_), String(dlg.x2_), String(dlg.y2_));
	kernel_->create("GeomCoverLineNode", path3);

	CWnd* pFrame = AfxGetMainWnd();
	if( pFrame ) pFrame->SendMessageToDescendants( WM_GEOMPART, GEOMPART_ADD, 0 );
	UpdateAllViews(NULL);
}



void bzMagDoc::OnModelerExpr()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	DialogExpressions diaExpression;
	diaExpression.DoModal();
}
