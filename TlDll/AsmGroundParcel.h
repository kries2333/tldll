#pragma once
#include "FileSystem.h"
#include "AsmItem.h"
#include <vector>
using namespace std;

typedef struct  TAsmGroundParcel
{
	DWORD dwTree;
	DWORD dwObject;
	int   nParcelId;
	float fParcelX;
	float fParcelY;
	char* szOreName;

	TAsmGroundParcel()
	{
		szOreName = "";
		dwTree = 0;
		dwObject = 0;
		nParcelId = -1;
		fParcelX = 0.0f;
		fParcelY = 0.0f;
	};
}TAsmCollect;

typedef vector<TAsmGroundParcel> VAsmGroundParcel;
typedef vector<TAsmItem> VAsmItem;

class CAsmGroundParcel
{
public:
	CAsmGroundParcel() {};
	~CAsmGroundParcel() {};

public:
	VAsmGroundParcel AsmGetGroundParcelData();
	void AsmOpenGroundParcelById(DWORD ParcelId); //打开地面包裹
	
private:
	void AsmGroundParcelInfo(TAsmTree* pTree, VAsmGroundParcel* vm_Parcel);
	BOOL AsmTreeObjectType(DWORD Object, char* szObjectName);
	void AsmGroundParcelTraverse(TAsmTree* pTree, VAsmGroundParcel* vm_Parcel, DWORD* pCount);
};

