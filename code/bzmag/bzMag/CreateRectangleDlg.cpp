// C:\Development\Projects\cpp\bzMag_new\code\bzmag\bzMag\CreateRectangleDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "bzMag.h"
#include "CreateRectangleDlg.h"
#include "afxdialogex.h"


// CreateRectangleDlg ��ȭ �����Դϴ�.

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


// CreateRectangleDlg �޽��� ó�����Դϴ�.


void CreateRectangleDlg::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData();

	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (x1_.IsEmpty()) {
		MessageBox(_T("������ x��ǥ�� �Է��� �ּ���"));
		x1Ctrl_.SetFocus();
		return;
	}

	if (y1_.IsEmpty()) {
		MessageBox(_T("������ y��ǥ�� �Է��� �ּ���"));
		y1Ctrl_.SetFocus();
		return;
	}

	if (x2_.IsEmpty()) {
		MessageBox(_T("���� x��ǥ�� �Է��� �ּ���"));
		x2Ctrl_.SetFocus();
		return;
	}

	if (y2_.IsEmpty()) {
		MessageBox(_T("���� y��ǥ�� �Է��� �ּ���"));
		y2Ctrl_.SetFocus();
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


BOOL CreateRectangleDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	bzmag::Kernel* kernel = bzmag::Kernel::instance();
	Node* cs_root = kernel->lookup("/coord/global");
	BindCoordinateSystem(cs_root, 0);
	coordCtrl_.SetCurSel(0);

	GotoDlgCtrl(GetDlgItem(IDC_EDIT_X1)); 
	return FALSE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}
