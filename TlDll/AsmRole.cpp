#include "pch.h"
#include "AsmRole.h"
#include "Base.h"
#include "Function.h"
#include "Message.h"

extern CMessage* g_pMsg;

DWORD CAsmRole::GetRoleBase()
{
	if (g_GameExeBase == 0) return 0;
	ULONG data = 0;
	data = *(int*)(g_GameExeBase + LROLE_BASE);
	if (IsBadReadPtr((int*)data + 0x58, 4) == 0) {
		data = *(int*)(data + 0x58);
		return data;
	}
	return 0;
}

TAsmRoleInfo CAsmRole::GetRoleInfo()
{
	TAsmRoleInfo TAsmRoleInfo;
	try
	{
		ULONG data = 0;
		data = GetRoleBase();
		if (data == 0)
		{
			return TAsmRoleInfo;
		}
		if (IsBadReadPtr((int*)(data + 0x34), 4) == 0)
			TAsmRoleInfo.nRoleID = *(int*)(data + 0x34);	//角色ID
		
		if (IsBadReadPtr((BYTE*)(data + 0x148), 1) == 0)
			TAsmRoleInfo.nState = *(BYTE*)(data + 0x148);	//角色状态

		//------------------------------------------
		if (IsBadReadPtr((PULONG)(data + 0x144), 4) == 0)
			data = *(PULONG)(data + 0x144);
		if (IsBadReadPtr((PULONG)(data + 0x4), 4) == 0)
			data = *(PULONG)(data + 0x4);
		//------------------------------------------
		
		if (IsBadReadPtr((int*)(data + 0x10), 4) == 0)
			TAsmRoleInfo.nRoleCode = *(int*)(data + 0x10);	//人物编号
		if (IsBadReadPtr((CHAR*)(data + 0x2C), 4) == 0)
			TAsmRoleInfo.szName = (char*)(data + 0x2C);
		if (IsBadReadPtr((int*)(data + 0xA4), 4) == 0)
			TAsmRoleInfo.nMenpai = *(int*)(data + 0xA4);	//门派
		if (IsBadReadPtr((int*)(data + 0x58), 4) == 0)
			TAsmRoleInfo.nLevel = *(int*)(data + 0x58);		//等级
		if (IsBadReadPtr((int*)(data + 0x980), 4) == 0)
			TAsmRoleInfo.nHP = *(int*)(data + 0x980);		//血 0x980
		if (IsBadReadPtr((int*)(data + 0x9EC), 4) == 0)
			TAsmRoleInfo.nHPMax = *(int*)(data + 0x9EC);	//血 0x980

		switch (TAsmRoleInfo.nMenpai)
		{
		case 0:
			TAsmRoleInfo.szMenpai = "少林派";
			break;
		case 1:
			TAsmRoleInfo.szMenpai = "明教";
			break;
		case 2:
			TAsmRoleInfo.szMenpai = "丐帮";
			break;
		case 3:
			TAsmRoleInfo.szMenpai = "武当派";
			break;
		case 4:
			TAsmRoleInfo.szMenpai = "峨嵋派";
			break;
		case 5:
			TAsmRoleInfo.szMenpai = "星宿派";
			break;
		case 6:
			TAsmRoleInfo.szMenpai = "天龙派";
			break;
		case 7:
			TAsmRoleInfo.szMenpai = "天山派";
			break;
		case 8:
			TAsmRoleInfo.szMenpai = "逍遥派";
			break;
		case 9:
			TAsmRoleInfo.szMenpai = "无门派";
			break;
		case 10:
			TAsmRoleInfo.szMenpai = "慕容世家";
			break;
		case 11:
			TAsmRoleInfo.szMenpai = "唐门";
			break;
		default:
			break;
		}
		TAsmRoleInfo.bool_ret = true;
		//dbgPrint("GetRoleInfo szName=%s, nState =%d nLevel=%d, szMenpai=%s, nMenpai=%d", 
		//	TAsmRoleInfo.szName, TAsmRoleInfo.nState, TAsmRoleInfo.nLevel, TAsmRoleInfo.szMenpai, TAsmRoleInfo.nMenpai);
	}
	catch (...)
	{
		dbgPrint(__FUNCTION__);
	}

	return TAsmRoleInfo;
}

// 血量和气量是否满足百分比值 大于返回true
bool CAsmRole::IsHpAndMp(int nP)
{
	float nHpVuale = -1;
	float nMapVuale = -1;
	int nHp = -1;
	int nMp = -1;
	int nHpMax = -1;
	int nMpMax = -1;
	try
	{
		ULONG data = 0;
		data = GetRoleBase();
		if (data != 0)
		{
			if (IsBadReadPtr((PULONG)(data + 0x144), 4) == 0)
				data = *(PULONG)(data + 0x144);
			if (IsBadReadPtr((PULONG)(data + 0x4), 4) == 0)
				data = *(PULONG)(data + 0x4);

			if (IsBadReadPtr((int*)(data + 0x980), 4) == 0)
				nHp = *(int*)(data + 0x980);		//血 0x980
			if (IsBadReadPtr((int*)(data + 0x9EC), 4) == 0)
				nHpMax = *(int*)(data + 0x9EC);	//血 0x980
			if (IsBadReadPtr((int*)(data + 0x984), 4) == 0)
				nMp = *(int*)(data + 0x984);		//气 0x980
			if (IsBadReadPtr((int*)(data + 0x9F0), 4) == 0)
				nMpMax = *(int*)(data + 0x9F0);	//气 0x980


			if (nHp > 0 && nMpMax > 0 && nMp > 0 && nMpMax > 0)
			{
				nHpVuale = (float)(nHp) / (float)(nMpMax);
				nMapVuale = (float)(nMp) / (float)(nMpMax);
				if (nHpVuale * 100 > nP && nMapVuale * 100 > nP)
				{
					return true;
				}
			}
		}

	}
	catch (const std::exception&)
	{
		dbgPrint(__FUNCTION__);
	}

	return false;
}

float CAsmRole::GetHpPercent()
{
	float p = -1;
	int nHp = -1;
	int nMax = -1;

	try
	{
		ULONG data = 0;
		data = GetRoleBase();
		if (data != 0)
		{
			if (IsBadReadPtr((PULONG)(data + 0x144), 4) == 0)
				data = *(PULONG)(data + 0x144);
			if (IsBadReadPtr((PULONG)(data + 0x4), 4) == 0)
				data = *(PULONG)(data + 0x4);

			if (IsBadReadPtr((int*)(data + 0x980), 4) == 0)
				nHp = *(int*)(data + 0x980);		//血 0x980
			if (IsBadReadPtr((int*)(data + 0x9EC), 4) == 0)
				nMax = *(int*)(data + 0x9EC);	//血 0x980

			if (nHp > 0 && nMax > 0)
			{
				p = (float)(nHp) / (float)(nMax);
				//dbgPrint("GetHpPercent = %f hp=%d, max=%d", p, nHp, nMax);
			}
		}

	}
	catch (const std::exception&)
	{
		dbgPrint(__FUNCTION__);
	}

	return p;
}

int	CAsmRole::GetMenPaiForId()
{
	TAsmRoleInfo role = GetRoleInfo();
	return role.nMenpai;
}

_tstring CAsmRole::GetMenPaiForName()
{
	int menpaiId = 0;
	_tstring strMenPai = "";
	try
	{
		ULONG data = 0;
		data = GetRoleBase();
		if (data == 0)
		{
			return "";
		}

		if (IsBadReadPtr((int*)data + 0xA4, 4) == 0) {
			menpaiId = *(int*)(data + 0xA4);
		}
	}
	catch (...)
	{
		//dPrintA(__FUNCTION__);
	}
	switch (menpaiId)
	{
	case 4:
		strMenPai = "峨眉";
		break;
	case 8:
		strMenPai = "逍遥";
		break;
	case 9:
		strMenPai = "无门派";
		break;
	default:
		break;
	}

	return strMenPai;
}

TAsmRolePos CAsmRole::GetPos()
{
	TAsmRolePos tAsmRolePos;

	try
	{
		ULONG data = 0;
		data = GetRoleBase();
		if (data == 0)
		{
			return tAsmRolePos;
		}

		if (IsBadReadPtr((float*)data + 0x54, 4) == 0) {
			tAsmRolePos.fx = *(float*)(data + 0x54);
			tAsmRolePos.fy = *(float*)(data + 0x5C);
		}
		tAsmRolePos.bool_ret = TRUE;
	}
	catch (...)
	{
		//dPrintA(__FUNCTION__);
	}

	return tAsmRolePos;
}

void CAsmRole::AsmNpcDialogue(int npcId)
{
	try
	{
		DWORD npcCall = (g_GameExeBase + NPC_CALL);
		DWORD base = GetRoleBase();
		if (IsBadReadPtr((DWORD*)(base + 0x150), 4) == 0)
		{
			base = *(DWORD*)(base + 0x150);

			//dbgPrint("AsmNpcDialogue bagCall=%X base=%X ParcelId=%X", npcCall, base, npcId);
			__asm
			{
				push npcId;
				mov  ecx, base;
				mov edx, npcCall;
				call edx;
			}
		}
	}
	catch (...)
	{
zheng		dbgPrint(__FUNCTION__);
	}
}

int CAsmRole::GetQiChenStatus()
{
	if (g_GameExeBase == 0) return 0;
	try
	{
		DWORD base = g_GameExeBase + QICHEN_STATUS;
		if (IsBadReadPtr((DWORD*)(base), 4) == 0) {
			base = *(DWORD*)(base);
			base = *(DWORD*)(base + 0x4452C + 0x4);
			return base;
		}
	}
	catch (const std::exception&)
	{

	}

	return 0;
}

TAsmHPMP CAsmRole::GetHPMP()
{

	TAsmHPMP tAsmHPMP;

	try
	{
		ULONG data = 0;
		data = GetRoleBase();
		if (data != 0)
		{
			if (IsBadReadPtr((PULONG)(data + 0x144), 4) == 0)
				data = *(PULONG)(data + 0x144);
			if (IsBadReadPtr((PULONG)(data + 0x4), 4) == 0)
				data = *(PULONG)(data + 0x4);

			if (IsBadReadPtr((int*)(data + 0x980), 4) == 0)
				tAsmHPMP.nCurhp = *(int*)(data + 0x980);		//血 0x980
			if (IsBadReadPtr((int*)(data + 0x9EC), 4) == 0)
				tAsmHPMP.nMaxhp = *(int*)(data + 0x9EC);	//血 0x980
			if (IsBadReadPtr((int*)(data + 0x984), 4) == 0)
				tAsmHPMP.nCurmp = *(int*)(data + 0x984);		//气 0x980
			if (IsBadReadPtr((int*)(data + 0x9F0), 4) == 0)
				tAsmHPMP.nMaxmp = *(int*)(data + 0x9F0);	//气 0x980

			tAsmHPMP.bool_ret = true;
		}

	}
	catch (const std::exception&)
	{
		dbgPrint(__FUNCTION__);
	}

	return tAsmHPMP;
}