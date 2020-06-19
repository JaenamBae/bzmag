
// bzmagModelerDoc.cpp : CbzmagModelerDoc 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "bzmagModeler.h"
#endif

#include "bzmagModelerDoc.h"
#include "engine/engine.h"


#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CbzmagModelerDoc

IMPLEMENT_DYNCREATE(CbzmagModelerDoc, CDocument)

BEGIN_MESSAGE_MAP(CbzmagModelerDoc, CDocument)
	ON_COMMAND(ID_FILE_NEW, &CbzmagModelerDoc::OnFileNew)
END_MESSAGE_MAP()


// CbzmagModelerDoc 생성/소멸

CbzmagModelerDoc::CbzmagModelerDoc()
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.
}

CbzmagModelerDoc::~CbzmagModelerDoc()
{
	
}


BOOL CbzmagModelerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}




// CbzmagModelerDoc serialization

void CbzmagModelerDoc::Serialize(CArchive& ar)
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
void CbzmagModelerDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
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
void CbzmagModelerDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 문서의 데이터에서 검색 콘텐츠를 설정합니다.
	// 콘텐츠 부분은 ";"로 구분되어야 합니다.

	// 예: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CbzmagModelerDoc::SetSearchContent(const CString& value)
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

// CbzmagModelerDoc 진단

#ifdef _DEBUG
void CbzmagModelerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CbzmagModelerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CbzmagModelerDoc 명령


void CbzmagModelerDoc::OnFileNew()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	USING_MODULE(Engine);
	bzmag::Kernel* kernel = bzmag::Kernel::instance();

	GeomHeadNode* h1 = dynamic_cast<GeomHeadNode*>(kernel->create("GeomHeadNode","/geom/name1"));
	GeomCircleNode* c1 = dynamic_cast<GeomCircleNode*>(kernel->create("GeomCircleNode","/geom/name1/circle"));
	c1->setParameters(0, 0, 400.5);
	kernel->create("GeomCoverLineNode","/geom/name1/circle/coverline");

	GeomHeadNode* h2 = dynamic_cast<GeomHeadNode*>(kernel->create("GeomHeadNode","/geom/name2"));
	GeomCircleNode* c2 = dynamic_cast<GeomCircleNode*>(kernel->create("GeomCircleNode","/geom/name2/circle"));
	c2->setParameters(0, 0, 400);
	kernel->create("GeomCoverLineNode","/geom/name2/circle/coverline");

	GeomCloneToNode* cs = dynamic_cast<GeomCloneToNode*>(kernel->create("GeomCloneToNode","/geom/name1/circle/coverline/cloneTo"));
	GeomHeadNode* h3 = dynamic_cast<GeomHeadNode*>(kernel->create("GeomHeadNode","/geom/name1_1"));
	GeomCloneFromNode* from = dynamic_cast<GeomCloneFromNode*>(kernel->create("GeomCloneFromNode","/geom/name1_1/clonefrom"));
	from->setReferedNode(cs);

	GeomSubtractNode* sub = dynamic_cast<GeomSubtractNode*>(kernel->create("GeomSubtractNode","/geom/name1/circle/coverline/cloneTo/ring"));
	GeomBooleanNode::ToolNodes tools;
	tools.push_back(h2);
	sub->setToolNode(tools);



	c2->setParameters(0, 0, 200);

	//	bool bres = cover->hitTest(100.4,0);
	//	bres = cover->hitTest(100.5,0);
	//	bres = cover->hitTest(100.6,0);

	UpdateAllViews(NULL);
}
