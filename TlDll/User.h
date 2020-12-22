 #pragma once
#include "FileSystem.h"
#include "AsmSkill.h"
#include <vector>
using namespace std;

struct TUserPos
{
	int nSceneId;//场景id
	int nPosX;//x坐标
	int nPosY;//y坐标
	int nPosR;//半径

	TUserPos()
	{
		nSceneId = 0;//场景id
		nPosX = 0;//x坐标
		nPosY = 0;//y坐标
		nPosR = 0;//半径
	};
};

struct TUserTime
{
	DWORD BeginTime;//这一点开始时间
	DWORD CurTime;//这一点当前时间
	DWORD ChangeTime;//多点轮换的时间间隔

	TUserTime()
	{
		BeginTime = 0;
		CurTime = 0;
		ChangeTime = 0;
	};
};

struct TUserProtect
{
	int nType;			//类型 1快速 2慢速
	int nPType;			//血量类型  1:血量 2:气量
	int nPer;			// 血量低于百分比
	vector<CString> vYaoName;

	TUserProtect()
	{
		nType = -1;
		nPer = -1;
	};
};

struct TUserSkill
{
	bool		bNormal;	//标识没蓝得时候使用攻击方式
	int			nType;		//1: 使用姿态, 2:起手式 3：群攻技能 4:状态buff 5:自身buff
	int			nSkillId;	//技能id
	TAsmSkill	tAsmSkill;
};

typedef vector<TUserPos> VUserPos;
typedef vector<_tstring> VUserMonsterName;
typedef vector<_tstring> VString;
typedef vector<TUserProtect> VUserProtect;
typedef vector<TUserSkill> VUserSkill;

_tstring Os_stringOferase(_tstring strSource, _tstring strErase);//移除指定字符或者字符串
VString stringIntercept(_tstring strSource, _tstring strBegin, int offset, _tstring strEnd);

VUserMonsterName UserSubMonsterName(_tstring strSource, TCHAR strErase);

class CUser
{
public:
	CUser() {};
	~CUser() {};

public:
	TAsmSkill tPuTongSkill;	//普通攻击技能
public:
	void UserSystemInitial();//初始化用户设置界面
public:
	/************************************************************************/
	/* 技能界面                                                             */
	/************************************************************************/
	//VAsmSkill vUserSkill;//获取用户设置技能
	//VAsmSkill UserGetSkill();//获取技能
	VUserSkill vUserSkill;	//获取用户设置技能
	VUserSkill UserGetSkill(); //获取技能

	/************************************************************************/
	/* 挂机中心                                                             */
	/************************************************************************/
	VUserPos vtKillPos;//用户获取挂机点总数
	VUserPos UserGetPos();

	/************************************************************************/
	/* 保护设置                                                             */
	/************************************************************************/
	//TUserProtect tHighProtect;	//快速恢复
	//TUserProtect tLowProtect;   //慢速恢复
	VUserProtect vRoleProtect;
	VUserProtect vPetProtect;   //宠物恢复
};

BOOL use_item_yao(CString name);