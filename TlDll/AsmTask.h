#pragma once
#include "Function.h"

struct TAsmTask
{
	DWORD  dwTree;
	PCHAR  szName;
	int    nTaskID;
	int   nSchedule;
	int   nVa1;
	int   nVa2;
	int   nVa3;

	TAsmTask()
	{
		dwTree = 0;
		szName = "";
		nTaskID = -1;
		nSchedule = FALSE;
		nVa1 = -1;
		nVa2 = -1;
		nVa3 = -1;
	};
};

typedef vector<TAsmTask> VAsmTask;

class CAsmTask : public CFunction
{
public:
	CAsmTask() {};
	~CAsmTask() {};

	void GetMissionId(CString mission);
	VAsmTask AsmGetTaskData();//��ȡ��������
	void AsmTaskTraverse(TAsmTree* pTree, VAsmTask& vm_Task, DWORD* pCount);
	void AsmGetTaskInfo(TAsmTree* pTree, VAsmTask& vm_Task);

	void SelectTaskForId(int nTaskId);	//ѡ���ύ����
	void CompleteTask(int nAward = 0); //�������
	void AcceptTask(); //����
	void ContinueTask(); //����
	void CancelTask(); // ȡ��

	//ģ������
	int Task33556();
	int Task3354F();
};