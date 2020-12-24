#include <Windows.h>
#define 人物基址 0x337F98 //0x1067F98

#define 周围怪物基址 0x337F3C //0x1067F3C

#define 技能CALL 0x3D5F0 //0xD6D5F0

#define 选择任务CALL 0x4A450 //0xD7A450

#define 任务完成CALL 0x4A7E0 //0xD7A7E0

#define 任务完成CALL_ECX 0x3381BC //0x10681BC

#define 接受任务CALL 0x4A630 //0xD7A630

#define 接受任务CALL_ECX 0x3381BC //0x10681BC

#define 继续任务CALL 0x4A750 //0xD7A750

#define 继续任务CALL_ECX 0x3381BC //0x10681BC

#define 任务二叉树对象 0x3383D8 //0x10683D8

#define 背包基址 0x3381C8 //0x10681C8

#define 怪物二叉树对象 0x337F98 //0x1067F98

#define L指针 0x33F85C //0x106F85C

#define 卖物ECX 0x2CFFFF //0xFFFFFF

#define 地面包裹CALL 0x3B960 //0xD6B960

#define 对话CALL 0x3D840 //0xD6D840

#define LuaCall 0x2760 //0xD32760

#define LuaHook 0x1E0EC0 //0xF10EC0

#define 坐骑状态 0x3381C8 //0x10681C8

#define 珍兽基址 0x3381C8 //0x10681C8

#define 珍兽基址偏移 0x44388 //0x44388

#define 商店购买CALL 0x1726F0 //0xEA26F0

#define 商店购买CALL_ECX 0x33EA98 //0x106EA98

#define 商店购买封包数据对象 0x294C9C //0xFC4C9C

#define 打开商店对象 0x3381C8 //0x10681C8

#define 吃药封包数据对象 0x294D50 //0xFC4D50

#define 穿戴CALL 0x2CFFFF //0xFFFFFF

#define 销毁CALL 0xBA1B0 //0xDEA1B0

#define 销毁CALL_ECX 0x3381C8 //0x10681C8

#define 组队数组 0x3381BC //0x10681BC

#define 邀请标识 0x3381BC //0x10681BC

#define 是否存在队伍标识 0x33EE28 //0x106EE28

#define 加入队伍CALL 0x1726F0 //0xEA26F0

#define 加入队伍CALL_ECX 0x33EA98 //0x106EA98

#define 加入队伍封包数据对象 0x2AAE60 //0xFDAE60

#define 修理CALL 0x1726F0 //0xEA26F0

#define 修理封包数据 0x271404 //0xFA1404

#define 邀请组队CALL 0x1726F0 //0xEA26F0

#define 邀请组队CALL_ECX 0x33EA98 //0x106EA98

#define 邀请组队封包数据 0x291FA4 //0xFC1FA4

#define 离开队伍数据对象 0x27BFDC //0xFABFDC

#define SKILLS_TREE_OFFSET  0xA9C
#define SKILLSXINFA_TREE_OFFSET	0xA94		//心法技能偏移

#define 吃药CALL			商店购买CALL
#define 吃药CALL_ECX		商店购买CALL_ECX


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