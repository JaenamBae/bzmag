#pragma once
#include "afxwin.h"
#include "bzmag/core/kernel.h"
#include "bzmag/core/module.h"
#include "bzmag/engine/engine.h"
#include "afxcmn.h"
#include "afxcolorbutton.h"

using namespace bzmag;
using namespace bzmag::engine;

INCLUDE_MODULE(Engine);

// CreateCircleDlg 대화 상자입니다.

class CreateCircleDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CreateCircleDlg)

public:
	CreateCircleDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CreateCircleDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_CREATECIRCLE };

	Node* getCSNode()
	{ return cs_; }

protected:
	void BindCoordinateSystem(Node* node, int idx);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:

	CString cx_;
	CString cy_;
	CString radius_;
	CString name_;
	CComboBox coordCtrl_;
	CString coord_;
	COLORREF color_;
	int alpha_;
	CEdit cxCtrl_;
	CEdit cyCtrl_;
	CEdit radiusCtrl_;
	CEdit nameCtrl_;
	CMFCColorButton colorCtrl_;
	CSliderCtrl alphaCtrl_;
		
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();

private:
	std::map<int, Node*> idxMap_;
	Node* cs_;
};
