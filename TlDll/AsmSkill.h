#pragma once
#include "FileSystem.h"
#include <vector>
#include <string>
using namespace std;

//技能信息结构
struct  TAsmSkill
{
	unsigned  unTree;//二叉树节点：当前技能对应的二叉树节点指针
	unsigned  unObject;//技能对象：当前技能对应的技能对象

	int nSkillId;// +0技能编号
	int	nMenPai;//  +8门派编号
	char* szName;//	+C技能名称
	int nXinFaLevel;// +14心法等级要求
	int nSkillType; // +3c 0蓄气技能 1引导技能 2直接使用技能 
	int nCoolTimeID;// +40 冷却时间ID
	int nSubType; // +98 技能子类型 0是增强自身  4自身范围  5单体攻击或者增强队友 7两次触发
	float fMin;	//+A0 是攻击距离上限
	float fMax;	//+A4 是攻击距离下限
	int	nStandFlag;	//+A8 这个是技能的立场标记，-1代表技能消弱目标，0是中性，1是技能增强目标
	int	nfriend;//+AC:  0: 目标和使用者应该是友好关系 1:目标和使用者应该是敌对关系
	int	nAutoShot;//+BC 1:自动连续释放技能
	float fRadius; //+D0 群攻的半径
	float fAngle; //+D4 群攻的范围 360周边
	int nTargetType; //+DC 0: 目标必须是玩家 1:目标必须是宠物 2:目标必须是怪物
	char* szDescription; //+150 技能描述信息
	int	nPassiveFlag;	// +154 0:主动技能,1:被动技能;

	TAsmSkill()
	{
		unTree = 0;
		unObject = 0;
		nSkillId = 0;
		nMenPai = 0;
		szName = "";
		nXinFaLevel = 0;
		nSkillType = 0;
		nCoolTimeID = 0;
		nSubType = 0;
		fMin = 0;
		fMax = 0;
		nStandFlag = 0;
		nfriend = 0;
		nAutoShot = 0;
		fRadius = 0;
		fAngle = 0;
		nTargetType = 0;
		szDescription = "";
		nPassiveFlag = 0;
	};
};

typedef vector<TAsmSkill> VAsmSkill;

class CAsmSkill
{
public:
	CAsmSkill() {};
	~CAsmSkill() {};

public:
	VAsmSkill AsmGetSkillData();//取角色技能信息
	bool AsmHaveMasterSkill(_tstring  skillName);//已经掌握的技能
	void AsmUseSkillCall(int MonsterId, int SkillId);//使用技能CALL
	void AsmUseSkillCallByPoint(int SkillId, float fx, float fy);//通过点击地面某一点释放技能
private:
	void AsmSkillTraverse(TAsmTree* pTree, VAsmSkill& vm_Skill, DWORD* pCount);//技能遍历
	void AsmGetSkillInfo(TAsmTree* pTree, VAsmSkill& vm_Skill);//技能属性信息
};

