// C:\Development\Projects\cpp\bzMag_new\code\bzmag\bzMag\CreateRectangleDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "bzMag.h"
#include "CreateRectangleDlg.h"
#include "afxdialogex.h"


// CreateRectangleDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CreateRectangleDlg, CDialogEx)

CreateRectangleDlg::CreateRectangleDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CreateRectangleDlg::IDD, pParent)
	, x1_(_T(""))
	, y1_(_T(""))
	, x2_(_T(""))
	, y2_(_T(""))
	, name_(_T(""))
	, coord_(_T(""))
	, alpha_(0)
	, cs_(0)
{
	USING_MODULE(Engine);
}

CreateRectangleDlg::~CreateRectangleDlg()
{
}

void CreateRectangleDlg::BindCoordinateSystem(Node* node, int idx)
{
	CString name(node->getName().c_str());
	coordCtrl_.InsertString(idx, name);
	idxMap_[idx] = node;

	Node::NodeIterator it;
	for (it=node->firstChildNode(); it!=node->lastChildNode(); ++it)
	{
		Node* next_node = *it;
		BindCoordinateSystem(next_node, idx+1);
	}
}

void CreateRectangleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_X1, x1_);
	DDX_Text(pDX, IDC_EDIT_Y1, y1_);
	DDX_Text(pDX, IDC_EDIT_X2, x2_);
	DDX_Text(pDX, IDC_EDIT_Y2, y2_);
	DDX_Text(pDX, IDC_EDIT_NAME, name_);
	DDX_CBString(pDX, IDC_COMBO_COORD, coord_);
	DDX_Text(pDX, IDC_MFCCOLORBUTTON, color_);
	DDX_Slider(pDX, IDC_SLIDER_ALPHA2, alpha_);
	DDV_MinMaxInt(pDX, alpha_, 0, 255);
	DDX_Control(pDX, IDC_EDIT_X1, x1Ctrl_);
	DDX_Control(pDX, IDC_EDIT_Y1, y1Ctrl_);
	DDX_Control(pDX, IDC_EDIT_X2, x2Ctrl_);
	DDX_Control(pDX, IDC_EDIT_Y2, y2Ctrl_);
	DDX_Control(pDX, IDC_EDIT_NAME, nameCtrl_);
	DDX_Control(pDX, IDC_COMBO_COORD, coordCtrl_);
	DDX_Control(pDX, IDC_MFCCOLORBUTTON, colorCtrl_);
	DDX_Control(pDX, IDC_SLIDER_ALPHA2, alphaCtrl_);
}


BEGIN_MESSAGE_MAP(CreateRectangleDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CreateRectangleDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CreateRectangleDlg 메시지 처리기입니다.


void CreateRectangleDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData();

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (x1_.IsEmpty()) {
		MessageBox(_T("시작점 x좌표를 입력해 주세요"));
		x1Ctrl_.SetFocus();
		return;
	}

	if (y1_.IsEmpty()) {
		MessageBox(_T("시작점 y좌표를 입력해 주세요"));
		y1Ctrl_.SetFocus();
		return;
	}

	if (x2_.IsEmpty()) {
		MessageBox(_T("끝점 x좌표를 입력해 주세요"));
		x2Ctrl_.SetFocus();
		return;
	}

	if (y2_.IsEmpty()) {
		MessageBox(_T("끝점 y좌표를 입력해 주세요"));
		y2Ctrl_.SetFocus();
		return;
	}

	if (name_.IsEmpty()) {
		MessageBox(_T("이름을 입력해 주세요"));
		nameCtrl_.SetFocus();
		return;
	}

	// 참조좌표계 얻기
	int idx = coordCtrl_.GetCurSel();
	cs_ = idxMap_[idx];

	CDialogEx::OnOK();
}


BOOL CreateRectangleDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	bzmag::Kernel* kernel = bzmag::Kernel::instance();
	Node* cs_root = kernel->lookup("/coord/global");
	BindCoordinateSystem(cs_root, 0);
	coordCtrl_.SetCurSel(0);

	GotoDlgCtrl(GetDlgItem(IDC_EDIT_X1)); 
	return FALSE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
