#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "bzmag/core/kernel.h"
#include "bzmag/core/module.h"
#include "bzmag/engine/engine.h"

using namespace bzmag;
using namespace bzmag::engine;

INCLUDE_MODULE(Engine);


// ExprProperty 대화 상자입니다.

class DialogExprProperty : public CDialogEx
{
	DECLARE_DYNAMIC(DialogExprProperty)

public:
	DialogExprProperty(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~DialogExprProperty();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOGEXPRPROPERTY };

public:
	void AdjustLayout();
	void SetPropertyType(bool hide, bool userdefined);
	void Bind();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
private:
	CListCtrl m_listVar;
	CButton m_btnAdd;
	CButton m_btnModify;
	CButton m_btnDelete;

	std::map<String, Expression*> m_expressions;
	int m_iSelectedItem;

	bool m_bHide;
	bool m_bUserDefined;

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnNMClickListvar(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedAdd();
	afx_msg void OnBnClickedDelete();
	afx_msg void OnBnClickedModify();
};
