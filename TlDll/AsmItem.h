#pragma once
#include "FileSystem.h"
#include <vector>
using namespace std;

//��Ʒ����
struct TAsmItem
{
	PCHAR szName;//Name����
	ULONG uNumber;//number����
	ULONG uLevel; //level�ȼ�
	int   nEquipType;//װ����Ʒ����
	ULONG uType;//��Ʒ����
	ULONG uObject;//��Ʒ����,�����Ƕ���
	DWORD uObj;
	BYTE  byCurnaijiu;//��ǰ�;ö�
	BYTE  byMaxnaijiu;//����;ö�
	char* szTypeName;//��Ʒ��������
	char* szDataAttribute;//����˵��
	int   nIntdex;//��Ʒ���ڸ���
	DWORD uAttributeObj1;
	DWORD uAttributeObj2;
	DWORD uAttributeObj3;

	TAsmItem()
	{
		nEquipType = -1;
		szName = "";//Name����
		uNumber = 0;//number����
		uLevel = 0; //level�ȼ�
		uType = 0;//��Ʒ����
		uObject = 0;//��Ʒ����
		uObj = 0;
		byCurnaijiu = 0;//��ǰ�;ö�
		byMaxnaijiu = 0;//����;ö�
		szTypeName = "";//��Ʒ��������
		szDataAttribute = "";//����˵��
		nIntdex = -1;
	};
};
typedef vector<TAsmItem> VAsmItem;

class CAsmItem
{
public:
	CAsmItem() {};
	~CAsmItem() {};
	BYTE byOneCount = 0;//���߱�����������
	BYTE byTwoCount = 0;//���ϱ�����������
	BYTE byThreeCount = 0;//���񱳰���������
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

