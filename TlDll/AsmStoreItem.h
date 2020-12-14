#pragma once

class CAsmStoreItem
{
public:
	CAsmStoreItem();
	~CAsmStoreItem() {};
public:
	void BuyPetDrugByName(CString szSceneName, CString szName, int nNum);
private:
	void AsmBuyStoreDrug(DWORD unStoreObject, int itemId, int nNum);
};

