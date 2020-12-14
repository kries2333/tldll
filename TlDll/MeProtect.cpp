#include "pch.h"
#include "Me.h"
#include "MeKill.h"
#include "Message.h"
#include "AsmRole.h"
#include "User.h"
#include "AsmItem.h"

extern CMessage* g_pMsg;
extern CMe* g_pMe;
extern CAsmRole* g_pAsmRole;
extern CAsmItem* g_pAsmItem;
extern CUser* g_pUser;
extern CCriticalSection* g_pCriticalSection;
static DWORD PT_HP = 0;


BOOL use_item_yao(CString name)
{
	VAsmItem items = g_pAsmItem->AsmGetItemData();
	for (auto item : items)
	{
		if (name.Find(item.szName) != -1)
		{
			DWORD dwTime = GetTickCount64();
			if (dwTime - PT_HP > 5000)
			{
				PT_HP = GetTickCount64();
				g_pMsg->CallInOutRide(0);
				g_pAsmItem->AsmUseHpItem(item.nIntdex, item.uAttributeObj1, item.uAttributeObj2, item.uAttributeObj3);
				return TRUE;
			}
		}
	}
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
			if ((int)fHpCurPer <= g_pUser->tHighProtect.nHpPer) //¿ìËÙ»Ö¸´µÄ²¹ÑªÒ©
			{
				for (auto tHp : g_pUser->tHighProtect.vYaoName)
				{
					use_item_yao(tHp);
				}
			}
			else if ((int)fMpCurPer <= g_pUser->tHighProtect.nMpPer) //¿ìËÙ»Ö¸´µÄ²¹ÆøÒ©
			{
				for (auto tMp : g_pUser->tHighProtect.vYaoName)
				{
					use_item_yao(tMp);
				}
			}
			else if ((int)fMpCurPer <= g_pUser->tLowProtect.nHpPer) //ÂýËÙ»Ö¸´µÄ²¹ÑªÒ©
			{
				for (auto tMp : g_pUser->tLowProtect.vYaoName)
				{
					use_item_yao(tMp);
				}
			}
			else if ((int)fMpCurPer <= g_pUser->tLowProtect.nMpPer) //ÂýËÙ»Ö¸´µÄ²¹ÆøÒ©
			{
				for (auto tMp : g_pUser->tLowProtect.vYaoName)
				{
					use_item_yao(tMp);
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

}

UINT __stdcall Protect_threadfunc(void* pType)
{
	while (g_pMe->bProtectRun)
	{
		auto ARoleInfo = g_pAsmRole->GetRoleInfo();
		if (!ARoleInfo.bool_ret)
		{
			continue;
		}
		if (ARoleInfo.nState == 9)//ËÀÍö×´Ì¬
		{
			g_pMsg->msg_dostring("setmetatable(_G, { __index = Relive_Env });Relive_Out_Ghost();");
		}
		else
		{
			role_buji();
			//pet_buji(pUser->nPetHpVal, pUser->nHappyVal);
		}
		Sleep(1000);
	}
	return 0;
}