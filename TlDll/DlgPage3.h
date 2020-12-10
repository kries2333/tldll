#pragma once
#include <afxdialogex.h>

// DlgPage3 对话框

class DlgPage3 : public CDialogEx
{
	DECLARE_DYNAMIC(DlgPage3)

public:
	DlgPage3(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~DlgPage3();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_PAGE3 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_editTaskList;
	afx_msg void OnBnClickedButton1();
};
