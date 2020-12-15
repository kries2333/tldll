#pragma once
#include "FileSystem.h"
#include <vector>
using namespace std;

//物品属性
struct TAsmItem
{
	PCHAR szName;//Name名称
	ULONG uNumber;//number数量
	ULONG uLevel; //level等级
	int   nEquipType;//装备物品类型
	ULONG uType;//物品类型
	ULONG uObject;//物品类型,这里是对象
	DWORD uObj;
	BYTE  byCurnaijiu;//当前耐久度
	BYTE  byMaxnaijiu;//最大耐久度
	char* szTypeName;//物品类型名称
	char* szDataAttribute;//属性说明
	int   nIntdex;//物品所在格子
	DWORD uAttributeObj1;
	DWORD uAttributeObj2;
	DWORD uAttributeObj3;

	TAsmItem()
	{
		nEquipType = -1;
		szName = "";//Name名称
		uNumber = 0;//number数量
		uLevel = 0; //level等级
		uType = 0;//物品类型
		uObject = 0;//物品类型
		uObj = 0;
		byCurnaijiu = 0;//当前耐久度
		byMaxnaijiu = 0;//最大耐久度
		szTypeName = "";//物品类型名称
		szDataAttribute = "";//属性说明
		nIntdex = -1;
		uAttributeObj1 = 0;
		uAttributeObj2 = 0;
		uAttributeObj3 = 0;
	};
};
typedef vector<TAsmItem> VAsmItem;

class CAsmItem
{
public:
	CAsmItem() {};
	~CAsmItem() {};
	BYTE byOneCount = 0;//道具背包格子总数
	BYTE byTwoCount = 0;//材料背包格子总数
	BYTE byThreeCount = 0;//任务背包格子总数
public:
	VAsmItem  AsmGetItemData();
	void AutoWearEquipment(_tstring itemNames);
	void WearEquipment(int nIndex, int nTypeName);
	void AutoSell(_tstring itemNames);
	void SellEquipment(DWORD uObj);
	void AutoDestroy(_tstring itemNames);
	void Destroy(int nIndex);
	int  AsmGetItemNum(CString name);

	void AsmUseHpItem(int nIndex, DWORD ItemObject1, DWORD ItemObject2, DWORD ItemObject3);
private:
	int GetEquipmentTypeForName(CString name);
};

