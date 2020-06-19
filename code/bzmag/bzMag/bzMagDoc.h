#pragma once

/*
Description : bzMagDoc 클래스의 인터페이스
              기본적인 면 오브젝트 생성과 불린연산, 그리고 자기 복제기능에 대해서
			  실험하기 위한 코드들이 작성되었음
Last Update : 2014.07.25
Author : Jaenam Bae (jnbae@katech.re.kr)
*/

#include "bzmag/core/kernel.h"
#include "bzmag/core/module.h"
#include "bzmag/engine/engine.h"

using namespace bzmag;
using namespace bzmag::engine;

INCLUDE_MODULE(Engine);
INCLUDE_MODULE(WinGraphics);

class bzMagDoc : public CDocument
{
protected: // serialization에서만 만들어집니다.
	bzMagDoc();
	DECLARE_DYNCREATE(bzMagDoc)

// 특성입니다.
public:

// 작업입니다.
public:
// 	void AddUserExpression(Expression* expr);
// 	void DeleteUserExpression(Expression* expr);
	void Clear();


// 재정의입니다.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 구현입니다.
public:
	virtual ~bzMagDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	Kernel* kernel_;
	ExpressionServer* exprserver_;
	GeomCSNode* globalCS_;
	String geom_root_;
	String coord_root_;

//	std::list<Expression*> expressions_;

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 검색 처리기에 대한 검색 콘텐츠를 설정하는 도우미 함수
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	afx_msg void OnFileNew();
	afx_msg void OnModelerCircle();
	afx_msg void OnModelerRectangle();
	afx_msg void OnModelerExpr();
};
