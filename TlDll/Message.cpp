#include "pch.h"
#include "Message.h"
#include "Base.h"
#include "AsmRole.h"
#include "AsmItem.h"
#include "AsmTask.h"
#include "Me.h"
#include "resource.h"

extern CMe* g_pMe;
extern CMessage* g_pMsg;
extern CAsmRole* g_pAsmRole;
extern CAsmItem* g_pAsmItem;
extern CAsmTask* g_pAsmTask;
extern CAsmSkill* g_pAsmSkill;

void CMessage::Init()
{
	GetGameBase();
	GetGameWindow();
	InitLuaFun();
	subclass_game_wndproc();
	lua_state = getlua_state();//��ȡ��Ϸlua״̬��ָ��
	//����system.lua
	char lpBuffer[MAX_PATH] = { 0 };
	GetSystemDirectory(lpBuffer, MAX_PATH);
	CString strPAth("C:");
	CString strPAth2;
	strPAth2.Format("\\Client%x.cfg", GetCurrentProcessId());
	strPAth += strPAth2;
	//dbgPrint(lpBuffer);
	LUAInitialize(strPAth);
	dwBeginTime = GetTickCount64();
}

int CMessage::InitLuaFun()//��ȡlua�⺯��
{

	HMODULE LuaPlus = GetModuleHandleA("LuaPlus.dll");
	
	if (LuaPlus)
	{
		GLua_Dostring = (Glua_dostring)GetProcAddress(LuaPlus, "lua_dostring");
		GLua_Gettable = (Glua_gettable)GetProcAddress(LuaPlus, "lua_gettable");
		GLua_Pushstring = (Glua_pushstring)GetProcAddress(LuaPlus, "lua_pushstring");
		GLua_Tonumber = (Glua_tonumber)GetProcAddress(LuaPlus, "lua_tonumber");
		GLua_Tostring = (Glua_tostring)GetProcAddress(LuaPlus, "lua_tostring");
		GLua_Settop = (Glua_settop)GetProcAddress(LuaPlus, "lua_settop");
		GpLua_Call = (Gplua_call)GetProcAddress(LuaPlus, "lua_pcall");
		GLua_Settable = (Glua_settable)GetProcAddress(LuaPlus, "lua_settable");
		GLua_Pushcclosure = (Glua_pushcclosure)GetProcAddress(LuaPlus, "lua_pushcclosure");
		GLua_Loadfile = (Glua_loadfile)GetProcAddress(LuaPlus, "luaL_loadfile");
		Gplua_type = (Glua_type)GetProcAddress(LuaPlus, "lua_type");


		if (GLua_Dostring && GLua_Gettable && GLua_Pushstring
			&& GLua_Tonumber && GLua_Tostring && GLua_Settop)
		{
			return 1;//��ȡlua������ַ�ɹ�
		}
		else
		{
			return 0;//��ȡ��ַʧ�ܣ�ģ������ȡ�ɹ�
		}
	}
	return -1;//��Чģ����
}

void CMessage::GetGameBase()
{
	GameBase = (DWORD)GetModuleHandle("Game.exe");
}

HWND CMessage::GetGameWindow()//ͨ������id��ȡ���ھ��
{
	while (true)
	{
		DWORD dwCurPId = ::GetCurrentProcessId();
		HWND  hwnd = ::GetTopWindow(NULL);//��ȡ���㴰�ھ��
		while (hwnd)
		{
			DWORD dwPId = 0;
			::GetWindowThreadProcessId(hwnd, &dwPId);//�ҳ�ĳ�����ڵĴ������̻߳���̷��ش����ߵı�־
			if (dwCurPId == dwPId)
			{
				if (::GetParent(hwnd) == 0)//GetParent�����������Ƿ��и�����
				{
					if (MyGetClassName(hwnd) == "TianLongBaBuHJ WndClass")//�Աȴ��������ǲ�����Ϸ����
					{
						GamehWnd = hwnd;
						return GamehWnd;
					}
				}
			}
			hwnd = ::GetNextWindow(hwnd, GW_HWNDNEXT);//��ȡ�¸����ھ��
		}
		Sleep(1000);
	}

	return NULL;
}

BOOL CMessage::LUAInitialize(const char* SzDriverPath)
{
	// ���Զ�����Դ���ͷų�sys
	HRSRC hResc = FindResource(g_pMe->hDll, MAKEINTRESOURCE(IDR_RCDATA2), RT_RCDATA);
	if (hResc == NULL)
	{
		dbgPrint("GetLastError  ==   %d", ::GetLastError());
		return FALSE;
	}

	DWORD dwImageSize = SizeofResource(g_pMe->hDll, hResc);

	HGLOBAL hResourecImage = LoadResource(g_pMe->hDll, hResc);
	if (hResourecImage == NULL)
	{
		return FALSE;
	}
	PVOID pMemory = LockResource(hResourecImage);
	if (pMemory == NULL)
	{
		return FALSE;
	}

	HANDLE hFile = CreateFile(SzDriverPath, GENERIC_WRITE, FILE_SHARE_READ,
		NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	DWORD dwByteWrite;
	if (!WriteFile(hFile, pMemory, dwImageSize, &dwByteWrite, NULL))
	{
		CloseHandle(hFile);
		return FALSE;
	}

	if (dwByteWrite != dwImageSize)
	{
		CloseHandle(hFile);
		return FALSE;
	}
	::CloseHandle(hFile);
	Sleep(1000);
	::SendMessage(GamehWnd, LUA_REGISTEREX, 0, (LPARAM)SzDriverPath);//ע��lua������lua_lib
	DeleteFile(SzDriverPath);

	return TRUE;
}

CString CMessage::MyGetClassName(HWND hwnd)
{//��ȡ��������
	CString strname;
	GetClassName(hwnd, strname.GetBuffer(MAX_PATH), MAX_PATH);
	strname.ReleaseBuffer();

	return strname;
}

CString CMessage::GetWindowTitle(HWND hwnd)
{//��ȡ���ڱ���

	CString CStitle;
	::GetWindowText(hwnd, CStitle.GetBuffer(MAX_PATH), MAX_PATH);
	CStitle.ReleaseBuffer();
	CStitle = CStitle.Left(14);

	return CStitle;
}

bool CMessage::IsWindowShow(const char* str)
{
	for (size_t i = 0; i < 5; i++)
	{
		if (msg_getnumber("g_GetValue = myIsWindowShow(\"%s\")", str) == 1)
		{
			return true;
		}
		Sleep(200);
	}

	return false;
}

bool CMessage::IsNpcDialog()
{
	dbgPrint("IsNpcDialog");
	for (size_t i = 0; i < 5; i++)
	{
		if (msg_getnumber("g_GetValue = IsNpcDialog()") == 1)
		{
			dbgPrint("IsNpcDialog=%d", 1);
			return true;
		}
		Sleep(200);
	}
	dbgPrint("IsNpcDialog=%d", 0);
	return false;
}

bool CMessage::IsQuizDiaLog()
{
	for (size_t i = 0; i < 5; i++)
	{
		if (msg_getnumber("g_GetValue = IsQuizDialog()") == 1)
		{
			return true;
		}
		Sleep(200);
	}

	return false;
}

bool CMessage::SelectServer(const char* ServerName)//ѡ�����
{

	msg_dostring("SelectServer_my(\"%s\")", ServerName);

	return true;
}

bool CMessage::SetNameText(const char* NameText)
{
	UINT _length = strlen(NameText);
	for (UINT i = 0; i < _length; i++)
	{
		if (NameText[i] < 0)//����ռ2�ֽ�
		{
			WPARAM wChar = (BYTE)NameText[i];
			wChar <<= 8;//��8λ
			wChar |= (BYTE)NameText[++i];//��8λ
			::SendMessageA(GamehWnd, WM_IME_CHAR, wChar, NULL);
			Sleep(10);
		}
		else
		{
			WPARAM wChar = (BYTE)NameText[i];
			::SendMessageA(GamehWnd, WM_IME_CHAR, wChar, NULL);
			Sleep(10);
		}
	}

	return true;
}

bool CMessage::IsQueueUp(CString& queueInfo)
{
	CString retStr;
	CString index;
	CString time;
	retStr = msg_getstring("mygetServeroText", "setmetatable(_G, {__index=SelectBetterServer_Env});if this:IsVisible() then mygetServeroText = SelectBetterServer_InfoWindow:GetText() else mygetServeroText = \"-1\" end;").c_str();
	int loc = retStr.Find("Ŀǰ����");
	int loc2 = retStr.Find("Ԥ��ʱ��");
	if ((loc2 != -1) && (loc != -1))
	{
		loc = retStr.Find("?");
		loc2 = retStr.Find("��", loc + 1);
		index = retStr.Mid(loc + 1, loc2 - loc + 1);

		loc = retStr.Find("ʱ��");
		loc2 = retStr.ReverseFind('?');
		time = retStr.Mid(loc + 8, loc2 - loc + 8);
		queueInfo.Format("��ǰ�Ŷ�%s,����%s", index, time);
		dbgPrint(queueInfo);
		return true;

	}
	else {
		return false;
	}
}

bool CMessage::IsQueueUpFull()
{
	return false;
}

void CMessage::UseItemName(_tstring itemName)
{
	for (int i = 0; i < 20; i++)
	{
		CString strTemp = g_pMsg->msg_getstring("ItemName", " ItemName = PlayerPackage:GetBagItemName(%d)", i).c_str();
		if (strTemp.Find(itemName.c_str(), 0) != -1) {
			g_pMsg->msg_dostring("PlayerPackage:UseItem(%d)", i);
			Sleep(1000);
			return;
		}
		Sleep(100);
	}
}


//==========================================================================
////////////////////////////////NPC�Ի�////////////////////////////////////

void CMessage::SetAutoRunTargetNPCName(const char* strNPCName)//��NPC�Ի���
{
	dbgPrint("SetAutoRunTargetNPCName %s", strNPCName);

	CString strTemp(strNPCName);
	if (strTemp.Find("����") > -1)
	{
		g_pAsmRole->AsmNpcDialogue(0x80);
	}
	else
	{
		msg_dostring("setmetatable(_G, {__index = Quest_Env});SetAutoRunTargetNPCName(\"%s\");", strNPCName);
	}
	Sleep(1000);
}

void CMessage::QuestFrameMissionContinue()//NPC�Ի�ѡ�������ť
{
	CString strTemp = g_pMsg->msg_getstring("g_GetValue", "setmetatable(_G, {__index = Quest_Env}); g_GetValue = Quest_Button_Continue:GetText()").c_str();
	//dbgPrint("QuestFrameMissionContinue strTemp=%s", strTemp);
	if (strTemp.Find("����") != -1) 
	{
		msg_dostring("setmetatable(_G, {__index = Quest_Env});QuestFrameMissionContinue();");
	}
	
}

void CMessage::MissionContinue_Clicked()//NPC�Ի�ѡ����ɰ�ť
{
	msg_dostring("setmetatable(_G, {__index = Quest_Env});MissionContinue_Clicked();");
}

void CMessage::QuestFrameMissionComplete(int nRewardItemID) //ѡ������Ʒ ��ɶԻ�
{
	CString strTemp = g_pMsg->msg_getstring("g_GetValue", "setmetatable(_G, {__index = Quest_Env}); g_GetValue = Quest_Button_Continue:GetText()").c_str();
	//dbgPrint("QuestFrameMissionComplete strTemp=%s", strTemp);
	if (strTemp.Find("���") != -1)
	{
		//g_pAsmTask->CompleteTask(nRewardItemID);
		msg_dostring("QuestFrameMissionComplete(%d);", nRewardItemID);
	}
}

void CMessage::Quest_OnHidden()//NPC�Ի�ѡ��ر�
{
	msg_dostring("setmetatable(_G, { __index = Quest_Env });this:Hide();Quest_OnHidden();");
}

void CMessage::MissionQuestAccept_Clicked()//��������
{
	CString strTemp = g_pMsg->msg_getstring("g_GetValue", "setmetatable(_G, {__index = Quest_Env}); g_GetValue = Quest_Button_Accept:GetText()").c_str();
	//dbgPrint("MissionQuestAccept_Clicked strTemp=%s", strTemp);
	if (strTemp.Find("����") != -1)
	{
		//g_pAsmTask->AcceptTask();
		msg_dostring("setmetatable(_G, {__index = Quest_Env});QuestAccept_Clicked();");
	}
	
}

void CMessage::QuestFrameOptionClicked(const char* strNpcOption)//NPC�Ի�ѡ��
{
	msg_dostring("myclickmis(\"%s\")", strNpcOption);//msg��lua��Ϣ����
}

int CMessage::GetCurMountID()//��ǰ����id
{
	return	msg_getnumber("g_GetValue = Player:GetMountID();");
}

//==========================================================================

int CMessage::getlua_state()
{
	int L = 0;
	DWORD base = GameBase + LLUASTATE_BASE;
	try
	{
		while (!L)
		{
			CString CStitle = GetWindowTitle(GamehWnd);
			if (CStitle.Find(_T("���������˲���"), 0) != -1)
			{
				__asm
				{
					mov edx, base;
					mov edi, [edx];
					mov eax, [edi];
					call dword ptr ds:[eax + 0x3c];
					mov eax, [eax];
					mov  L, eax;
				}
				dbgPrint("L  %x", L);
			}
			Sleep(500);
		}
	}
	catch (...)
	{
		L = -1;
		dbgPrint(__FUNCTION__);
	}

	return L;
}

bool CMessage::telua_getstring(const char* buf, const char* want_get_string)
{
	__try {
		if (telua_dostring(buf))
		{
			if (telua_getglobal(want_get_string))
			{
				const char* str = telua_tostring(-1);
				_stprintf((char*)buf, str);
				telua_pop(1);
				return true;
			}
			else
			{
				dbgPrint("%s telua_getglobal erro", __FUNCTION__);
				return false;
			}
		}
		else
		{
			dbgPrint("%s telua_dostring erro", __FUNCTION__);
			return false;
		}
	}
	__except (1) {
		dbgPrint(__FUNCTION__); return false;
	}
}

bool CMessage::telua_getglobal(const char* name)
{
	__try {
		GLua_Pushstring(lua_state, name);
		GLua_Gettable(lua_state, -10001);
		return true;
	}
	__except (1) { dbgPrint(__FUNCTION__); return false; }
}

bool CMessage::telua_dostring(const char* buf)
{
	__try {
		//dbgPrint(buf);
		GLua_Dostring(lua_state, buf);
		return true;
	}
	__except (1) { dbgPrint(__FUNCTION__); return false; }
}

double CMessage::telua_tonumber(int n)
{
	__try {
		return GLua_Tonumber(lua_state, n);
	}
	__except (1) {
		dbgPrint(__FUNCTION__);
		return 1.234567;
	}
}

bool CMessage::telua_getnumber(const char* buf, PVOID out)
{
	__try {
		if (telua_dostring(buf))
		{
			if (telua_getglobal("g_GetValue"))
			{
				*(int*)out = (int)telua_tonumber(-1);
				telua_pop(1);
				return true;
			}
			else
			{
				dbgPrint("%s telua_getglobal erro", __FUNCTION__);
				return false;
			}
		}
		else
		{
			dbgPrint("%s telua_dostring erro", __FUNCTION__);
			return false;
		}
	}
	__except (1) {
		dbgPrint(__FUNCTION__); return false;
	}
}

void CMessage::telua_pop(int n)
{
	__try {
		GLua_Settop(lua_state, -(n)-1);
	}
	__except (1) {
		dbgPrint(__FUNCTION__);
	}
}

const char* CMessage::telua_tostring(int n)
{
	__try {
		return GLua_Tostring(lua_state, n);
	}
	__except (1) {
		dbgPrint(__FUNCTION__);
		return "ERROR :lua_Tostring";
	}
}

int CMessage::telua_loadfile(const char* file)
{
	__try {
		if (GLua_Loadfile(lua_state, file) || GpLua_Call(lua_state, 0, 0, 0))
		{
			return 1;
		}
		return 0;
	}
	__except (1) { dbgPrint(__FUNCTION__); return -1; }
}

void CMessage::msg_dostring(const char* _Format, ...)//ִ��dostring
{
	CriticalSectionLock lock(&m_State);

	try
	{
		TCHAR do_stringBuf[2048] = { 0 };//���ڴ��do_string���ַ���������
		va_list list;
		va_start(list, _Format);
		vsprintf(do_stringBuf, _Format, list);
		va_end(list);
		//dbgPrint("msg_dostring=%s", do_stringBuf);
		::SendMessage(GamehWnd, LUA_MEASSAGE, 0, (LPARAM)do_stringBuf);
	}
	catch (...)
	{
		//dbgPrint(__FUNCTION__);
	}
}

int CMessage::msg_getnumber(char* _Format, ...)//��ȡ����ֵ
{
	CriticalSectionLock lock(&m_State);

	int nValue = 0;
	try
	{
		TCHAR getnumberBuf[2048] = { 0 };//���ڴ��getnumber���ַ���������
		va_list list;
		va_start(list, _Format);
		vsprintf(getnumberBuf, _Format, list);
		va_end(list);
		//dbgPrint(getnumberBuf);
		::SendMessage(GamehWnd, LUA_GETVALUE, (WPARAM)&nValue, (LPARAM)getnumberBuf);
	}
	catch (...)
	{
		dbgPrint(__FUNCTION__);
	}

	return nValue;
}

_tstring CMessage::msg_getstring(const char* str_arg, char* _Format, ...)//��ȡ�ַ���
{
	CriticalSectionLock lock(&m_State);
	_tstring str;
	try
	{
		TCHAR getstringBuf[2048] = { 0 };//���ڴ��getstring���ַ���������
		va_list list;
		va_start(list, _Format);
		vsprintf(getstringBuf, _Format, list);
		va_end(list);
		//dbgPrint(getstringBuf);
		::SendMessage(GamehWnd, LUA_GETSTRING, (WPARAM)str_arg, (LPARAM)getstringBuf);
		str = getstringBuf;
	}
	catch (...)
	{
		dbgPrint(__FUNCTION__);
	}

	return str;
}

void CMessage::subclass_game_wndproc()//���໯���ڹ��̺���
{
	if (GamehWnd)
	{
		funWndProc = (WNDPROC)::SetWindowLong(GamehWnd, GWL_WNDPROC, (LONG)our_wndproc);
		dbgPrint(_T("���໯����"));
	}
}

void CMessage::un_subclass_game_wndproc()//ж�����໯���ڹ���
{
	if (GamehWnd && funWndProc)
	{
		//dbgPrint(_T("ж�����໯����"));
		msg_dostring("UNhookHuoyuehaoli_OK()");
		::SetWindowLong(GamehWnd, GWL_WNDPROC, (LONG)funWndProc);
	}
}

//�����Լ��Ĵ��ڹ��̺���
LRESULT CALLBACK CMessage::our_wndproc(HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam)
{
	if (wMsg == LUA_MEASSAGE)
	{
		g_pMsg->telua_dostring((char*)lParam);
	}
	else if (wMsg == LUA_GETVALUE)
	{
		g_pMsg->telua_getnumber((char*)lParam, (PVOID)wParam);
	}
	else if (wMsg == LUA_GETSTRING)
	{
		g_pMsg->telua_getstring((char*)lParam, (char*)wParam);
	}
	else if (wMsg == LUA_REGISTEREX)//����lua lib�ļ�
	{
		g_pMsg->telua_loadfile((char*)lParam);
		//g_pMsg->telua_register("myluaregstertogame", (int)myluaregstertogame);
		//g_pMsg->telua_register("myluadbgprint", (int)myluadbgprint);
	}

	////////////////////////////////////////////////////////////////////////////
	///////////////////////////////�������ڵ���ʾ���//////////////////////////

	//if (wMsg == WM_KEYDOWN && wParam == VK_HOME//�ж��ȼ���ֵ
	//	&& pUI && pUI->m_hWnd && ::IsWindow(pUI->m_hWnd))//�жϴ����Ƿ񴴽�����
	//{
	//	if (::IsWindowVisible(pUI->m_hWnd))//�жϴ���״̬�ɼ��������������
	//	{
	//		::ShowWindow(pUI->m_hWnd, SW_HIDE);
	//		//::SetWindowPos(pUI->m_hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE || SWP_NOSIZE);
	//	}
	//	else
	//	{
	//		::SendMessage(pUI->m_hWnd, WM_MYSTYLEEX_MESSAGE, 0, 0);//�����������ڣ����ö�
	//	}
	//}
	////////////////////////////////////////////////////////////////////////////

	return ::CallWindowProc(g_pMsg->funWndProc, hWnd, wMsg, wParam, lParam);
}

void CMessage::CallInOutRide(int nValue)//�ٻ��һ�����
{
	if (nValue == 0)
	{
		if (GetCurMountID() != -1)
		{
			g_pAsmSkill->AsmUseSkillCall(-1, 0x15);
			Sleep(1000);
		}
	}
	else if (nValue == 1)
	{
		if (GetCurMountID() == -1)//��ǰ�������״̬
		{
			g_pAsmSkill->AsmUseSkillCall(-1, 0x15);
			//CMeKill MeKill;
			//MeKill.FUN_WaitTime(5);
			Sleep(5000);
		}
	}
}