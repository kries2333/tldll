#include <Windows.h>

#define LROLE_BASE			0x336F78		//�����ַ#pragma once
#define LLUASTATE_BASE		0x33E83C		//lua״̬Lָ�� 

#define SKILLS_TREE_OFFSET  0xA9C
#define SKILLS_CALL			0x3DBA0			//���ܵ���call

#define COLLECT_OREPLANT	0x3BF10			//�򿪰���

#define NPC_CALL			0x3D7D0			//npc�Ի�Call
#define BAG_BASE			0x3371A8		//������ַ

#define WEAR_CALL			0x33DDEC		//������Ʒcall
#define DESTROY_CALL		0xB98D0			//������Ʒcall
#define DESTROY_ECX			0x3371A8		//������Ʒecx

#define QICHEN_STATUS		0x3371A8		//���״̬

#define PET_BASE			0x3371A8		//��������
#define PET_OFFSET			0x44388			//����ƫ��

#define SHOP_CALL			0x171950		//�̵깺����ƷCall
#define SHOP_CALL_ECX		0x33DA78
#define SHOP_CALL_OBJ		0x293C54

#define CHIYAO_CALL			SHOP_CALL
#define CHIYAO_CALL_ECX		SHOP_CALL_ECX
#define CHIYAO_CALL_OBJ		0x293D08		//��ҩ��������

#define	TEAM_ECX			0x33DA78		//��ӷ��CALL_ecx
#define	TEAM_CALL			0x171500		//��ӷ��CALL
#define	TEAM_DATA1			0x290FA0		//��ӷ������
#define	TEAM_INFO			0x33719C		//��������ַ
#define TEAM_STATUS			0x33DDF0		//����״̬

//#define PET_CALL				0x170530	//�����սCall
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