#include "pch.h"
#include "MeKillMonster.h"

extern CMain *g_pMain;
extern CUser* g_pUser;

void CMeKillMonster::Me_TimeDetectionRadius(TUserTime* ptUserTime, VUserPos vUserPos, size_t nNum)//挂机半径
{
	//复选框已勾选
	if (vUserPos.size() != 0)//挂机点不为空
	{
		//if (FUN_TimeChange(ptUserTime))//时间到了
		//{
		//	if (role_curmap().nSceneId == vUserPos[nNum].nSceneId)
		//	{//dbgPrint(_T("半径范围检查"));
		//		if (FUN_GetDistance((float)vUserPos[nNum].nPosX, (float)vUserPos[nNum].nPosY) >
		//			(float)vUserPos[nNum].nPosR)
		//		{
		//			FUN_RunToTargetEx((float)vUserPos[nNum].nPosX, (float)vUserPos[nNum].nPosY,
		//				vUserPos[nNum].nSceneId);
		//		}
		//	}
		//}
	}
}

////0号杀怪方式
//BOOL CMeKillMonster::Me_KillMonster(int nMonsterType)//正常杀怪
//{
//	//跨场景(pAsmRole, return TRUE)
//		//if (pUser->nKillByName_dlg2_check/*UserKillByName()*/)
//		//{
//		//	if (pUser->vtMonsterName.size() == 0)
//		//		pUser->vtMonsterName = pUser->UserGetMonsterName();//获取怪物名单
//		//	else
//		//		return FUN_KillMonsterByName(pUser->vtMonsterName, nMonsterType, 1);//指定名称杀怪
//		//}
//		//else
//
//	return FUN_KillAnyMonster(nMonsterType, 1);//杀任意怪
//}

UINT __stdcall KillMonster_ThreadFunc(LPVOID p)
{

	g_pUser->UserSystemInitial();

	size_t nNum = 0;
	TUserTime tUserRadiusTime;
	VUserPos vUserPos = g_pUser->vtKillPos;//挂机点信息

	CMeKillMonster* pMeKill = new CMeKillMonster();

	while (g_pMain->bKillMonsterThread) {
		//半径检测
		pMeKill->Me_TimeDetectionRadius(&tUserRadiusTime, vUserPos, nNum);

		Sleep(50);
	}
	
	return 0;
}