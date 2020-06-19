#pragma once
#include "afxcmn.h"
#include "DialogExprProperty.h"

// DialogExpressions ��ȭ �����Դϴ�.

class DialogExpressions : public CDialogEx
{
	DECLARE_DYNAMIC(DialogExpressions)

public:
	DialogExpressions(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~DialogExpressions();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOGEXPRESSIONS };

public:
	void AdjustLayout();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public:
	CTabCtrl m_tabVar;
	CButton m_btnOK;
	CButton m_btnCancel;
	CButton m_btnApply;

	DialogExprProperty m_UserVar;
	DialogExprProperty m_SysVar;
	DialogExprProperty m_HideVar;


public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTcnSelchangeVartab(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTcnSelchangingVartab(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClose();
};
