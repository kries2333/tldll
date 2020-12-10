#pragma once
#define BUF_SIZE_RES 1025

class CMain
{
public:
	void CreateLogin(int nNUM);//创建登录线程

public:
	HMODULE hDll;//模块句柄
	
public:
	void EndThread();
	friend UINT __stdcall UI_ThreadFunc(LPVOID p);//UI线程函数
	void CreateUI();//创建UI
	friend UINT __stdcall KillMonster_ThreadFunc(LPVOID p);//杀怪线程函数
	void CreateKillMonster();//创建杀怪线程
	friend UINT __stdcall Task_ThreadFunc(void* pType);
	void CreateTask(const char* tasks);//创建Task线程
	void CreateGift();
	void CreateInfo();
	BOOL OpenShareMemory();	//打开共享内存

private:
	HANDLE hUIThread;
	HANDLE hKillMonsterThread;
	HANDLE hTaskThread;
	HANDLE hGiftThread;
	HANDLE hInfoThread;
private:

	bool bUiThread;
	bool bKillMonsterThread;


public:
	bool bRun;
	bool bTaskThread;//任务线程标志
	bool bGiftThread;
	bool bInfoThread;
};

HWND GetWindowHandle();// 获取创建偶句柄
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);