#include <Windows.h>

#define LROLE_BASE			0x336F78		//人物基址#pragma once
#define LLUASTATE_BASE		0x33E83C		//lua状态L指针 

#define SKILLS_TREE_OFFSET  0xA9C
#define SKILLS_CALL			0x3DBA0			//技能调用call

#define COLLECT_OREPLANT	0x3B8F0			//打开包裹

#define NPC_CALL			0x3D7D0			//npc对话Call
#define BAG_BASE			0x3371A8		//背包基址

#define WEAR_CALL			0x33DDEC		//穿戴物品call
#define DESTROY_CALL		0xB98D0			//销毁物品call
#define DESTROY_ECX			0x3371A8		//销毁物品ecx

#define QICHEN_STATUS		0x3371A8		//骑乘状态

#define PET_BASE			0x3371A8		//宠物数据
#define PET_OFFSET			0x44388			//宠物偏移

#define SHOP_CALL			0x171950
#define SHOP_CALL_ECX		0x33DA78
#define SHOP_CALL_OBJ		0x293C54

#define CHIYAO_CALL			SHOP_CALL
#define CHIYAO_CALL_ECX		SHOP_CALL_ECX
#define CHIYAO_CALL_OBJ		0x293D08		//吃药发包数据

#define	TEAM_ECX			0x33DA78		//组队封包CALL_ecx
#define	TEAM_CALL			0x171950		//组队封包CALL
#define	TEAM_DATA1			0x290FB0		//组队封包数据
#define	TEAM_INFO			0x33719C		//组队数组基址
#define TEAM_STATUS			0x33DDF0		//队伍状态
#define TEAM_JOIN_DATA		0x2A9C98		//加入组队数据

//#define PET_CALL				0x170530	//宠物出战Call
//

//#define TASK_INFO 0x3373B8
//#define TASK_ECX 0x33719C
//#define TASK_SELECT_CALL 0x49FA0
//#define TASK_COMPLETE_CALL 0x4A330
//#define TASK_ACCEPT_CALL 0x4A180

//#define TASK_CONTINUE_CALL 0x4A230
//#define TASK_CANCEL_CALL 0x4A1A0

struct TAsmSend
{
	DWORD p0;
	DWORD p1;
	DWORD p2;
	DWORD p3;
	DWORD p4;
	DWORD p5;
	DWORD p6;
	DWORD p7;
	DWORD p8;
	DWORD p9;

	TAsmSend()
	{
		p0 = 0;
		p1 = 0;
		p2 = 0;
		p3 = 0;
		p4 = 0;
		p5 = 0;
		p6 = 0;
		p7 = 0;
		p8 = 0;
		p9 = 0;
	};
};

struct TAsmSendEx
{
	DWORD p0;
	DWORD p1;
	DWORD p2;
	DWORD p3;
	DWORD p4;
	DWORD p5;
	DWORD p6;
	DWORD p7;
	DWORD p8;
	DWORD p9;
	DWORD p10;
	DWORD p11;

	TAsmSendEx()
	{
		p0 = 0;
		p1 = 0;
		p2 = 0;
		p3 = 0;
		p4 = 0;
		p5 = 0;
		p6 = 0;
		p7 = 0;
		p8 = 0;
		p9 = 0;
		p10 = 0;
		p11 = 0;
	};
};

TAsmSend* BasePackerSend(DWORD p0, DWORD p1, DWORD p2, DWORD p3, DWORD p4,
	DWORD p5, DWORD p6, DWORD p7, DWORD p8, DWORD p9);

TAsmSendEx* BasePackerSendEx(DWORD p0, DWORD p1, DWORD p2, DWORD p3, DWORD p4,
	DWORD p5, DWORD p6, DWORD p7, DWORD p8, DWORD p9, DWORD p10, DWORD p11);