#include "pch.h"
#include "AsmGroundParcel.h"
#include "Function.h"
#include "FileSystem.h"
#include "base.h"

DWORD AsmGetEnvTreeBase()
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

DWORD GetRoleBase()
{
	if (g_GameExeBase == 0) return 0;
	ULONG data = 0;
	data = *(int*)(g_GameExeBase + LROLE_BASE);
	data = *(int*)(data + 0x58);
	return data;
}

BOOL CAsmGroundParcel::AsmTreeObjectType(DWORD Object, char* szObjectName)
{
	bool bOk = false;

	try
	{
		DWORD ObjectTypeString = 0;

		__asm
		{
			mov ecx, Object;
			mov eax, [ecx];
			call [eax + 0x38];
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

void CAsmGroundParcel::AsmGroundParcelTraverse(TAsmTree* pTree, VAsmGroundParcel* vm_Parcel, DWORD* pCount)
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
				AsmGroundParcelInfo(pTree, vm_Parcel);//取信息

			if (IsBadReadPtr(pTree->left, 4) == 0)
				AsmGroundParcelTraverse(pTree->left, vm_Parcel, pCount);//左递归

			if (IsBadReadPtr(pTree->right, 4) == 0)
				AsmGroundParcelTraverse(pTree->right, vm_Parcel, pCount);//右递归
		}
	}
	catch (...)
	{
		//dPrintA(__FUNCTION__);
	}
}

void CAsmGroundParcel::AsmGroundParcelInfo(TAsmTree* pTree, VAsmGroundParcel* vm_Parcel)//地面包裹属性信息
{
	try
	{
		TAsmGroundParcel tAsmGroundParcel;
		tAsmGroundParcel.dwTree = (DWORD)pTree;

		
		if (IsBadReadPtr((DWORD*)((DWORD)pTree + 0x14), 4) == 0)
		{
			tAsmGroundParcel.dwObject = *(DWORD*)((DWORD)pTree + 0x14);	//这里是对象

			if (tAsmGroundParcel.dwObject > 0x10401000)
			{
				if (AsmTreeObjectType(tAsmGroundParcel.dwObject, "CTripperObject_ItemBox") == TRUE)
				{
					if (IsBadReadPtr((DWORD*)(tAsmGroundParcel.dwTree + 0x10), 4) == 0)
						tAsmGroundParcel.nParcelId = *(int*)((DWORD)pTree + 0x10);
					if (IsBadReadPtr((DWORD*)(tAsmGroundParcel.dwObject + 0x3c), 4) == 0)
						tAsmGroundParcel.fParcelX = *(float*)(tAsmGroundParcel.dwObject + 0x3c);
					if (IsBadReadPtr((DWORD*)(tAsmGroundParcel.dwObject + 0x44), 4) == 0)
						tAsmGroundParcel.fParcelY = *(float*)(tAsmGroundParcel.dwObject + 0x44);

					dbgPrint("AsmGroundParcelInfo pTree=%X", (DWORD)pTree);
					dbgPrint("AsmGroundParcelInfo dwObject=%X, nParcelId=%X, fParcelX=%f, fParcelY=%f,",
						tAsmGroundParcel.dwObject, tAsmGroundParcel.nParcelId, tAsmGroundParcel.fParcelX, tAsmGroundParcel.fParcelY);

					vm_Parcel->push_back(tAsmGroundParcel);
				}
			}
		}
	}
	catch (...)
	{

	}
}

VAsmGroundParcel CAsmGroundParcel::AsmGetGroundParcelData()
{
	VAsmGroundParcel vm_Parcel;
	DWORD dwCount = 0;
	try
	{
		DWORD dwTree = AsmGetEnvTreeBase();
		if (dwTree)//正确取到二叉树基址
			AsmGroundParcelTraverse((TAsmTree*)dwTree, &vm_Parcel, &dwCount);//遍历二叉树
	}
	catch (...)
	{
		//dPrintA(__FUNCTION__);
	}

	return vm_Parcel;
}

void CAsmGroundParcel::AsmOpenGroundParcelById(DWORD ParcelId)//打开地面包裹
{
	try
	{
		DWORD bagCall = (g_GameExeBase + COLLECT_OREPLANT);
		DWORD base = GetRoleBase();
		if (IsBadReadPtr((DWORD*)(base+0x150), 4) == 0)
		{
			base = *(DWORD*)(base + 0x150);

			dbgPrint("AsmOpenGroundParcelById bagCall=%X base=%X ParcelId=%X", bagCall, base, ParcelId);
			__asm
			{
				push ParcelId;
				mov  ecx, base;
				mov edx, bagCall;
				call edx;
			}
		}
	}
	catch (...)
	{
		//dPrintA(__FUNCTION__);
	}
}
