// DialogExpressions.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "bzMag.h"
#include "DialogExpressions.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "bzMagDoc.h"


// DialogExpressions ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(DialogExpressions, CDialogEx)

DialogExpressions::DialogExpressions(CWnd* pParent /*=NULL*/)
	: CDialogEx(DialogExpressions::IDD, pParent)
{

}

DialogExpressions::~DialogExpressions()
{
}

void DialogExpressions::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TABVAR, m_tabVar);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDAPPLY, m_btnApply);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
}


BEGIN_MESSAGE_MAP(DialogExpressions, CDialogEx)
	ON_WM_SIZE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TABVAR, &DialogExpressions::OnTcnSelchangeVartab)
	ON_NOTIFY(TCN_SELCHANGING, IDC_TABVAR, &DialogExpressions::OnTcnSelchangingVartab)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// DialogExpressions �޽��� ó�����Դϴ�.


BOOL DialogExpressions::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�
	MainFrame* pFrame = (MainFrame*)AfxGetMainWnd();
	bzMagDoc* pDoc = (bzMagDoc*)pFrame->GetActiveDocument();

	CString strTabTitle[3] = {_T("����� ����"), _T("�ý��� ����"), _T("���躯��")};
	for( int i=0; i<3; i++ )
		m_tabVar.InsertItem(i, strTabTitle[i]);

	m_UserVar.Create(DialogExprProperty::IDD, &m_tabVar);
	m_SysVar.Create(DialogExprProperty::IDD,  &m_tabVar);
	m_HideVar.Create(DialogExprProperty::IDD, &m_tabVar);

	m_UserVar.SetPropertyType(false, true);
	m_UserVar.Bind();

	m_SysVar.SetPropertyType(false, false);
	m_SysVar.Bind();

	m_HideVar.SetPropertyType(true, true);
	m_HideVar.Bind();

	m_tabVar.SetCurSel( 0 );
	m_UserVar.ShowWindow(SW_SHOW);
	m_UserVar.SetFocus();

	AdjustLayout();

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void DialogExpressions::AdjustLayout()
{
	if( (GetSafeHwnd() == NULL) 
		|| (m_tabVar.GetSafeHwnd() == NULL) 
		|| (m_btnOK.GetSafeHwnd() == NULL) 
		|| (m_btnApply.GetSafeHwnd() == NULL) 
		|| (m_btnCancel.GetSafeHwnd() == NULL)
		|| (m_UserVar.GetSafeHwnd() == NULL)
		|| (m_SysVar.GetSafeHwnd() == NULL)
		|| (m_HideVar.GetSafeHwnd() == NULL))
	{
		return;
	}

	CRect rectClient, rectButton;
	GetClientRect(rectClient);
	m_btnOK.GetWindowRect(rectButton);

	m_tabVar.SetWindowPos(NULL, 
		rectClient.left, 
		rectClient.top, 
		rectClient.Width(), 
		rectClient.Height() - rectButton.Height(), 
		SWP_NOACTIVATE | SWP_NOZORDER);

	m_btnCancel.SetWindowPos(NULL, 
		rectClient.right - rectButton.Width(), 
		rectClient.bottom - rectButton.Height(), 
		rectButton.Width(), 
		rectButton.Height(), 
		SWP_NOACTIVATE | SWP_NOZORDER);

	m_btnApply.SetWindowPos(NULL, 
		rectClient.right - (2*rectButton.Width()+5), 
		rectClient.bottom - rectButton.Height(), 
		rectButton.Width(), 
		rectButton.Height(), 
		SWP_NOACTIVATE | SWP_NOZORDER);

	m_btnOK.SetWindowPos(NULL, 
		rectClient.right - (3*rectButton.Width()+10), 
		rectClient.bottom - rectButton.Height(), 
		rectButton.Width(), 
		rectButton.Height(), 
		SWP_NOACTIVATE | SWP_NOZORDER);

	CRect rcTab;
	m_tabVar.GetClientRect(&rcTab);

	m_UserVar.SetWindowPos(NULL, 
		rcTab.left + 5, 
		rcTab.top + 25, 
		rcTab.Width() - 10, 
		rcTab.Height() - 30, 
		SWP_NOACTIVATE | SWP_NOZORDER);

	m_SysVar.SetWindowPos(NULL, 
		rcTab.left + 5, 
		rcTab.top + 25, 
		rcTab.Width() - 10, 
		rcTab.Height() - 30, 
		SWP_NOACTIVATE | SWP_NOZORDER);

	m_HideVar.SetWindowPos(NULL, 
		rcTab.left + 5, 
		rcTab.top + 25, 
		rcTab.Width() - 10, 
		rcTab.Height() - 30, 
		SWP_NOACTIVATE | SWP_NOZORDER);

	Invalidate();
}

void DialogExpressions::OnTcnSelchangeVartab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	if( m_tabVar.GetSafeHwnd() == NULL )
		return;

	int iSel = m_tabVar.GetCurSel();
	switch (iSel)
	{
	case 0:
		m_UserVar.ShowWindow(SW_SHOW);
		break;

	case 1:
		m_SysVar.ShowWindow(SW_SHOW);
		break;

	case 2:
		m_HideVar.ShowWindow(SW_SHOW);
		break;

	default:
		break;
	}
	*pResult = 0;
}


void DialogExpressions::OnTcnSelchangingVartab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	if( m_tabVar.GetSafeHwnd() == NULL )
		return;

	int iSel = m_tabVar.GetCurSel();
	switch (iSel)
	{
	case 0:
		m_UserVar.ShowWindow(SW_HIDE);
		break;

	case 1:
		m_SysVar.ShowWindow(SW_HIDE);
		break;

	case 2:
		m_HideVar.ShowWindow(SW_HIDE);
		break;

	default:
		break;
	}
	*pResult = 0;
}



void DialogExpressions::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	AdjustLayout();
}


void DialogExpressions::OnClose()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	m_UserVar.DestroyWindow();
	m_SysVar.DestroyWindow();
	m_HideVar.DestroyWindow();

	CDialogEx::OnClose();
}
