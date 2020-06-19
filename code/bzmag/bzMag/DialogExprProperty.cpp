// DialogExprProperty.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "bzMag.h"
#include "DialogExprProperty.h"
#include "afxdialogex.h"
#include "DialogVariable.h"
#include "bzMagDoc.h"
#include "MainFrm.h"



// ExprProperty ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(DialogExprProperty, CDialogEx)

DialogExprProperty::DialogExprProperty(CWnd* pParent /*=NULL*/)
	: CDialogEx(DialogExprProperty::IDD, pParent)
{

}

DialogExprProperty::~DialogExprProperty()
{
}

void DialogExprProperty::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LISTVAR, m_listVar);
	DDX_Control(pDX, IDC_ADD, m_btnAdd);
	DDX_Control(pDX, IDC_MODIFY, m_btnModify);
	DDX_Control(pDX, IDC_DELETE, m_btnDelete);
}


BEGIN_MESSAGE_MAP(DialogExprProperty, CDialogEx)
	ON_WM_SIZE()
	ON_NOTIFY(NM_CLICK, IDC_LISTVAR, &DialogExprProperty::OnNMClickListvar)
	ON_BN_CLICKED(IDC_ADD, &DialogExprProperty::OnBnClickedAdd)
	ON_BN_CLICKED(IDC_DELETE, &DialogExprProperty::OnBnClickedDelete)
	ON_BN_CLICKED(IDC_MODIFY, &DialogExprProperty::OnBnClickedModify)
END_MESSAGE_MAP()


// ExprProperty �޽��� ó�����Դϴ�.


void DialogExprProperty::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	AdjustLayout();
}


void DialogExprProperty::AdjustLayout()
{
	if( (GetSafeHwnd() == NULL) 
		|| (m_listVar.GetSafeHwnd() == NULL) 
		|| (m_btnAdd.GetSafeHwnd() == NULL) 
		|| (m_btnModify.GetSafeHwnd() == NULL) 
		|| (m_btnDelete.GetSafeHwnd() == NULL) )
	{
		return;
	}


	CRect rectClient, rectButton;
	GetClientRect(&rectClient);
	m_btnAdd.GetClientRect(&rectButton);

	m_listVar.SetWindowPos(NULL, 
		rectClient.left + 5, 
		rectClient.top + 5, 
		rectClient.Width() - 10, 
		rectClient.Height() - rectButton.Height() - 15, 
		SWP_NOACTIVATE | SWP_NOZORDER);

	m_btnAdd.SetWindowPos(NULL, 
		rectClient.left + 5, 
		rectClient.bottom - (5 + rectButton.Height()), 
		rectButton.Width(), 
		rectButton.Height(), 
		SWP_NOACTIVATE | SWP_NOZORDER);

	m_btnModify.SetWindowPos(NULL, 
		rectClient.left + 10 + rectButton.Width(), 
		rectClient.bottom - (5 + rectButton.Height()), 
		rectButton.Width(), 
		rectButton.Height(), 
		SWP_NOACTIVATE | SWP_NOZORDER);

	m_btnDelete.SetWindowPos(NULL, 
		rectClient.left+15+2*rectButton.Width(), 
		rectClient.bottom-(5+rectButton.Height()), 
		rectButton.Width(), 
		rectButton.Height(), 
		SWP_NOACTIVATE | SWP_NOZORDER);
}

void DialogExprProperty::SetPropertyType(bool hide, bool userdefined)
{
	m_bHide = hide;
	m_bUserDefined = userdefined;

	if (!m_bUserDefined || m_bHide) {
		m_btnAdd.EnableWindow(FALSE);
	}
	else {
		m_btnAdd.EnableWindow(TRUE);
	}

}

void DialogExprProperty::Bind()
{
	m_listVar.DeleteAllItems();

	
	ExpressionServer* eserver = ExpressionServer::instance();
	assert(eserver != 0);
	ExpressionServer::ExprIter it;

	int i = 0;
	for( it = eserver->firstExpression(); it != eserver->lastExpression(); ++it )
	{
		Expression* expr = (*it).second;

		String key = expr->getKey();
		CString kkey(key.c_str());

		CString str_expr(expr->getExpression().c_str());
		double evalue = expr->getValue();
		CString str_eval;
		str_eval.Format(_T("%f"), evalue);
		CString str_comment(expr->getComment().c_str());

		LVITEM lvItem = { 0 };
		lvItem.mask = LVIF_TEXT | LVIF_PARAM;
		lvItem.iItem = i;
		lvItem.iSubItem = 0;
		lvItem.lParam = (LPARAM)expr;
		lvItem.pszText = (LPWSTR)(LPCTSTR)kkey;

		if (m_bHide && m_bUserDefined)
		{
			if (expr->isHidden())
			{
				m_listVar.InsertItem(&lvItem);
				m_listVar.SetItemText(i, 1, str_expr);
				m_listVar.SetItemText(i, 2, str_eval);
				m_listVar.SetItemText(i, 3, str_comment);

				i++;
			}
		}
		else if (!m_bHide && m_bUserDefined)
		{
			if (!expr->isHidden() && expr->isUserDefined())
			{
				m_listVar.InsertItem(&lvItem);
				m_listVar.SetItemText(i, 1, str_expr);
				m_listVar.SetItemText(i, 2, str_eval);
				m_listVar.SetItemText(i, 3, str_comment);

				i++;
			}
		}
		else if (!m_bHide && !m_bUserDefined)
		{
			if (!expr->isHidden() && !expr->isUserDefined())
			{
				m_listVar.InsertItem(&lvItem);
				m_listVar.SetItemText(i, 1, str_expr);
				m_listVar.SetItemText(i, 2, str_eval);
				m_listVar.SetItemText(i, 3, str_comment);

				i++;
			}
		}
	}
}

BOOL DialogExprProperty::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	m_listVar.SetExtendedStyle( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	//m_VarList.SetExtendedStyle( m_VarList.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT );

	CString listName[4] = {_T("������"), _T("ǥ����"), _T("��갪"), _T("����")};
	if( m_listVar.InsertColumn(0, listName[0], LVCFMT_LEFT, 100) == -1 ) return false;
	if( m_listVar.InsertColumn(1, listName[1], LVCFMT_LEFT, 250) == -1 ) return false;
	if( m_listVar.InsertColumn(2, listName[2], LVCFMT_LEFT, 100) == -1 ) return false;
	if( m_listVar.InsertColumn(3, listName[3], LVCFMT_LEFT, 150) == -1 ) return false;


	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


BOOL DialogExprProperty::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	switch(pMsg->message)
	{
	case WM_KEYDOWN:
		{
			switch(pMsg->wParam)
			{
			case VK_ESCAPE:
				return TRUE;
			case VK_RETURN:
				return TRUE;
			}
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


void DialogExprProperty::OnNMClickListvar(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	m_iSelectedItem = pNMItemActivate->iItem;
	if( m_iSelectedItem != -1) {
		m_btnModify.EnableWindow( TRUE );
		m_btnDelete.EnableWindow( TRUE );
	}
	else {
		m_btnModify.EnableWindow( FALSE );
		m_btnDelete.EnableWindow( FALSE );
	}

	if (!m_bUserDefined) {
		m_btnAdd.EnableWindow(FALSE);
		m_btnModify.EnableWindow(FALSE);
		m_btnDelete.EnableWindow(FALSE);
	}
	*pResult = 0;
}


void DialogExprProperty::OnBnClickedAdd()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	MainFrame* pFrame = (MainFrame*)AfxGetMainWnd();
	if( pFrame == NULL ) return;

	bzMagDoc* pDoc = (bzMagDoc*)pFrame->GetActiveDocument();
	if( pDoc == NULL ) return;

	DialogVariable diaModVar;

	if( IDOK == diaModVar.DoModal() )
	{
		Bind();
	}
}


void DialogExprProperty::OnBnClickedDelete()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	MainFrame* pFrame = (MainFrame*)AfxGetMainWnd();
	if( pFrame == NULL ) return;

	bzMagDoc* pDoc = (bzMagDoc*)pFrame->GetActiveDocument();
	if( pDoc == NULL ) return;

	int i = m_listVar.GetNextItem( -1, LVNI_SELECTED );
	ASSERT( i == m_iSelectedItem );

	Expression* expr = (Expression*)m_listVar.GetItemData(i);
	if (expr->deleteExpression())
	{
		m_btnModify.EnableWindow( FALSE );
		m_btnDelete.EnableWindow( FALSE );
		Bind();
	} else {
		MessageBox(_T("������ ������ �� �����ϴ�!"), _T("Ȯ��"));
	}
}


void DialogExprProperty::OnBnClickedModify()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if( m_iSelectedItem == -1 )  return;

	int i = m_listVar.GetNextItem( -1, LVNI_SELECTED );
	ASSERT( i == m_iSelectedItem );

	CString strKey = m_listVar.GetItemText( i, 0 );
	CString strExpr = m_listVar.GetItemText( i, 1 );
	CString strComment = m_listVar.GetItemText( i, 3 );

	DialogVariable diaModVar;
	diaModVar.m_sKey = strKey;
	diaModVar.m_sExpr = strExpr;
	diaModVar.m_sComment = strComment;
	diaModVar.m_pExpr = (Expression*)m_listVar.GetItemData(i);
	if( IDOK == diaModVar.DoModal() )
	{
		Bind();
	}
}
