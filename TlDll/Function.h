#pragma once
#include "AsmMonster.h"
#include "AsmSkill.h"
#include "User.h"

enum TASK_TYPE
{
	��ʼ�һ� = 1,
	��ʼ���� = 2,
	��ʼ���� = 3,
	��ʼ�ɼ� = 4,
	��ʼˢ�� = 5,
	��ʼ��ͼ = 6,
	��ʼ���� = 7
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
	/////////////////////////////������صĺ�����װ//////////////
	/************************************************************************/
	BOOL FUN_GetPlayerMission(_tstring taskName);

	/************************************************************************/
	/////////////////////////////Ѱ·��صĺ�����װ//////////////
	/************************************************************************/
	int FUN_GetSceneID(const char* szSceneName);//ͨ�����ƻ�ȡ��ͼ����ID
	_tstring FUN_GetSceneName(int nSceneId);
	//BOOL  FUN_RunToTarget(float fx, float fy, float dis = 2); //����Ѱ·����
	BOOL  FUN_RunToTargetEx(float x, float y, int SceneId, float dis = 2);//���ͼѰ·
	float FUN_GetDistance(float x2, float y2);//��ȡ��Ŀ��ľ���

	void FUN_SetAttackMonsterPoints(CString szPoints);

	//ִ��ɱ��
	void FUN_AttackMonster(_tstring monsterName, _tstring sceneName, float x, float y);

	/************************************************************************/
	/* ���������ֺ���                                                     */
	/************************************************************************/
	void FUN_MisKillMonsterByName(_tstring MonsterName);

	//ץ��
	bool FUN_MisUseSkillCatchMonster(_tstring monsterName);

	//ͨ����Ʒ���ƻ�ȡIdx
	int FUN_GetItemIdxForName(_tstring strName);

	VString FUN_GetTaskInfo(_tstring taskName);

	_tstring FUN_GetMissionTaskKillMonsterInfo(_tstring taskName);
	//===================================================================================
	//��������
	BOOL FUN_Questions();

	//�ж������Ƿ�ɽ�
	bool FUN_IsAvailableTask(_tstring taskName);

	_tstring FUN_GetAvailableTaskInfo(_tstring taskName);

	_tstring FUN_GetMissionFinishInfo(_tstring taskName);

	bool FUN_IsTask(_tstring taskName);

	//�ж��ѽ��б�  -1 δ�ҵ� 0 δ���  1 �Ѿ����
	int FUN_IsTaskSuccess(_tstring taskName);

	float FUN_MaximumRange(TAsmSkill tAsmSkill);

	//�ж��Ƿ�ѧϰ����
	bool FUN_IsSkillName(CString taskName);

	bool FUN_SkillUseIDPos(int nSkillId, float x, float y);

	//��ȡ����
	bool FUN_TaskAccep(_tstring taskName, _tstring npcName, _tstring sceneName, float x, float y);

	bool FUN_TaskAccepEx(_tstring taskName);

	//ִ�жԻ�����
	bool FUN_ExecuteTask(_tstring taskName);

	//ִ��ɱ������
	bool FUN_ExecuteTaskKill(_tstring taskName, int nType, int nMonsterType, _tstring monsterName, bool bPickUp);
	
	//ִ������ѧϰ����--һ����ָ��npc�Ի�
	void FUN_TaskSuccess(_tstring taskName, _tstring npcName, _tstring sceneName, float x, float y);

	//ִ�жԻ�����
	bool FUN_TaskSuccessEx(_tstring taskName, int nType);

	//��������
	void FUN_JoinMenPai(_tstring szMenPaiName);

	//����
	void FUN_Levelup();

	//ʰȡ
	void FUN_PickUp();

	int FUN_AutoMove();

	BOOL FUN_IsQuitRelative();

	void FUN_DeathResurrection();

	void FUN_SkillUseID(int nSkillId);

	BOOL FUN_RunTo(float x, float y, int nTime);

	void FUN_AutoSell(CString sceneName, int nPosX, int nPosY, CString npcName, CString _talkName);

	void FUN_AutoBuy(CString itemNames, CString sceneName, int nPosX, int nPosY, CString npcName, CString _talkName);

	void FUN_AutoStorage(_tstring sceneName, int nPosX, int nPosY, _tstring npcName, _tstring _talkName);

	//�Զ��ָ���ɫ״̬
	void FUN_AutoRoleStatus(int nPercentage);

	int FUN_GetBagItemBlankNum();

	//��ɫ������ҩ����
	void FUN_RoleHMProtection(CString szLp, CString szTypeName, int Per, CString szYaoNames);

	//���ﱣ����ҩ����
	void FUN_PetHMProtection(CString szTypeName, int Per, CString szYaoNames);

	/************************************************************************/
	/////////////////////////////��Ӻ�����װ//////////////
	/************************************************************************/
	BOOL FUN_AutoTeam(int nNum);
	BOOL FUN_CheckTeam(int nNum);

protected:
	/************************************************************************/
	/////////////////////////////���ܴ����صĺ�����װ//////////////
	/************************************************************************/
	bool FUN_MinDistanceObject(float* fdistance, float fx, float fy);	//true�������
	void FUN_UseMovAttackSkill(TAsmSkill tAsmSkill, TAsmMonster tAsmMonster);//��ʼ�ͷŹ�������
	bool FUN_MovToMonster(TAsmMonster tAsmMonster, TAsmSkill tAsmSkill);//�ߵ����������
	bool FUN_IsMonsterDie(TAsmMonster* ptAsmMonster);//falseΪ����
	TAsmMonster FUN_GetMonsterByName(_tstring MonsterName, int nMonsterType, int nKillType);//���ƻ�ȡ����Ĺ���
	TAsmMonster FUN_GetCatchMonsterByName(_tstring MonsterName);

	BOOL FUN_KillMonsterByName(VUserMonsterName vm_UserMonsterName);
	void FUN_UseSkillKillMonster(TAsmMonster tAsmMonster);
	void FUN_UseAttackSkill(TUserSkill tAsmSkill/*CString SkillName*/, TAsmMonster tAsmMonster);
};

