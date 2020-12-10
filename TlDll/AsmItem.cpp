#include "pch.h"
#include "AsmItem.h"
#include "Base.h"
#include "FileSystem.h"
#include "Function.h"
#include "Message.h"

extern CMessage* g_pMsg;

DWORD GetBaseAddr()
{
	if (g_GameExeBase == 0) return 0;
	ULONG data = 0;
	data = *(int*)(g_GameExeBase + BAG_BASE);
	return data;
}

VAsmItem CAsmItem::AsmGetItemData(int nNum)
{
	VAsmItem vm_Item;//包裹内物品
	try
	{
		int nIndex = 0;
		int nCount = 20;
		DWORD NameAddr = 0;
		for (int i = nIndex; i < nCount; i++)
		{
			DWORD uObj = GetBaseAddr();
			uObj = *(DWORD*)(uObj + 0xF90);//道具背包格子总数
			uObj = *(DWORD*)(uObj + i * 4);//道具背包格子总数
			if (0 != uObj)
			{
				TAsmItem tItems;
				tItems.uObject = *(DWORD*)(uObj);
				tItems.uObj = uObj;
				tItems.nIntdex = i;
				__asm {
					mov ecx, uObj
					mov eax, tItems.uObject
					call [eax + 0x10]
					mov NameAddr, eax
				}
				tItems.szName = (PCHAR)(NameAddr);
				dbgPrint("AsmGetItemData uObj=%X",
					uObj);

				__asm {
					mov ecx, uObj
					mov eax, tItems.uObject
					call[eax + 0x14]
					mov NameAddr, eax
				}		
				tItems.szTypeName = (PCHAR)(NameAddr);	//类别名称

				tItems.nEquipType = GetEquipmentTypeForName(tItems.szTypeName);

				uObj = *(DWORD*)(uObj + 0x2C);//获取物品信息指针
				tItems.uType = *(DWORD*)(uObj + 0x4);
				if (tItems.uType == 1)
				{
					tItems.uLevel = *(DWORD*)(uObj + 0x2C);
				}
				else if (tItems.uType == 2)
				{
					tItems.uLevel = *(DWORD*)(uObj + 0x20);
				}
				vm_Item.push_back(tItems);
			}
		}
	}
	catch (...)
	{
		dbgPrint(__FUNCTION__);
	}

	return vm_Item;
}

int CAsmItem::GetEquipmentTypeForName(CString name)
{
	if (name.Find("枪棒类") != -1 ||
		name.Find("双短类") != -1)
	{
		return 0;
	}
	else if (name.Find("帽子") != -1)
	{
		return 1;
	}
	else if (name.Find("衣服") != -1)
	{
		return 2;
	}
	else if (name.Find("手套") != -1)
	{
		return 3;
	}
	else if (name.Find("鞋") != -1)
	{
		return 4;
	}
	else if (name.Find("戒指") != -1)
	{
		return 6;
	}
	else if (name.Find("项链") != -1)
	{
		return 7;
	}
	else if (name.Find("时装") == -1)
	{
		return 10;
	}
	else {
		return  -1;
	}

	return  -1;
}

// 穿戴装备
void CAsmItem::AutoWearEquipment(_tstring itemNames)
{
	dbgPrint("穿戴装备列表%s", itemNames.c_str());
	VAsmItem items = AsmGetItemData(1);

	if (itemNames.empty())
	{
		for (auto item : items)
		{
			if (item.nEquipType != -1)
			{
				WearEquipment(item.nIntdex, item.nEquipType);
				Sleep(200);

				CString strTemp = g_pMsg->msg_getstring("strTemp", "strTemp = getMessageBoxText();").c_str();
				dbgPrint("MessageBox_Self_Env strTemp=%s", strTemp);
				if (strTemp.Find("绑定") != -1)
				{
					g_pMsg->msg_dostring("setmetatable(_G, { __index = MessageBox_Self_Env }); MessageBox_Self_OK_Clicked();");
					Sleep(200);
				}
			}
		}
	}
	else {
		auto  vstr = UserSubMonsterName(itemNames, _T('|'));
		for (auto item : items)
		{
			if (item.nEquipType != -1)
			{
				CString strTemp(item.szName);
				for (auto v : vstr)
				{
					if (strTemp.Find(v.c_str()) != -1)
					{
						WearEquipment(item.nIntdex, item.nEquipType);
						Sleep(200);

						CString strTemp = g_pMsg->msg_getstring("strTemp", "strTemp = getMessageBoxText();").c_str();
						dbgPrint("MessageBox_Self_Env strTemp=%s", strTemp);
						if (strTemp.Find("绑定") != -1)
						{
							g_pMsg->msg_dostring("setmetatable(_G, { __index = MessageBox_Self_Env }); MessageBox_Self_OK_Clicked();");
							Sleep(200);
						}
					}
				}
			}
		}
	}
}

void CAsmItem::WearEquipment(int nIndex, int nTypeName)
{

	if (g_GameExeBase == 0) return;
	DWORD base = g_GameExeBase + WEAR_CALL;
	_asm {
		push nTypeName
		push nIndex
		mov edx, base
		mov ecx, [edx]
		mov eax, [ecx]
		call[eax + 0x90]
	}
}

void CAsmItem::AutoSell(_tstring itemNames)
{
	VAsmItem items = AsmGetItemData(1);

	if (itemNames.empty())
	{
		for (auto item : items)
		{
			dbgPrint("出售物品%s", item.szName);
			SellEquipment(item.uObj);
			Sleep(1000);
		}
	}
	else {
		auto  vstr = UserSubMonsterName(itemNames, _T('|'));
		for (auto item : items)
		{
			if (item.nEquipType != -1)
			{
				CString str(item.szName);
				for (auto v : vstr)
				{
					if (str.Find(v.c_str()) != -1)
					{
						SellEquipment(item.uObj);
						Sleep(1000);
					}
				}
			}
		}
	}
}

void CAsmItem::SellEquipment(DWORD uObj)
{
	if (g_GameExeBase == 0) return;
	DWORD base = g_GameExeBase + WEAR_CALL;
	_asm {
		mov edx, base
		mov ecx, [edx]
		push uObj			//物品对象
		mov eax, [ecx]
		call [eax + 0xC0]
	}
}

//参数为不销毁的对象
void CAsmItem::AutoDestroy(_tstring itemNames)
{
	dbgPrint("销毁物品列表%s", itemNames.c_str());
	VAsmItem items = AsmGetItemData(1);

	if (itemNames.empty())
	{
		for (auto item : items)
		{
			if (CString(item.szName).Find("丹") == -1 &&
				CString(item.szName).Find("葫芦") == -1 &&
				CString(item.szName).Find("珍兽") == -1)
			{
				if (item.nEquipType != -1)
				{
					Destroy(item.nIntdex);
					Sleep(200);
				}
			}

		}
	}
}

//销毁物品
void CAsmItem::Destroy(int nIndex)
{
	if (g_GameExeBase == 0) return;
	DWORD _CALL = g_GameExeBase + DESTROY_CALL;
	DWORD _ECX = g_GameExeBase + DESTROY_ECX;
	_asm {
		mov eax, _ECX
		mov ecx, [eax]
		mov eax, 0x2
		mov dword ptr ds : [ecx + 0x4414C] , eax
		mov eax, nIndex
		mov dword ptr ds : [ecx + 0x44150] , eax
		mov edx, _CALL
		call edx
	}
}