#pragma once
#include <afxmt.h>
#include "FileSystem.h"

typedef struct _TLGameInfo {
	char Account[30];			//账号 注入器写入
	char Password[30];			//密码 注入器写入
	char Server[30];			//服务器
	char LuaScript[512];		//需要执行的lua脚本 注入器写入
	char Location[20];			//游戏地图位置
	char RoleName[30];			//角色名
	char RoleSects[20];			//角色门派
	char RoleState[256];		//角色执行的任务状态
	char Team[128];				//队伍成员
	char JoinSects[20];			//加入的门派
	int RoleIndex;				//登录角色下标 注入器写入
	int KillCount;				//刷怪数量
	int RoleLv;					//角色等级
	int RoleHp;					//角色血量
	bool IsInject;				//是否已经注入	dll写入
	DWORD ControlId;			//控制标识
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

	DWORD GameBase = 0; //exe进程基址
	HWND GamehWnd = nullptr;//游戏的窗口句柄
	WNDPROC funWndProc = nullptr;
	TLGAMEINFO* mGameInfo = nullptr;	//共享内存地址

	DWORD	dwBeginTime = 0;	//开始时间
public:
	int  telua_loadfile(const char* file);
	bool telua_dostring(const char* buf);
	bool telua_getglobal(const char* name);
	const char* telua_tostring(int n);
	double telua_tonumber(int n);
	void telua_pop(int n);
	void msg_dostring(const char* _Format, ...);	//执行dostring
	int msg_getnumber(char* _Format, ...);//获取数字
	_tstring msg_getstring(const char* str_arg, char* _Format, ...);	//获取字符串

		/************************************************************************/
	/* lua取值的函数                                                                     */
	/************************************************************************/
	bool telua_getnumber(const char* buf, PVOID  out);
	bool telua_getstring(const char* buf, const char* want_get_string);

private:
	int InitLuaFun();//获取lua库函数
	HWND GetGameWindow();//通过进程id获取窗口句柄
	void GetGameBase(); //获取进程基址
	CString GetWindowTitle(HWND hwnd);
	CString MyGetClassName(HWND hwnd);
	BOOL LUAInitialize(const char* SzDriverPath);
	CCriticalSection m_State;

private:
	static LRESULT CALLBACK our_wndproc(HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam);

private:
	int getlua_state();
	void subclass_game_wndproc();		//子类化窗口过程函数
	void un_subclass_game_wndproc();	//卸载子类化窗口过程

public:
	void UseItemName(_tstring itemName);
	int GetBagItemBlankNum();

public:
	bool IsWindowShow(const char* str);
	bool SelectServer(const char* ServerName);//选择大区
	bool SetNameText(const char* NameText);//输入角色名
	bool IsQueueUp(CString& strRet);//是否排队
	bool IsQueueUpFull();//是否排队满员
	bool IsNpcDialog(); //是否是NPC对话框
	void CallInOutRide(int nValue);//召唤找回坐骑 --召唤(1)，收回(0)坐骑
public:
	void SetAutoRunTargetNPCName(const char* strNPCName);//打开NPC对话框
	void QuestFrameOptionClicked(const char* strNpcOption);//NPC对话选项
	void QuestFrameMissionContinue();//NPC对话选项继续按钮
	void MissionContinue_Clicked();//NPC对话选项完成按钮
	void QuestFrameMissionComplete(int nRewardItemID = 0); //选择奖励物品 完成对话
	void Quest_OnHidden();//NPC对话选项关闭
	void MissionQuestAccept_Clicked();//接受任务
	bool IsQuizDiaLog();
	int  GetCurMountID();//当前坐骑id
private:
	/************************************************************************/
	/* 游戏的lua函数指针定义                                                */
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
	int lua_state = 0;//lua状态机指针
};

//封装的临界区线程同步的类
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

//自定义宏：用于向窗口过程发送消息，调用lua_dostring
#define  LUA_MEASSAGE     WM_USER + 1000
#define  LUA_GETVALUE     WM_USER + 1001
#define  LUA_GETSTRING    WM_USER + 1002           
#define  LUA_REGISTEREX	  WM_USER + 1007 