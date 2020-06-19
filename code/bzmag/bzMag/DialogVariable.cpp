// C:\Development\Projects\cpp\bzMag_new\code\bzmag\bzMag\DialogVariable.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "bzMag.h"
#include "DialogVariable.h"
#include "afxdialogex.h"


// DialogVariable 대화 상자입니다.

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


// DialogVariable 메시지 처리기입니다.


BOOL DialogVariable::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
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
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


BOOL DialogVariable::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
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
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	bzmag::Kernel* kernel = bzmag::Kernel::instance();
	m_name.GetWindowText( m_sKey );
	m_expression.GetWindowText( m_sExpr );
	m_comment.GetWindowText( m_sComment );

	String key(m_sKey);
	String expression(m_sExpr);
	String comment(m_sComment);

	if (m_pExpr == 0) {
		m_pExpr = dynamic_cast<Expression*>(kernel->create("Expression"));

		// setKey() 호출시 ExpressinServer에 등록됨
		if (m_pExpr->setKey(key)) {
			if (m_pExpr->setExpression(expression)) {
				m_pExpr->setComment(comment);
				m_pExpr->setHidden(false);
			}
			else {
				MessageBox(_T("표현식에 오류가 있습니다!"), _T("확인"));
				return;
			}
			
		} else {
			m_pExpr = 0;
			MessageBox(_T("키를 등록할수 없습니다!"), _T("확인"));
			return;
		}
	} else {
		if (m_pExpr->setExpression(expression)) {
			m_pExpr->setComment(comment);
		}
		else {
			MessageBox(_T("표현식에 오류가 있습니다!"), _T("확인"));
			return;
		}
	}

	CDialogEx::OnOK();
}
