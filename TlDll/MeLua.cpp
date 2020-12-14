#include "pch.h"
#include "MeLua.h"
#include "FileSystem.h"
#include "AsmRole.h"
#include "Message.h"
#include "User.h"
#include "AsmTask.h"
#include "AsmPet.h"
#include "Function.h"
#include "Me.h"
#include "AsmSkill.h"
#include "AsmItem.h"
#include "MeKill.h"

extern CFileSystem* g_pFileSystem;
extern CMe* g_pMe;
extern CMessage* g_pMsg;
extern CAsmRole* g_pAsmRole;
extern CAsmTask* g_pAsmTask;
extern CAsmPet* g_pAsmPet;
extern CAsmSkill* g_pAsmSkill;
extern CAsmItem* g_pAsmItem;
extern CAsmPet* g_pAsmPet;
extern CMeKill* g_pMeKill;

/************************************************************************/
/////////////////////////////System相关的函数封装//////////////
/************************************************************************/

int CMeLua::LUA_Sleep(LuaState* pState)
{
	LuaStack args(pState);

	int nNum = args[2].GetInteger();
	Sleep(nNum);

	return 0;
}

int CMeLua::LUA_LoadScript(LuaState* pState)
{
	LuaStack args(pState);

	if (args[1].IsString())
	{
		_tstring strScriptName = args[1].GetString();
		strScriptName = g_pFileSystem->Script.c_str() + strScriptName;
		dbgPrint("LUA_LoadScript strScriptName=%s", strScriptName.c_str());
		pState->DoFile(strScriptName.c_str());//执行脚本文件
		return 1;
	}


	return 0;
}

int CMeLua::LUA_GetTickCount(LuaState* pState)
{
	DWORD nVal = GetTickCount();
	pState->PushNumber(nVal);

	return 1;
}

int CMeLua::LUA_Stop(LuaState* pState)
{
	pState->PushBoolean(g_pMe->bTaskThread);

	return 1;
}

int CMeLua::LUA_ShowMessage(LuaState* pState)
{
	LuaStack args(pState);

	const char* str = args[1].GetString();
	g_pMsg->msg_dostring("PushDebugMessage(\"%s\");", str);

	return 0;
}

int CMeLua::LUA_ShowDebug(LuaState* pState)
{
	LuaStack args(pState);

	const char* str = args[1].GetString();
	dbgPrint("LUA_ShowDebug %s", str);

	return 0;
}


int CMeLua::LUA_DoString(LuaState* pState)
{
	LuaStack args(pState);

	const char* str = args[1].GetString();
	g_pMsg->msg_dostring(str);
	Sleep(2000);

	return 0;
}

//任务数据相关
int CMeLua::LUA_UpdateQuest(LuaState* pState)
{
	dbgPrint("LUA_UpdateQuest");
	//g_pMsg->msg_dostring("QuestLog_UpdateListbox();");
	int a = g_pMsg->msg_getnumber("g_GetValue = GetMountID();");
	dbgPrint("LUA_UpdateQuest = %d", a);
	return 1;
}

//是否已经接取了任务
int CMeLua::LUA_GetPlayerMission(LuaState* pState)
{
	LuaStack args(pState);

	_tstring strTaskName = args[2].GetString();
	_tstring strInfo = g_pMsg->msg_getstring("MyPlayerMissionString", "MyPlayerMissionString = mymis(\"%s\")", strTaskName.c_str());

	dbgPrint("LUA_GetPlayerMission strInfo = %s", strInfo.c_str());
	auto vString = UserSubMonsterName(strInfo, _T('|'));
	if (vString.size() == 7)
	{
		pState->PushInteger(stoi(vString[2]));

		return	1;
	}

	pState->PushInteger(-1);
	return 0;
}

int CMeLua::LUA_GetPlayerMission_Variable(LuaState* pState)//是否可交付
{
	LuaStack args(pState);

	dbgPrint("LUA_GetPlayerMission_Variable");
	if (args[2].IsString())
	{
		_tstring strTaskName = args[2].GetString();
		_tstring strInfo = g_pMsg->msg_getstring("MyMonVariableString", "MyMonVariableString = mymis(\"%s\")", strTaskName.c_str());

		dbgPrint("LUA_GetPlayerMission_Variable strInfo=%s", strInfo.c_str());
		auto vString = UserSubMonsterName(strInfo, _T('|'));
		dbgPrint("LUA_GetPlayerMission_Variable 返回0=%s", vString[0].c_str());
		dbgPrint("LUA_GetPlayerMission_Variable 返回1=%s", vString[1].c_str());
		dbgPrint("LUA_GetPlayerMission_Variable 返回2=%s", vString[2].c_str());
		if (vString.size() > 3) 
		{
			pState->PushInteger(stoi(vString[2]));
			return	1;
		}
	}
	dbgPrint("LUA_GetPlayerMission_Variable 返回=%d", 0);
	pState->PushInteger(0);
	return	1;
}

int CMeLua::HuoDongRiCheng_next_click(LuaState* pState)
{
	LuaStack args(pState);

	g_pMsg->msg_dostring("HuoDongRiCheng_next_click();");

	return 1;
}

/************************************************************************/
/////////////////////////////寻路相关的函数封装//////////////
/************************************************************************/
int  CMeLua::LUA_GetSceneId(LuaState* pState)
{
	LuaStack args(pState);
	_tstring strName = args[2].GetString();

	int nSceneId = FUN_GetSceneID(strName.c_str());

	dbgPrint(_T("LUA_GetSceneId %s %x"), strName.c_str(), nSceneId);

	pState->PushInteger(nSceneId);
	return 1;

}

//获取门派
int CMeLua::LUA_GetMenPaiName(LuaState* pState)
{
	LuaStack args(pState);
	TAsmRoleInfo info = g_pAsmRole->GetRoleInfo();
	switch (info.nMenpai)
	{
	case 8:
		pState->PushString("逍遥");
		break;
	default:
		break; 
	}
	return 1;
}

int  CMeLua::LUA_AutoRunToTarget(LuaState* pState)//移动到目标
{
	LuaStack args(pState);
		
	if (args[1].IsNumber() && args[2].IsNumber() && args[3].IsNumber())
	{
		float fx = args[1].GetFloat();
		float fy = args[2].GetFloat();

		int SceneId = args[3].GetInteger();

		if (SceneId == -1)
		{
			g_pMsg->msg_dostring("AutoRunToTarget(%d, %d)", (int)fx, (int)fy);
		}
		else
		{
			g_pMsg->msg_dostring("AutoRunToTargetEx(%d, %d, %d)", (int)fx, (int)fy, SceneId);
		}

		
	}

	return 0;
}

int  CMeLua::LUA_MoveToNPC(LuaState* pState)
{
	dbgPrint("LUA_MoveToNPC");
	LuaStack args(pState);

	if (args[1].IsNumber() && args[2].IsNumber() && args[3].IsInteger() && args[4].IsString())
	{
		float x = args[1].GetFloat();
		float y = args[2].GetFloat();
		int SceneId = args[3].GetInteger();
		_tstring strNpcName = args[4].GetString();

		dbgPrint(_T("LUA_MoveToNPC %s %f %f %d"), strNpcName.c_str(), x, y, SceneId);
		for (int i = 0; i < 5; i++)
		{
			if (CString(strNpcName.c_str()).Find("柴进") != -1)
			{
				x = 199;
				y = 68;
			}
			if (FUN_RunToTargetEx(x, y, SceneId))
			{
				g_pMsg->SetAutoRunTargetNPCName(strNpcName.c_str());//msg的游戏lua函数
				if (g_pMsg->IsNpcDialog())
				{
					pState->PushBoolean(true);
					return 1;
				}
			}
			Sleep(1000);
		}
	}

	pState->PushBoolean(false);
	return 1;
}

int CMeLua::LUA_MoveToNPCEx(LuaState* pState)
{
	dbgPrint("LUA_MoveToNPCEx");
	LuaStack args(pState);

	if (args[1].IsString() && args[2].IsString() && args[3].IsNumber() && args[4].IsNumber() && args[5].IsString())
	{
		_tstring strNpcName = args[1].GetString();
		_tstring SceneName = args[2].GetString();
		float x = args[3].GetFloat();
		float y = args[4].GetFloat();
		_tstring szTalkName = args[5].GetString();

		dbgPrint(_T("LUA_MoveToNPCEx %s %f %f %s"), strNpcName.c_str(), x, y, SceneName.c_str());
		for (int i = 0; i < 5; i++)
		{
			if (CString(strNpcName.c_str()).Find("柴进") != -1)
			{
				x = 199;
				y = 68;
			}
			int nSceneId = FUN_GetSceneID(SceneName.c_str());
			if (FUN_RunToTargetEx(x, y, nSceneId))
			{
				g_pMsg->SetAutoRunTargetNPCName(strNpcName.c_str());//msg的游戏lua函数
				if (g_pMsg->IsNpcDialog())
				{
					g_pMsg->msg_dostring("ClickMission(\"%s\")", szTalkName.c_str());//msg的lua消息函数
					Sleep(500);
					pState->PushBoolean(true);
					return 1;
				}
			}
			Sleep(1000);
		}
	}

	pState->PushBoolean(false);
	return 1;
}

int  CMeLua::LUA_MoveTo(LuaState* pState)//移动函数
{
	dbgPrint("LUA_MoveTo");

	LuaStack args(pState);

	if (args[1].IsNumber() && args[2].IsNumber() && args[3].IsString())
	{
		float x = args[1].GetFloat();
		float y = args[2].GetFloat();
		_tstring sceneName = args[3].GetString();
		dbgPrint("LUA_MoveTo %f %f %s", x, y, sceneName.c_str());

		int sceneId = FUN_GetSceneID(sceneName.c_str());
		int nRet = FUN_RunToTargetEx(x, y, sceneId);
		pState->PushInteger(nRet);
		return 1;
	}
	pState->PushInteger(0);
	return 0;
}

int CMeLua::LUA_Script(LuaState* pState)
{
	LuaStack args(pState);

	int nScript = args[1].GetInteger();

	return 0;
}
/************************************************************************/
/////////////////////////////怪物宠物周围环境相关的函数封装///////
/************************************************************************/
int CMeLua::LUA_IsQuitRelative(LuaState* pState)
{
	FUN_IsQuitRelative();
	return 1;
}

//=========================================
int CMeLua::LUA_IsQuizDiaLog(LuaState* pState) // 判断是否在江湖答题
{
	LuaStack args(pState);
	if (g_pMsg->IsQuizDiaLog()) {
		pState->PushBoolean(TRUE);
		return 1;
	}
	pState->PushBoolean(FALSE);
	return 0;
}

int CMeLua::LUA_Questions(LuaState* pState)	//开始答题
{
	LuaStack args(pState);
	FUN_Questions();
	return 1;
}

int CMeLua::LUA_PetGoFight(LuaState* pState)//宠物出战
{
	LuaStack args(pState);

	if (args[1].IsInteger())
	{
		int nIndex = args[1].GetInteger();
		g_pAsmPet->PetGoFight(nIndex);
		return 1;
	}
	return 0;
}

int CMeLua::LUA_PetCatch(LuaState* pState) //抓捕
{
	dbgPrint("LUA_PetCatch");

	LuaStack args(pState);
	_tstring strName = args[1].GetString();
	dbgPrint("LUA_PetCatch strName=%s", strName.c_str());
	if (FUN_MisUseSkillCatchMonster(strName.c_str()))
	{
		pState->PushBoolean(TRUE);
		return 1;
	}
	pState->PushBoolean(FALSE);
	return 0;
}

int CMeLua::LUA_GetMapName(LuaState* pState)
{
	dbgPrint("LUA_GetMapName");

	_tstring mapName = role_curmap().szName;
	pState->PushString(mapName.c_str());

	dbgPrint("LUA_GetMapName mapName=%s", mapName.c_str());
	return 1;
}

int CMeLua::LUA_GetRolePosX(LuaState* pState)
{
	dbgPrint("LUA_GetRolePosX");
	TAsmRolePos tAsmRolePos = g_pAsmRole->GetPos();
	pState->PushNumber(tAsmRolePos.fx);
	dbgPrint("LUA_GetRolePos fx=%d", (int)tAsmRolePos.fx);
	return 1;
}

int CMeLua::LUA_GetRolePosY(LuaState* pState)
{
	dbgPrint("LUA_GetRolePosY");
	TAsmRolePos tAsmRolePos = g_pAsmRole->GetPos();
	pState->PushNumber(tAsmRolePos.fy);
	dbgPrint("LUA_GetRolePos fy=%d", (int)tAsmRolePos.fy);
	return 1;
}

int CMeLua::LUA_SkillUseID(LuaState* pState)	//使用指向地面技能
{
	dbgPrint("LUA_SkillUseID");

	LuaStack args(pState);
	if (args[1].IsInteger() && args[2].IsNumber())
	{
		int monsterId = args[1].GetInteger();
		int nSkillId = args[2].GetInteger();

		dbgPrint("LUA_SkillUseID monsterId=%d nSkillId=%d", monsterId, nSkillId);
		FUN_SkillUseID(nSkillId);
		return 1;
	}
	return 0;
}

int CMeLua::LUA_SkillUseIDPos(LuaState* pState)	//使用指向地面技能
{
	dbgPrint("LUA_SkillUseIDPos");

	LuaStack args(pState);
	if (args[1].IsInteger() && args[2].IsNumber() && args[3].IsNumber())
	{
		int nSkillId = args[1].GetInteger();
		float x = args[2].GetFloat();
		float y = args[3].GetFloat();

		dbgPrint("LUA_SkillUseIDPos nSkillId=%d x=%f y=%f", nSkillId, x, y);
		FUN_SkillUseIDPos(nSkillId, x, y);
	}


	return 1;
}

int CMeLua::LUA_DeathResurrection(LuaState* pState)
{
	dbgPrint("LUA_DeathResurrection");
	FUN_DeathResurrection();

	return 1;
}

int CMeLua::LUA_SetAttackMonsterPoints(LuaState* pState)
{
	dbgPrint("LUA_SetAttackMonsterPoints");

	LuaStack args(pState);
	if (args[1].IsString())
	{
		CString szPoint = args[1].GetString();
		FUN_SetAttackMonsterPoints(szPoint);
	}

	return 0;
}

//简化杀怪
int CMeLua::LUA_AttackMonster(LuaState* pState)
{
	dbgPrint("LUA_AttackMonster");

	LuaStack args(pState);

	if (args[1].IsNumber() && args[2].IsNumber() && args[3].IsString() && args[4].IsInteger() && args[5].IsString())
	{
		float x = args[1].GetFloat();
		float y = args[2].GetFloat();
		_tstring sceneName = args[3].GetString();
		int nType = args[4].GetInteger();
		_tstring monsterName = args[5].GetString();

		dbgPrint("LUA_AttackMonster x=%f, y=%f, nType=%d, sceneName=%s, monsterName=%s", x, y, nType, sceneName.c_str(), monsterName.c_str());
		FUN_AttackMonster(monsterName, sceneName, x, y);
		
		return 1;
	}

	return 0;
}

int CMeLua::LUA_AttackMonsterEx(LuaState* pState)
{
	dbgPrint("LUA_AttackMonsterEx");

	LuaStack args(pState);

	if (args[1].IsString())
	{
		_tstring monsterNames = args[1].GetString();
		dbgPrint("LUA_AttackMonsterEx = %s", monsterNames.c_str());
		auto  VstrName = UserSubMonsterName(monsterNames, _T('|'));
		if (g_pMeKill->Me_KillMonster(VstrName)) {
			pState->PushInteger(1);
			return 1;
		}
	}
	pState->PushInteger(0);
	return 0;
}

// 使用物品
int CMeLua::LUA_UseBagItem(LuaState* pState)
{
	dbgPrint("LUA_UseItem");

	LuaStack args(pState);
	_tstring strName = args[1].GetString();
	dbgPrint("LUA_UseItem strName=%s", strName.c_str());
	g_pMsg->UseItemName(strName);
	return 1;
}

int CMeLua::LUA_IsAvailableTask(LuaState* pState)
{
	LuaStack args(pState);
	_tstring strName = args[1].GetString();
	if (FUN_IsAvailableTask(strName))
	{
		pState->PushBoolean(TRUE);
		return 1;
	}
	pState->PushBoolean(FALSE);
	return 0;
}

// 判断在已接列表里是否有此任务
int CMeLua::LUA_IsTask(LuaState* pState)
{
	dbgPrint("LUA_IsTask");

	LuaStack args(pState);
	if (args[1].IsString())
	{
		_tstring strName = args[1].GetString();
		if (FUN_IsTask(strName))
		{
			pState->PushBoolean(TRUE);
			return 1;
		}
	}
	pState->PushBoolean(FALSE);
	return 0;
}

int CMeLua::LUA_IsTaskSuccess(LuaState* pState)
{
	dbgPrint("LUA_IsTaskSuccess");

	LuaStack args(pState);
	_tstring strName = args[1].GetString();
	if (FUN_IsTaskSuccess(strName) == 1)
	{
		pState->PushBoolean(TRUE);
		return 1;
	}
	pState->PushBoolean(FALSE);
	return 0;
}

int CMeLua::LUA_TaskAccept(LuaState* pState)
{
	dbgPrint("LUA_TaskAccept");

	LuaStack args(pState);

	if (args[1].IsString() && args[2].IsString() && args[3].IsString() && args[4].IsNumber() && args[5].IsNumber())
	{
		_tstring taskName = args[1].GetString();
		_tstring npcName = args[2].GetString();
		_tstring sceneName = args[3].GetString();
		float x = args[4].GetFloat();
		float y = args[5].GetFloat();

		dbgPrint("LUA_TaskAccept taskName=%s npcName=%s sceneName=%s x=%f y=%f", taskName.c_str(), npcName.c_str(), sceneName.c_str(), x, y);

		if (FUN_TaskAccep(taskName, npcName, sceneName, x, y))
		{
			pState->PushBoolean(TRUE);
			return 1;
		}
	}
	pState->PushBoolean(FALSE);
	return 0;
}

int CMeLua::LUA_TaskAcceptEx(LuaState* pState)
{
	dbgPrint("LUA_TaskAcceptEx");

	LuaStack args(pState);

	if (args[1].IsString())
	{
		_tstring taskName = args[1].GetString();

		dbgPrint("LUA_TaskAcceptEx taskName=%s", taskName.c_str());

		if (FUN_TaskAccepEx(taskName))
		{
			pState->PushBoolean(TRUE);
			return 1;
		}
	}
	pState->PushBoolean(FALSE);
	return 0;
}

// 执行对话任务
// 未找到任务中坐标和场景 所以还是得传 npc 坐标 场景等
int CMeLua::LUA_ExecuteTask(LuaState* pState)
{
	dbgPrint("LUA_ExecuteTask");

	LuaStack args(pState);

	if (args[1].IsString())
	{
		_tstring taskName = args[1].GetString();
		dbgPrint("LUA_ExecuteTask taskName=%s", taskName.c_str());

		if (FUN_ExecuteTask(taskName))
		{
			dbgPrint("LUA_ExecuteTask %s执行完成", taskName.c_str());
			pState->PushBoolean(TRUE);
			return 1;
		}
	}
	pState->PushBoolean(FALSE);
	return 0;
}

//执行杀怪任务
int CMeLua::LUA_ExecuteTaskKill(LuaState* pState)
{
	dbgPrint("Lua_ExecuteTaskKill");

	LuaStack args(pState);

	if (args[1].IsString() && args[2].IsInteger() && args[3].IsInteger())
	{
		_tstring monsterName;
		_tstring taskName = args[1].GetString();
		int nType = args[2].GetInteger();
		int nn = args[3].GetInteger();
		if (args[4] != NULL && args[4].IsString())
		{
			monsterName = args[4].GetString();
		}

		bool bPickUp = false;
		if (args[5] != NULL && args[5].IsBoolean())
		{
			bPickUp = args[5].GetBoolean();
		}
		
		dbgPrint("Lua_ExecuteTaskKill taskName=%s monsterName=%s ", taskName.c_str(), monsterName.c_str());

		if (FUN_ExecuteTaskKill(taskName, nType, nn, monsterName, bPickUp))
		{
			dbgPrint("Lua_ExecuteTaskKill %s执行完成", taskName.c_str());
			pState->PushBoolean(TRUE);
			return 1;
		}
	}
	pState->PushBoolean(FALSE);
	return 0;
}

//任务回执
int CMeLua::LUA_TaskSuccessEx(LuaState* pState)
{
	dbgPrint("LUA_TaskSuccessEx");

	LuaStack args(pState);

	if (args[1].IsString())
	{
		_tstring taskName = args[1].GetString();
		int nType = 1;

		if (args[2] != NULL && args[2].IsInteger())
		{
			nType = args[2].GetInteger();
		}

		dbgPrint("LUA_TaskSuccessEx taskName=%s nType=%d", taskName.c_str(), nType);

		FUN_TaskSuccessEx(taskName, nType);
		return 1;
	}
	return 0;
}

//执行任务
int CMeLua::LUA_TaskSuccess(LuaState* pState)
{
	dbgPrint("LUA_TaskSuccess");

	LuaStack args(pState);

	if (args[1].IsString() && args[2].IsString() && args[3].IsString() && args[4].IsNumber() && args[5].IsNumber())
	{
		_tstring taskName = args[1].GetString();
		_tstring npcName = args[2].GetString();
		_tstring sceneName = args[3].GetString();
		float x = args[4].GetFloat();
		float y = args[5].GetFloat();

		dbgPrint("LUA_TaskSuccess taskName=%s npcName=%s sceneName=%s x=%f y=%f", taskName.c_str(), npcName.c_str(), sceneName.c_str(), x, y);

		FUN_TaskSuccess(taskName, npcName, sceneName, x, y);
		return 1;
	}
	return 0;
}

int CMeLua::LUA_IsSkillName(LuaState* pState)	//判断是否学习了技能
{
	dbgPrint("LUA_IsSkillName");

	LuaStack args(pState);

	if (args[1].IsString())
	{
		_tstring skillName = args[1].GetString();
		if (!FUN_IsSkillName(skillName))
		{
			pState->PushBoolean(FALSE);
			return 1;
		}
	}

	return pState->PushBoolean(TRUE);
	return 0;
}

int CMeLua::LUA_GetBagItemIndex(LuaState* pState)	//判断是否学习了技能
{
	dbgPrint("LUA_GetBagItemIndex");

	LuaStack args(pState);

	if (args[1].IsString())
	{
		_tstring itemName = args[1].GetString();

		int index = FUN_GetItemIdxForName(itemName.c_str());
		if (index != -1 )
		{
			pState->PushInteger(1);
			return 1;
		}
		else {
			pState->PushInteger(0);
			return 1;
		}
	}

    pState->PushInteger(0);
	return 0;
}

int CMeLua::LUA_JoinMenPai(LuaState* pState)	//加入门派
{
	dbgPrint("LUA_JoinMenPai");

	LuaStack args(pState);

	if (args[1].IsString())
	{
		_tstring szMenPaiName = args[1].GetString();

		FUN_JoinMenPai(szMenPaiName);
		return 1;
	}

	return 0;
}

int CMeLua::LUA_InMenPai(LuaState* pState)
{
	dbgPrint("LUA_InMenPai");
	pState->PushInteger(8);
	return 0;
}

int CMeLua::LUA_GetMissionFinishInfo(LuaState* pState)
{
	LuaStack args(pState);

	if (args[1].IsString())
	{
		_tstring takeName = args[1].GetString();

		_tstring ret = FUN_GetMissionFinishInfo(takeName);
		pState->PushString(ret.c_str());
		return 1;
	}
	pState->PushString("");
	return 0;
}

int CMeLua::LUA_GetLevel(LuaState* pState)
{
	LuaStack args(pState);

	TAsmRoleInfo role = g_pAsmRole->GetRoleInfo();
	pState->PushInteger(role.nLevel);
	return 1;
}

int CMeLua::LUA_AutoWearEquipment(LuaState* pState)
{
	LuaStack args(pState);

	if (args[1].IsString())
	{
		_tstring itemNames = args[1].GetString();
		g_pAsmItem->AutoWearEquipment(itemNames);
		return 1;
	}
	return 0;
}

int CMeLua::LUA_AutoSell(LuaState* pState)
{
	dbgPrint("LUA_AutoSell");
	LuaStack args(pState);

	if (args[2].IsString() && args[3].IsInteger() && args[4].IsInteger() && args[5].IsString() && args[6].IsString())
	{
		CString sceneName = args[2].GetString();
		int nPosX = args[3].GetInteger();
		int nPosY = args[4].GetInteger();
		CString npcName = args[5].GetString();
		CString _talkName = args[6].GetString();
		FUN_AutoSell(sceneName, nPosX, nPosY, npcName, _talkName);
		return 1;
	}

	return 0;
}

int CMeLua::LUA_AutoBuy(LuaState* pState)
{
	dbgPrint("LUA_AutoBuy");
	LuaStack args(pState);

	if (args[1].IsString() && args[2].IsString() && args[3].IsInteger() && args[4].IsInteger() && args[5].IsString() && args[6].IsString())
	{
		CString itemNames = args[1].GetString();
		CString sceneName = args[2].GetString();
		int nPosX = args[3].GetInteger();
		int nPosY = args[4].GetInteger();
		CString npcName = args[5].GetString();
		CString _talkName = args[6].GetString();
		FUN_AutoBuy(itemNames, sceneName, nPosX, nPosY, npcName, _talkName);
		return 1;
	}

	return 0;
}

int CMeLua::LUA_AutoDestroy(LuaState* pState)
{
	LuaStack args(pState);

	if (args[1].IsString())
	{
		_tstring itemNames = args[1].GetString();
		g_pAsmItem->AutoDestroy(itemNames);
		return 1;
	}
	return 0;
}

//自动恢复角色状态
int CMeLua::LUA_AutoRoleStatus(LuaState* pState)
{
	LuaStack args(pState);

	if (args[1].IsInteger())
	{
		int nP = args[1].GetInteger();
		FUN_AutoRoleStatus(nP);
		return 1;
	}
	return 0;
}

int CMeLua::LUA_AutoStorage(LuaState* pState)
{
	LuaStack args(pState);

	if (args[1].IsString() && args[2].IsInteger() && args[3].IsInteger() && args[4].IsString() && args[5].IsString())
	{
		_tstring sceneName = args[1].GetString();
		int nPosX = args[2].GetInteger();
		int nPosY = args[3].GetInteger();
		_tstring npcName = args[4].GetString();
		_tstring _talkName = args[5].GetString();
		FUN_AutoStorage(sceneName, nPosX, nPosY, npcName, _talkName);
		return 1;
	}
	return 0;
}

int CMeLua::LUA_GetBagItemNum(LuaState* pState)
{
	dbgPrint("LUA_GetBagItemNum");
	LuaStack args(pState);

	if (args[1].IsString())
	{
		CString name = args[1].GetString();
		int nNum = g_pAsmItem->AsmGetItemNum(name);
		pState->PushInteger(nNum);
		return 1;
	}
	pState->PushInteger(-1);
	return 0;
}

int CMeLua::LUA_GetBagItemBlankNum(LuaState* pState)	//获取背包物品空白数量
{
	dbgPrint("LUA_GetBagItemBlankNum");
	LuaStack args(pState);

	int nCount = FUN_GetBagItemBlankNum();
	pState->PushInteger(nCount);
	return 1;
}

int CMeLua::LUA_RoleHMProtection(LuaState* pState)	//角色保护设置
{
	dbgPrint("LUA_RoleHMProtection");
	LuaStack args(pState);

	if (args[1].IsString() && args[2].IsString() && args[3].IsInteger() && args[4].IsString())
	{
		CString szLp = args[1].GetString();
		CString szTypeName = args[2].GetString();
		int nPer = args[3].GetInteger();
		CString szYaoNames = args[4].GetString();
		FUN_RoleHMProtection(szLp, szTypeName, nPer, szYaoNames);
		return 0;
	}

	return 1;
}

int CMeLua::LUA_AutoTeam(LuaState* pState)
{	
	dbgPrint("LUA_AutoTeam");
	LuaStack args(pState);
	if (FUN_AutoTeam()) {
		pState->PushBoolean(TRUE);
		return 1;
	}
	pState->PushBoolean(FALSE);
	return 0;
}

int CMeLua::LUA_CheckTeam(LuaState* pState)
{
	dbgPrint("LUA_CheckTeam");
	LuaStack args(pState);
	if (FUN_CheckTeam()) {
		pState->PushBoolean(TRUE);
		return 1;
	}
	pState->PushBoolean(FALSE);
	return 0;
}
