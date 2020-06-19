#pragma once

#include "bzmag/core/kernel.h"
#include "bzmag/core/module.h"
#include "bzmag/engine/engine.h"
#include "afxwin.h"
#include "afxcolorbutton.h"
#include "afxcmn.h"

using namespace bzmag;
using namespace bzmag::engine;

INCLUDE_MODULE(Engine);

// CreateRectangleDlg ��ȭ �����Դϴ�.

class CreateRectangleDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CreateRectangleDlg)

public:
	CreateRectangleDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CreateRectangleDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_CREATERECTANGLE };

	Node* getCSNode()
	{ return cs_; }

protected:
	void BindCoordinateSystem(Node* node, int idx);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CString x1_;
	CString y1_;
	CString x2_;
	CString y2_;
	CString name_;
	CString coord_;
	COLORREF color_;
	CEdit x1Ctrl_;
	CEdit y1Ctrl_;
	CEdit x2Ctrl_;
	CEdit y2Ctrl_;
	CEdit nameCtrl_;
	CComboBox coordCtrl_;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	CMFCColorButton colorCtrl_;
	int alpha_;
	CSliderCtrl alphaCtrl_;

private:
	std::map<int, Node*> idxMap_;
	Node* cs_;
};
