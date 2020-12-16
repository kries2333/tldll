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

TStoreItemId g_RoleItemArray[] 	//物品编号地址
{
	{"行血散", 0x00},
	{"活血散", 0x01},
	{"金疮药", 0x02},
	{"行气散", 0x03},
	{"还灵丹", 0x04},
	{"归灵丹", 0x05},
	{"跌打药膏", 0x06},
	{"茯苓膏", 0x07},
	{"天麻膏", 0x08},
	{"白峰糕", 0x09},
	{"牛丸粉", 0x0A},
	{"核桃酪", 0x0B},
	{"金钱麻花", 0x0C},
	{"春饼", 0x0D},
	{"豆焖饭", 0x0E},
	{"重阳糕", 0x0F},
	{"冰花球", 0x10},
	{"扒鸡", 0x11},
	{"云吞面", 0x12},
	{"桑菊粥", 0x13},
	{"山药粥", 0x14},
	{"松子粥", 0x15},
	{"黑米粥", 0x16},
	{"胡桃粥", 0x17},
	{"海鲜粥", 0x18},
	{"沙参银耳粥", 0x19},
	{"川贝百合粥", 0x1A},
	{"生地麦冬粥", 0x19},
	{"白果养生粥", 0x1A},
};

TStoreItemId g_PetItemArray[] 	//物品编号地址
{
	{"新鲜肉口粮", 0x00},
	{"细嫩肉口粮", 0x01},
	{"前肘肉口粮", 0x02},
	{"后肘肉口粮", 0x03},
	{"紫薇口粮", 0x04},
	{"葛藤口粮", 0x05},
	{"甘蓝口粮", 0x06},
	{"棕榈口粮", 0x07},
	{"蚱蜢口粮", 0x08},
	{"天牛口粮", 0x0A},
	{"蝗虫口粮", 0x0B},
	{"小麦口粮", 0x0C},
	{"大米口粮", 0x0D},
	{"玉米口粮", 0x0E},
	{"花生口粮", 0x0F},
	{"黄锡弹弓", 0x10},
	{"回旋金球", 0x11},
};

TStoreItemId g_StoreArray[] // 城市对应商店得ID
{
	{ "胡繁", 0x1001E }, //苏州药店
	{ "云霏霏", 0x1002F }, //苏州珍兽店
	{ "卢三七", 0x2000C }, //大理药店
	{ "云飘飘", 0x20023 }, //大理珍兽店
	{ "白萌生", 0x9F },	//洛阳药店
	{ "云渺渺", 0x31 },	//洛阳珍兽店
};

CAsmStoreItem::CAsmStoreItem()
{
}

void CAsmStoreItem::AsmBuyStoreDrug(DWORD unStoreId, int itemId, int nNum)
{
	//购买得封包得Call

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

void CAsmStoreItem::BuyPetDrugByName(CString szNpcName, CString szName, int nNum)
{
	DWORD unStoreId = -1;
	DWORD unItemId = -1;
	size_t Count = sizeof(g_StoreArray) / sizeof(TStoreItemId);
	for (size_t i = 0; i < Count; i++)
	{
		if (szNpcName.Find(g_StoreArray[i].szName) != -1)
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
		int nCount = nNum / 20;//计算要买几组药品
		if (nCount > 0)
		{
			for (int i = 0; i < nCount; i++)
			{
				AsmBuyStoreDrug(unStoreId, unItemId, 20);
				Sleep(1000);
			}
		}

		if (nNum % 20 > 0)//计算零头
		{
			AsmBuyStoreDrug(unStoreId, unItemId, nNum % 20);
			Sleep(1000);
		}
	}
}

void CAsmStoreItem::BuyRoleDrugByName(CString szNpcName, CString szName, int nNum)
{
	dbgPrint("NPC名称:%s 购买药品:%s 数量:%d", szNpcName, szName, nNum);
	DWORD unStoreId = -1;
	DWORD unItemId = -1;
	size_t Count = sizeof(g_StoreArray) / sizeof(TStoreItemId);
	for (size_t i = 0; i < Count; i++)
	{
		if (szNpcName.Find(g_StoreArray[i].szName) != -1)
		{
			unStoreId = g_StoreArray[i].nItemId;
			break;
		}
	}

	Count = sizeof(g_RoleItemArray) / sizeof(g_RoleItemArray[0]);
	for (size_t i = 0; i < Count; i++)
	{
		if (szName.Find(g_RoleItemArray[i].szName) != -1)
		{
			unItemId = g_RoleItemArray[i].nItemId;
			break;
		}
	}

	if (unStoreId != -1 && unItemId != -1)
	{
		int nCount = nNum / 20;//计算要买几组药品
		if (nCount > 0)
		{
			for (int i = 0; i < nCount; i++)
			{
				AsmBuyStoreDrug(unStoreId, unItemId, 20);
				Sleep(1000);
			}
		}

		if (nNum % 20 > 0)//计算零头
		{
			AsmBuyStoreDrug(unStoreId, unItemId, nNum % 20);
			Sleep(1000);
		}
	}
}