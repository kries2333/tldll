#include <Windows.h>
#define �����ַ 0x336F78 //0x746F78

#define ��Χ�����ַ 0x336F1C //0x746F1C

#define ����CALL 0x3DB70 //0x44DB70

#define ѡ������CALL 0x4A8D0 //0x45A8D0

#define �������CALL 0x4AC60 //0x45AC60

#define �������CALL_ECX 0x33719C //0x74719C

#define ��������CALL 0x4AAB0 //0x45AAB0

#define ��������CALL_ECX 0x33719C //0x74719C

#define ��������CALL 0x4ABD0 //0x45ABD0

#define ��������CALL_ECX 0x33719C //0x74719C

#define ������������� 0x3373B8 //0x7473B8

#define ������ַ 0x3371A8 //0x7471A8

#define ������������� 0x336F78 //0x746F78

#define Lָ�� 0x33E83C //0x74E83C

#define ����ECX 0x33DDEC //0x74DDEC

#define �������CALL 0x3BEE0 //0x44BEE0

#define �Ի�CALL 0x3DDC0 //0x44DDC0

#define LuaCall 0x2750 //0x412750

#define LuaHook 0x1E0490 //0x5F0490

#define ����״̬ 0x3371A8 //0x7471A8

#define ���޻�ַ 0x3371A8 //0x7471A8

#define ���޻�ַƫ�� 0x44388 //0x44388

#define �̵깺��CALL 0x171A20 //0x581A20

#define �̵깺��CALL_ECX 0x33DA78 //0x74DA78

#define �̵깺�������ݶ��� 0x293C3C //0x6A3C3C

#define ���̵���� 0x3371A8 //0x7471A8

#define ��ҩ������ݶ��� 0x293CF0 //0x6A3CF0

#define ����CALL 0x33DDEC //0x74DDEC

#define ����CALL 0xB9910 //0x4C9910

#define ����CALL_ECX 0x3371A8 //0x7471A8

#define ������� 0x33719C //0x74719C

#define �����ʶ 0x33719C //0x74719C

#define �Ƿ���ڶ����ʶ 0x33DDF0 //0x74DDF0

#define �������CALL 0x171A20 //0x581A20

#define �������CALL_ECX 0x33DA78 //0x74DA78

#define ������������ݶ��� 0x2A9C80 //0x6B9C80

#define ����CALL 0x171A20 //0x581A20

#define ���������� 0x270404 //0x680404

#define �������CALL 0x171A20 //0x581A20

#define �������CALL_ECX 0x33DA78 //0x74DA78

#define ������ӷ������ 0x290F98 //0x6A0F98

#define �뿪�������ݶ��� 0x27AFE4 //0x68AFE4

#define SKILLS_TREE_OFFSET  0xA9C
#define SKILLSXINFA_TREE_OFFSET	0xA94		//�ķ�����ƫ��

#define COLLECT_OREPLANT	0x3BF10			//�򿪰���

#define QICHEN_STATUS		0x3371A8		//���״̬

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