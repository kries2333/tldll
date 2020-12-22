// DlgPage4.cpp: 实现文件
//

#include "pch.h"
#include "TlDll.h"
#include "DlgPage4.h"
#include "afxdialogex.h"
#include "AsmGroundParcel.h"
#include "AsmRole.h"
#include "Message.h"
#include "AsmItem.h"
#include "AsmStoreItem.h"
#include "AsmTask.h"
#include "AsmPet.h"
#include "Function.h"
#include "User.h"

extern CAsmGroundParcel* g_pGroundParcel;
extern CAsmRole* g_pAsmRole;
extern CAsmItem* g_pAsmItem;
extern CAsmTask* g_pAsmTask;
extern CMessage* g_pMsg;
extern CAsmPet* g_pAsmPet;
extern CUser* g_pUser;
extern CAsmStoreItem* g_pAsmStoreItem;
extern CAsmSkill* g_pAsmSkill;

// DlgPage4 对话框

IMPLEMENT_DYNAMIC(DlgPage4, CDialogEx)

DlgPage4::DlgPage4(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_PAGE4, pParent)
{

}

DlgPage4::~DlgPage4()
{
}

void DlgPage4::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgPage4, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &DlgPage4::OnBnClickedButton1)
END_MESSAGE_MAP()


// DlgPage4 消息处理程序


void DlgPage4::OnBnClickedButton1()
{
	//g_pAsmItem->AsmGetItemData();
	g_pUser->UserGetSkill();
}
