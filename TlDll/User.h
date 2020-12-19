 #pragma once
#include "FileSystem.h"
#include "AsmSkill.h"
#include <vector>
using namespace std;

struct TUserPos
{
	int nSceneId;//����id
	int nPosX;//x����
	int nPosY;//y����
	int nPosR;//�뾶

	TUserPos()
	{
		nSceneId = 0;//����id
		nPosX = 0;//x����
		nPosY = 0;//y����
		nPosR = 0;//�뾶
	};
};

struct TUserTime
{
	DWORD BeginTime;//��һ�㿪ʼʱ��
	DWORD CurTime;//��һ�㵱ǰʱ��
	DWORD ChangeTime;//����ֻ���ʱ����

	TUserTime()
	{
		BeginTime = 0;
		CurTime = 0;
		ChangeTime = 0;
	};
};

struct TUserProtect
{
	int nType;			//����
	int nHpPer;			// Ѫ�����ڰٷֱ�
	int nMpPer;			// �������ڰٷֱ�
	vector<CString> vYaoName;

	TUserProtect()
	{
		nType = -1;
		nHpPer = -1;
		nMpPer = -1;
	};
};

struct TUserSkill
{
	bool		bNormal;	//��ʶû����ʱ��ʹ�ù�����ʽ
	int			nType;		//1: ʹ����̬, 2:����ʽ 3��Ⱥ������ 4:״̬buff 5:����buff
	int			nSkillId;	//����id
	TAsmSkill	tAsmSkill;
};

typedef vector<TUserPos> VUserPos;
typedef vector<_tstring> VUserMonsterName;
typedef vector<_tstring> VString;
//typedef vector<TUserProtect> VUserProtect;
typedef vector<TUserSkill> VUserSkill;

_tstring Os_stringOferase(_tstring strSource, _tstring strErase);//�Ƴ�ָ���ַ������ַ���
VString stringIntercept(_tstring strSource, _tstring strBegin, int offset, _tstring strEnd);

VUserMonsterName UserSubMonsterName(_tstring strSource, TCHAR strErase);

class CUser
{
public:
	CUser() {};
	~CUser() {};

public:
	TAsmSkill tPuTongSkill;	//��ͨ��������
public:
	void UserSystemInitial();//��ʼ���û����ý���
public:
	/************************************************************************/
	/* ���ܽ���                                                             */
	/************************************************************************/
	//VAsmSkill vUserSkill;//��ȡ�û����ü���
	//VAsmSkill UserGetSkill();//��ȡ����
	VUserSkill vUserSkill;	//��ȡ�û����ü���
	VUserSkill UserGetSkill(); //��ȡ����

	/************************************************************************/
	/* �һ�����                                                             */
	/************************************************************************/
	VUserPos vtKillPos;//�û���ȡ�һ�������
	VUserPos UserGetPos();

	/************************************************************************/
	/* ��������                                                             */
	/************************************************************************/
	TUserProtect tHighProtect;	//���ٻָ�
	TUserProtect tLowProtect;   //���ٻָ�
	TUserProtect tPetProtect;   //����ָ�
};

BOOL use_item_yao(CString name);