#include "pch.h"
#include "ScriptSystem.h"

void CScriptSystem::LUA_ScriptSystemInitial(/*CString strScriptName*/)
{
	MeLua = new CMeLua();
	pState = new LuaStateOwner(true);
	LuaObject objGlobal = (*pState)->GetGlobals();

	objGlobal.Register("Script", *MeLua,
		&CMeLua::LUA_Script);//��������ַ�������

	objGlobal.Register("Stop", *MeLua,
		&CMeLua::LUA_Stop);//ֹͣlua�ű��������

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

	//////////////////////////////�������������//////////////////////////////////////
	objGlobal.Register("ui_IsQuizDiaLog", *MeLua,
			&CMeLua::LUA_IsQuizDiaLog);//�ж��Ƿ񽭺����ⴰ��

	objGlobal.Register("task_Questions", *MeLua,
		&CMeLua::LUA_Questions);//ִ�н�������

	objGlobal.Register("lua_MoveName", *MeLua,
		&CMeLua::LUA_MoveTo);//�ƶ�����
	
	objGlobal.Register("task_IsAvailableTask", *MeLua,
		&CMeLua::LUA_IsAvailableTask);	//�ж������Ƿ�ɽ�

	objGlobal.Register("task_IsTask", *MeLua,
		&CMeLua::LUA_IsTask);	//�ж������Ƿ����ѽ��б���

	objGlobal.Register("task_IsTaskSuccess", *MeLua,
		&CMeLua::LUA_IsTaskSuccess);	//�ж������Ƿ����

	objGlobal.Register("lua_TaskAccept", *MeLua,
		&CMeLua::LUA_TaskAccept);		//��ȡ����

	objGlobal.Register("lua_TaskAcceptEx", *MeLua,
		&CMeLua::LUA_TaskAcceptEx);		//��ȡ����

	objGlobal.Register("lua_ExecuteTask", *MeLua,
		&CMeLua::LUA_ExecuteTask);		//ִ�жԻ�����

	objGlobal.Register("lua_ExecuteTaskKill", *MeLua,
		&CMeLua::LUA_ExecuteTaskKill);		//ִ��ɱ������

	objGlobal.Register("lua_TaskSuccessEx", *MeLua,
		&CMeLua::LUA_TaskSuccessEx);		//����ִ����ɺ��ִ

	objGlobal.Register("lua_TaskSuccess", *MeLua,
		&CMeLua::LUA_TaskSuccess);		//����ִ����ɺ��ִ

	objGlobal.Register("lua_SetAttackMonsterPoints", *MeLua,
		&CMeLua::LUA_SetAttackMonsterPoints);		//ɱ���б� Ӧ���뵥��ɱ�ֺϲ�
	
	objGlobal.Register("lua_AttackMonster", *MeLua,
		&CMeLua::LUA_AttackMonster);		//����ɱ��

	objGlobal.Register("lua_AttackMonsterEx", *MeLua,
		&CMeLua::LUA_AttackMonsterEx);		//ɱ���б� Ӧ���뵥��ɱ�ֺϲ�

	objGlobal.Register("role_GetLevel", *MeLua,
		&CMeLua::LUA_GetLevel);		//��ȡ�ȼ�

	objGlobal.Register("role_GetRaceName", *MeLua,
		&CMeLua::LUA_GetMenPaiName);		//��ȡ����
	
	objGlobal.Register("lua_JoinMenPai", *MeLua,
		&CMeLua::LUA_JoinMenPai);		//��������

	objGlobal.Register("skill_IsSkillName", *MeLua,
		&CMeLua::LUA_IsSkillName);		//�ж��Ƿ�ѧϰ�˼���

	objGlobal.Register("role_GetMapName", *MeLua,
		&CMeLua::LUA_GetMapName);		//��ȡ��ǰ��ͼ����

	objGlobal.Register("skill_UseIDPos", *MeLua,
		&CMeLua::LUA_SkillUseIDPos);		//ʹ��ָ����漼��

	objGlobal.Register("skill_UseID", *MeLua,
		&CMeLua::LUA_SkillUseID);		//ʹ�ü���

	objGlobal.Register("AutoSell", *MeLua,
		&CMeLua::LUA_AutoSell);		//

	objGlobal.Register("AutoWearEquipment", *MeLua,
		&CMeLua::LUA_AutoWearEquipment);		//ʹ�ü���

	objGlobal.Register("AutoDestroy", *MeLua,
		&CMeLua::LUA_AutoDestroy);		//ʹ�ü���

	objGlobal.Register("lua_DeathResurrection", *MeLua,
		&CMeLua::LUA_DeathResurrection);		//��������

	objGlobal.Register("lua_RoleHMProtection", *MeLua,
		&CMeLua::LUA_RoleHMProtection);		//���ý�ɫ����

	objGlobal.Register("lua_PetHMProtection", *MeLua,
		&CMeLua::LUA_PetHMProtection);		//���ó��ﱣ��

	objGlobal.Register("lua_AutoTeam", *MeLua, 
		&CMeLua::LUA_AutoTeam);			//�Զ����

	objGlobal.Register("lua_CheckTeam", *MeLua,
		&CMeLua::LUA_CheckTeam);			//������

	objGlobal.Register("lua_AutoCleanBag", *MeLua,
		&CMeLua::LUA_AutoCleanBag);			//������
	//======����Ϊ����ӿ�=============================================

	//========���²��Խӿ�ʹ�ã�������Ҫ����=============================================


	objGlobal.Register("HuoDongRiCheng_next_click", *MeLua,
		&CMeLua::HuoDongRiCheng_next_click);//��·call

	objGlobal.Register("AutoRunToTarget", *MeLua,
		&CMeLua::LUA_AutoRunToTarget);//��·call

	objGlobal.Register("MoveToNPC", *MeLua,
		&CMeLua::LUA_MoveToNPC);//�ƶ���NPC

	objGlobal.Register("MoveToNPCEx", *MeLua,
		&CMeLua::LUA_MoveToNPCEx);//�ƶ���NPC���Ի�

	objGlobal.Register("InMenPai", *MeLua,
		&CMeLua::LUA_InMenPai);//�ƶ���NPC

	objGlobal.Register("lua_AutoRoleStatus", *MeLua,
		&CMeLua::LUA_AutoRoleStatus);//�ָ�

	objGlobal.Register("lua_AutoStorage", *MeLua,
		&CMeLua::LUA_AutoStorage);//�洢

	objGlobal.Register("lua_AutoBuy", *MeLua,
		&CMeLua::LUA_AutoBuy);//�Զ�����

	objGlobal.Register("lua_AutoSell", *MeLua,
		&CMeLua::LUA_AutoSell);//�Զ�����

	objGlobal.Register("lua_GetBagItemNum", *MeLua,
		&CMeLua::LUA_GetBagItemNum);//��ȡ������Ʒ����

	objGlobal.Register("lua_GetBagItemBlankNum", *MeLua,
		&CMeLua::LUA_GetBagItemBlankNum);//��ȡ������Ʒ�հ�����

	objGlobal.Register("GoFight", *MeLua, &CMeLua::LUA_PetGoFight);//�����ս

	objGlobal.Register("PetCatch", *MeLua, &CMeLua::LUA_PetCatch);//ץ������

	// ��Ʒ
	objGlobal.Register("pack_GetBagItemNum", *MeLua, &CMeLua::LUA_GetBagItemNum);//ʹ����Ʒ
	objGlobal.Register("pack_UseBagItem", *MeLua, &CMeLua::LUA_UseBagItem);//ʹ����Ʒ

	//-------------------------------------------------------------------------------------
	//ע��System��������///////////////////////////////////////////////////////////////////////
	LuaObject metatable_System = objGlobal.CreateTable("MetaTableSystem");
	metatable_System.SetObject("__index", metatable_System);

	metatable_System.RegisterObjectFunctor("Sleep", &CMeLua::LUA_Sleep);//�ӳٺ���
	LuaObject obj_System = (*pState)->BoxPointer(&MeLua);
	obj_System.SetMetaTable(metatable_System);
	objGlobal.SetObject("System", obj_System);

	//-------------------------------------------------------------------------------------
	//ע��DataBase��������//////////////////////////////////////////////////////////////////////
	LuaObject metatable_DataBase = objGlobal.CreateTable("MetaTableDataBase");
	metatable_DataBase.SetObject("__index", metatable_DataBase);

	metatable_DataBase.RegisterObjectFunctor("GetSceneID", &CMeLua::LUA_GetSceneId);//��ȡ����id

	LuaObject obj_DataBase = (*pState)->BoxPointer(&MeLua);
	obj_DataBase.SetMetaTable(metatable_DataBase);
	objGlobal.SetObject("DataBase", obj_DataBase);

	//ע��DataPool��������///////////////////////////////////////////////////////////////////////
	LuaObject metatable_DataPool = objGlobal.CreateTable("MetaTableDataPool");
	metatable_DataPool.SetObject("__index", metatable_DataPool);

	metatable_DataPool.RegisterObjectFunctor("GetPlayerMission"
		, &CMeLua::LUA_GetPlayerMission);//��ȡ��ǰ�ѽ�����

	metatable_DataPool.RegisterObjectFunctor("GetPlayerMission_Variable"
		, &CMeLua::LUA_GetPlayerMission_Variable);//������ɽ��� 1��ɣ�2ʧ�ܣ�0����

	LuaObject obj_DataPool = (*pState)->BoxPointer(&MeLua);
	obj_DataPool.SetMetaTable(metatable_DataPool);
	objGlobal.SetObject("DataPool", obj_DataPool);
}
