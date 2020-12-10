#include "pch.h"
#include "Base.h"
#include "AsmMonster.h"
#include "FileSystem.h"
#include "Function.h"

DWORD CAsmMonster::AsmGetEnvTreeBase()
{
	if (g_GameExeBase == 0) return 0;
	ULONG data = 0;
	data = *(int*)(g_GameExeBase + LROLE_BASE);
	if (IsBadReadPtr((int*)data + 0x4c, 4) == 0) {
		data = *(DWORD*)(data + 0x4c);
	}
	if (IsBadReadPtr((int*)data + 0x4, 4) == 0) {
		data = *(DWORD*)(data + 0x4);
	}
	return data;
}

BOOL CAsmMonster::AsmTreeObjectType(DWORD Object, char* szObjectName)
{
	bool bOk = false;

	try
	{
		DWORD ObjectTypeString = 0;

		__asm
		{
			mov ecx, Object;
			mov eax, [ecx];
			call[eax + 0x38];
			mov eax, [eax];
			mov ObjectTypeString, eax;
		}
		//dbgPrint("ObjectTypeString = %s", (CHAR*)ObjectTypeString);
		if (ObjectTypeString != 0 && strstr(szObjectName, (CHAR*)ObjectTypeString) != 0)
		{
			bOk = true;
		}

	}
	catch (...)
	{
		//dPrintA(__FUNCTION__);
	}

	return bOk;
}

VAsmMonster CAsmMonster::AsmGetMonsterData()
{
	VAsmMonster vm_Monster;
	DWORD dwCount = 0;
	try
	{
		DWORD dwTree = AsmGetEnvTreeBase();
		if (dwTree)//正确取到二叉树基址
			AsmMonsterTraverse((TAsmTree*)dwTree, vm_Monster, &dwCount);//遍历二叉树
	}
	catch (...)
	{
		//dPrintA(__FUNCTION__);
	}

	return vm_Monster;
}

void CAsmMonster::AsmMonsterTraverse(TAsmTree* pTree, VAsmMonster& vm_Monster, DWORD* pCount)
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
				AsmGetMonsterInfo(pTree, vm_Monster);//取信息

			if (IsBadReadPtr(pTree->left, 4) == 0)
				AsmMonsterTraverse((PTAsmTree)pTree->left, vm_Monster, pCount);//左递归

			if (IsBadReadPtr(pTree->right, 4) == 0)
				AsmMonsterTraverse((PTAsmTree)pTree->right, vm_Monster, pCount);//右递归
		}
	}
	catch (...)
	{
		//dPrintA(__FUNCTION__);
	}
}

void CAsmMonster::AsmGetMonsterInfo(TAsmTree* pTree, VAsmMonster& vm_Monster)
{
	try
	{
		TAsmMonster tMonster;
		tMonster.dwTree = (DWORD)pTree;


		if (IsBadReadPtr((DWORD*)((DWORD)pTree + 0x14), 4) == 0)
		{
			tMonster.dwObject = *(DWORD*)((DWORD)pTree + 0x14);	//这里是对象

			if (tMonster.dwObject > 0x10401000)
			{
				if (AsmTreeObjectType(tMonster.dwObject, "CObject_PlayerNPC") == TRUE)
				{
					if (IsBadReadPtr((int*)(tMonster.dwTree + 0x10), 4) == 0)
						tMonster.nMonsterId = *(int*)((DWORD)tMonster.dwTree + 0x10);
					if (IsBadReadPtr((float*)(tMonster.dwObject + 0x3c), 4) == 0)
						tMonster.fMonsterX = *(float*)(tMonster.dwObject + 0x3c);
					if (IsBadReadPtr((float*)(tMonster.dwObject + 0x44), 4) == 0)
						tMonster.fMonsterY = *(float*)(tMonster.dwObject + 0x44);

					if (IsBadReadPtr((int*)(tMonster.dwObject + 0x2c8), 4) == 0)
						tMonster.nAttack = *(int*)(tMonster.dwObject + 0x2c8);

					DWORD newOBJ = 0;
					if (IsBadReadPtr((DWORD*)(tMonster.dwObject + 0x144), 4) == 0)
						newOBJ = *(DWORD*)(tMonster.dwObject + 0x144);
					newOBJ = *(DWORD*)(newOBJ + 0x4);
					if (IsBadReadPtr((DWORD*)(newOBJ + 0x2c), 4) == 0)
						tMonster.szName = (PCHAR)(newOBJ + 0x2c);

					//if (IsBadReadPtr((DWORD*)(newOBJ + 0x90), 4) == 0)
					//	tMonster.szBaseName = (PCHAR)(newOBJ + 0x90);
					if (IsBadReadPtr((float*)(newOBJ + 0x8), 4) == 0)
						tMonster.fMonsterHp = *(float*)(newOBJ + 0x8);
					if (IsBadReadPtr((float*)(newOBJ + 0x60), 4) == 0)
						tMonster.nCatch = *(DWORD*)(newOBJ + 0x60);
					if (IsBadReadPtr((float*)(newOBJ + 0x28), 4) == 0)
						tMonster.nType = *(DWORD*)(newOBJ + 0x28);
					tMonster.nNpcType = 0;

					//dbgPrint("tMonster szName=%s fMonsterHp=%f nAttack=%d", tMonster.szName, tMonster.fMonsterHp, tMonster.nAttack);
					vm_Monster.push_back(tMonster);
				}
			}
		}
	}
	catch (...)
	{

	}
}

void CAsmMonster::UpdateTagerMonster(TAsmMonster* monster)
{
	TAsmMonster tMonster;
	tMonster.dwTree = monster->dwTree;
	tMonster.dwObject = monster->dwObject;
	if (tMonster.dwObject > 0x10401000)
	{
		if (AsmTreeObjectType(tMonster.dwObject, "CObject_PlayerNPC") == TRUE)
		{
			//if (IsBadReadPtr((int*)(tMonster.dwTree + 0x10), 4) == 0)
			//	tMonster.nMonsterId = *(int*)((DWORD)tMonster.dwTree + 0x10);
			//if (IsBadReadPtr((float*)(tMonster.dwObject + 0x3c), 4) == 0)
			//	tMonster.fMonsterX = *(float*)(tMonster.dwObject + 0x3c);
			//if (IsBadReadPtr((float*)(tMonster.dwObject + 0x44), 4) == 0)
			//	tMonster.fMonsterY = *(float*)(tMonster.dwObject + 0x44);

			//if (IsBadReadPtr((int*)(tMonster.dwObject + 0x2c8), 4) == 0)
			//	tMonster.nAttack = *(int*)(tMonster.dwObject + 0x2c8);

			DWORD newOBJ = 0;
			if (IsBadReadPtr((DWORD*)(tMonster.dwObject + 0x144), 4) == 0)
				newOBJ = *(DWORD*)(tMonster.dwObject + 0x144);
				newOBJ = *(DWORD*)(newOBJ + 0x4);
			//if (IsBadReadPtr((DWORD*)(newOBJ + 0x2c), 4) == 0)
			//	tMonster.szName = (PCHAR)(newOBJ + 0x2c);

			//if (IsBadReadPtr((DWORD*)(newOBJ + 0x90), 4) == 0)
			//	tMonster.szBaseName = (PCHAR)(newOBJ + 0x90);
			if (IsBadReadPtr((float*)(newOBJ + 0x8), 4) == 0)
				tMonster.fMonsterHp = *(float*)(newOBJ + 0x8);
			//if (IsBadReadPtr((float*)(newOBJ + 0x60), 4) == 0)
			//	tMonster.nCatch = *(DWORD*)(newOBJ + 0x60);
			//if (IsBadReadPtr((float*)(newOBJ + 0x28), 4) == 0)
			//	tMonster.nType = *(DWORD*)(newOBJ + 0x28);

			//tMonster.nNpcType = 0;

			//dbgPrint("UpdateTagerMonster szName=%s fMonsterHp=%f nAttack=%d", tMonster.szName, tMonster.fMonsterHp, tMonster.nAttack);
			*monster = tMonster;
		}
	}

	monster = NULL;
}

float CAsmMonster::GetHp(TAsmMonster monster)
{
	TAsmMonster tMonster;
	try
	{
		tMonster.dwTree = monster.dwTree;
		tMonster.dwObject = monster.dwObject;
		if (tMonster.dwObject > 0x10401000)
		{
			if (AsmTreeObjectType(tMonster.dwObject, "CObject_PlayerNPC") == TRUE)
			{
				DWORD newOBJ = 0;
				if (IsBadReadPtr((DWORD*)(tMonster.dwObject + 0x144), 4) == 0)
					newOBJ = *(DWORD*)(tMonster.dwObject + 0x144);
				if (IsBadReadPtr((DWORD*)(newOBJ + 0x4), 4) == 0)
					newOBJ = *(DWORD*)(newOBJ + 0x4);
				if (IsBadReadPtr((float*)(newOBJ + 0x8), 4) == 0)
					tMonster.fMonsterHp = *(float*)(newOBJ + 0x8);
			}
		}
	}
	catch (const std::exception&)
	{
		dbgPrint(__FUNCTION__);
	}
	return tMonster.fMonsterHp;
}