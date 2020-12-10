#pragma once
#include <afxdialogex.h>

// DlgPage4 对话框

class DlgPage4 : public CDialogEx
{
	DECLARE_DYNAMIC(DlgPage4)

public:
	DlgPage4(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~DlgPage4();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_PAGE4 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
};
