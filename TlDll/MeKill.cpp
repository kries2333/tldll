#include "pch.h"
#include "MeKill.h"

BOOL CMeKill::Me_KillMonster(VUserMonsterName vtMonsterName)//正常杀怪
{
	return FUN_KillMonsterByName(vtMonsterName);//指定名称杀怪
}