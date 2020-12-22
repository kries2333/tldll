#pragma once
#define BUF_SIZE_RES 1025

class CMe
{
public:
	void CreateLogin(int nNUM);//������¼�߳�

public:
	HMODULE hDll;//ģ����
	
public:
	void EndThread();
	friend UINT __stdcall UI_ThreadFunc(LPVOID p);//UI�̺߳���
	void CreateUI();//����UI
	friend UINT __stdcall Task_ThreadFunc(void* pType);
	void CreateTask(const char* tasks);//����Task�߳�
	void CreateGift();
	void CreateInfo();

	friend UINT __stdcall Protect_threadfunc(void* p);//�����̺߳���
	void CreatProtect();//���������߳�

	BOOL OpenShareMemory();	//�򿪹����ڴ�

public:
	HANDLE hUIThread;
	HANDLE hKillMonsterThread;
	HANDLE hTaskThread;
	HANDLE hGiftThread;
	HANDLE hInfoThread;
	HANDLE hProtectThread;
private:

	bool bUiThread;
	bool bKillMonsterThread;


public:
	bool bRun;
	bool bTaskThread;//�����̱߳�־
	bool bGiftThread;
	bool bInfoThread;
	bool bProtectThread;
	bool bPauseProtect;	//���ƶ���ʱ����ͣ��ҩ��������ֹ����ͣͣ
};

HWND GetWindowHandle();// ��ȡ����ż���
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);