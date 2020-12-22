#include "pch.h"
#include "ScriptSystem.h"

void CScriptSystem::LUA_ScriptSystemInitial(/*CString strScriptName*/)
{
	MeLua = new CMeLua();
	pState = new LuaStateOwner(true);
	LuaObject objGlobal = (*pState)->GetGlobals();

	objGlobal.Register("Script", *MeLua,
		&CMeLua::LUA_Script);//调试输出字符串函数

	objGlobal.Register("Stop", *MeLua,
		&CMeLua::LUA_Stop);//停止lua脚本的命令函数

	objGlobal.Register("DoString", *MeLua,
		&CMeLua::LUA_DoString);//LUA_DoString

	objGlobal.Register("LoadScript", *MeLua,
		&CMeLua::LUA_LoadScript);//LUA_LoadScript

	objGlobal.Register("ShowDebug", *MeLua,
		&CMeLua::LUA_ShowDebug);//LUA_DoString

	objGlobal.Register("GetMissionFinishInfo", *MeLua,
		&CMeLua::LUA_GetMissionFinishInfo);//LUA_DoString

	objGlobal.Register("IsQuitRelative", *MeLua,
		&CMeLua::LUA_IsQuitRelative);//LUA_DoString

	objGlobal.Register("role_GetPosX", *MeLua,
		&CMeLua::LUA_GetRolePosX);//LUA_DoString

	objGlobal.Register("role_GetPosY", *MeLua,
		&CMeLua::LUA_GetRolePosY);//LUA_DoString

	//////////////////////////////任务动作需求相关//////////////////////////////////////
	objGlobal.Register("ui_IsQuizDiaLog", *MeLua,
			&CMeLua::LUA_IsQuizDiaLog);//判断是否江湖答题窗口

	objGlobal.Register("task_Questions", *MeLua,
		&CMeLua::LUA_Questions);//执行江湖答题

	objGlobal.Register("lua_MoveName", *MeLua,
		&CMeLua::LUA_MoveTo);//移动函数
	
	objGlobal.Register("task_IsAvailableTask", *MeLua,
		&CMeLua::LUA_IsAvailableTask);	//判断任务是否可接

	objGlobal.Register("task_IsTask", *MeLua,
		&CMeLua::LUA_IsTask);	//判断任务是否在已接列表里

	objGlobal.Register("task_IsTaskSuccess", *MeLua,
		&CMeLua::LUA_IsTaskSuccess);	//判断任务是否完成

	objGlobal.Register("lua_TaskAccept", *MeLua,
		&CMeLua::LUA_TaskAccept);		//接取任务

	objGlobal.Register("lua_TaskAcceptEx", *MeLua,
		&CMeLua::LUA_TaskAcceptEx);		//接取任务

	objGlobal.Register("lua_ExecuteTask", *MeLua,
		&CMeLua::LUA_ExecuteTask);		//执行对话任务

	objGlobal.Register("lua_ExecuteTaskKill", *MeLua,
		&CMeLua::LUA_ExecuteTaskKill);		//执行杀怪任务

	objGlobal.Register("lua_TaskSuccessEx", *MeLua,
		&CMeLua::LUA_TaskSuccessEx);		//任务执行完成后回执

	objGlobal.Register("lua_TaskSuccess", *MeLua,
		&CMeLua::LUA_TaskSuccess);		//任务执行完成后回执

	objGlobal.Register("lua_SetAttackMonsterPoints", *MeLua,
		&CMeLua::LUA_SetAttackMonsterPoints);		//杀怪列表 应该与单次杀怪合并
	
	objGlobal.Register("lua_AttackMonster", *MeLua,
		&CMeLua::LUA_AttackMonster);		//单次杀怪

	objGlobal.Register("lua_AttackMonsterEx", *MeLua,
		&CMeLua::LUA_AttackMonsterEx);		//杀怪列表 应该与单次杀怪合并

	objGlobal.Register("role_GetLevel", *MeLua,
		&CMeLua::LUA_GetLevel);		//获取等级

	objGlobal.Register("role_GetRaceName", *MeLua,
		&CMeLua::LUA_GetMenPaiName);		//获取门派
	
	objGlobal.Register("lua_JoinMenPai", *MeLua,
		&CMeLua::LUA_JoinMenPai);		//加入门派

	objGlobal.Register("skill_IsSkillName", *MeLua,
		&CMeLua::LUA_IsSkillName);		//判断是否学习了技能

	objGlobal.Register("role_GetMapName", *MeLua,
		&CMeLua::LUA_GetMapName);		//获取当前地图名称

	objGlobal.Register("skill_UseIDPos", *MeLua,
		&CMeLua::LUA_SkillUseIDPos);		//使用指向地面技能

	objGlobal.Register("skill_UseID", *MeLua,
		&CMeLua::LUA_SkillUseID);		//使用技能

	objGlobal.Register("AutoSell", *MeLua,
		&CMeLua::LUA_AutoSell);		//

	objGlobal.Register("AutoWearEquipment", *MeLua,
		&CMeLua::LUA_AutoWearEquipment);		//使用技能

	objGlobal.Register("AutoDestroy", *MeLua,
		&CMeLua::LUA_AutoDestroy);		//使用技能

	objGlobal.Register("lua_DeathResurrection", *MeLua,
		&CMeLua::LUA_DeathResurrection);		//死亡复活

	objGlobal.Register("lua_RoleHMProtection", *MeLua,
		&CMeLua::LUA_RoleHMProtection);		//设置角色保护

	objGlobal.Register("lua_PetHMProtection", *MeLua,
		&CMeLua::LUA_PetHMProtection);		//设置宠物保护

	objGlobal.Register("lua_AutoTeam", *MeLua, 
		&CMeLua::LUA_AutoTeam);			//自动组队

	objGlobal.Register("lua_CheckTeam", *MeLua,
		&CMeLua::LUA_CheckTeam);			//检查队伍

	objGlobal.Register("lua_AutoCleanBag", *MeLua,
		&CMeLua::LUA_AutoCleanBag);			//清理背包
	//======以上为对外接口=============================================

	//========以下测试接口使用，发布需要屏蔽=============================================


	objGlobal.Register("HuoDongRiCheng_next_click", *MeLua,
		&CMeLua::HuoDongRiCheng_next_click);//走路call

	objGlobal.Register("AutoRunToTarget", *MeLua,
		&CMeLua::LUA_AutoRunToTarget);//走路call

	objGlobal.Register("MoveToNPC", *MeLua,
		&CMeLua::LUA_MoveToNPC);//移动到NPC

	objGlobal.Register("MoveToNPCEx", *MeLua,
		&CMeLua::LUA_MoveToNPCEx);//移动到NPC并对话

	objGlobal.Register("InMenPai", *MeLua,
		&CMeLua::LUA_InMenPai);//移动到NPC

	objGlobal.Register("lua_AutoRoleStatus", *MeLua,
		&CMeLua::LUA_AutoRoleStatus);//恢复

	objGlobal.Register("lua_AutoStorage", *MeLua,
		&CMeLua::LUA_AutoStorage);//存储

	objGlobal.Register("lua_AutoBuy", *MeLua,
		&CMeLua::LUA_AutoBuy);//自动购买

	objGlobal.Register("lua_AutoSell", *MeLua,
		&CMeLua::LUA_AutoSell);//自动贩卖

	objGlobal.Register("lua_GetBagItemNum", *MeLua,
		&CMeLua::LUA_GetBagItemNum);//获取背包物品数量

	objGlobal.Register("lua_GetBagItemBlankNum", *MeLua,
		&CMeLua::LUA_GetBagItemBlankNum);//获取背包物品空白数量

	objGlobal.Register("GoFight", *MeLua, &CMeLua::LUA_PetGoFight);//宠物出战

	objGlobal.Register("PetCatch", *MeLua, &CMeLua::LUA_PetCatch);//抓捕宠物

	// 物品
	objGlobal.Register("pack_GetBagItemNum", *MeLua, &CMeLua::LUA_GetBagItemNum);//使用物品
	objGlobal.Register("pack_UseBagItem", *MeLua, &CMeLua::LUA_UseBagItem);//使用物品

	//-------------------------------------------------------------------------------------
	//注册System函数对象///////////////////////////////////////////////////////////////////////
	LuaObject metatable_System = objGlobal.CreateTable("MetaTableSystem");
	metatable_System.SetObject("__index", metatable_System);

	metatable_System.RegisterObjectFunctor("Sleep", &CMeLua::LUA_Sleep);//延迟函数
	LuaObject obj_System = (*pState)->BoxPointer(&MeLua);
	obj_System.SetMetaTable(metatable_System);
	objGlobal.SetObject("System", obj_System);

	//-------------------------------------------------------------------------------------
	//注册DataBase函数对象//////////////////////////////////////////////////////////////////////
	LuaObject metatable_DataBase = objGlobal.CreateTable("MetaTableDataBase");
	metatable_DataBase.SetObject("__index", metatable_DataBase);

	metatable_DataBase.RegisterObjectFunctor("GetSceneID", &CMeLua::LUA_GetSceneId);//获取场景id

	LuaObject obj_DataBase = (*pState)->BoxPointer(&MeLua);
	obj_DataBase.SetMetaTable(metatable_DataBase);
	objGlobal.SetObject("DataBase", obj_DataBase);

	//注册DataPool函数对象///////////////////////////////////////////////////////////////////////
	LuaObject metatable_DataPool = objGlobal.CreateTable("MetaTableDataPool");
	metatable_DataPool.SetObject("__index", metatable_DataPool);

	metatable_DataPool.RegisterObjectFunctor("GetPlayerMission"
		, &CMeLua::LUA_GetPlayerMission);//获取当前已接任务

	metatable_DataPool.RegisterObjectFunctor("GetPlayerMission_Variable"
		, &CMeLua::LUA_GetPlayerMission_Variable);//任务完成进度 1完成，2失败，0进行

	LuaObject obj_DataPool = (*pState)->BoxPointer(&MeLua);
	obj_DataPool.SetMetaTable(metatable_DataPool);
	objGlobal.SetObject("DataPool", obj_DataPool);
}
