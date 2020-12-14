#pragma once
#include "pch.h"
#include "FileSystem.h"
#include <vector>
using namespace std;

struct TAsmTempInfo {
	PCHAR szName;
	PCHAR szMenpai;
	int nState;//人物状态
	int nMenpai;//门派
	int nRoleID;//人物ID
	int nLevel;
	int nHP;//
	int nHPMax;
	int nMP;//
	int nMPMax;
	float nPosX;
	float nPosY;
	TAsmTempInfo()
	{
		szName = "";
		szMenpai = "";
		nState = -1;//人物状态
		nMenpai = -1;//门派
		nRoleID = -1;//人物ID
		nLevel = -1;
		nHP = -1;//
		nHPMax = -1;
		nMP = -1;//
		nMPMax = -1;
		nPosX = 0.0;
		nPosY = 0.0;
	};
};

class CAsmTeam
{
public:
	void GetTeamInfo();
	void TeamJoin();		//加入队伍
	void TeamLeave();		//离开队伍
	void TeamFollow();		//发起队伍跟随

	bool GetTeamExist();	//队伍存在
	bool GetInvite();
	void AsmInvaiteTeam(CString Name);

	vector<TAsmTempInfo> m_TeamArr;
};

