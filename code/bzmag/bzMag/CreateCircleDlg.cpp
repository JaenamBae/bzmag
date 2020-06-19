// CreateCircleDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "bzMag.h"
#include "CreateCircleDlg.h"
#include "afxdialogex.h"
#include <regex>

// CreateCircleDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CreateCircleDlg, CDialogEx)

CreateCircleDlg::CreateCircleDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CreateCircleDlg::IDD, pParent)
	, cx_(_T(""))
	, cy_(_T(""))
	, radius_(_T(""))
	, name_(_T(""))
	, coord_(_T(""))
	, alpha_(0)
	, cs_(0)
{
	USING_MODULE(Engine);
}

CreateCircleDlg::~CreateCircleDlg()
{
}

void CreateCircleDlg::BindCoordinateSystem(Node* node, int idx)
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

void CreateCircleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_CX, cx_);
	DDX_Text(pDX, IDC_EDIT_CY, cy_);
	DDX_Text(pDX, IDC_EDIT_RADIUS, radius_);
	DDX_Text(pDX, IDC_EDIT_NAME, name_);
	DDX_CBString(pDX, IDC_COMBO_COORD, coord_);
	DDX_Text(pDX, IDC_MFCCOLORBUTTON, color_);
	DDX_Slider(pDX, IDC_SLIDER_ALPHA, alpha_);
	DDV_MinMaxInt(pDX, alpha_, 0, 255);
	DDX_Control(pDX, IDC_EDIT_CX, cxCtrl_);
	DDX_Control(pDX, IDC_EDIT_CY, cyCtrl_);
	DDX_Control(pDX, IDC_EDIT_RADIUS, radiusCtrl_);
	DDX_Control(pDX, IDC_EDIT_NAME, nameCtrl_);
	DDX_Control(pDX, IDC_COMBO_COORD, coordCtrl_);
	DDX_Control(pDX, IDC_MFCCOLORBUTTON, colorCtrl_);
	DDX_Control(pDX, IDC_SLIDER_ALPHA, alphaCtrl_);

}


BEGIN_MESSAGE_MAP(CreateCircleDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CreateCircleDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CreateCircleDlg 메시지 처리기입니다.


void CreateCircleDlg::OnBnClickedOk()
{
	UpdateData();

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (cx_.IsEmpty()) {
		MessageBox(_T("x좌표를 입력해 주세요"));
		cxCtrl_.SetFocus();
		return;
	}

	if (cy_.IsEmpty()) {
		MessageBox(_T("y좌표를 입력해 주세요"));
		cyCtrl_.SetFocus();
		return;
	}

	if (radius_.IsEmpty()) {
		MessageBox(_T("반경을 입력해 주세요"));
		radiusCtrl_.SetFocus();
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


BOOL CreateCircleDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	bzmag::Kernel* kernel = bzmag::Kernel::instance();
	Node* cs_root = kernel->lookup("/coord/global");
	BindCoordinateSystem(cs_root, 0);
	coordCtrl_.SetCurSel(0);

	GotoDlgCtrl(GetDlgItem(IDC_EDIT_CX)); 
	return FALSE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
 