#pragma once
#include "User.h"
#include "Main.h"
#include "Function.h"

class CMeKillMonster : public CFunction
{
public:
	void Me_TimeDetectionRadius(TUserTime* ptUserTime, VUserPos vUserPos, size_t nNum);//�һ��뾶

public:
	BOOL Me_KillMonster(int nMonsterType);//����ɱ��
};

