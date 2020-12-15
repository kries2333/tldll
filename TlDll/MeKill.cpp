#include "pch.h"
#include "MeKill.h"
#include "AsmPet.h"

extern CAsmPet* g_pAsmPet;

BOOL CMeKill::Me_KillMonster(VUserMonsterName vtMonsterName)//正常杀怪
{
	g_pAsmPet->PetGoFight(0);
	return FUN_KillMonsterByName(vtMonsterName);//指定名称杀怪
}