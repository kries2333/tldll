#include "pch.h"
#include "Login.h"
#include "FileSystem.h"
#include "Me.h"
#include "MeKill.h"
#include "AsmRole.h"
#include "ScriptSystem.h"
#include "Message.h"
#include "HPInit.h"
#include "User.h"
#include "AsmTask.h"
#include "AsmPet.h"
#include "AsmGroundParcel.h"
#include "AsmItem.h"
#include "AsmTeam.h"
#include "AsmStoreItem.h"
#include "MeKill.h"
#include "HideDll.h"

extern CMe* g_pMe;

CHPInit* g_pHPInit = nullptr;
CMessage* g_pMsg;
CFileSystem* g_pFileSystem;
CAsmRole *g_pAsmRole;
CAsmSkill* g_pAsmSkill;
CAsmTask* g_pAsmTask;
CAsmPet* g_pAsmPet;
CScriptSystem* g_pScriptSystem;
CUser*g_pUser;
CAsmMonster* g_pAsmMonster;
CAsmGroundParcel* g_pGroundParcel;
CAsmItem* g_pAsmItem;
CAsmStoreItem* g_pAsmStoreItem;
CAsmTeam* g_pAsmTeam;
CCriticalSection* g_pCriticalSection;
CMeKill* g_pMeKill;

enum ELogin
{
	In = 1,
};

void Initial()
{
	dbgPrint("开始初始化");
	g_pHPInit = new CHPInit;

	g_GameExeBase = GetGameBase();
	dbgPrint("g_GameExeBase== %X", g_GameExeBase);
	//-----------------------------------------------------------------------
	g_pAsmRole = new CAsmRole();
	g_pMsg = new CMessage;
	g_pCriticalSection = new CCriticalSection();
	g_pUser = new CUser();
	g_pAsmSkill = new CAsmSkill();
	g_pAsmTask = new CAsmTask();
	g_pAsmTeam = new CAsmTeam();
	g_pGroundParcel = new CAsmGroundParcel();
	g_pAsmItem = new CAsmItem();
	g_pAsmStoreItem = new CAsmStoreItem();
	g_pAsmMonster = new CAsmMonster();
	g_pAsmPet = new CAsmPet();
	g_pFileSystem = new CFileSystem();
	g_pScriptSystem = new CScriptSystem();

	g_pMeKill = new CMeKill();
	g_pScriptSystem->LUA_ScriptSystemInitial();
	//-----------------------------------------------------------------------
	//打开共享内存 
	//-----------------------------------------------------------------------
	g_pMe->OpenShareMemory();
	//-----------------------------------------------------------------------
	g_pFileSystem->MyGetFilePath(g_pMe->hDll);//获取文件路径

	g_pMsg->Init();

	//登陆流程
	if (!g_pHPInit->Login_Thread())
	{
		return;
	}

	//-----------------------------------------------------------------------
	//隐藏dll
	//-----------------------------------------------------------------------
	//hide_module("TlDll.dll");
	//clean_pe("TlDll.dll");

	g_pMe->CreateUI();
}

void ExitLogin()//卸载dll
{
	//g_pMe->bLogin = false;
	//g_pMe->bKillThread = false;
	g_pMe->bRun = false;//寻路状态
	g_pMe->bTaskThread = false;//任务线程标志
	g_pMe->bProtectThread = false;
	g_pMe->bPauseProtect = false;

	if (g_pMe->hProtectThread)//保护线程
	{
		::WaitForSingleObject(g_pMe->hProtectThread, INFINITE);
		::CloseHandle(g_pMe->hProtectThread);
	}
}

UINT __stdcall Login_threadFunc(void* p)//登录线程函数
{

	int nType = (int)p;

	if (nType == In)	// 注入模块
	{
		Initial();
		dbgPrint("注入模块");
		return 0;
	}

	dbgPrint("卸载模块");
	ExitLogin();
	//UnDetoursHook();
	//UnAnserHook();
	//::FreeLibraryAndExitThread(pMe->hDll, 1);

	return 1;
}