#pragma once
#include "FileSystem.h"
#include <vector>
using namespace std;

struct TAsmMonster 
{
	DWORD dwTree;    //怪物二叉树节点
	DWORD dwObject;  //怪物对象
	int   nMonsterId;//怪物对象+34是怪物的ID
	float fMonsterX;//怪物对象+48怪物的x坐标
	float fMonsterY;//怪物对象+50怪物的y坐标
	DWORD dwOffset;    //偏移怪物的名称，血值，等级所在的偏移基址
	float fMonsterHp;   //怪物的属性属性偏移+8怪物的血值百分比
	int   nWhoHave;    //怪物的属性属性偏移+24怪物是否有拥有者
	PCHAR szName;     //怪物的属性属性偏移+3C怪物的名字
	PCHAR szBaseName;	//判断是否是头目
	int   nLevel;    //怪物的属性属性偏移+6C等级
	int   nType;//这个是怪物归属，区分，-1绿怪，角色对象里的一个值-红怪，不是我们角色对象的里的值-白怪
	int   nNpcType;//这个区分npc的类型
	int   nAttack; //是否可以攻击
	int   nCatch;  //是否可以捕获
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

