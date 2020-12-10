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

	int LUA_DoString(LuaState* pState);//lua�ַ�������

	int LUA_GetMissionFinishInfo(LuaState* pState);

	int LUA_IsQuitRelative(LuaState* pState);

	/************************************************************************/
	/////////////////////////////Ѱ·��صĺ�����װ//////////////
	/************************************************************************/
	int  LUA_GetSceneId(LuaState* pState);
	int  LUA_AutoRunToTarget(LuaState* pState);//�ƶ���Ŀ��
	int  LUA_MoveToNPC(LuaState* pState);//������������ɱ��
	int  LUA_MoveToNPCEx(LuaState* pState);
	int  LUA_MoveTo(LuaState* pState);//�ƶ�����

	/************************************************************************/
	//////////////////////////////����������صĺ�����װ///////
	/************************************************************************/
	int LUA_GetPlayerMission(LuaState* pState);//�Ƿ��Ѿ���ȡ������
	int LUA_GetPlayerMission_Variable(LuaState* pState);//�Ƿ�ɽ���

	int HuoDongRiCheng_next_click(LuaState* pState);

	int LUA_Script(LuaState* pState);


	/************************************************************************/
	/////////////////////////////NPC�Ի���صĺ�����װ//////////////
	/************************************************************************/
	//int LUA_QuestFrameOptionClicked(LuaState* pState);//����Ի�ѡ������ǶԻ�ѡ���ַ���
	//int LUA_QuestFrameMissionContinue(LuaState* pState);//����
	//int LUA_MissionContinue_Clicked(LuaState* pState);//���
	//int LUA_MissionQuestAccept_Clicked(LuaState* pState);//����
	//int LUA_Quest_OnHidden(LuaState* pState);//�ر�

	/************************************************************************/
	/////////////////////////////���������Χ������صĺ�����װ///////
	/************************************************************************/
	//int LUA_KillAnyMonster(LuaState* pState);//ɱ����,����ָ������
	//int LUA_KillMonsterByName(LuaState* pState);//����ɱ����

	//=========================================================================
	int LUA_IsQuizDiaLog(LuaState* pState); // �ж��Ƿ��ڽ�������
	int LUA_Questions(LuaState* pState);	//��ʼ����
	int LUA_PetGoFight(LuaState* pState); //�����ս
	int LUA_PetCatch(LuaState* pState); //ץ��
	int LUA_SkillUseID(LuaState* pState); //ʹ�ü���
	int LUA_SkillUseIDPos(LuaState* pState); //ʹ��ָ����漼��
	int LUA_GetMapName(LuaState* pState); // ��ȡ��ǰ��ͼ����
	int LUA_GetRolePosX(LuaState* pState);	//��ȡ��ɫ��ǰ����
	int LUA_GetRolePosY(LuaState* pState);

	int LUA_DeathResurrection(LuaState* pState);

	// ��Ʒ
	int LUA_UseBagItem(LuaState* pState); //ʹ����Ʒ

	int LUA_UpdateQuest(LuaState* pState);

	// ���·�װ���ⲿ���ýӿ�
	int LUA_IsAvailableTask(LuaState* pState); //�ж������Ƿ��ڿɽ��б���

	int LUA_IsTask(LuaState* pState);  //�ж������Ƿ����ѽ��б���

	int LUA_IsTaskSuccess(LuaState* pState);   //�ж������Ƿ����

	int LUA_TaskAccept(LuaState* pState);	//��ȡ����

	int LUA_TaskAcceptEx(LuaState* pState); //��ȡ����

	int LUA_ExecuteTask(LuaState* pState);	//ִ������

	int LUA_ExecuteTaskKill(LuaState* pState); //ִ��ɱ������

	int LUA_TaskSuccessEx(LuaState* pState);  //��������ִ

	int LUA_TaskSuccess(LuaState* pState); //ִ�жԻ�����

	int LUA_AttackMonster(LuaState* pState);	//����ɱ��

	int LUA_AttackMonsterEx(LuaState* pState);

	int LUA_IsSkillName(LuaState* pState);	//�ж��Ƿ�ѧϰ�˼���

	int LUA_GetBagItemNum(LuaState* pState); //�жϱ����Ƿ��д���Ʒ

	int LUA_GetMenPaiName(LuaState* pState); //��ȡ����

	int LUA_JoinMenPai(LuaState* pState);

	int LUA_InMenPai(LuaState* pState);	//��ȡ����������

	int LUA_GetLevel(LuaState* pState);

	int LUA_AutoWearEquipment(LuaState* pState);

	int LUA_AutoSell(LuaState* pState);

	int LUA_AutoDestroy(LuaState* pState);

	int LUA_AutoBuy(LuaState* pState);

	int LUA_AutoStorage(LuaState* pState);

	int LUA_AutoRoleStatus(LuaState* pState);
};

