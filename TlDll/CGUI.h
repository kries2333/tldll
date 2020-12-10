#pragma once
#include <afxdialogex.h>
#include "DlgPage1.h"
#include "DlgPage2.h"
#include "DlgPage3.h"
#include "DlgPage4.h"

// CGUI 对话框
#define WM_MY_MESSAGE (WM_USER+200)
class CGUI : public CDialogEx
{
	DECLARE_DYNAMIC(CGUI)

public:
	CGUI(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CGUI();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_MAIN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	CTabCtrl m_UITable;
	virtual BOOL OnInitDialog();

	CString strButton;
public:
	DlgPage1 m_Page1;
	DlgPage2 m_Page2;
	DlgPage3 m_Page3;	//任务
	DlgPage4 m_Page4;	//地面
public:
	afx_msg void OnTcnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedBtnStart();
	afx_msg LRESULT OnMyMessage(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedBtnEnd();
};
