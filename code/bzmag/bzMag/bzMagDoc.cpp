
// bzMagDoc.cpp : bzMagDoc 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
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


// bzMagDoc 생성/소멸

bzMagDoc::bzMagDoc() : geom_root_("/geom"), coord_root_("/coord"), globalCS_(0), exprserver_(0)
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.
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
	// 메모리 헤제 순서 매우 중요함
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

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}




// bzMagDoc serialization

void bzMagDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
	}
}

#ifdef SHARED_HANDLERS

// 축소판 그림을 지원합니다.
void bzMagDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 문서의 데이터를 그리려면 이 코드를 수정하십시오.
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

// 검색 처리기를 지원합니다.
void bzMagDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 문서의 데이터에서 검색 콘텐츠를 설정합니다.
	// 콘텐츠 부분은 ";"로 구분되어야 합니다.

	// 예: strSearchContent = _T("point;rectangle;circle;ole object;");
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

// bzMagDoc 진단

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


// bzMagDoc 명령
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


	// 기존 내용 초기화
	Node* geom_root = kernel_->lookup(geom_root_);
	if (geom_root) geom_root->clearChildren<GeomBaseNode>();

	Node* cs_root = kernel_->lookup(coord_root_);
	if (cs_root) cs_root->clearChildren<GeomCSNode>();

	// 사용자 수식 제거 --> 강제 제거해야 함
//	expressions_.clear();

	globalCS_ = dynamic_cast<GeomCSNode*>(kernel_->create("GeomCSNode", coord_root_ + "/global"));
	globalCS_->setParameters("0", "0", "0");
}


void bzMagDoc::OnFileNew()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	// 기존 내용 초기화
	Clear();

	bzmag::Kernel* kernel = bzmag::Kernel::instance();
	Node* geom_root = kernel->lookup("/geom");

	// 좌표계 생성
	GeomCSNode* cs1 = dynamic_cast<GeomCSNode*>(kernel->create("GeomCSNode","/coord/global/cs1"));
	cs1->setParameters("100", "150", "_pi/8.0");

	GeomCSNode* cs2 = dynamic_cast<GeomCSNode*>(kernel->create("GeomCSNode","/coord/global/cs1/cs2"));
	cs2->setParameters("0", "50", "_pi/8.0");

	GeomCSNode* cs3 = dynamic_cast<GeomCSNode*>(kernel->create("GeomCSNode","/coord/global/cs3"));
	cs3->setParameters("200", "100", "_pi/4.0");


	// 원1 그리기
	GeomHeadNode* h1 = dynamic_cast<GeomHeadNode*>(kernel->create("GeomHeadNode", "/geom/name1"));
	GeomCircleNode* c1 = dynamic_cast<GeomCircleNode*>(kernel->create("GeomCircleNode", "/geom/name1/circle"));
	c1->setParameters("0", "0", "10");
	kernel->create("GeomCoverLineNode", "/geom/name1/circle/coverline");

/*
	// 원2 그리기
	GeomHeadNode* h2 = dynamic_cast<GeomHeadNode*>(kernel->create("GeomHeadNode", "/geom/name2"));
	GeomCircleNode* c2 = dynamic_cast<GeomCircleNode*>(kernel->create("GeomCircleNode", "/geom/name2/circle"));
	c2->setParameters("0", "0", "400");
	kernel->create("GeomCoverLineNode", "/geom/name2/circle/coverline");


*/
	// 사각형1 그리기
	GeomHeadNode* h4 = dynamic_cast<GeomHeadNode*>(kernel->create("GeomHeadNode", "/geom/name4"));
	GeomRectNode* r1 = dynamic_cast<GeomRectNode*>(kernel->create("GeomRectNode", "/geom/name4/rectangle"));
	r1->setParameters("0", "0", "200", "100");
	kernel->create("GeomCoverLineNode", "/geom/name4/rectangle/coverline");
	GeomRotateNode* m1 = dynamic_cast<GeomRotateNode*>(kernel->create("GeomRotateNode", "/geom/name4/rectangle/coverline/rotate"));
	m1->setParameters("_pi/4.0");


/*
	// 불린연산을 위한 코드
	GeomSubtractNode* sub = dynamic_cast<GeomSubtractNode*>(kernel->create("GeomSubtractNode", "/geom/name1/circle/coverline/object"));
	GeomBooleanNode::ToolNodes tools;
	tools.push_back(h4);
	sub->setToolNode(tools);

	// A' = A - B의 연산에 있어, 연산수행후의 결과는 A'가 되며, B는 종속노드가 됨
	// 만약 B를 독립노드로 계속 활용하고 싶을 경우, B를 복제한 다음 연산을 실행하도록 함
	// 아래 4줄은 B의 복제를 위한 코드이며 옵션사항임
	// 아래 코드는 불린연산코드 이전 또는 이후에 올 수 있음
	GeomCloneToNode* cs = dynamic_cast<GeomCloneToNode*>(kernel->create("GeomCloneToNode", "/geom/name4/rectangle/coverline/rotate/cloneTo"));
	GeomHeadNode* h3 = dynamic_cast<GeomHeadNode*>(kernel->create("GeomHeadNode", "/geom/name4_1"));
	GeomCloneFromNode* from = dynamic_cast<GeomCloneFromNode*>(kernel->create("GeomCloneFromNode", "/geom/name4_1/clonefrom"));
	from->setReferedNode(cs);

	// 노드 속성 변경시 자동으로 업데이트가 되는지 확인하기 위한 테스트 코드
	c2->setParameters("50", "0", "200");



	// 원3 그리기
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
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CreateCircleDlg dlg;  // CDialog 파생 클래스의 인스턴스를 생성
	INT_PTR nResponse = dlg.DoModal();  // CDialog 클래스의 멤버 함수를 호출
	
	if (nResponse == IDCANCEL) return;

	// 노드이름 유니코드 --> String 형변환
	String name(dlg.name_);

	// 컬러
	COLORREF rgb = dlg.colorCtrl_.GetColor();
	int nBitmaskRed   = 0x0000ff; //255
	int nBitmaskGreen = 0x00ff00; //65280
	int nBitmaskBlue  = 0xff0000; //16711680

	int r = (( rgb & nBitmaskRed ));
	int g = (( rgb & nBitmaskGreen ) >> 8);
	int b = (( rgb & nBitmaskBlue ) >> 16);
	int a = (int)(1.0 - ((double)(dlg.alpha_))) * 250;

	// 참조좌표계 얻기
	GeomCSNode* cs = dynamic_cast<GeomCSNode*>(dlg.getCSNode());

	// 다이얼로그 박스에서 입력 받은 값을 이용하는 루틴
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
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CreateRectangleDlg dlg;  // CDialog 파생 클래스의 인스턴스를 생성
	INT_PTR nResponse = dlg.DoModal();  // CDialog 클래스의 멤버 함수를 호출

	if (nResponse == IDCANCEL) return;

	double x1 = _tstof(dlg.x1_);
	double y1 = _tstof(dlg.y1_);
	double x2 = _tstof(dlg.x2_);
	double y2 = _tstof(dlg.y2_);

	// 노드이름 유니코드 --> String 형변환
	String name(dlg.name_);

	// 컬러
	COLORREF rgb = dlg.colorCtrl_.GetColor();
	int nBitmaskRed   = 0x0000ff; //255
	int nBitmaskGreen = 0x00ff00; //65280
	int nBitmaskBlue  = 0xff0000; //16711680

	int r = (( rgb & nBitmaskRed ));
	int g = (( rgb & nBitmaskGreen ) >> 8);
	int b = (( rgb & nBitmaskBlue ) >> 16);
	int a = (int)(1.0 - ((double)(dlg.alpha_))) * 255;

	// 참조좌표계 얻기
	GeomCSNode* cs = dynamic_cast<GeomCSNode*>(dlg.getCSNode());

	// 다이얼로그 박스에서 입력 받은 값을 이용하는 루틴
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
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	DialogExpressions diaExpression;
	diaExpression.DoModal();
}
