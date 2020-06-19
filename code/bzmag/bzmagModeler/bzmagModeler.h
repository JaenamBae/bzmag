
// bzmagModeler.h : bzmagModeler ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.

#include "bzmag/core/kernel.h"
#include "bzmag/core/module.h"
#include "bzmag/engine/engine.h"

// CbzmagModelerApp:
// �� Ŭ������ ������ ���ؼ��� bzmagModeler.cpp�� �����Ͻʽÿ�.
//

INCLUDE_MODULE(Engine);

using namespace bzmag;

class CbzmagModelerApp : public CWinAppEx
{
public:
	CbzmagModelerApp();


// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CbzmagModelerApp theApp;
