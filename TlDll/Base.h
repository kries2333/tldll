#include <Windows.h>
#define �����ַ 0x337F98 //0x1067F98

#define ��Χ�����ַ 0x337F3C //0x1067F3C

#define ����CALL 0x3D5F0 //0xD6D5F0

#define ѡ������CALL 0x4A450 //0xD7A450

#define �������CALL 0x4A7E0 //0xD7A7E0

#define �������CALL_ECX 0x3381BC //0x10681BC

#define ��������CALL 0x4A630 //0xD7A630

#define ��������CALL_ECX 0x3381BC //0x10681BC

#define ��������CALL 0x4A750 //0xD7A750

#define ��������CALL_ECX 0x3381BC //0x10681BC

#define ������������� 0x3383D8 //0x10683D8

#define ������ַ 0x3381C8 //0x10681C8

#define ������������� 0x337F98 //0x1067F98

#define Lָ�� 0x33F85C //0x106F85C

#define ����ECX 0x2CFFFF //0xFFFFFF

#define �������CALL 0x3B960 //0xD6B960

#define �Ի�CALL 0x3D840 //0xD6D840

#define LuaCall 0x2760 //0xD32760

#define LuaHook 0x1E0EC0 //0xF10EC0

#define ����״̬ 0x3381C8 //0x10681C8

#define ���޻�ַ 0x3381C8 //0x10681C8

#define ���޻�ַƫ�� 0x44388 //0x44388

#define �̵깺��CALL 0x1726F0 //0xEA26F0

#define �̵깺��CALL_ECX 0x33EA98 //0x106EA98

#define �̵깺�������ݶ��� 0x294C9C //0xFC4C9C

#define ���̵���� 0x3381C8 //0x10681C8

#define ��ҩ������ݶ��� 0x294D50 //0xFC4D50

#define ����CALL 0x2CFFFF //0xFFFFFF

#define ����CALL 0xBA1B0 //0xDEA1B0

#define ����CALL_ECX 0x3381C8 //0x10681C8

#define ������� 0x3381BC //0x10681BC

#define �����ʶ 0x3381BC //0x10681BC

#define �Ƿ���ڶ����ʶ 0x33EE28 //0x106EE28

#define �������CALL 0x1726F0 //0xEA26F0

#define �������CALL_ECX 0x33EA98 //0x106EA98

#define ������������ݶ��� 0x2AAE60 //0xFDAE60

#define ����CALL 0x1726F0 //0xEA26F0

#define ���������� 0x271404 //0xFA1404

#define �������CALL 0x1726F0 //0xEA26F0

#define �������CALL_ECX 0x33EA98 //0x106EA98

#define ������ӷ������ 0x291FA4 //0xFC1FA4

#define �뿪�������ݶ��� 0x27BFDC //0xFABFDC

#define SKILLS_TREE_OFFSET  0xA9C
#define SKILLSXINFA_TREE_OFFSET	0xA94		//�ķ�����ƫ��

#define ��ҩCALL			�̵깺��CALL
#define ��ҩCALL_ECX		�̵깺��CALL_ECX


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