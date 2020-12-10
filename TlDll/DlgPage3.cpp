// DlgPage3.cpp: 实现文件
//

#include "pch.h"
#include "TlDll.h"
#include "DlgPage3.h"
#include "afxdialogex.h"
#include "AsmTask.h"

extern CAsmTask* g_pAsmTask;

// DlgPage3 对话框

IMPLEMENT_DYNAMIC(DlgPage3, CDialogEx)

DlgPage3::DlgPage3(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_PAGE3, pParent)
{

}

DlgPage3::~DlgPage3()
{
}

void DlgPage3::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_editTaskList);
}


BEGIN_MESSAGE_MAP(DlgPage3, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &DlgPage3::OnBnClickedButton1)
END_MESSAGE_MAP()


// DlgPage3 消息处理程序


void DlgPage3::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	g_pAsmTask->AsmGetTaskData();
}
