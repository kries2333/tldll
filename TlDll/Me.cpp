#include "pch.h"
#include "Me.h"
#include "Login.h"
#include "CGUI.h"
#include "Function.h"
#include "ScriptSystem.h"
#include "FileSystem.h"
#include "Message.h"
#include "User.h"
#include "AsmTask.h"
#include "AsmRole.h"
#include "AsmPet.h"
#include "AsmItem.h"
#include "HPInit.h"

CGUI* g_pUI;
extern CMe* g_pMe;
extern CMessage *g_pMsg;
extern CFileSystem* g_pFileSystem;
extern CScriptSystem *g_pScriptSystem;
extern CUser* g_pUser;
extern CAsmTask* g_pAsmTask;
extern CHPInit* g_pHPInit;
extern CAsmRole* g_pAsmRole;
extern CAsmPet* g_pAsmPet;
extern CAsmItem* g_pAsmItem;

HWND g_Gamehwnd;
CString g_GameTitle;

UINT __stdcall UI_ThreadFunc(LPVOID p)
{
	dbgPrint("初始化界面");

	Sleep(1000);
	if (strlen(g_pMsg->mGameInfo->LuaScript) > 0)
	{
		g_pAsmRole->GetRoleInfo();
		//g_pMe->CreateTask(g_pMsg->mGameInfo->LuaScript);
		//g_pMe->CreatProtect();
		//g_pMe->CreateInfo();
		//g_pMe->CreateGift();
	}

	//采用成员变量创建一个模态对话框
	g_pUI = new CGUI(); //给指针分配内存
	g_pUI->DoModal(); //创建一个模态对话框
	return 0;
}

//更新信息
UINT __stdcall Info_ThreadFunc(void* p)
{
	if (g_pMe == NULL)
	{
		return 0;
	}
	while (true)
	{
		if (!g_pMe->bInfoThread)
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

//定时领取礼物
UINT __stdcall Gift_ThreadFunc(void* p)
{
	if (g_pMe == NULL)
	{
		return 0;
	}

	static bool isZuoQiGift = false;

	while (true)
	{
		if (!g_pMe->bGiftThread)
		{
			return 0;
		}
		if (g_pMsg != NULL)
		{
			TAsmRoleInfo tAsmRole = g_pAsmRole->GetRoleInfo();
			if (tAsmRole.bool_ret)
			{
				if (tAsmRole.nLevel > 9 && tAsmRole.nLevel < 12 && isZuoQiGift == false)
				{
					int nBagNum = g_pAsmItem->GetBagItemBlankNum();

					dbgPrint("领取坐骑 背包剩余%d", nBagNum);
					
					//领取登录奖励，登录的时候领取一次
					if (nBagNum > 5)
					{
						g_pMsg->msg_dostring("setmetatable(_G, { __index = MiniMap_Env }); MiniMap_XinShouNewBtnClk();");
						Sleep(500);

						g_pMsg->msg_dostring("setmetatable(_G, {__index = XinShouNew_Env}); XinShouNew_SevenDay_Click(1);");
						Sleep(500);

						g_pMsg->msg_dostring("setmetatable(_G, {__index = XinShouNew_Env}); XinShouNew_SevenDay_GetPrize();");
						Sleep(500);

						g_pMsg->msg_dostring("setmetatable(_G, { __index = XinShouNew_Env }); XinShouNew_Close();");
						Sleep(500);

						isZuoQiGift = true;
					}
				}
			}

			g_pMsg->msg_dostring("setmetatable(_G, {__index = FreshmanWatch_Env}); FreshmanWatch_Bn2Click();");
			Sleep(500);
			for (int i = 0; i < 5; i++)
			{
				CString strTemp = g_pMsg->msg_getstring("ItemName", " ItemName = PlayerPackage:GetBagItemName(%d)", i).c_str();
				Sleep(500);
				if (strTemp.Find("珍兽蛋", 0) != -1)	//如果背包里有宠物蛋，就放生所有宠物,然后获取松鼠宝宝
				{
					dbgPrint("背包里有珍兽蛋");
					TAsmPet tPet = g_pAsmPet->GetPetForName("松鼠宝宝");
					if (tPet.nPetId == 0)
					{
						int nCurPetCount = g_pAsmPet->GetCount();
						dbgPrint("当前宠物数=%d", nCurPetCount);
						for (int j = 0; j < nCurPetCount; j++)
						{
							dbgPrint("宠物先需要休息");
							g_pMsg->msg_dostring("Pet:Go_Relax(%d);", j);
							Sleep(500);

							dbgPrint("宠物放生");
							g_pMsg->msg_dostring("Pet:Free_Confirm(%d);", j);
							Sleep(500);

							g_pMsg->msg_dostring("setmetatable(_G, { __index = MessageBox_Self_Env }); MessageBox_Self_OK_Clicked();");
							Sleep(500);
							
						}
						g_pMsg->msg_dostring("setmetatable(_G, { __index = Packet_Env }); Packet_ItemBtnClicked(1, %d);", i + 1);
						Sleep(500);
					}
				}

				if (strTemp.Find("坐骑：如意熊") != -1)
				{
					g_pMsg->msg_dostring("setmetatable(_G, { __index = Packet_Env }); Packet_ItemBtnClicked(1, %d);", i + 1);
					Sleep(500);

					int nIndex = g_pAsmItem->AsmGetItemNum("翅膀：幻羽");
					g_pAsmItem->Destroy(nIndex);
				}

				if (strTemp.Find("翅膀", 0) != -1 ||
					strTemp.Find("江湖乾坤袋", 0) != -1) {
					g_pMsg->msg_dostring("setmetatable(_G, { __index = Packet_Env }); Packet_ItemBtnClicked(1, %d);", i + 1);
					Sleep(500);
				}
				Sleep(100);
			}

			CString strTemp = g_pMsg->msg_getstring("MyMonCanCommitString", "setmetatable(_G, {__index = QuitRelative_Env}); MyMonCanCommitString = QuitRelative_Text:GetText()").c_str();
			Sleep(500);
			//dbgPrint("服务器的连接 strTemp=%s", strTemp);
			if (strTemp.Find("服务器的连接") != -1)
			{
				g_pMsg->msg_dostring("setmetatable(_G, { __index = QuitRelative_Env }); QuitRelative_Cancel_Clicked();");
				Sleep(1000);
				return TRUE;
			}
		}
		Sleep(1000 * 30);
	}
	dbgPrint("停止领取礼物");
	return 0;
}

UINT __stdcall Task_ThreadFunc(void* pType)
{
	//++g_pMe->atomic_int_work_thread;//线程计数
	Sleep(1000);
	::SendMessage(g_pUI->m_hWnd, WM_MY_MESSAGE, 0, 0);//激活按钮
	g_pUser->UserSystemInitial();

	CStringArray listText;
	CString strTemp((char*)pType);
	SplitCStringArr(strTemp, "|", &listText);
	for (int i = 0; i < listText.GetCount(); i++)
	{
		dbgPrint("执行脚本=%s", listText.GetAt(i));
		g_pHPInit->MySendGameInfo("执行脚本" + listText.GetAt(i));
		strTemp.Format("%s%s.lua", g_pFileSystem->Script.c_str(), listText.GetAt(i).GetString());
		g_pScriptSystem->LUA_DoFile(strTemp);//执行脚本文件
	}
	return 0;
}

void CMe::EndThread()
{
	bRun = false;
	bTaskThread = false;
	bGiftThread = false;
	bInfoThread = false;
	bProtectThread = false;
	if (hTaskThread)//任务线程停止时可能要等待很久
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

	if (bProtectThread)
	{
		::CloseHandle(hProtectThread);
		hProtectThread = nullptr;
	}
}

void CMe::CreateLogin(int nNUM)//创建登录线程
{
	bGiftThread = true;
	HANDLE	hlg = (HANDLE)_beginthreadex(NULL, 0, &Login_threadFunc, (void*)nNUM, 0, NULL);
	if (hlg != NULL)
	{
		::CloseHandle(hlg);
	}
}

void CMe::CreateUI()	//创建模态对话框
{
	bUiThread = true;
	hUIThread = (HANDLE)_beginthreadex(NULL, 0, &UI_ThreadFunc, this, 0, NULL);
}

//执行任务脚本
void CMe::CreateTask(const char* tasks)//创建Task线程
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

//执行领取礼物
void CMe::CreateInfo()	//创建Task线程
{
	if (hInfoThread)
	{
		::CloseHandle(hInfoThread);
		hInfoThread = nullptr;
	}

	bInfoThread = true;
	hInfoThread = (HANDLE)_beginthreadex(nullptr, 0, &Info_ThreadFunc, 0, 0, nullptr);
}

//执行领取礼物
void CMe::CreateGift()
{
	if (hGiftThread)
	{
		::CloseHandle(hGiftThread);
		hGiftThread = nullptr;
	}

	bGiftThread = true;
	hGiftThread = (HANDLE)_beginthreadex(nullptr, 0, &Gift_ThreadFunc, 0, 0, nullptr);
}

// 创建保护线程
void CMe::CreatProtect()
{
	if (hProtectThread)//任务线程停止时可能要等待很久
	{
		::CloseHandle(hProtectThread);
		hProtectThread = nullptr;
	}
	bProtectThread = true;
	bPauseProtect = false;
	hProtectThread = (HANDLE)_beginthreadex(NULL, 0, &Protect_threadfunc, this, 0, NULL);
}

BOOL CMe::OpenShareMemory()
{
	CString szNameresult;			//共享内存名字作为标识

	GetWindowHandle();
	dbgPrint("当前窗口句柄:%x 窗口名称:%s", g_Gamehwnd, g_GameTitle);
	g_pMsg->GamehWnd = g_Gamehwnd;
	szNameresult.Format("TL_Memory_%x", g_Gamehwnd);
	TRACE("当前共享内存名字标识:%s\n", szNameresult);
	HANDLE hMapFileresult = CreateFileMapping(
		INVALID_HANDLE_VALUE,    // 物理文件句柄
		NULL,                    // 默认安全级别
		PAGE_READWRITE,          // 可读可写
		0,                       // 高位文件大小
		BUF_SIZE_RES,            // 地位文件大小
		szNameresult             // 共享内存名称
	);

	LPVOID RESULT = (LPVOID)MapViewOfFile(
		hMapFileresult,            // 共享内存的句柄
		FILE_MAP_ALL_ACCESS, // 可读写许可
		NULL,
		NULL,
		BUF_SIZE_RES);
	if (RESULT != NULL) {
		g_pMsg->mGameInfo = (TLGAMEINFO*)RESULT;
		//写入共享内存数据
		//mUsername = mtGameInfo->Account;
		//mPassword = mtGameInfo->Password;
		//dbgPrint("当前窗口账号:%s 密码:%s", mUsername, mPassword);
	}
	else {//申请共享内存失败
		UnmapViewOfFile(RESULT);
		CloseHandle(hMapFileresult);
		return false;
	}
	return true;
}


HWND GetWindowHandle()// 获取创建偶句柄
{
	DWORD dwCurrentProcessId = GetCurrentProcessId();

	if (!EnumWindows(EnumWindowsProc, (LPARAM)&dwCurrentProcessId))
	{
		return (HWND)dwCurrentProcessId;
	}

	return NULL;
}

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)// 回调函数
{
	DWORD dwCurProcessId = *((DWORD*)lParam);
	DWORD dwProcessId = 0;
	CString TempTitle;
	char WindowTitle[100] = { 0 };
	::GetWindowText(hwnd, WindowTitle, 100);
	TempTitle = WindowTitle;
	GetWindowThreadProcessId(hwnd, &dwProcessId);
	if (dwProcessId == dwCurProcessId && GetParent(hwnd) == NULL && TempTitle.Find("新天龙八部") > -1)
	{
		g_GameTitle = WindowTitle;
		g_Gamehwnd = hwnd;
		*((HWND*)lParam) = hwnd;
		return FALSE;
	}
	return TRUE;
}