// C:\Development\Projects\cpp\bzMag_new\code\bzmag\bzMag\DialogVariable.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "bzMag.h"
#include "DialogVariable.h"
#include "afxdialogex.h"


// DialogVariable ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(DialogVariable, CDialogEx)

DialogVariable::DialogVariable(CWnd* pParent /*=NULL*/)
	: CDialogEx(DialogVariable::IDD, pParent)
{
	m_pExpr = 0;
}

DialogVariable::~DialogVariable()
{
}

void DialogVariable::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_NAME, m_name);
	DDX_Control(pDX, IDC_EXPRESSION, m_expression);
	DDX_Control(pDX, IDC_COMMENT, m_comment);
}


BEGIN_MESSAGE_MAP(DialogVariable, CDialogEx)
END_MESSAGE_MAP()


// DialogVariable �޽��� ó�����Դϴ�.


BOOL DialogVariable::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	m_name.SetWindowText( m_sKey );
	m_expression.SetWindowText( m_sExpr );
	m_comment.SetWindowText( m_sComment );

	if (m_pExpr) {
		m_name.SetReadOnly(TRUE);
		ASSERT(m_name.GetStyle() & ES_READONLY);
		m_expression.SetFocus();
	} else {
		m_name.SetFocus();
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


BOOL DialogVariable::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	switch(pMsg->message)
	{
	case WM_KEYDOWN:
		{
			switch(pMsg->wParam)
			{
			case VK_ESCAPE:
			case VK_RETURN:
				return TRUE;
			}
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


void DialogVariable::OnOK()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	bzmag::Kernel* kernel = bzmag::Kernel::instance();
	m_name.GetWindowText( m_sKey );
	m_expression.GetWindowText( m_sExpr );
	m_comment.GetWindowText( m_sComment );

	String key(m_sKey);
	String expression(m_sExpr);
	String comment(m_sComment);

	if (m_pExpr == 0) {
		m_pExpr = dynamic_cast<Expression*>(kernel->create("Expression"));

		// setKey() ȣ��� ExpressinServer�� ��ϵ�
		if (m_pExpr->setKey(key)) {
			if (m_pExpr->setExpression(expression)) {
				m_pExpr->setComment(comment);
				m_pExpr->setHidden(false);
			}
			else {
				MessageBox(_T("ǥ���Ŀ� ������ �ֽ��ϴ�!"), _T("Ȯ��"));
				return;
			}
			
		} else {
			m_pExpr = 0;
			MessageBox(_T("Ű�� ����Ҽ� �����ϴ�!"), _T("Ȯ��"));
			return;
		}
	} else {
		if (m_pExpr->setExpression(expression)) {
			m_pExpr->setComment(comment);
		}
		else {
			MessageBox(_T("ǥ���Ŀ� ������ �ֽ��ϴ�!"), _T("Ȯ��"));
			return;
		}
	}

	CDialogEx::OnOK();
}
