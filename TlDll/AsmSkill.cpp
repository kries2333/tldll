#include "pch.h"
#include "AsmSkill.h"
#include "FileSystem.h"
#include "Function.h"
#include "Base.h"

DWORD GetSkillBase()
{
	if (g_GameExeBase == 0) return 0;
	ULONG data = 0;
	if (IsBadReadPtr((DWORD*)(g_GameExeBase + LROLE_BASE), 4) == 0)
	{
		data = *(int*)(g_GameExeBase + LROLE_BASE);
		if (IsBadReadPtr((DWORD*)(data + 0x58), 4) == 0)
			data = *(int*)(data + 0x58);
	}
	return data;
}

bool CAsmSkill::AsmHaveMasterSkill(_tstring  skillName)//返回非零已学会
{
	VAsmSkill vSkill = AsmGetSkillData();
	for (int i = 0; i < vSkill.size(); i++)
	{
		if (vSkill[i].szName == skillName)
		{
			return TRUE;
		}
	}
	return FALSE;
}

VAsmSkill CAsmSkill::AsmGetSkillData()
{
	VAsmSkill vm_Skill;//技能容器
	DWORD data = 0;
	data = GetSkillBase();
	if (data == 0)
	{
		dbgPrint("技能基址错误");
		return vm_Skill;
	}

	try
	{
		dbgPrint("获取角色技能");

		DWORD dwTree = 0;
		

		if (IsBadReadPtr((DWORD*)(data + 0x144), 4) == 0)
		{
			data = *(DWORD*)(data + 0x144);
		}

		if (IsBadReadPtr((DWORD*)(data + 0x4), 4) == 0)
		{
			data = *(PULONG)(data + 0x4);
		}

		if (IsBadReadPtr((DWORD*)(data + SKILLS_TREE_OFFSET), 4) == 0)
		{
			data = *(PULONG)(data + SKILLS_TREE_OFFSET);
		}

		if (IsBadReadPtr((DWORD*)(data + 0x4), 4) == 0)
		{
			dwTree = *(PULONG)(data + 0x4);
		}
		
		dbgPrint("角色技能二叉树对象入口:%x", dwTree);
		DWORD dwCount = 0;
		if (dwTree != NULL)
		{
			AsmSkillTraverse((TAsmTree*)dwTree, vm_Skill, &dwCount);//取得二叉树基址后，开始遍历
		}
	}
	catch (...)
	{
		dbgPrint(__FUNCTION__);
	}

	return vm_Skill;
}


void CAsmSkill::AsmSkillTraverse(TAsmTree* pTree, VAsmSkill& vm_Skill, DWORD* pCount)//先序遍历  
{
	*pCount += 1;
	if (*pCount > 500)
	{
		return;
	}
	try
	{
		BYTE byData = -1;
		byData = *(BYTE*)((UINT)pTree + 0xD);

		if (byData == 0)
		{
			if (IsBadReadPtr(pTree, 4) == 0)
				this->AsmGetSkillInfo((PTAsmTree)pTree, vm_Skill);//取信息

			if (IsBadReadPtr(&(pTree->left), 4) == 0)
				this->AsmSkillTraverse((PTAsmTree)pTree->left, vm_Skill, pCount);//左递归

			if (IsBadReadPtr(pTree->right, 4) == 0)
				this->AsmSkillTraverse((PTAsmTree)pTree->right, vm_Skill, pCount);//右递归
		}
	}
	catch (...)
	{
		//dPrintA(__FUNCTION__);
	}
}

void CAsmSkill::AsmGetSkillInfo(TAsmTree* pTree, VAsmSkill& vm_Skill)//技能属性信息
{
	try
	{
		TAsmSkill tSkill;
		tSkill.unObject = *(PUINT)((UINT)pTree + 0x18);
		tSkill.unTree = (UINT)pTree;

		if (tSkill.unObject != 0)
		{
			//////////////////////////////////////////////////////////////////////////
			tSkill.nSkillId = *(int*)(tSkill.unTree + 0x10);// 技能编号
			//tSkill.nMenPai = *(int*)(tSkill.unObject + 0x8);//+8 门派编号
			tSkill.szName = (char*)(*(PUINT)(tSkill.unObject + 0xC));//+C技能名称
			tSkill.nSkillType = *(int*)(tSkill.unObject + 0x3C);//+3c 0蓄气技能 1引导技能 2直接使用技能 
			//tSkill.nCoolTimeID = *(int*)(tSkill.unObject + 0x40);//+3c 0蓄气技能 1引导技能 2直接使用技能 
			//tSkill.nSubType = *(int*)(tSkill.unObject + 0x98);//0增强自身  4自身范围 5单体攻击或者增强 7两次触发
			tSkill.fMin = *(float*)(tSkill.unObject + 0xA0);//+A0 是攻击距离上限
			tSkill.fMax = *(float*)(tSkill.unObject + 0xA4);//+A4 是攻击距离下限
			//tSkill.nStandFlag = *(int*)(tSkill.unObject + 0xA8);//+A8 //这个是技能的立场标记，-1代表技能消弱目标，0是中性，1是技能增强目标
			//tSkill.nfriend = *(int*)(tSkill.unObject + 0xAC);//+AC: 目标和使用者应该是友好关系 //1:目标和使用者应该是敌对关系
			//tSkill.nAutoShot = *(int*)(tSkill.unObject + 0xBC);//+BC 自动连续释放技能
			//tSkill.fRadius = *(float*)(tSkill.unObject + 0xD0);//+D0 //群攻的半径
			//tSkill.fAngle = *(float*)(tSkill.unObject + 0xD4);//+D4 //群攻的范围 360周边
			//tSkill.nTargetType = *(int*)(tSkill.unObject + 0xDC);//+DC 0: 目标必须是玩家 1:目标必须是宠物 2:目标必须是怪物
			//tSkill.szDescription = (char*)(*(PUINT)(tSkill.unObject + 0x150));//技能描述信息
			//tSkill.nPassiveFlag = *(int*)(tSkill.unObject + 0x154);//主动释放 还是自动释放
			vm_Skill.push_back(tSkill);
			//dbgPrint("技能信息 szName=%s, nSkillId=%d, fMin=%f, fMax=%f", 
			//	tSkill.szName, tSkill.nSkillId, tSkill.fMin, tSkill.fMax);
		}
	}
	catch (...)
	{
		//dPrintA(__FUNCTION__);
	}
}

void CAsmSkill::AsmUseSkillCall(int MonsterId, int SkillId)
{
	try
	{
		DWORD base = GetSkillBase();
		if (base == 0)
		{
			dbgPrint("GetSkillBase 错误");
			return;
		}

		DWORD skillCall = 0;
		DWORD unEcx = 0;
		if (IsBadReadPtr((DWORD*)(g_GameExeBase + SKILLS_CALL), 4) == 0)
			skillCall = (DWORD)(g_GameExeBase + SKILLS_CALL);

		
		if (IsBadReadPtr((DWORD*)(base + 0x150), 4) == 0)
			unEcx = *(PULONG)(base + 0x150);

		__asm {
			push 0xBF800000
			push 0xBF800000
			push 0xBF800000
			mov ebx, MonsterId
			push ebx
			push 0xFFFFFFFF
			mov ebx, SkillId
			push ebx
			mov ecx, unEcx
			mov ebx, skillCall
			call ebx
		}

	}
	catch (...)
	{
		dbgPrint("调用技能失败:", __FUNCTION__);
	}
}

void CAsmSkill::AsmUseSkillCallByPoint(int SkillId, float fx, float fy)
{
	try
	{
		DWORD base = GetSkillBase();
		if (base == 0)
		{
			dbgPrint("GetSkillBase 错误");
			return;
		}

		DWORD skillCall = 0;
		DWORD unEcx = NULL;
		if (IsBadReadPtr((DWORD*)(base + 0x150), 4) == 0)
			unEcx = *(DWORD*)(base + 0x150);
		if (unEcx != NULL && IsBadReadPtr((DWORD*)(unEcx), 4) == 0)
			base = *(DWORD*)(unEcx);
		if (IsBadReadPtr((DWORD*)(base +0x18), 4) == 0)
			skillCall = *(DWORD*)(base + 0x18);

		//dbgPrint("skillCall=%X unEcx=%X", skillCall, unEcx);
		if (skillCall == 0 || unEcx == 0)
		{
			return;
		}

		__asm {
			push fy
			push fx
			push SkillId
			mov ecx, unEcx
			mov eax, skillCall
			call eax
		}

	}
	catch (...)
	{
		dbgPrint("调用怪物失败:", __FUNCTION__);
	}
}