#include "pch.h"
#include "AsmPet.h"
#include "Base.h"
#include "Message.h"
#include "Function.h"

extern CMessage* g_pMsg;

DWORD CAsmPet::GetBase()
{
	if (g_GameExeBase == 0) return 0;
	DWORD data = *(DWORD*)(g_GameExeBase + PET_BASE);
	if (IsBadReadPtr((int*)data + PET_OFFSET, 4) == 0) {
		data = *(DWORD*)(data + PET_OFFSET);
		return data;
	}
	return 0;
}

VAsmPet CAsmPet::AsmGetPetData()
{
	VAsmPet vm_Pet;

	try
	{
		DWORD Base = GetBase();
		for (UINT i = 0; i < 10; i++)
		{
			DWORD  data = Base + i * 0x12C;
			if (*(int*)data == -1)
			{
				break;
			}
			TAsmPet tPet;
			tPet.dwObject = data;
			tPet.nIndex = *(int*)(data + 0x8);
			tPet.nPetId = *(int*)(data + 0xC);
			tPet.szName = (char*)(data + 0x1c);
			//tPet.nPetType = *(int*)(data + 0x34);
			//tPet.nLevel = *(int*)(data + 0x3c);
			tPet.nCurHP = *(int*)(data + 0x3c);
			tPet.nMaxHP = *(int*)(data + 0x40);
			//tPet.nKuaiLe = *(int*)(data + 0x58);
			//tPet.nAttackType = *(int*)(data + 0x8c);
			//tPet.nPotential = *(int*)(data + 0x108);

			vm_Pet.push_back(tPet);
		}
	}
	catch (...)
	{
		//dPrintA(__FUNCTION__);
	}

	return vm_Pet;
}

int CAsmPet::GetCount()
{
	int nCount = g_pMsg->msg_getnumber("Pet:Other_GetPet_Count();");
	return nCount;
}

int CAsmPet::GetMaxCount()
{
	int nMaxPetCount = g_pMsg->msg_getnumber("GetOtherCurMaxPetCount();");
	return nMaxPetCount;
}

BOOL CAsmPet::GetIsFighting()
{
	int nPetId = 0;
	dbgPrint("GetIsFighting");
	if (g_GameExeBase == 0) return FALSE;
	ULONG data = 0;
	data = *(int*)(g_GameExeBase + LROLE_BASE);
	if (IsBadReadPtr((int*)data + 0x58, 4) == 0)
		data = *(int*)(data + 0x58);
	if (IsBadReadPtr((PULONG)(data + 0x144), 4) == 0)
		data = *(PULONG)(data + 0x144);
	if (IsBadReadPtr((PULONG)(data + 0x4), 4) == 0)
		data = *(PULONG)(data + 0x4);
	if (IsBadReadPtr((PULONG)(data + 0x9C0), 4) == 0)
		data = *(PULONG)(data + 0x9C0);

	dbgPrint("GetIsFighting data = %X", data);
	if (IsBadReadPtr((PULONG)(data), 4) == 0)
		nPetId = *(int*)(data);

	dbgPrint("GetIsFighting nPetId = %X", nPetId);
	if (nPetId == 0)
	{
		return FALSE;
	}
	return TRUE;
}

void CAsmPet::PetGoFight(int nIndex)
{
	if (GetIsFighting() == FALSE)
	{
		g_pMsg->msg_dostring("Pet:Go_Fight(%d);", nIndex);
		Sleep(4000);
	}
	else {
		dbgPrint("已经有宠物出战了!");
	}
}