#include "pch.h"
#include "Login.h"
#include "FileSystem.h"
#include "Main.h"
#include "AsmRole.h"
#include "ScriptSystem.h"
#include "Message.h"
#include "HPInit.h"
#include "User.h"
#include "AsmTask.h"
#include "AsmPet.h"
#include "AsmGroundParcel.h"
#include "AsmItem.h"

extern CMain* g_pMain;

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
CCriticalSection* g_pCriticalSection;

enum ELogin
{
	In = 1,
};

void Initial()
{
	dbgPrint("��ʼ��ʼ��");
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
	g_pGroundParcel = new CAsmGroundParcel();
	g_pAsmItem = new CAsmItem();

	g_pAsmMonster = new CAsmMonster();
	g_pAsmPet = new CAsmPet();
	g_pFileSystem = new CFileSystem();
	g_pScriptSystem = new CScriptSystem;
	g_pScriptSystem->LUA_ScriptSystemInitial();
	//-----------------------------------------------------------------------
	//�򿪹����ڴ�
	//-----------------------------------------------------------------------
	g_pMain->OpenShareMemory();
	//-----------------------------------------------------------------------
	g_pFileSystem->MyGetFilePath(g_pMain->hDll);//��ȡ�ļ�·��

	g_pMsg->Init();

	//��½����
	if (!g_pHPInit->Login_Thread())
	{
		return;
	}

	g_pMain->CreateUI();
}

UINT __stdcall Login_threadFunc(void* p)//��¼�̺߳���
{

	int nType = (int)p;

	if (nType == In)	// ע��ģ��
	{
		Initial();
		dbgPrint("ע��ģ��");
		return 0;
	}

	dbgPrint("ж��ģ��");
	//ExitLogin();
	//UnDetoursHook();
	//UnAnserHook();
	//::FreeLibraryAndExitThread(pMe->hDll, 1);

	return 1;
}