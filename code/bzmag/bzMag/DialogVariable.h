#pragma once
#include "afxwin.h"
#include "bzmag/core/kernel.h"
#include "bzmag/core/module.h"
#include "bzmag/engine/engine.h"

using namespace bzmag;
using namespace bzmag::engine;

INCLUDE_MODULE(Engine);

// DialogVariable 대화 상자입니다.

class DialogVariable : public CDialogEx
{
	DECLARE_DYNAMIC(DialogVariable)

public:
	DialogVariable(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~DialogVariable();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOGVAR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	Expression* m_pExpr;
	CString m_sKey;
	CString m_sExpr;
	CString m_sComment;

	CEdit m_name;
	CEdit m_expression;
	CEdit m_comment;
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnOK();
};
