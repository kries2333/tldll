#pragma once
#include "AsmMonster.h"
#include "AsmSkill.h"
#include "User.h"

enum TASK_TYPE
{
	开始挂机 = 1,
	开始任务 = 2,
	开始调度 = 3,
	开始采集 = 4,
	开始刷马 = 5,
	开始打图 = 6,
	开始经商 = 7
};

struct TAsmMap
{
	BOOL bool_ret;
	int nSceneId;
	CString szName;

	TAsmMap()
	{
		bool_ret = FALSE;
		nSceneId = -1;
		szName = "";
	};
};

TAsmMap role_curmap();

DWORD GetGameBase();
bool SplitCStringArr(CString expression, CString delimiter, CStringArray* resultArr);

extern DWORD g_GameExeBase;

class CFunction
{
public:
	/************************************************************************/
	/////////////////////////////任务相关的函数封装//////////////
	/************************************************************************/
	BOOL FUN_GetPlayerMission(_tstring taskName);

	/************************************************************************/
	/////////////////////////////寻路相关的函数封装//////////////
	/************************************************************************/
	int FUN_GetSceneID(const char* szSceneName);//通过名称获取地图场景ID
	_tstring FUN_GetSceneName(int nSceneId);
	//BOOL  FUN_RunToTarget(float fx, float fy, float dis = 2); //本地寻路函数
	BOOL  FUN_RunToTargetEx(float x, float y, int SceneId, float dis = 2);//跨地图寻路
	float FUN_GetDistance(float x2, float y2);//获取与目标的距离

	void FUN_SetAttackMonsterPoints(CString szPoints);

	//执行杀怪
	void FUN_AttackMonster(_tstring monsterName, _tstring sceneName, float x, float y);

	/************************************************************************/
	/* 主线任务打怪函数                                                     */
	/************************************************************************/
	void FUN_MisKillMonsterByName(_tstring MonsterName);

	//抓捕
	bool FUN_MisUseSkillCatchMonster(_tstring monsterName);

	//通过物品名称获取Idx
	int FUN_GetItemIdxForName(_tstring strName);

	VString FUN_GetTaskInfo(_tstring taskName);

	_tstring FUN_GetMissionTaskKillMonsterInfo(_tstring taskName);
	//===================================================================================
	//江湖答题
	BOOL FUN_Questions();

	//判断任务是否可接
	bool FUN_IsAvailableTask(_tstring taskName);

	_tstring FUN_GetAvailableTaskInfo(_tstring taskName);

	_tstring FUN_GetMissionFinishInfo(_tstring taskName);

	bool FUN_IsTask(_tstring taskName);

	//判断已接列表  -1 未找到 0 未完成  1 已经完成
	int FUN_IsTaskSuccess(_tstring taskName);

	float FUN_MaximumRange(TAsmSkill tAsmSkill);

	//判断是否学习技能
	bool FUN_IsSkillName(CString taskName);

	bool FUN_SkillUseIDPos(int nSkillId, float x, float y);

	//接取任务
	bool FUN_TaskAccep(_tstring taskName, _tstring npcName, _tstring sceneName, float x, float y);

	bool FUN_TaskAccepEx(_tstring taskName);

	//执行对话任务
	bool FUN_ExecuteTask(_tstring taskName);

	//执行杀怪任务
	bool FUN_ExecuteTaskKill(_tstring taskName, int nType, int nMonsterType, _tstring monsterName, bool bPickUp);
	
	//执行任务学习任务--一般是指定npc对话
	void FUN_TaskSuccess(_tstring taskName, _tstring npcName, _tstring sceneName, float x, float y);

	//执行对话任务
	bool FUN_TaskSuccessEx(_tstring taskName, int nType);

	//加入门派
	void FUN_JoinMenPai(_tstring szMenPaiName);

	//升级
	void FUN_Levelup();

	//拾取
	void FUN_PickUp();

	int FUN_AutoMove();

	BOOL FUN_IsQuitRelative();

	void FUN_DeathResurrection();

	void FUN_SkillUseID(int nSkillId);

	BOOL FUN_RunTo(float x, float y, int nTime);

	void FUN_AutoSell(CString sceneName, int nPosX, int nPosY, CString npcName, CString _talkName);

	void FUN_AutoBuy(CString itemNames, CString sceneName, int nPosX, int nPosY, CString npcName, CString _talkName);

	void FUN_AutoStorage(_tstring sceneName, int nPosX, int nPosY, _tstring npcName, _tstring _talkName);

	//自动恢复角色状态
	void FUN_AutoRoleStatus(int nPercentage);

	int FUN_GetBagItemBlankNum();

	//角色保护吃药设置
	void FUN_RoleHMProtection(CString szLp, CString szTypeName, int Per, CString szYaoNames);

	//宠物保护吃药设置
	void FUN_PetHMProtection(CString szTypeName, int Per, CString szYaoNames);

	/************************************************************************/
	/////////////////////////////组队函数封装//////////////
	/************************************************************************/
	BOOL FUN_AutoTeam(int nNum);
	BOOL FUN_CheckTeam(int nNum);

protected:
	/************************************************************************/
	/////////////////////////////技能打怪相关的函数封装//////////////
	/************************************************************************/
	bool FUN_MinDistanceObject(float* fdistance, float fx, float fy);	//true是最近的
	void FUN_UseMovAttackSkill(TAsmSkill tAsmSkill, TAsmMonster tAsmMonster);//开始释放攻击技能
	bool FUN_MovToMonster(TAsmMonster tAsmMonster, TAsmSkill tAsmSkill);//走到技能射程内
	bool FUN_IsMonsterDie(TAsmMonster* ptAsmMonster);//false为死亡
	TAsmMonster FUN_GetMonsterByName(_tstring MonsterName, int nMonsterType, int nKillType);//名称获取最近的怪物
	TAsmMonster FUN_GetCatchMonsterByName(_tstring MonsterName);

	BOOL FUN_KillMonsterByName(VUserMonsterName vm_UserMonsterName);
	void FUN_UseSkillKillMonster(TAsmMonster tAsmMonster);
	void FUN_UseAttackSkill(TUserSkill tAsmSkill/*CString SkillName*/, TAsmMonster tAsmMonster);
};

