#pragma once
#include <afxdialogex.h>

// CMonsterNameDlg 对话框

class CMonsterNameDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMonsterNameDlg)

public:
	CMonsterNameDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CMonsterNameDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLGMONSTERNAME };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
};
