#pragma once
#include "FileSystem.h"

struct TAsmRolePos
{
	BOOL bool_ret;
	float fx;
	float fy;

	TAsmRolePos()
	{
		bool_ret = FALSE;
		fx = -1;
		fy = -1;
	};
};

struct TAsmRoleInfo {
	PCHAR szName;
	PCHAR szMenpai;
	int nState;//人物状态
	int nMenpai;//门派
	int nRoleID;//人物ID
	int nLevel;
	int nMoney;//
	int nHP;//
	int nHPMax;

	TAsmRoleInfo()
	{
		szName = "";
		szMenpai = "";
		nState = -1;//人物状态
		nMenpai = -1;//门派
		nRoleID = -1;//人物ID
		nLevel = -1;
		nMoney = -1;//
		nHP = -1;//
		nHPMax = -1;
	};
};

class CAsmRole
{
public:
	CAsmRole() {};
	~CAsmRole() {};

	bool IsHpAndMp(int nP);
	TAsmRolePos GetPos();
	_tstring GetMenPaiForName();
	int	GetMenPaiForId();
	TAsmRoleInfo GetRoleInfo();
	DWORD GetRoleBase();
	float GetHpPercent();

	void AsmNpcDialogue(int npcId);

	int GetQiChenStatus();
};

