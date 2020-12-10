#include "pch.h"
#include "Main.h"
#include "Login.h"
#include "CGUI.h"
#include "Function.h"
#include "ScriptSystem.h"
#include "FileSystem.h"
#include "Message.h"
#include "User.h"
#include "AsmTask.h"
#include "AsmRole.h"
#include "HPInit.h"

CGUI* g_pUI;
extern CMain* g_pMain;
extern CMessage *g_pMsg;
extern CFileSystem* g_pFileSystem;
extern CScriptSystem *g_pScriptSystem;
extern CUser* g_pUser;
extern CAsmTask* g_pAsmTask;
extern CHPInit* g_pHPInit;
extern CAsmRole* g_pAsmRole;

HWND g_Gamehwnd;
CString g_GameTitle;

UINT __stdcall UI_ThreadFunc(LPVOID p)
{
	dbgPrint("��ʼ������");

	Sleep(2000);
	g_pMain->CreateTask(g_pMsg->mGameInfo->LuaScript);
	g_pMain->CreateInfo();
	g_pMain->CreateGift();
	

	//���ó�Ա��������һ��ģ̬�Ի���
	g_pUI = new CGUI(); //��ָ������ڴ�
	g_pUI->DoModal(); //����һ��ģ̬�Ի���
	return 0;
}

//������Ϣ
UINT __stdcall Info_ThreadFunc(void* p)
{
	if (g_pMain == NULL)
	{
		return 0;
	}
	while (true)
	{
		if (!g_pMain->bInfoThread)
		{
			return 0;
		}
		if (g_pMsg != NULL)
		{
			TAsmRoleInfo tAsmRoleInfo = g_pAsmRole->GetRoleInfo();
			g_pMsg->mGameInfo->RoleHp = tAsmRoleInfo.nHP;
			g_pMsg->mGameInfo->RoleLv = tAsmRoleInfo.nLevel;
			if (tAsmRoleInfo.szName != NULL)
			{
				strcpy_s(g_pMsg->mGameInfo->RoleName, tAsmRoleInfo.szName);
			}
			if (tAsmRoleInfo.szMenpai != NULL)
			{
				strcpy_s(g_pMsg->mGameInfo->RoleSects, tAsmRoleInfo.szMenpai);
			}
		}
		Sleep(1000 * 5);
	}
}
//��ʱ��ȡ����
UINT __stdcall Gift_ThreadFunc(void* p)
{
	if (g_pMain == NULL)
	{
		return 0;
	}
	while (true)
	{
		if (!g_pMain->bGiftThread)
		{
			return 0;
		}
		if (g_pMsg != NULL)
		{
			g_pMsg->msg_dostring("setmetatable(_G, {__index = FreshmanWatch_Env}); FreshmanWatch_Bn2Click();");
			for (int i = 0; i < 20; i++)
			{
				CString strTemp = g_pMsg->msg_getstring("ItemName", " ItemName = PlayerPackage:GetBagItemName(%d)", i).c_str();
				
				if (strTemp.Find("���", 0) != -1 ||
					strTemp.Find("���޵�", 0) != -1 ||
					strTemp.Find("����Ǭ����", 0) != -1) {
					g_pMsg->msg_dostring("setmetatable(_G, { __index = Packet_Env }); Packet_ItemBtnClicked(1, %d);", i + 1);
					Sleep(1000);
				}
				Sleep(100);
			}

			CString strTemp = g_pMsg->msg_getstring("MyMonCanCommitString", "setmetatable(_G, {__index = QuitRelative_Env}); MyMonCanCommitString = QuitRelative_Text:GetText()").c_str();
			dbgPrint("������������ strTemp=%s", strTemp);
			if (strTemp.Find("������������") != -1)
			{
				g_pMsg->msg_dostring("setmetatable(_G, { __index = QuitRelative_Env }); QuitRelative_Cancel_Clicked();");
				Sleep(1000);
				return TRUE;
			}
		}
		Sleep(1000 * 30);
	}
	dbgPrint("ֹͣ��ȡ����");
	return 0;
}


UINT __stdcall Task_ThreadFunc(void* pType)
{
	//++g_pMain->atomic_int_work_thread;//�̼߳���
	Sleep(1000);
	::SendMessage(g_pUI->m_hWnd, WM_MY_MESSAGE, 0, 0);//���ť
	g_pUser->UserSystemInitial();

	CStringArray listText;
	CString strTemp((char*)pType);
	SplitCStringArr(strTemp, "|", &listText);
	for (int i = 0; i < listText.GetCount(); i++)
	{
		dbgPrint("ִ�нű�=%s", listText.GetAt(i));
		g_pHPInit->MySendGameInfo("ִ�нű�" + listText.GetAt(i));
		strTemp.Format("%s%s.lua", g_pFileSystem->Script.c_str(), listText.GetAt(i).GetString());
		g_pScriptSystem->LUA_DoFile(strTemp);//ִ�нű��ļ�
	}
	

	return 0;
}


void CMain::EndThread()
{
	bRun = false;
	bTaskThread = false;
	bGiftThread = false;
	bInfoThread = false;
	if (hTaskThread)//�����߳�ֹͣʱ����Ҫ�ȴ��ܾ�
	{
		::CloseHandle(hTaskThread);
		hTaskThread = nullptr;
	}
	if (hGiftThread)
	{
		::CloseHandle(hGiftThread);
		hGiftThread = nullptr;
	}
	if (bInfoThread)
	{
		::CloseHandle(hInfoThread);
		hInfoThread = nullptr;
	}
}

void CMain::CreateLogin(int nNUM)//������¼�߳�
{
	bGiftThread = true;
	HANDLE	hlg = (HANDLE)_beginthreadex(NULL, 0, &Login_threadFunc, (void*)nNUM, 0, NULL);
	::CloseHandle(hlg);
}

void CMain::CreateUI()	//����ģ̬�Ի���
{
	bUiThread = true;
	hUIThread = (HANDLE)_beginthreadex(NULL, 0, &UI_ThreadFunc, this, 0, NULL);
}

void CMain::CreateKillMonster()//��������߳�
{
	if (hKillMonsterThread)//ɱ���߳�
	{
		::CloseHandle(hKillMonsterThread);
		hKillMonsterThread = nullptr;
	}

	//bRun = true;
	bKillMonsterThread = true;
	hKillMonsterThread = (HANDLE)_beginthreadex(NULL, 0, &KillMonster_ThreadFunc, this, 0, NULL);
}

//ִ������ű�
void CMain::CreateTask(const char* tasks)//����Task�߳�
{
	if (hTaskThread)
	{
		::CloseHandle(hTaskThread);
		hTaskThread = nullptr;
	}
	
	dbgPrint("CreateTask=%s", tasks);
	bRun = true;
	bTaskThread = true;
	hTaskThread = (HANDLE)_beginthreadex(nullptr, 0, &Task_ThreadFunc, (void*)tasks, 0, nullptr);
}

//ִ����ȡ����
void CMain::CreateInfo()	//����Task�߳�
{
	if (hInfoThread)
	{
		::CloseHandle(hInfoThread);
		hInfoThread = nullptr;
	}

	bInfoThread = true;
	hInfoThread = (HANDLE)_beginthreadex(nullptr, 0, &Info_ThreadFunc, 0, 0, nullptr);
}

//ִ����ȡ����
void CMain::CreateGift()	//����Task�߳�
{
	if (hGiftThread)
	{
		::CloseHandle(hGiftThread);
		hGiftThread = nullptr;
	}

	bGiftThread = true;
	hGiftThread = (HANDLE)_beginthreadex(nullptr, 0, &Gift_ThreadFunc, 0, 0, nullptr);
}

BOOL CMain::OpenShareMemory()
{
	CString szNameresult;			//�����ڴ�������Ϊ��ʶ

	GetWindowHandle();
	dbgPrint("��ǰ���ھ��:%x ��������:%s", g_Gamehwnd, g_GameTitle);
	g_pMsg->GamehWnd = g_Gamehwnd;
	szNameresult.Format("TL_Memory_%x", g_Gamehwnd);
	TRACE("��ǰ�����ڴ����ֱ�ʶ:%s\n", szNameresult);
	HANDLE hMapFileresult = CreateFileMapping(
		INVALID_HANDLE_VALUE,    // �����ļ����
		NULL,                    // Ĭ�ϰ�ȫ����
		PAGE_READWRITE,          // �ɶ���д
		0,                       // ��λ�ļ���С
		BUF_SIZE_RES,            // ��λ�ļ���С
		szNameresult             // �����ڴ�����
	);

	LPVOID RESULT = (LPVOID)MapViewOfFile(
		hMapFileresult,            // �����ڴ�ľ��
		FILE_MAP_ALL_ACCESS, // �ɶ�д���
		NULL,
		NULL,
		BUF_SIZE_RES);
	if (RESULT != NULL) {
		g_pMsg->mGameInfo = (TLGAMEINFO*)RESULT;
		//д�빲���ڴ�����
		//mUsername = mtGameInfo->Account;
		//mPassword = mtGameInfo->Password;
		//dbgPrint("��ǰ�����˺�:%s ����:%s", mUsername, mPassword);
	}
	else {//���빲���ڴ�ʧ��
		UnmapViewOfFile(RESULT);
		CloseHandle(hMapFileresult);
		return false;
	}
	return true;
}


HWND GetWindowHandle()// ��ȡ����ż���
{
	DWORD dwCurrentProcessId = GetCurrentProcessId();

	if (!EnumWindows(EnumWindowsProc, (LPARAM)&dwCurrentProcessId))
	{
		return (HWND)dwCurrentProcessId;
	}

	return NULL;
}

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)// �ص�����
{
	DWORD dwCurProcessId = *((DWORD*)lParam);
	DWORD dwProcessId = 0;
	CString TempTitle;
	char WindowTitle[100] = { 0 };
	::GetWindowText(hwnd, WindowTitle, 100);
	TempTitle = WindowTitle;
	GetWindowThreadProcessId(hwnd, &dwProcessId);
	if (dwProcessId == dwCurProcessId && GetParent(hwnd) == NULL && TempTitle.Find("�������˲�") > -1)
	{
		g_GameTitle = WindowTitle;
		g_Gamehwnd = hwnd;
		*((HWND*)lParam) = hwnd;
		return FALSE;
	}
	return TRUE;
}