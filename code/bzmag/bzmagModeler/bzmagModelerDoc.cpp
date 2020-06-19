
// bzmagModelerDoc.cpp : CbzmagModelerDoc Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
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


// CbzmagModelerDoc ����/�Ҹ�

CbzmagModelerDoc::CbzmagModelerDoc()
{
	// TODO: ���⿡ ��ȸ�� ���� �ڵ带 �߰��մϴ�.
}

CbzmagModelerDoc::~CbzmagModelerDoc()
{
	
}


BOOL CbzmagModelerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: ���⿡ ���ʱ�ȭ �ڵ带 �߰��մϴ�.
	// SDI ������ �� ������ �ٽ� ����մϴ�.

	return TRUE;
}




// CbzmagModelerDoc serialization

void CbzmagModelerDoc::Serialize(CArchive& ar)
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
void CbzmagModelerDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
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
void CbzmagModelerDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// ������ �����Ϳ��� �˻� �������� �����մϴ�.
	// ������ �κ��� ";"�� ���еǾ�� �մϴ�.

	// ��: strSearchContent = _T("point;rectangle;circle;ole object;");
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

// CbzmagModelerDoc ����

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


// CbzmagModelerDoc ���


void CbzmagModelerDoc::OnFileNew()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
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
