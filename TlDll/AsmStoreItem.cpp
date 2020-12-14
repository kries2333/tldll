#include "pch.h"
#include "AsmStoreItem.h"
#include "Base.h"
#include "FileSystem.h"
#include "Function.h"
#include "Message.h"

struct TStoreItemId
{
	char	szName[50];
	int		nItemId;
};


TStoreItemId g_PetItemArray[] 	//��Ʒ��ŵ�ַ
{
	{"���������", 0x00},
	{"ϸ�������", 0x01},
	{"ǰ�������", 0x02},
	{"���������", 0x03},
	{"��ޱ����", 0x04},
	{"���ٿ���", 0x05},
	{"��������", 0x06},
	{"��鵿���", 0x07},
	{"�������", 0x08},
	{"��ţ����", 0x0A},
	{"�ȳ����", 0x0B},
	{"С�����", 0x0C},
	{"���׿���", 0x0D},
	{"���׿���", 0x0E},
	{"��������", 0x0F},
	{"��������", 0x10},
	{"��������", 0x11},
};

TStoreItemId g_StoreArray[] // ���ж�Ӧ�̵��ID
{
	{ "����ҩ��", 0x1001E },
	{ "�������޵�", 0x1002F },
	{"����ҩ��", 0x2000C},
	{ "�������޵�", 0x20023 },
};

CAsmStoreItem::CAsmStoreItem()
{
}

void CAsmStoreItem::AsmBuyStoreDrug(DWORD unStoreId, int itemId, int nNum)
{
	//����÷����Call

	if (g_GameExeBase == 0) return;

	try
	{
		DWORD CallBase = g_GameExeBase + SHOP_CALL;
		DWORD CallEcx = g_GameExeBase + SHOP_CALL_ECX;
		if (IsBadReadPtr((DWORD*)CallEcx, 4) == 0)
			CallEcx = *(DWORD*)CallEcx;

		dbgPrint("unStoreObject=%X, itemId=%d nNum=%d", unStoreId, itemId, nNum);

		DWORD CallObj = g_GameExeBase + SHOP_CALL_OBJ;
		TAsmSend* tAsmSend = BasePackerSend(CallObj, 0, 0, -1, itemId, unStoreId, 0, 0, nNum, 0);

		dbgPrint("CallBase=%X, CallEcx=%X CallObj=%X tAsmSend=%X", CallBase, CallEcx, CallObj, tAsmSend);
		_asm {
			mov ecx, [CallEcx]
			push tAsmSend
			mov eax, CallBase
			call eax
		}
		delete tAsmSend;
		tAsmSend = NULL;
	}
	catch (const std::exception&)
	{

	}
}

void CAsmStoreItem::BuyPetDrugByName(CString szSceneName, CString szName, int nNum)
{
	DWORD unStoreId = -1;
	DWORD unItemId = -1;
	size_t Count = sizeof(g_StoreArray) / sizeof(TStoreItemId);
	for (size_t i = 0; i < Count; i++)
	{
		if (szSceneName.Find(g_StoreArray[i].szName) != -1)
		{
			unStoreId = g_StoreArray[i].nItemId;
			break;
		}
	}

	Count = sizeof(g_PetItemArray) / sizeof(g_PetItemArray[0]);
	for (size_t i = 0; i < Count; i++)
	{
		if (szName.Find(g_PetItemArray[i].szName) != -1)
		{
			unItemId = g_PetItemArray[i].nItemId;
			break;
		}
	}

	if (unStoreId != -1 && unItemId != -1)
	{
		int nCount = nNum / 20;//����Ҫ����ҩƷ
		if (nCount > 0)
		{
			for (int i = 0; i < nCount; i++)
			{
				AsmBuyStoreDrug(unStoreId, unItemId, 20);
				Sleep(1000);
			}
		}

		if (nNum % 20 > 0)//������ͷ
		{
			AsmBuyStoreDrug(unStoreId, unItemId, nNum % 20);
			Sleep(1000);
		}
	}
}