#include "pch.h"
#include "MeKill.h"

BOOL CMeKill::Me_KillMonster(VUserMonsterName vtMonsterName)//����ɱ��
{
	return FUN_KillMonsterByName(vtMonsterName);//ָ������ɱ��
}