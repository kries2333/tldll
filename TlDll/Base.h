#include <Windows.h>

#define 人物基址 0x336F78 //0x746F78
#define L指针 0x33E83C //0x74E83C

#define SKILLS_TREE_OFFSET  0xA9C
#define 技能CALL 0x3DB70 //0x44DB70

#define COLLECT_OREPLANT	0x3BF10			//打开包裹

#define 对话CALL 0x3DDC0 //0x44DDC0
#define 背包基址 0x3371A8 //0x7471A8

#define 穿戴CALL 0x33DDEC //0x74DDEC
#define 销毁CALL 0xB9910 //0x4C9910
#define 销毁CALL_ECX 0x3371A8 //0x7471A8

#define QICHEN_STATUS		0x3371A8		//骑乘状态

#define 珍兽基址 0x3371A8 //0x7471A8
#define 珍兽基址偏移 0xFFC34388 //0x44388

#define 商店购买CALL 0x171A20 //0x581A20
#define 商店购买CALL_ECX 0x33DA78 //0x74DA78
#define 商店购买封包数据对象 0x293C3C //0x6A3C3C

#define 吃药CALL			商店购买CALL
#define 吃药CALL_ECX		商店购买CALL_ECX
#define 吃药封包数据对象 0x293CF0 //0x6A3CF0

#define 邀请组队CALL_ECX 0x33DA78 //0x74DA78
#define 邀请组队CALL 0x171A20 //0x581A20
#define 邀请组队封包数据 0x290F98 //0x6A0F98
#define 加入队伍CALL 0x171A20 //0x581A20

#define 加入队伍CALL_ECX 0x33DA78 //0x74DA78

#define 加入队伍封包数据对象 0x2A9C80 //0x6B9C80

#define 组队数组 0x33719C //0x74719C
#define 是否存在队伍标识 0x33DDF0 //0x74DDF0


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