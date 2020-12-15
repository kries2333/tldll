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

struct TAsmHPMP
{
	BOOL bool_ret;
	int nCurhp;
	int nMaxhp;
	int nCurmp;
	int nMaxmp;

	TAsmHPMP()
	{
		bool_ret = FALSE;
		nCurhp = -1;
		nMaxhp = -1;
		nCurmp = -1;
		nMaxmp = -1;
	};
};

struct TAsmRoleInfo {
	PCHAR szName;
	PCHAR szMenpai;
	int nState;//人物状态
	int nMenpai;//门派
	int nRoleID;//人物ID
	int nRoleCode;	//人物编号
	int nLevel;
	int nMoney;//
	int nHP;//
	int nHPMax;
	BOOL bool_ret;

	TAsmRoleInfo()
	{
		szName = "";
		szMenpai = "";
		nState = -1;//人物状态
		nMenpai = -1;//门派
		nRoleID = -1;//人物ID
		nRoleCode = -1; //人物编号
		nLevel = -1;
		nMoney = -1;//
		nHP = -1;//
		nHPMax = -1;
		bool_ret = false;
	};
};

class CAsmRole
{
public:
	CAsmRole() {};
	~CAsmRole() {};
public:
	TAsmHPMP GetHPMP();//人物的当前HPMP最大HPMP
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

