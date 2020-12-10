#pragma once
#define BUF_SIZE_RES 1025

class CMain
{
public:
	void CreateLogin(int nNUM);//������¼�߳�

public:
	HMODULE hDll;//ģ����
	
public:
	void EndThread();
	friend UINT __stdcall UI_ThreadFunc(LPVOID p);//UI�̺߳���
	void CreateUI();//����UI
	friend UINT __stdcall KillMonster_ThreadFunc(LPVOID p);//ɱ���̺߳���
	void CreateKillMonster();//����ɱ���߳�
	friend UINT __stdcall Task_ThreadFunc(void* pType);
	void CreateTask(const char* tasks);//����Task�߳�
	void CreateGift();
	void CreateInfo();
	BOOL OpenShareMemory();	//�򿪹����ڴ�

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
	bool bTaskThread;//�����̱߳�־
	bool bGiftThread;
	bool bInfoThread;
};

HWND GetWindowHandle();// ��ȡ����ż���
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);