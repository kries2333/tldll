#include "pch.h"
#include "AsmTask.h"
#include "FileSystem.h"
#include "Function.h"
#include "Message.h"
#include "Base.h"

extern CMessage* g_pMsg;
extern CAsmTask* g_pTask;
extern DWORD g_GameExeBase;

VAsmTask CAsmTask::AsmGetTaskData()
{
	VAsmTask vm_Task;
	try
	{
		//DWORD dwTree = 0;
		//DWORD data = *(DWORD*)(g_GameExeBase + TASK_INFO);
		//data = *(DWORD*)(data + 0x4);
		//DWORD dwCount = 0;
		//AsmTaskTraverse((TAsmTree*)data, vm_Task, &dwCount);
	}
	catch (...)
	{
		//dPrintA(__FUNCTION__);
	}

	return vm_Task;
}

void CAsmTask::AsmTaskTraverse(TAsmTree* pTree, VAsmTask& vm_Task, DWORD* pCount)//先序遍历  
{
	*pCount += 1;
	if (*pCount > 20)
	{
		return;
	}
	try
	{
		BYTE byData = -1;
		byData = *(BYTE*)((UINT)pTree + 0xD);
		if (byData == 0)
		{
			if (IsBadReadPtr(pTree, 4) == 0)
				this->AsmGetTaskInfo((PTAsmTree)pTree, vm_Task);//取信息

			if (IsBadReadPtr(&(pTree->left), 4) == 0)
				this->AsmTaskTraverse((PTAsmTree)pTree->left, vm_Task, pCount);//左递归

			if (IsBadReadPtr(pTree->right, 4) == 0)
				this->AsmTaskTraverse((PTAsmTree)pTree->right, vm_Task, pCount);//右递归
		}
	}
	catch (...)
	{
		//dPrintA(__FUNCTION__);
	}

}

void CAsmTask::AsmGetTaskInfo(TAsmTree* pTree, VAsmTask& vm_Task)
{
	try
	{
		TAsmTask tTask;
		dbgPrint("AsmGetTaskInfo pTree = %X", pTree);
		if (pTree != 0)
		{
			//////////////////////////////////////////////////////////////////////////
			tTask.szName = (PCHAR)((DWORD)pTree + 0xC0);
			tTask.nTaskID = *(DWORD*)((DWORD)pTree + 0x10);
			
			dbgPrint("AsmGetTaskInfo szName=%s nTaskID=%X", tTask.szName, tTask.nTaskID);
			vm_Task.push_back(tTask);
		}
	}
	catch (...)
	{
		//dPrintA(__FUNCTION__);
	}
}

void CAsmTask::GetMissionId(CString mission)
{
	CString strInfo = g_pMsg->msg_getstring("g_GetValue", "g_GetValue = GetAvailableMissionList()").c_str();
	dbgPrint("GetMissionId=%s", strInfo);
}

void CAsmTask::SelectTaskForId(int nTaskId)
{
	//DWORD taskCall = (g_GameExeBase + TASK_SELECT_CALL);
	//DWORD unEcx = (g_GameExeBase + TASK_ECX);
	//_asm {
	//	mov edx, nTaskId
	//	push 0xFFFFFFFF
	//	push edx
	//	push 0xFFFFFFFF
	//	mov edx, unEcx
	//	mov ecx, [edx]
	//	call taskCall
	//	xor eax, eax
	//}

}

// 完成
void CAsmTask::CompleteTask(int nAward)
{
	//DWORD unEcx = (g_GameExeBase + TASK_ECX);
	//DWORD taskCall = (g_GameExeBase + TASK_COMPLETE_CALL);
	//dbgPrint("CompleteTask taskCall=%X unEcx=%X nAward=%d", taskCall, unEcx, nAward);
	//_asm {
	//	mov ebx, unEcx
	//	mov ecx, [ebx]
	//	push nAward
	//	mov eax, taskCall
	//	call eax
	//	xor eax,eax
	//}
}

// 004A4DC0    8B0D DC3F7200   MOV     ECX, DWORD PTR DS:[0x723FDC]
// 004A4DC6    E8 4553F9FF     CALL    Game.0043A110; 接受任务CALL
void CAsmTask::AcceptTask()
{
	//DWORD unEcx = (g_GameExeBase + TASK_ECX);
	//DWORD taskCall = (g_GameExeBase + TASK_ACCEPT_CALL);
	//dbgPrint("AcceptTask taskCall=%X unEcx=%X", taskCall, unEcx);
	//_asm {
	//	mov ebx, unEcx
	//	mov ecx, [ebx]
	//	mov eax, taskCall
	//	call eax
	//	xor eax, eax
	//}
}

void CAsmTask::ContinueTask()
{
	//DWORD unEcx = (g_GameExeBase + TASK_OFFER);
	//DWORD taskCall = (g_GameExeBase + TASK_CONTINUE_CALL);
	//dbgPrint("AcceptTask taskCall=%X unEcx=%X", taskCall, unEcx);
	//_asm {
	//	mov ebx, unEcx
	//	mov ecx, [ebx]
	//	mov eax, taskCall
	//	call eax
	//}
}

void CAsmTask::CancelTask()
{
	//DWORD unEcx = (g_GameExeBase + TASK_OFFER);
	//DWORD taskCall = (g_GameExeBase + TASK_CANCEL_CALL);
	//dbgPrint("AcceptTask taskCall=%X unEcx=%X", taskCall, unEcx);
	//_asm {
	//	mov ebx, unEcx
	//	mov ecx, [ebx]
	//	mov eax, taskCall
	//	call eax
	//}
}