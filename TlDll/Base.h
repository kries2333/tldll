#include <Windows.h>
#define �����ַ 0x337F98 //0xD17F98

#define ��Χ�����ַ 0x337F3C //0xD17F3C

#define ����CALL 0x3D5F0 //0xA1D5F0

#define ѡ������CALL 0x4A450 //0xA2A450

#define �������CALL 0x4A7E0 //0xA2A7E0

#define �������CALL_ECX 0x3381BC //0xD181BC

#define ��������CALL 0x4A630 //0xA2A630

#define ��������CALL_ECX 0x3381BC //0xD181BC

#define ��������CALL 0x4A750 //0xA2A750

#define ��������CALL_ECX 0x3381BC //0xD181BC

#define ������������� 0x3383D8 //0xD183D8

#define ������ַ 0x3381C8 //0xD181C8

#define ������������� 0x337F98 //0xD17F98

#define Lָ�� 0x33F85C //0xD1F85C

#define ����ECX 0x61FFFF //0xFFFFFF

#define �������CALL 0x3B960 //0xA1B960

#define �Ի�CALL 0x3D840 //0xA1D840

#define LuaCall 0x2760 //0x9E2760

#define LuaHook 0x1E0EC0 //0xBC0EC0

#define ����״̬ 0x3381C8 //0xD181C8

#define ���޻�ַ 0x3381C8 //0xD181C8

#define ���޻�ַƫ�� 0x44388 //0x44388

#define �̵깺��CALL 0x1726F0 //0xB526F0

#define �̵깺��CALL_ECX 0x33EA98 //0xD1EA98

#define �̵깺�������ݶ��� 0x294C9C //0xC74C9C

#define ���̵���� 0x3381C8 //0xD181C8

#define ��ҩ������ݶ��� 0x294D50 //0xC74D50

#define ����CALL 0x61FFFF //0xFFFFFF

#define ����CALL 0xBA1B0 //0xA9A1B0

#define ����CALL_ECX 0x3381C8 //0xD181C8

#define ������� 0x3381BC //0xD181BC

#define �����ʶ 0x3381BC //0xD181BC

#define �Ƿ���ڶ����ʶ 0x33EE28 //0xD1EE28

#define �������CALL 0x1726F0 //0xB526F0

#define �������CALL_ECX 0x33EA98 //0xD1EA98

#define ������������ݶ��� 0x2AAE60 //0xC8AE60

#define ����CALL 0x1726F0 //0xB526F0

#define ���������� 0x271404 //0xC51404

#define �������CALL 0x1726F0 //0xB526F0

#define �������CALL_ECX 0x33EA98 //0xD1EA98

#define ������ӷ������ 0x291FA4 //0xC71FA4

#define �뿪�������ݶ��� 0x27BFDC //0xC5BFDC


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