#pragma once
#include <afxdialogex.h>

// DlgPage1 对话框

class DlgPage1 : public CDialogEx
{
	DECLARE_DYNAMIC(DlgPage1)

public:
	DlgPage1(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~DlgPage1();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_PAGE1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedBtnEditmonster();

	
	CListCtrl m_KillPos;
	virtual BOOL OnInitDialog();
};
