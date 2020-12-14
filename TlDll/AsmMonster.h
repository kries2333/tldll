#pragma once
#include "FileSystem.h"
#include <vector>
using namespace std;

struct TAsmMonster 
{
	DWORD dwTree;		//����������ڵ�
	DWORD dwObject;		//�������
	int   nMonsterId;	//�������+34�ǹ����ID
	float fMonsterX;	//�������+48�����x����
	float fMonsterY;	//�������+50�����y����
	DWORD dwOffset;		//ƫ�ƹ�������ƣ�Ѫֵ���ȼ����ڵ�ƫ�ƻ�ַ
	float fMonsterHp;   //�������������ƫ��+8�����Ѫֵ�ٷֱ�
	int   nWhoHave;		//�������������ƫ��+24�����Ƿ���ӵ����
	PCHAR szName;		//�������������ƫ��+3C���������
	int   nLevel;		//�������������ƫ��+6C�ȼ�
	int   nType;		//����ǹ�����������֣�-1�̹֣���ɫ�������һ��ֵ-��֣��������ǽ�ɫ��������ֵ-�׹�
	int   nNpcType;		//�������npc������
	int   nAttack;		//�Ƿ���Թ���
	int   nCatch;		//�Ƿ���Բ���
	TAsmMonster()
	{
		dwTree = 0;
		nMonsterId = -1;
		nLevel = 0;
		dwObject = 0;
		fMonsterX = 0.0f;
		fMonsterY = 0.0f;
		fMonsterHp = 0.0f;
		szName = "";
		nWhoHave = 0;
		dwOffset = 0;
		nType = 0;
		nNpcType = -1;
		nAttack = -1;
		nCatch = 0;
	};
};

typedef vector<TAsmMonster> VAsmMonster;

class CAsmMonster
{
public:
	VAsmMonster AsmGetMonsterData();
	void UpdateTagerMonster(TAsmMonster* monster);
	float GetHp(TAsmMonster monster);

private:
	DWORD AsmGetEnvTreeBase();
	BOOL AsmTreeObjectType(DWORD Object, char* szObjectName);
	void AsmMonsterTraverse(TAsmTree* pTree, VAsmMonster& vm_Monster, DWORD* pCount);
	void AsmGetMonsterInfo(TAsmTree* pTree, VAsmMonster& vm_Monster);


};

