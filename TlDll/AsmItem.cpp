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
	VAsmItem vm_Item;//��������Ʒ
	try
	{
		int nIndex = 0;
		int nCount = 20;
		DWORD NameAddr = 0;
		for (int i = nIndex; i < nCount; i++)
		{
			DWORD uObj = GetBaseAddr();
			uObj = *(DWORD*)(uObj + 0xF90);//���߱�����������
			uObj = *(DWORD*)(uObj + i * 4);//���߱�����������
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
				tItems.szTypeName = (PCHAR)(NameAddr);	//�������

				tItems.nEquipType = GetEquipmentTypeForName(tItems.szTypeName);

				uObj = *(DWORD*)(uObj + 0x2C);//��ȡ��Ʒ��Ϣָ��
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
	if (name.Find("ǹ����") != -1 ||
		name.Find("˫����") != -1)
	{
		return 0;
	}
	else if (name.Find("ñ��") != -1)
	{
		return 1;
	}
	else if (name.Find("�·�") != -1)
	{
		return 2;
	}
	else if (name.Find("����") != -1)
	{
		return 3;
	}
	else if (name.Find("Ь") != -1)
	{
		return 4;
	}
	else if (name.Find("��ָ") != -1)
	{
		return 6;
	}
	else if (name.Find("����") != -1)
	{
		return 7;
	}
	else if (name.Find("ʱװ") == -1)
	{
		return 10;
	}
	else {
		return  -1;
	}

	return  -1;
}

// ����װ��
void CAsmItem::AutoWearEquipment(_tstring itemNames)
{
	dbgPrint("����װ���б�%s", itemNames.c_str());
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
				if (strTemp.Find("��") != -1)
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
						if (strTemp.Find("��") != -1)
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
			dbgPrint("������Ʒ%s", item.szName);
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
		push uObj			//��Ʒ����
		mov eax, [ecx]
		call [eax + 0xC0]
	}
}

//����Ϊ�����ٵĶ���
void CAsmItem::AutoDestroy(_tstring itemNames)
{
	dbgPrint("������Ʒ�б�%s", itemNames.c_str());
	VAsmItem items = AsmGetItemData(1);

	if (itemNames.empty())
	{
		for (auto item : items)
		{
			if (CString(item.szName).Find("��") == -1 &&
				CString(item.szName).Find("��«") == -1 &&
				CString(item.szName).Find("����") == -1)
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

//������Ʒ
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