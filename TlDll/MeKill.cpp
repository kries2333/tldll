#include "pch.h"
#include "MeKill.h"
#include "AsmPet.h"

extern CAsmPet* g_pAsmPet;

BOOL CMeKill::Me_KillMonster(VUserMonsterName vtMonsterName)//����ɱ��
{
	g_pAsmPet->PetGoFight(0);
	return FUN_KillMonsterByName(vtMonsterName);//ָ������ɱ��
}