// DlgPage1.cpp: 实现文件
//

#include "pch.h"
#include "TlDll.h"
#include "DlgPage1.h"
#include "afxdialogex.h"
#include "AsmRole.h"
#include "MonsterNameDlg.h"
#include "FileSystem.h"
#include "Function.h"

extern CAsmRole* g_pAsmRole;

// DlgPage1 对话框

IMPLEMENT_DYNAMIC(DlgPage1, CDialogEx)

DlgPage1::DlgPage1(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_PAGE1, pParent)
{

}

DlgPage1::~DlgPage1()
{
}

void DlgPage1::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_KillPos);
}


BEGIN_MESSAGE_MAP(DlgPage1, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &DlgPage1::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BTN_EDITMONSTER, &DlgPage1::OnBnClickedBtnEditmonster)
END_MESSAGE_MAP()


// DlgPage1 消息处理程序
BOOL DlgPage1::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//挂机中心表格设置
	m_KillPos.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_KillPos.InsertColumn(0, _T("场景"), NULL, 80, -1);
	m_KillPos.InsertColumn(1, _T("场景ID"), NULL, 60, -1);
	m_KillPos.InsertColumn(2, _T("坐标X"), NULL, 60, -1);
	m_KillPos.InsertColumn(3, _T("坐标Y"), NULL, 60, -1);
	m_KillPos.InsertColumn(4, _T("半径R"), NULL, 60, -1);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void DlgPage1::OnBnClickedButton1()
{
	auto APos = g_pAsmRole->GetPos();

	CString strPoint;
	GetDlgItem(IDC_EDIT1)->GetWindowText(strPoint);
	_tstring str_x = _to_string((int)APos.fx);
	_tstring str_y = _to_string((int)APos.fy);
	TAsmMap tAsmMap = role_curmap();
	_tstring str_id = _to_string(tAsmMap.nSceneId);
	int nCount = m_KillPos.GetItemCount();
	m_KillPos.InsertItem(nCount, tAsmMap.szName);
	m_KillPos.SetItemText(nCount, 1, str_id.c_str());
	m_KillPos.SetItemText(nCount, 2, str_x.c_str());
	m_KillPos.SetItemText(nCount, 3, str_y.c_str());
	m_KillPos.SetItemText(nCount, 4, strPoint);
}


void DlgPage1::OnBnClickedBtnEditmonster()
{
	CMonsterNameDlg dlg;
	dlg.DoModal();
}
