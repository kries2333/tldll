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
	data = *(int*)(g_GameExeBase + 背包基址);
	return data;
}

VAsmItem CAsmItem::AsmGetItemData(int nPage)
{
	VAsmItem vm_Item;//包裹内物品
	try
	{
		int nIndex = 0;
		DWORD NameAddr = 0;
		int nBegin = 0;
		int nEnd = 0;
		if (nPage == -1)
		{
			nBegin = 0;
			nEnd = 60;
		}
		else
		{
			nBegin = nPage * 20;
			nEnd = (nPage + 1) * 20;
		}
		for (int i = nBegin; i < nEnd; i++)
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

				DWORD newObj = *(DWORD*)(uObj + 0x14);
				tItems.uAttributeObj1 = *(DWORD*)(newObj + 0x0);
				tItems.uAttributeObj2 = *(DWORD*)(newObj + 0x4);
				tItems.uAttributeObj3 = *(DWORD*)(newObj + 0x8);

				__asm {
					mov ecx, uObj
					mov eax, tItems.uObject
					call [eax + 0x10]
					mov NameAddr, eax
				}
				tItems.szName = (PCHAR)(NameAddr);

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
				dbgPrint("背包物品 类别=%s 名称=%s 等级=%d 位置=%d",
					tItems.szTypeName, tItems.szName, tItems.uLevel, tItems.nIntdex);
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
	VAsmItem items = AsmGetItemData(-1);

	if (itemNames.empty())
	{
		for (auto item : items)
		{
			if (item.nEquipType != -1)
			{
				WearEquipment(item.nIntdex, item.nEquipType);
				Sleep(200);

				CString strTemp = g_pMsg->msg_getstring("strTemp", "strTemp = getMessageBoxText();").c_str();
				//dbgPrint("MessageBox_Self_Env strTemp=%s", strTemp);
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
						//dbgPrint("MessageBox_Self_Env strTemp=%s", strTemp);
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
	DWORD base = g_GameExeBase + 穿戴CALL;
	_asm {
		push nTypeName
		push nIndex
		mov edx, base
		mov ecx, [edx]
		mov eax, [ecx]
		call[eax + 0x90]
	}
}

void CAsmItem::ItemSell(CString itemNames)
{
	VAsmItem items = AsmGetItemData(-1);

	if (itemNames.IsEmpty())
	{
		for (auto item : items)
		{
			dbgPrint("出售物品%s", item.szName);
			SellEquipment(item.uObj);
			Sleep(1000);
		}
	}
	else {
		_tstring strItems = itemNames;
		auto  vstr = UserSubMonsterName(strItems, _T('|'));
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
	DWORD base = g_GameExeBase + 穿戴CALL;
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
	VAsmItem items = AsmGetItemData(0);

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
	DWORD _CALL = g_GameExeBase + 销毁CALL;
	DWORD _ECX = g_GameExeBase + 销毁CALL_ECX;
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

int CAsmItem::AsmGetItemNum(CString name)
{
	int nNum = g_pMsg->msg_getnumber("g_GetValue = GetBagItemNum(\"%s\");", name);
	//dbgPrint("AsmGetItemNum %s = %d", name, nNum);
	return nNum;
}

//吃药发包
void CAsmItem::AsmUseHpItem(int nRoleId, int nIndex, DWORD ItemObject1, DWORD ItemObject2, DWORD ItemObject3)
{
	//吃药封包得Call

	if (g_GameExeBase == 0) return;

	try
	{
		DWORD CallBase = g_GameExeBase + 吃药CALL;
		DWORD CallEcx = g_GameExeBase + 吃药CALL_ECX;
		if (IsBadReadPtr((DWORD*)CallEcx, 4) == 0)
			CallEcx = *(DWORD*)CallEcx;

		dbgPrint("nIndex=%X, ItemObject1=%d ItemObject2=%X ItemObject3=%X", nIndex, ItemObject1, ItemObject2, ItemObject3);

		DWORD CallObj = g_GameExeBase + 吃药封包数据对象;
		TAsmSendEx* tAsmSendEx = BasePackerSendEx(CallObj, 0, 0, -1, nIndex, ItemObject3, ItemObject1, ItemObject2, 
			nRoleId, 0xBF800000, 0xBF800000, 0xBF800000);

		dbgPrint("CallBase=%X, CallEcx=%X CallObj=%X tAsmSend=%X", CallBase, CallEcx, CallObj, tAsmSendEx);
		_asm {
			mov ecx, [CallEcx]
			push tAsmSendEx
			mov eax, CallBase
			call eax
		}
		delete tAsmSendEx;
		tAsmSendEx = NULL;
	}
	catch (const std::exception&)
	{

	}

}

int CAsmItem::GetBagItemBlankNum()
{
	VAsmItem vAsmItem = AsmGetItemData(0);
	return 20 - vAsmItem.size();
}