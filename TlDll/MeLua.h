#pragma once
#include "Function.h"
#include "LuaPlus.h"
using namespace LuaPlus;

class CMeLua : public CFunction
{
public:
	CMeLua() {};
	~CMeLua() {};

	int LUA_Sleep(LuaState* pState);

	int LUA_LoadScript(LuaState* pState);

	int LUA_GetTickCount(LuaState* pState);

	int LUA_Stop(LuaState* pState);

	int LUA_ShowMessage(LuaState* pState);

	int LUA_ShowDebug(LuaState* pState);

	int LUA_DoString(LuaState* pState);//lua字符串函数

	int LUA_GetMissionFinishInfo(LuaState* pState);

	int LUA_IsQuitRelative(LuaState* pState);

	/************************************************************************/
	/////////////////////////////寻路相关的函数封装//////////////
	/************************************************************************/
	int  LUA_GetSceneId(LuaState* pState);
	int  LUA_AutoRunToTarget(LuaState* pState);//移动到目标
	int  LUA_MoveToNPC(LuaState* pState);//参数怪物名称杀怪
	int  LUA_MoveToNPCEx(LuaState* pState);
	int  LUA_MoveTo(LuaState* pState);//移动函数

	/************************************************************************/
	//////////////////////////////任务数据相关的函数封装///////
	/************************************************************************/
	int LUA_GetPlayerMission(LuaState* pState);//是否已经接取了任务
	int LUA_GetPlayerMission_Variable(LuaState* pState);//是否可交付

	int HuoDongRiCheng_next_click(LuaState* pState);

	int LUA_Script(LuaState* pState);


	/************************************************************************/
	/////////////////////////////NPC对话相关的函数封装//////////////
	/************************************************************************/
	//int LUA_QuestFrameOptionClicked(LuaState* pState);//点击对话选项，参数是对话选项字符串
	//int LUA_QuestFrameMissionContinue(LuaState* pState);//继续
	//int LUA_MissionContinue_Clicked(LuaState* pState);//完成
	//int LUA_MissionQuestAccept_Clicked(LuaState* pState);//接受
	//int LUA_Quest_OnHidden(LuaState* pState);//关闭

	/************************************************************************/
	/////////////////////////////怪物宠物周围环境相关的函数封装///////
	/************************************************************************/

	//=========================================================================
	int LUA_IsQuizDiaLog(LuaState* pState); // 判断是否在江湖答题
	int LUA_Questions(LuaState* pState);	//开始答题
	int LUA_PetGoFight(LuaState* pState); //宠物出战
	int LUA_PetCatch(LuaState* pState); //抓捕
	int LUA_SkillUseID(LuaState* pState); //使用技能
	int LUA_SkillUseIDPos(LuaState* pState); //使用指向地面技能
	int LUA_GetMapName(LuaState* pState); // 获取当前地图名称
	int LUA_GetRolePosX(LuaState* pState);	//获取角色当前坐标
	int LUA_GetRolePosY(LuaState* pState);

	int LUA_DeathResurrection(LuaState* pState);

	// 物品
	int LUA_UseBagItem(LuaState* pState); //使用物品

	int LUA_UpdateQuest(LuaState* pState);

	// 重新封装，外部调用接口
	int LUA_IsAvailableTask(LuaState* pState); //判断任务是否在可接列表里

	int LUA_IsTask(LuaState* pState);  //判断任务是否在已接列表里

	int LUA_IsTaskSuccess(LuaState* pState);   //判断任务是否完成

	int LUA_TaskAccept(LuaState* pState);	//接取任务

	int LUA_TaskAcceptEx(LuaState* pState); //接取任务

	int LUA_ExecuteTask(LuaState* pState);	//执行任务

	int LUA_ExecuteTaskKill(LuaState* pState); //执行杀怪任务

	int LUA_TaskSuccessEx(LuaState* pState);  //完成任务回执

	int LUA_TaskSuccess(LuaState* pState); //执行对话任务

	int LUA_SetAttackMonsterPoints(LuaState* pState);	//设置杀怪定点坐标

	int LUA_AttackMonster(LuaState* pState);	//单个杀怪

	int LUA_AttackMonsterEx(LuaState* pState);

	int LUA_IsSkillName(LuaState* pState);	//判断是否学习了技能

	int LUA_GetBagItemIndex(LuaState* pState); //获取背包物品Index

	int LUA_GetMenPaiName(LuaState* pState); //获取门派

	int LUA_JoinMenPai(LuaState* pState);

	int LUA_InMenPai(LuaState* pState);	//获取想加入的门派

	int LUA_GetLevel(LuaState* pState);

	int LUA_AutoWearEquipment(LuaState* pState);

	int LUA_AutoSell(LuaState* pState);

	int LUA_AutoDestroy(LuaState* pState);

	int LUA_AutoBuy(LuaState* pState);

	int LUA_AutoStorage(LuaState* pState);

	int LUA_AutoRoleStatus(LuaState* pState);

	int LUA_GetBagItemNum(LuaState* pState);	//获取背包物品数量

	int LUA_GetBagItemBlankNum(LuaState* pState);	//获取背包物品空白数量

	/************************************************************************/
	//////////////////////////////角色函数封装///////
	/************************************************************************/
	int LUA_RoleHMProtection(LuaState* pState);	//角色保护设置
	int LUA_PetHMProtection(LuaState* pState);	//宠物保护设置
	
	int LUA_AutoTeam(LuaState* pState);		//自动组队

	int LUA_CheckTeam(LuaState* pState);	//校验队伍
};

