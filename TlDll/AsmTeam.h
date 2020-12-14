#pragma once
#include "pch.h"
#include "FileSystem.h"
#include <vector>
using namespace std;

struct TAsmTempInfo {
	PCHAR szName;
	PCHAR szMenpai;
	int nState;//����״̬
	int nMenpai;//����
	int nRoleID;//����ID
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
		nState = -1;//����״̬
		nMenpai = -1;//����
		nRoleID = -1;//����ID
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
	void TeamJoin();		//�������
	void TeamLeave();		//�뿪����
	void TeamFollow();		//����������

	bool GetTeamExist();	//�������
	bool GetInvite();
	void AsmInvaiteTeam(CString Name);

	vector<TAsmTempInfo> m_TeamArr;
};

