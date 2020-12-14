#include "pch.h"
#include "Base.h"

TAsmSend* BasePackerSend(DWORD p0, DWORD p1, DWORD p2, DWORD p3, DWORD p4,
	DWORD p5, DWORD p6, DWORD p7, DWORD p8, DWORD p9)
{
	TAsmSend *tAsmSend = new TAsmSend();
	tAsmSend->p0 = p0;
	tAsmSend->p1 = p1;
	tAsmSend->p2 = p2;
	tAsmSend->p3 = p3;
	tAsmSend->p4 = p4;
	tAsmSend->p5 = p5;
	tAsmSend->p6 = p6;
	tAsmSend->p7 = p7;
	tAsmSend->p8 = p8;
	tAsmSend->p9 = p9;

	return tAsmSend;
}

TAsmSendEx* BasePackerSendEx(DWORD p0, DWORD p1, DWORD p2, DWORD p3, DWORD p4,
	DWORD p5, DWORD p6, DWORD p7, DWORD p8, DWORD p9, DWORD p10, DWORD p11)
{
	TAsmSendEx* tAsmSendEx = new TAsmSendEx();
	tAsmSendEx->p0 = p0;
	tAsmSendEx->p1 = p1;
	tAsmSendEx->p2 = p2;
	tAsmSendEx->p3 = p3;
	tAsmSendEx->p4 = p4;
	tAsmSendEx->p5 = p5;
	tAsmSendEx->p6 = p6;
	tAsmSendEx->p7 = p7;
	tAsmSendEx->p8 = p8;
	tAsmSendEx->p9 = p9;
	tAsmSendEx->p10 = p10;
	tAsmSendEx->p11 = p11;

	return tAsmSendEx;
}