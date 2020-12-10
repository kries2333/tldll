// DlgPage2.cpp: 实现文件
//

#include "pch.h"
#include "TlDll.h"
#include "DlgPage2.h"
#include "afxdialogex.h"
#include "AsmSkill.h"
#include "FileSystem.h"
#include "Function.h"
#include "AsmRole.h"

extern CAsmRole* g_pAsmRole;
extern CAsmSkill* g_pAsmSkill;


// DlgPage2 对话框

IMPLEMENT_DYNAMIC(DlgPage2, CDialogEx)

DlgPage2::DlgPage2(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_PAGE2, pParent)
{

}

DlgPage2::~DlgPage2()
{
}

void DlgPage2::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_SkillList);
}


BEGIN_MESSAGE_MAP(DlgPage2, CDialogEx)
	ON_BN_CLICKED(IDC_BTN4_1, &DlgPage2::OnBnClickedBtn41)
	ON_BN_CLICKED(IDC_BUTTON1, &DlgPage2::OnBnClickedButton1)
END_MESSAGE_MAP()


// DlgPage2 消息处理程序


BOOL DlgPage2::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_SkillList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES);
	m_SkillList.InsertColumn(0, _T("技能名"), NULL, 100, -1);
	m_SkillList.InsertColumn(1, _T("类型"), NULL, 60, -1);
	m_SkillList.InsertColumn(2, _T("ID"), NULL, 60, -1);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void DlgPage2::OnBnClickedBtn41()
{
	//刷新技能
	LoadSkillData();
}


void  DlgPage2::LoadSkillData()
{
	m_SkillList.DeleteAllItems();

	auto ASkill = g_pAsmSkill->AsmGetSkillData();
	for (auto s : ASkill)
	{
		m_SkillList.InsertItem(0, s.szName);
		CString str;
		str.Format("%d", s.nSkillId);
		m_SkillList.SetItemText(0, 1, str);
	}
}

void DlgPage2::OnBnClickedButton1()
{
	g_pAsmSkill->AsmUseSkillCall(0xFFFFFFFF, 0x23);
}
