#include "pch.h"
#include "MeKillMonster.h"

extern CMain *g_pMain;
extern CUser* g_pUser;

void CMeKillMonster::Me_TimeDetectionRadius(TUserTime* ptUserTime, VUserPos vUserPos, size_t nNum)//�һ��뾶
{
	//��ѡ���ѹ�ѡ
	if (vUserPos.size() != 0)//�һ��㲻Ϊ��
	{
		//if (FUN_TimeChange(ptUserTime))//ʱ�䵽��
		//{
		//	if (role_curmap().nSceneId == vUserPos[nNum].nSceneId)
		//	{//dbgPrint(_T("�뾶��Χ���"));
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

////0��ɱ�ַ�ʽ
//BOOL CMeKillMonster::Me_KillMonster(int nMonsterType)//����ɱ��
//{
//	//�糡��(pAsmRole, return TRUE)
//		//if (pUser->nKillByName_dlg2_check/*UserKillByName()*/)
//		//{
//		//	if (pUser->vtMonsterName.size() == 0)
//		//		pUser->vtMonsterName = pUser->UserGetMonsterName();//��ȡ��������
//		//	else
//		//		return FUN_KillMonsterByName(pUser->vtMonsterName, nMonsterType, 1);//ָ������ɱ��
//		//}
//		//else
//
//	return FUN_KillAnyMonster(nMonsterType, 1);//ɱ�����
//}

UINT __stdcall KillMonster_ThreadFunc(LPVOID p)
{

	g_pUser->UserSystemInitial();

	size_t nNum = 0;
	TUserTime tUserRadiusTime;
	VUserPos vUserPos = g_pUser->vtKillPos;//�һ�����Ϣ

	CMeKillMonster* pMeKill = new CMeKillMonster();

	while (g_pMain->bKillMonsterThread) {
		//�뾶���
		pMeKill->Me_TimeDetectionRadius(&tUserRadiusTime, vUserPos, nNum);

		Sleep(50);
	}
	
	return 0;
}