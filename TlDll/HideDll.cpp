#include "pch.h"
#include "HideDll.h"
#include "FileSystem.h"

void hide_module(char* szDllName)
{
    HMODULE hMod = GetModuleHandleA(szDllName);
    PLIST_ENTRY Head, Cur;
    PPEB_LDR_DATA ldr;
    PLDR_MODULE ldm;
    __asm
    {
        mov eax, fs: [0x30]
        mov ecx, [eax + 0x0c] //Ldr
        mov ldr, ecx
    }
    Head = &(ldr->InLoadOrderModuleList);
    Cur = Head->Flink;
    do
    {
        ldm = CONTAINING_RECORD(Cur, LDR_MODULE, InLoadOrderModuleList);
        if (hMod == ldm->BaseAddress)
        {
            // ��������ͬʱ���ϵ�
            ldm->InLoadOrderModuleList.Blink->Flink =
                ldm->InLoadOrderModuleList.Flink;
            ldm->InLoadOrderModuleList.Flink->Blink =
                ldm->InLoadOrderModuleList.Blink;

            //
            ldm->InInitializationOrderModuleList.Blink->Flink =
                ldm->InInitializationOrderModuleList.Flink;
            ldm->InInitializationOrderModuleList.Flink->Blink =
                ldm->InInitializationOrderModuleList.Blink;

            //
            ldm->InMemoryOrderModuleList.Blink->Flink =
                ldm->InMemoryOrderModuleList.Flink;
            ldm->InMemoryOrderModuleList.Flink->Blink =
                ldm->InMemoryOrderModuleList.Blink;
            break;
        }
        Cur = Cur->Flink;
    } while (Head != Cur);
}

void clean_pe(char* szDllName)
{
    HMODULE hMod = GetModuleHandleA(szDllName);
    DWORD        dwMemPro = NULL;
    IMAGE_DOS_HEADER* pDosHeader;
    IMAGE_NT_HEADERS* pNtHeader;

    pDosHeader = (IMAGE_DOS_HEADER*)(hMod);
    pNtHeader = (IMAGE_NT_HEADERS*)((DWORD)hMod + pDosHeader->e_lfanew);
    dbgPrint("ģ���ַ��%x PEͷ��%x NTͷ��%x", hMod, pDosHeader->e_magic, pNtHeader->Signature);
    VirtualProtect((void*)hMod, 0x1000, PAGE_EXECUTE_READWRITE, &dwMemPro);
    memset((void*)hMod, 0, 0x1000);
    VirtualProtect((void*)hMod, 0x1000, dwMemPro, &dwMemPro);
    dbgPrint("PEָ��Ĩ��");
    //pDosHeader->e_magic = 0;
    //pNtHeader->Signature = 0;
}
