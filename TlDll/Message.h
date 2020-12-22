#pragma once
#include <afxmt.h>
#include "FileSystem.h"

typedef struct _TLGameInfo {
	char Account[30];			//�˺� ע����д��
	char Password[30];			//���� ע����д��
	char Server[30];			//������
	char LuaScript[512];		//��Ҫִ�е�lua�ű� ע����д��
	char Location[20];			//��Ϸ��ͼλ��
	char RoleName[30];			//��ɫ��
	char RoleSects[20];			//��ɫ����
	char RoleState[256];		//��ɫִ�е�����״̬
	char Team[128];				//�����Ա
	int RoleIndex;				//��¼��ɫ�±� ע����д��
	int KillCount;				//ˢ������
	int RoleLv;					//��ɫ�ȼ�
	int RoleHp;					//��ɫѪ��
	bool IsInject;				//�Ƿ��Ѿ�ע��	dllд��
	DWORD ControlId;			//���Ʊ�ʶ
}TLGAMEINFO, PTLGAMEINFO;

typedef int(*Glua_dostring)(int, const char*);//do_string
typedef int(*Glua_pushstring)(int, const char*);//pushstring
typedef void(*Glua_gettable)(int, int);//gettable
typedef double(*Glua_tonumber)(int, double);//tonumber
typedef int(*Glua_settop)(int, int);//settop
typedef const char* (*Glua_tostring)(int, int);//tostring
typedef int(*Glua_pushcclosure)(int, int, int);//
typedef void(*Glua_settable)(int, int);
typedef int(*Glua_loadfile)(int, const char*);
typedef int(*Gplua_call)(int, int, int, int);
typedef int(*Glua_type)(int, int);

class CMessage
{
public:
	CMessage() {};
	~CMessage() {};

	void Init();

	DWORD GameBase = 0; //exe���̻�ַ
	HWND GamehWnd = nullptr;//��Ϸ�Ĵ��ھ��
	WNDPROC funWndProc = nullptr;
	TLGAMEINFO* mGameInfo = nullptr;	//�����ڴ��ַ

	DWORD	dwBeginTime = 0;	//��ʼʱ��
public:
	int  telua_loadfile(const char* file);
	bool telua_dostring(const char* buf);
	bool telua_getglobal(const char* name);
	const char* telua_tostring(int n);
	double telua_tonumber(int n);
	void telua_pop(int n);
	void msg_dostring(const char* _Format, ...);	//ִ��dostring
	int msg_getnumber(char* _Format, ...);//��ȡ����
	_tstring msg_getstring(const char* str_arg, char* _Format, ...);	//��ȡ�ַ���

		/************************************************************************/
	/* luaȡֵ�ĺ���                                                                     */
	/************************************************************************/
	bool telua_getnumber(const char* buf, PVOID  out);
	bool telua_getstring(const char* buf, const char* want_get_string);

private:
	int InitLuaFun();//��ȡlua�⺯��
	HWND GetGameWindow();//ͨ������id��ȡ���ھ��
	void GetGameBase(); //��ȡ���̻�ַ
	CString GetWindowTitle(HWND hwnd);
	CString MyGetClassName(HWND hwnd);
	BOOL LUAInitialize(const char* SzDriverPath);
	CCriticalSection m_State;

private:
	static LRESULT CALLBACK our_wndproc(HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam);

private:
	int getlua_state();
	void subclass_game_wndproc();		//���໯���ڹ��̺���
	void un_subclass_game_wndproc();	//ж�����໯���ڹ���

public:
	void UseItemName(_tstring itemName);
	int GetBagItemBlankNum();

public:
	bool IsWindowShow(const char* str);
	bool SelectServer(const char* ServerName);//ѡ�����
	bool SetNameText(const char* NameText);//�����ɫ��
	bool IsQueueUp(CString& strRet);//�Ƿ��Ŷ�
	bool IsQueueUpFull();//�Ƿ��Ŷ���Ա
	bool IsNpcDialog(); //�Ƿ���NPC�Ի���
	void CallInOutRide(int nValue);//�ٻ��һ����� --�ٻ�(1)���ջ�(0)����
public:
	void SetAutoRunTargetNPCName(const char* strNPCName);//��NPC�Ի���
	void QuestFrameOptionClicked(const char* strNpcOption);//NPC�Ի�ѡ��
	void QuestFrameMissionContinue();//NPC�Ի�ѡ�������ť
	void MissionContinue_Clicked();//NPC�Ի�ѡ����ɰ�ť
	void QuestFrameMissionComplete(int nRewardItemID = 0); //ѡ������Ʒ ��ɶԻ�
	void Quest_OnHidden();//NPC�Ի�ѡ��ر�
	void MissionQuestAccept_Clicked();//��������
	bool IsQuizDiaLog();
	int  GetCurMountID();//��ǰ����id
private:
	/************************************************************************/
	/* ��Ϸ��lua����ָ�붨��                                                */
	/************************************************************************/
	Glua_dostring GLua_Dostring = nullptr;
	Glua_pushstring GLua_Pushstring = nullptr;
	Glua_gettable GLua_Gettable = nullptr;
	Glua_tonumber GLua_Tonumber = nullptr;
	Glua_settop   GLua_Settop = nullptr;
	Glua_tostring GLua_Tostring = nullptr;
	Glua_pushcclosure GLua_Pushcclosure = nullptr;
	Glua_settable	GLua_Settable = nullptr;
	Glua_loadfile	GLua_Loadfile = nullptr;
	Gplua_call	  GpLua_Call = nullptr;
	Glua_type   Gplua_type = nullptr;
	int lua_state = 0;//lua״̬��ָ��
};

//��װ���ٽ����߳�ͬ������
class CriticalSectionLock
{
public:
	CriticalSectionLock(CCriticalSection* cs)
	{
		m_cs = cs;
		m_cs->Lock();
	}
	~CriticalSectionLock()
	{
		m_cs->Unlock();
	}
private:
	CCriticalSection* m_cs;
};

//�Զ���꣺�����򴰿ڹ��̷�����Ϣ������lua_dostring
#define  LUA_MEASSAGE     WM_USER + 1000
#define  LUA_GETVALUE     WM_USER + 1001
#define  LUA_GETSTRING    WM_USER + 1002           
#define  LUA_REGISTEREX	  WM_USER + 1007 