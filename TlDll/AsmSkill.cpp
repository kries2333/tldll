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
	}

	if (IsBadReadPtr((DWORD*)(data + 0x58), 4) == 0)
	{
		data = *(int*)(data + 0x58);
	}
	
	return data;
}

bool CAsmSkill::AsmHaveMasterSkill(_tstring  skillName)//���ط�����ѧ��
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

	//dd [[[[[[[0x9FF458]+0x74+]+0x1ec]+0x4]+0x2860]+0x4]+0x14]+0xc]
	VAsmSkill vm_Skill;//��������
	DWORD data = 0;
	data = GetSkillBase();
	if (data == NULL)
	{
		dbgPrint("���ܻ�ַ����");
		return vm_Skill;
	}

	try
	{
		dbgPrint("��ȡ��ɫ����");

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
		
		dbgPrint("��ɫ���ܶ������������:%x", dwTree);
		DWORD dwCount = 0;
		if (dwTree != NULL)
		{
			AsmSkillTraverse((TAsmTree*)dwTree, vm_Skill, &dwCount);//ȡ�ö�������ַ�󣬿�ʼ����
		}
	}
	catch (...)
	{
		dbgPrint(__FUNCTION__);
	}

	return vm_Skill;
}


void CAsmSkill::AsmSkillTraverse(TAsmTree* pTree, VAsmSkill& vm_Skill, DWORD* pCount)//�������  
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
				this->AsmGetSkillInfo((PTAsmTree)pTree, vm_Skill);//ȡ��Ϣ

			if (IsBadReadPtr(&(pTree->left), 4) == 0)
				this->AsmSkillTraverse((PTAsmTree)pTree->left, vm_Skill, pCount);//��ݹ�

			if (IsBadReadPtr(pTree->right, 4) == 0)
				this->AsmSkillTraverse((PTAsmTree)pTree->right, vm_Skill, pCount);//�ҵݹ�
		}
	}
	catch (...)
	{
		//dPrintA(__FUNCTION__);
	}
}

void CAsmSkill::AsmGetSkillInfo(TAsmTree* pTree, VAsmSkill& vm_Skill)//����������Ϣ
{
	try
	{
		TAsmSkill tSkill;
		tSkill.unObject = *(PUINT)((UINT)pTree + 0x18);
		tSkill.unTree = (UINT)pTree;

		if (tSkill.unObject != 0)
		{
			//////////////////////////////////////////////////////////////////////////
			tSkill.nSkillId = *(int*)(tSkill.unTree + 0x10);// ���ܱ��
			//tSkill.nMenPai = *(int*)(tSkill.unObject + 0x8);//+8 ���ɱ��
			tSkill.szName = (char*)(*(PUINT)(tSkill.unObject + 0xC));//+C��������
			tSkill.nSkillType = *(int*)(tSkill.unObject + 0x3C);//+3c 0�������� 1�������� 2ֱ��ʹ�ü��� 
			//tSkill.nCoolTimeID = *(int*)(tSkill.unObject + 0x40);//+3c 0�������� 1�������� 2ֱ��ʹ�ü��� 
			//tSkill.nSubType = *(int*)(tSkill.unObject + 0x98);//0��ǿ����  4����Χ 5���幥��������ǿ 7���δ���
			tSkill.fMin = *(float*)(tSkill.unObject + 0xA0);//+A0 �ǹ�����������
			tSkill.fMax = *(float*)(tSkill.unObject + 0xA4);//+A4 �ǹ�����������
			//tSkill.nStandFlag = *(int*)(tSkill.unObject + 0xA8);//+A8 //����Ǽ��ܵ�������ǣ�-1����������Ŀ�꣬0�����ԣ�1�Ǽ�����ǿĿ��
			//tSkill.nfriend = *(int*)(tSkill.unObject + 0xAC);//+AC: Ŀ���ʹ����Ӧ�����Ѻù�ϵ //1:Ŀ���ʹ����Ӧ���ǵжԹ�ϵ
			//tSkill.nAutoShot = *(int*)(tSkill.unObject + 0xBC);//+BC �Զ������ͷż���
			//tSkill.fRadius = *(float*)(tSkill.unObject + 0xD0);//+D0 //Ⱥ���İ뾶
			//tSkill.fAngle = *(float*)(tSkill.unObject + 0xD4);//+D4 //Ⱥ���ķ�Χ 360�ܱ�
			//tSkill.nTargetType = *(int*)(tSkill.unObject + 0xDC);//+DC 0: Ŀ���������� 1:Ŀ������ǳ��� 2:Ŀ������ǹ���
			//tSkill.szDescription = (char*)(*(PUINT)(tSkill.unObject + 0x150));//����������Ϣ
			//tSkill.nPassiveFlag = *(int*)(tSkill.unObject + 0x154);//�����ͷ� �����Զ��ͷ�
			vm_Skill.push_back(tSkill);
			//dbgPrint("������Ϣ szName=%s, nSkillId=%d, fMin=%f, fMax=%f", 
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
			dbgPrint("GetSkillBase ����");
			return;
		}

		DWORD skillCall = 0;
		if (IsBadReadPtr((DWORD*)(g_GameExeBase + SKILLS_CALL), 4) == 0)
		{
			skillCall = (DWORD)(g_GameExeBase + SKILLS_CALL);
		}

		DWORD unEcx = 0;
		if (IsBadReadPtr((DWORD*)(base + 0x150), 4) == 0)
		{
			unEcx = *(PULONG)(base + 0x150);
		}

		if (skillCall == 0 || unEcx == 0)
		{
			return;
		}

		__asm {
			pushad
			pushfd
			push 0xBF800000
			push 0xBF800000
			push 0xBF800000
			push MonsterId
			push 0xFFFFFFFF
			push SkillId
			mov ecx, unEcx
			mov eax, skillCall
			call eax
			popfd
			popad
		}

	}
	catch (...)
	{
		dbgPrint("���ù���ʧ��:", __FUNCTION__);
	}
}

void CAsmSkill::AsmUseSkillCallByPoint(int SkillId, float fx, float fy)
{
	try
	{
		DWORD base = GetSkillBase();
		if (base == 0)
		{
			dbgPrint("GetSkillBase ����");
			return;
		}

		DWORD skillCall = 0;
		DWORD unEcx = 0;
		if (IsBadReadPtr((DWORD*)(base + 0x150), 4) == 0)
		{
			unEcx = *(DWORD*)(base + 0x150);
		}

		if (IsBadReadPtr((DWORD*)(unEcx), 4) == 0)
		{
			base = *(DWORD*)(unEcx);
		}

		if (IsBadReadPtr((DWORD*)(base +0x18), 4) == 0)
		{
			skillCall = *(DWORD*)(base + 0x18);
		}

		//dbgPrint("skillCall=%X unEcx=%X", skillCall, unEcx);
		if (skillCall == 0 || unEcx == 0)
		{
			return;
		}

		__asm {
			pushad
			pushfd
			push fy
			push fx
			push SkillId
			mov ecx, unEcx
			mov eax, skillCall
			call eax
			popfd
			popad
		}

	}
	catch (...)
	{
		dbgPrint("���ù���ʧ��:", __FUNCTION__);
	}
}