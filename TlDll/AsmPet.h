#pragma once
#include <vector>
using namespace std;

struct	TAsmPet
{
	DWORD  dwObject;
	int    nPetId;//+C ID
	int	   nIndex;
	PCHAR  szName;//+3C珍兽名字
	int    nPetType;//+34宠物类型 6是肉食闪电貂  a是草食花仙子 呆呆牛宝宝
	int    nLevel;//+24等级	
	int    nMaxHP;//+48最大血
	int    nCurHP;//+4C当前血
	int    nKuaiLe;//+58快乐
	int    nAttackType;//+8c 类型  b外功    c内功     a或d是均衡性或者无类型
	int    nPotential;//+108宠物剩余点
	//VAsmPetSkill vm_PetSkill;// +120指向技能列表的指针,这里用容器存储技能数组

	TAsmPet()
	{
		szName = "";
		nLevel = 0;
		nKuaiLe = 0;
		nAttackType = 0;
		nMaxHP = 0;
		nCurHP = 0;
		dwObject = 0;
		nPetId = 0;
		nPotential = 0;
		nPetType = 0;
	};
};

typedef vector<TAsmPet> VAsmPet;

class CAsmPet
{
public:
	CAsmPet() {};
	~CAsmPet() {};
public:

	VAsmPet AsmGetPetData();
	int GetCount();
	int GetMaxCount();
	void PetGoFight(int nIndex);
	int GetIsFighting();
private:
	DWORD GetBase();
	
};

