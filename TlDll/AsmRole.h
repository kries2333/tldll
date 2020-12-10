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
	int nState;//����״̬
	int nMenpai;//����
	int nRoleID;//����ID
	int nLevel;
	int nMoney;//
	int nHP;//
	int nHPMax;

	TAsmRoleInfo()
	{
		szName = "";
		szMenpai = "";
		nState = -1;//����״̬
		nMenpai = -1;//����
		nRoleID = -1;//����ID
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

