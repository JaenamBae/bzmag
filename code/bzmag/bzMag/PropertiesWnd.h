
#pragma once
#include "GeomPropertyGridCtrl.h"
#include "engine/engine.h"
#include "propertyGrid.h"

using namespace bzmag;
using namespace bzmag::engine;

class CPropertiesWnd : public CDockablePane
{
// 생성입니다.
public:
	CPropertiesWnd();

	void AdjustLayout();

// 특성입니다.
public:
	void SetVSDotNetLook(BOOL bSet)
	{
		m_wndPropList.SetVSDotNetLook(bSet);
		m_wndPropList.SetGroupNameFullWidth(bSet);
	}

protected:
	CFont m_fntPropList;
	CGeomPropertyGridCtrl m_wndPropList;

// 구현입니다.
public:
	virtual ~CPropertiesWnd();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnGeomPart(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

	void InitPropList();
	void SetPropListFont();
	void Bind(GeomBaseNode* pGeom);
public:

	afx_msg void OnSetFocus(CWnd* pOldWnd);
};

