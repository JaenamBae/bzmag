// CreateCircleDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "bzMag.h"
#include "CreateCircleDlg.h"
#include "afxdialogex.h"
#include <regex>

// CreateCircleDlg ��ȭ �����Դϴ�.

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


// CreateCircleDlg �޽��� ó�����Դϴ�.


void CreateCircleDlg::OnBnClickedOk()
{
	UpdateData();

	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (cx_.IsEmpty()) {
		MessageBox(_T("x��ǥ�� �Է��� �ּ���"));
		cxCtrl_.SetFocus();
		return;
	}

	if (cy_.IsEmpty()) {
		MessageBox(_T("y��ǥ�� �Է��� �ּ���"));
		cyCtrl_.SetFocus();
		return;
	}

	if (radius_.IsEmpty()) {
		MessageBox(_T("�ݰ��� �Է��� �ּ���"));
		radiusCtrl_.SetFocus();
		return;
	}

	if (name_.IsEmpty()) {
		MessageBox(_T("�̸��� �Է��� �ּ���"));
		nameCtrl_.SetFocus();
		return;
	}

	// ������ǥ�� ���
	int idx = coordCtrl_.GetCurSel();
	cs_ = idxMap_[idx];

	CDialogEx::OnOK();
}


BOOL CreateCircleDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	bzmag::Kernel* kernel = bzmag::Kernel::instance();
	Node* cs_root = kernel->lookup("/coord/global");
	BindCoordinateSystem(cs_root, 0);
	coordCtrl_.SetCurSel(0);

	GotoDlgCtrl(GetDlgItem(IDC_EDIT_CX)); 
	return FALSE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}
 