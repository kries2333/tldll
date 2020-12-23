// CGUI.cpp: 实现文件
//

#include "pch.h"
#include "TlDll.h"
#include "CGUI.h"
#include "afxdialogex.h"
#include "Me.h"
#include "ScriptSystem.h"
#include "FileSystem.h"
#include "AsmRole.h"

extern CMe *g_pMe;
extern CFileSystem* g_pFileSystem;
extern CAsmRole* g_pAsmRole;

// CGUI 对话框

IMPLEMENT_DYNAMIC(CGUI, CDialogEx)

CGUI::CGUI(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

CGUI::~CGUI()
{
}

void CGUI::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_UITable);
}


BEGIN_MESSAGE_MAP(CGUI, CDialogEx)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CGUI::OnTcnSelchangeTab1)
	ON_BN_CLICKED(IDC_BTN_START, &CGUI::OnBnClickedBtnStart)
	ON_BN_CLICKED(IDC_BTN_END, &CGUI::OnBnClickedBtnEnd)
END_MESSAGE_MAP()


BOOL CGUI::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//----------创建选项卡start
	m_UITable.InsertItem(0, _T("挂机"));
	m_UITable.InsertItem(1, _T("技能"));
	m_UITable.InsertItem(2, _T("任务"));
	m_UITable.InsertItem(4, _T("物品"));

	m_Page1.Create(IDD_DIALOG_PAGE1, &m_UITable);
	m_Page2.Create(IDD_DIALOG_PAGE2, &m_UITable);
	m_Page3.Create(IDD_DIALOG_PAGE3, &m_UITable);
	m_Page4.Create(IDD_DIALOG_PAGE4, &m_UITable);

	RECT rect;
	m_UITable.GetClientRect(&rect);
	rect.top += 22;

	m_Page1.MoveWindow(&rect);
	m_Page2.MoveWindow(&rect);
	m_Page3.MoveWindow(&rect);
	m_Page4.MoveWindow(&rect);

	m_Page1.ShowWindow(TRUE);
	m_UITable.SetCurSel(0);

	ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW);//初始化不在任务栏显示

	TAsmRoleInfo role = g_pAsmRole->GetRoleInfo();
	CString strTemp(role.szName);
	SetWindowText("角色名:" + strTemp);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void CGUI::OnTcnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult)
{
	int index = m_UITable.GetCurSel();
	dbgPrint("GetCurSel index = %d", index);
	switch (index)
	{
		case 0:
		{
			m_Page1.ShowWindow(TRUE);
			m_Page2.ShowWindow(FALSE);
			m_Page3.ShowWindow(FALSE);
			m_Page4.ShowWindow(FALSE);
			break;
		}
		case 1:
		{
			m_Page1.ShowWindow(FALSE);
			m_Page2.ShowWindow(TRUE);
			m_Page3.ShowWindow(FALSE);
			m_Page4.ShowWindow(FALSE);
			break;
		}
		case 2:
		{
			m_Page1.ShowWindow(FALSE);
			m_Page2.ShowWindow(FALSE);
			m_Page3.ShowWindow(TRUE);
			m_Page4.ShowWindow(FALSE);
			break;
		case 3:
			m_Page1.ShowWindow(FALSE);
			m_Page2.ShowWindow(FALSE);
			m_Page3.ShowWindow(FALSE);
			m_Page4.ShowWindow(TRUE);
			break;
		}
	}
	*pResult = 0;
}



// CGUI 消息处理程序


void CGUI::OnBnClickedButton1()
{
}

void CGUI::OnBnClickedBtnStart()
{
	g_pMe->CreateTask("自动30级");
	g_pMe->CreatProtect();
	g_pMe->CreateInfo();
	g_pMe->CreateGift();
}


void CGUI::OnBnClickedBtnEnd()
{
	// TODO: 在此添加控件通知处理程序代码
	g_pMe->EndThread();
}
