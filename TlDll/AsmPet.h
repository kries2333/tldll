#pragma once
#include <vector>
using namespace std;

struct	TAsmPet
{
	DWORD  dwObject;
	int    nPetId;//+C ID
	int	   nIndex;
	PCHAR  szName;//+3C��������
	int    nPetType;//+34�������� 6����ʳ������  a�ǲ�ʳ������ ����ţ����
	int    nLevel;//+24�ȼ�	
	int    nMaxHP;//+48���Ѫ
	int    nCurHP;//+4C��ǰѪ
	int    nKuaiLe;//+58����
	int    nAttackType;//+8c ����  b�⹦    c�ڹ�     a��d�Ǿ����Ի���������
	int    nPotential;//+108����ʣ���
	//VAsmPetSkill vm_PetSkill;// +120ָ�����б��ָ��,�����������洢��������

	TAsmPet()
	{
		szName = "";
		nLevel = 0;
		nKuaiLe = 0;
		nAttackType = 0;
		nMaxHP = 0;
		nCurHP = 0;
		dwObject = 0;
		nPetId = 0;
		nPotential = 0;
		nPetType = 0;
	};
};

typedef vector<TAsmPet> VAsmPet;

class CAsmPet
{
public:
	CAsmPet() {};
	~CAsmPet() {};
public:

	VAsmPet AsmGetPetData();
	int GetCount();
	int GetMaxCount();
	void PetGoFight(int nIndex);
	int GetIsFighting();
private:
	DWORD GetBase();
	
};

