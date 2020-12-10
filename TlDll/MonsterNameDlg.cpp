// MonsterNameDlg.cpp: 实现文件
//

#include "pch.h"
#include "afxdialogex.h"
#include "TlDll.h"
#include "MonsterNameDlg.h"
#include "AsmMonster.h"
#include "FileSystem.h"

extern CAsmMonster* g_pAsmMonster;

// CMonsterNameDlg 对话框

IMPLEMENT_DYNAMIC(CMonsterNameDlg, CDialogEx)

CMonsterNameDlg::CMonsterNameDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLGMONSTERNAME, pParent)
{

}

CMonsterNameDlg::~CMonsterNameDlg()
{
}

void CMonsterNameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMonsterNameDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CMonsterNameDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CMonsterNameDlg 消息处理程序

// 获取周围怪物
void CMonsterNameDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	auto vers = g_pAsmMonster->AsmGetMonsterData();
	for (auto monster : vers)
	{
		dbgPrint("怪物名字 = %s  X = %f Y = %f 怪物ID=%X type  = %d", monster.szName, monster.fMonsterX, monster.fMonsterY, monster.nMonsterId, monster.nType);
	}
}
