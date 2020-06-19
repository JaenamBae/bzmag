
// bzmagModelerDoc.h : CbzmagModelerDoc Ŭ������ �������̽�
//


#pragma once

#include "bzmag/core/kernel.h"
#include "bzmag/core/module.h"
#include "bzmag/engine/engine.h"

using namespace bzmag;
using namespace bzmag::engine;

INCLUDE_MODULE(Engine);

class CbzmagModelerDoc : public CDocument
{
protected: // serialization������ ��������ϴ�.
	CbzmagModelerDoc();
	DECLARE_DYNCREATE(CbzmagModelerDoc)

// Ư���Դϴ�.
public:

// �۾��Դϴ�.
public:

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
	virtual ~CbzmagModelerDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif



protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// �˻� ó���⿡ ���� �˻� �������� �����ϴ� ����� �Լ�
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	afx_msg void OnFileNew();
};
