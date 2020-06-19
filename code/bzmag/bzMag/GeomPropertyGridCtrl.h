#pragma once

#include "engine/engine.h"
using namespace bzmag;
using namespace bzmag::engine;


// CGeomPropertyGridCtrl

class CGeomPropertyGridCtrl : public CMFCPropertyGridCtrl
{
	DECLARE_DYNAMIC(CGeomPropertyGridCtrl)

public:
	CGeomPropertyGridCtrl();
	virtual ~CGeomPropertyGridCtrl();
	void BindGeometryNode(GeomBaseNode* pGeom);

	void ApplyChangedItems();

protected:
	void BindGeometryNode(GeomHeadNode* pGeom);
	void BindGeometryNode(GeomCircleNode* pGeom);
	void BindGeometryNode(GeomRectNode* pGeom);
	void BindGeometryNode(GeomCoverLineNode* pGeom);
	void BindGeometryNode(GeomCloneToNode* pGeom);
	void BindGeometryNode(GeomCloneFromNode* pGeom);
	void BindGeometryNode(GeomBooleanNode* pGeom);
	void BindGeometryNode(GeomRotateNode* pGeom);
	void BindGeometryNode(GeomMoveNode* pGeom);

	void PropertyChanged(GeomHeadNode* node) const;
	void PropertyChanged(GeomCircleNode* node) const;
	void PropertyChanged(GeomRectNode* node) const;
	void PropertyChanged(GeomRotateNode* node) const;
	void PropertyChanged(GeomMoveNode* node) const;

protected:
	GeomBaseNode* m_pCurrGeom;
	bool m_bReady;
	bool m_bChanged;

protected:
	DECLARE_MESSAGE_MAP()
	
public:
	virtual void OnPropertyChanged(CMFCPropertyGridProperty* pProp) const;
//	virtual BOOL PreTranslateMessage(MSG* pMsg);
//	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};


