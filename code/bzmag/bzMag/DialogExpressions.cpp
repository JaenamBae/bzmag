// DialogExpressions.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "bzMag.h"
#include "DialogExpressions.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "bzMagDoc.h"


// DialogExpressions 대화 상자입니다.

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


// DialogExpressions 메시지 처리기입니다.


BOOL DialogExpressions::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다
	MainFrame* pFrame = (MainFrame*)AfxGetMainWnd();
	bzMagDoc* pDoc = (bzMagDoc*)pFrame->GetActiveDocument();

	CString strTabTitle[3] = {_T("사용자 변수"), _T("시스템 변수"), _T("숨김변수")};
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
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
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

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	AdjustLayout();
}


void DialogExpressions::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_UserVar.DestroyWindow();
	m_SysVar.DestroyWindow();
	m_HideVar.DestroyWindow();

	CDialogEx::OnClose();
}
