
// MainFrm.cpp : MainFrame Ŭ������ ����
//

#include "stdafx.h"
#include "bzMag.h"

#include "MainFrm.h"
#include "bzMagView.h"
#include "bzMagTreeView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// MainFrame

IMPLEMENT_DYNCREATE(MainFrame, CFrameWndEx)

BEGIN_MESSAGE_MAP(MainFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_CUSTOMIZE, &MainFrame::OnViewCustomize)
	ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, &MainFrame::OnToolbarCreateNew)
	ON_WM_SETTINGCHANGE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // ���� �� ǥ�ñ�
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// MainFrame ����/�Ҹ�

MainFrame::MainFrame()
{
	// TODO: ���⿡ ��� �ʱ�ȭ �ڵ带 �߰��մϴ�.
}

MainFrame::~MainFrame()
{
}

int MainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	BOOL bNameValid;

	if (!m_wndMenuBar.Create(this))
	{
		TRACE0("�޴� ������ ������ ���߽��ϴ�.\n");
		return -1;      // ������ ���߽��ϴ�.
	}

	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);

	// �޴� ������ Ȱ��ȭ�ص� ��Ŀ���� �̵����� �ʰ� �մϴ�.
	CMFCPopupMenu::SetForceMenuFocus(FALSE);

	DWORD dwStyle = WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC;
	int IDR = theApp.m_bHiColorIcons ? IDR_MAINFRAME_256 : IDR_MAINFRAME;
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, dwStyle, CRect(1,1,1,1), IDR) || !m_wndToolBar.LoadToolBar(IDR))
	{
		TRACE0("���� ������ ������ ���߽��ϴ�.\n");
		return -1;      // ������ ���߽��ϴ�.
	}

	CString strToolBarName;
	bNameValid = strToolBarName.LoadString(IDS_TOOLBAR_STANDARD);
	ASSERT(bNameValid);
	m_wndToolBar.SetWindowText(strToolBarName);

	// �� �� ���� ���� ��������
	if (!m_wndToolBarZoom.CreateEx(this, TBSTYLE_FLAT, dwStyle, CRect(1,1,1,1), IDS_TOOLBAR_ZOOM) || !m_wndToolBarZoom.LoadToolBar(IDR_TOOLBAR_ZOOM))
	{
		TRACE0("���� ������ ������ ���߽��ϴ�.\n");
		return -1;      // ������ ���߽��ϴ�.
	}

	bNameValid = strToolBarName.LoadString(IDS_TOOLBAR_ZOOM);
	ASSERT(bNameValid);
	m_wndToolBarZoom.SetWindowText(strToolBarName);

	// �� �� ���� ���� ��������
	if (!m_wndToolBarModeler.CreateEx(this, TBSTYLE_FLAT, dwStyle, CRect(1,1,1,1), IDR_TOOLBAR_MODELER) || !m_wndToolBarModeler.LoadToolBar(IDR_TOOLBAR_MODELER))
	{
		TRACE0("���� ������ ������ ���߽��ϴ�.\n");
		return -1;      // ������ ���߽��ϴ�.
	}
	bNameValid = strToolBarName.LoadString(IDS_TOOLBAR_MODELER);
	ASSERT(bNameValid);
	m_wndToolBarModeler.SetWindowText(strToolBarName);


	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);
	m_wndToolBar.EnableCustomizeButton(FALSE, ID_VIEW_CUSTOMIZE, strCustomize);


	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("���� ǥ������ ������ ���߽��ϴ�.\n");
		return -1;      // ������ ���߽��ϴ�.
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO: ���� ���� �� �޴� ������ ��ŷ�� �� ���� �Ϸ��� �� �ټ� ���� �����Ͻʽÿ�.
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBarZoom.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBarModeler.EnableDocking(CBRS_ALIGN_ANY);

	EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndMenuBar);
	DockPane(&m_wndToolBar);
	DockPane(&m_wndToolBarZoom);
	DockPane(&m_wndToolBarModeler);
	DockPaneLeftOf(&m_wndToolBar, &m_wndToolBarZoom);
	DockPaneLeftOf(&m_wndToolBarZoom, &m_wndToolBarModeler);


	// Visual Studio 2005 ��Ÿ�� ��ŷ â ������ Ȱ��ȭ�մϴ�.
	CDockingManager::SetDockingMode(DT_SMART);
	// Visual Studio 2005 ��Ÿ�� ��ŷ â �ڵ� ���� ������ Ȱ��ȭ�մϴ�.
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// ��ŷ â�� ����ϴ�.
	if (!CreateDockingWindows())
	{
		TRACE0("��ŷ â�� ������ ���߽��ϴ�.\n");
		return -1;
	}

	m_wndOutput.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndOutput);
	m_wndProperties.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndProperties);


	// ��� ����� �������̽� ��Ҹ� �׸��� �� ����ϴ� ���־� �����ڸ� �����մϴ�.
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));

	// ���� ���� �� ��ŷ â �޴� �ٲٱ⸦ Ȱ��ȭ�մϴ�.
	EnablePaneMenu(TRUE, ID_VIEW_CUSTOMIZE, strCustomize, ID_VIEW_TOOLBAR);

	// ����(<Alt> Ű�� ���� ä ����) ���� ���� ����� ������ Ȱ��ȭ�մϴ�.
	CMFCToolBar::EnableQuickCustomization();

	return 0;
}

BOOL MainFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/,
	CCreateContext* pContext)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if( !m_wndSplitter.CreateStatic( this, 1, 2 ) )
		return FALSE;

	if( !m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(bzMagTreeView), CSize(200,200), pContext) || 
		!m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(bzMagView), CSize(500, 200), pContext) )
	{
		m_wndSplitter.DestroyWindow();
		return FALSE;
	}

	SetActiveView( (CView*)m_wndSplitter.GetPane(0,1));

	return TRUE;
}

BOOL MainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return TRUE;
}

BOOL MainFrame::CreateDockingWindows()
{
	BOOL bNameValid;
	// ��� â�� ����ϴ�.
	CString strOutputWnd;
	bNameValid = strOutputWnd.LoadString(IDS_OUTPUT_WND);
	ASSERT(bNameValid);
	if (!m_wndOutput.Create(strOutputWnd, this, CRect(0, 0, 100, 100), TRUE, ID_VIEW_OUTPUTWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI))
	{
		TRACE0("��� â�� ������ ���߽��ϴ�.\n");
		return FALSE; // ������ ���߽��ϴ�.
	}

	// �Ӽ� â�� ����ϴ�.
	CString strPropertiesWnd;
	bNameValid = strPropertiesWnd.LoadString(IDS_PROPERTIES_WND);
	ASSERT(bNameValid);
	if (!m_wndProperties.Create(strPropertiesWnd, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_PROPERTIESWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
	{
		TRACE0("�Ӽ� â�� ������ ���߽��ϴ�.\n");
		return FALSE; // ������ ���߽��ϴ�.
	}

	SetDockingWindowIcons(theApp.m_bHiColorIcons);
	return TRUE;
}

void MainFrame::SetDockingWindowIcons(BOOL bHiColorIcons)
{
	HICON hOutputBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_OUTPUT_WND_HC : IDI_OUTPUT_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndOutput.SetIcon(hOutputBarIcon, FALSE);

	HICON hPropertiesBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_PROPERTIES_WND_HC : IDI_PROPERTIES_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndProperties.SetIcon(hPropertiesBarIcon, FALSE);

}

// MainFrame ����

#ifdef _DEBUG
void MainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void MainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// MainFrame �޽��� ó����

void MainFrame::OnViewCustomize()
{
	CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* �޴��� �˻��մϴ�. */);
	pDlgCust->Create();
}

LRESULT MainFrame::OnToolbarCreateNew(WPARAM wp,LPARAM lp)
{
	LRESULT lres = CFrameWndEx::OnToolbarCreateNew(wp,lp);
	if (lres == 0)
	{
		return 0;
	}
	
	CMFCToolBar* pUserToolbar = (CMFCToolBar*)lres;
	ASSERT_VALID(pUserToolbar);

	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
	return lres;
}


void MainFrame::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CFrameWndEx::OnSettingChange(uFlags, lpszSection);
	m_wndOutput.UpdateFonts();
}
