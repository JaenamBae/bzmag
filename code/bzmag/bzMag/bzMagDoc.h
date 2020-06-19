#pragma once

/*
Description : bzMagDoc Ŭ������ �������̽�
              �⺻���� �� ������Ʈ ������ �Ҹ�����, �׸��� �ڱ� ������ɿ� ���ؼ�
			  �����ϱ� ���� �ڵ���� �ۼ��Ǿ���
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
protected: // serialization������ ��������ϴ�.
	bzMagDoc();
	DECLARE_DYNCREATE(bzMagDoc)

// Ư���Դϴ�.
public:

// �۾��Դϴ�.
public:
// 	void AddUserExpression(Expression* expr);
// 	void DeleteUserExpression(Expression* expr);
	void Clear();


// �������Դϴ�.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// �����Դϴ�.
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

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// �˻� ó���⿡ ���� �˻� �������� �����ϴ� ����� �Լ�
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	afx_msg void OnFileNew();
	afx_msg void OnModelerCircle();
	afx_msg void OnModelerRectangle();
	afx_msg void OnModelerExpr();
};
