#pragma once
#include "User.h"
#include "Me.h"
#include "Function.h"

class CMeKillMonster : public CFunction
{
public:
	void Me_TimeDetectionRadius(TUserTime* ptUserTime, VUserPos vUserPos, size_t nNum);//¹Ò»ú°ë¾¶

public:
	BOOL Me_KillMonster(int nMonsterType);//Õý³£É±¹Ö
};

