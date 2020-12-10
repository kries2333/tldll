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
	VAsmTask AsmGetTaskData();//获取任务数据
	void AsmTaskTraverse(TAsmTree* pTree, VAsmTask& vm_Task, DWORD* pCount);
	void AsmGetTaskInfo(TAsmTree* pTree, VAsmTask& vm_Task);

	void SelectTaskForId(int nTaskId);	//选择提交任务
	void CompleteTask(int nAward = 0); //完成任务
	void AcceptTask(); //接受
	void ContinueTask(); //继续
	void CancelTask(); // 取消

	//模拟任务
	int Task33556();
	int Task3354F();
};