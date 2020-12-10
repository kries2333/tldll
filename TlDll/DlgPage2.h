#pragma once
#include <afxdialogex.h>

// DlgPage2 对话框
// 技能模块
class DlgPage2 : public CDialogEx
{
	DECLARE_DYNAMIC(DlgPage2)

public:
	DlgPage2(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~DlgPage2();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_PAGE2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtn41();

private:
	void  LoadSkillData();
public:
	CListCtrl m_SkillList;
	afx_msg void OnBnClickedButton1();
};
