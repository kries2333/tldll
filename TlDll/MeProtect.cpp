#include "pch.h"
#include "Me.h"
#include "MeKill.h"
#include "Message.h"
#include "AsmRole.h"
#include "AsmPet.h"
#include "User.h"
#include "AsmItem.h"

extern CMessage* g_pMsg;
extern CMe* g_pMe;
extern CAsmRole* g_pAsmRole;
extern CAsmPet* g_pAsmPet;
extern CAsmItem* g_pAsmItem;
extern CUser* g_pUser;
extern CCriticalSection* g_pCriticalSection;
static DWORD PT_HP = 0;

BOOL use_item_yao(CString name)
{
	//g_pCriticalSection->Lock();
	VAsmItem items = g_pAsmItem->AsmGetItemData(0);
	for (auto item : items)
	{
		if (name.Find(item.szName) != -1)
		{
			DWORD dwTime = GetTickCount64();
			if (dwTime - PT_HP > 5000)
			{
				PT_HP = GetTickCount64();
				g_pMsg->CallInOutRide(0);
				dbgPrint("use_item_yao item=%s", item.szName);
				g_pAsmItem->AsmUseHpItem(item.nIntdex, item.uAttributeObj1, item.uAttributeObj2, item.uAttributeObj3);
				break;
			}
		}
	}
	//g_pCriticalSection->Unlock();
	return FALSE;
}

void role_buji()
{
	//g_pCriticalSection->Lock();
	try
	{
		int nUseState = 0;
		auto AHPMP = g_pAsmRole->GetHPMP();
		if (AHPMP.bool_ret)
		{
			float fHpCurPer = ((float)AHPMP.nCurhp / (float)AHPMP.nMaxhp) * 100;
			float fMpCurPer = ((float)AHPMP.nCurmp / (float)AHPMP.nMaxmp) * 100;
			dbgPrint("role_buji fHpCurPer=%f, fMpCurPer=%f", fHpCurPer, fMpCurPer);
			for (size_t i = 0; i < g_pUser->vRoleProtect.size(); i++)
			{
				if (g_pUser->vRoleProtect[i].nType == 2 &&
					g_pUser->vRoleProtect[i].nPType == 1 &&
					g_pUser->vRoleProtect[i].nPer >= fHpCurPer)
				{
					for (size_t j = 0; j < g_pUser->vRoleProtect[i].vYaoName.size(); j++)
					{
						use_item_yao(g_pUser->vRoleProtect[i].vYaoName[j]);
					}

				}
				else if (g_pUser->vRoleProtect[i].nType == 2 &&
					g_pUser->vRoleProtect[i].nPType == 2 &&
					g_pUser->vRoleProtect[i].nPer >= fMpCurPer)
				{
					for (size_t j = 0; j < g_pUser->vRoleProtect[i].vYaoName.size(); j++)
					{
						use_item_yao(g_pUser->vRoleProtect[i].vYaoName[j]);
					}

				}
				else if (g_pUser->vRoleProtect[i].nType == 1 &&
					g_pUser->vRoleProtect[i].nPType == 1 &&
					g_pUser->vRoleProtect[i].nPer >= fHpCurPer)
				{
					for (size_t j = 0; j < g_pUser->vRoleProtect[i].vYaoName.size(); j++)
					{
						use_item_yao(g_pUser->vRoleProtect[i].vYaoName[j]);
					}

				}
				else if (g_pUser->vRoleProtect[i].nType == 1 &&
					g_pUser->vRoleProtect[i].nPType == 2 &&
					g_pUser->vRoleProtect[i].nPer >= fMpCurPer)
				{
					for (size_t j = 0; j < g_pUser->vRoleProtect[i].vYaoName.size(); j++)
					{
						use_item_yao(g_pUser->vRoleProtect[i].vYaoName[j]);
					}
				}
			}
		}
	}
	catch (const std::exception&)
	{

	}
	//g_pCriticalSection->Unlock();

	return;
}

void pet_buji()
{
	//g_pCriticalSection->Lock();
	try
	{
		VAsmPet vPets = g_pAsmPet->AsmGetPetData();
		for (auto pet : vPets)
		{
			if (pet.nPetId == g_pAsmPet->GetIsFighting())
			{
				float fHpCurPer = ((float)pet.nCurHP / (float)pet.nMaxHP) * 100;
				dbgPrint("pet_buji fHpCurPer=%f", fHpCurPer);
				for (size_t i = 0; i < g_pUser->vPetProtect.size(); i++)
				{
					dbgPrint("pet_buji nType=%d, nPType=%d, nPer=%d", g_pUser->vPetProtect[i].nType,
						g_pUser->vPetProtect[i].nPType,
						g_pUser->vPetProtect[i].nPer);

					if (g_pUser->vPetProtect[i].nType == 1 &&
						g_pUser->vPetProtect[i].nPType == 1 &&
						g_pUser->vPetProtect[i].nPer >= fHpCurPer)
					{
						for (size_t j = 0; j < g_pUser->vPetProtect[i].vYaoName.size(); j++)
						{
							use_item_yao(g_pUser->vPetProtect[i].vYaoName[j]);
						}
					}
				}
			}
			Sleep(100);
		}
	}
	catch (const std::exception&)
	{

	}
	//g_pCriticalSection->Unlock();

	return;
}

UINT __stdcall Protect_threadfunc(void* pType)
{
	while (g_pMe->bProtectThread)
	{
		if (g_pMe->bPauseProtect)
		{
			Sleep(5000);
			continue;
		}
		role_buji();
		pet_buji();
		Sleep(3000);
	}
	return 0;
}