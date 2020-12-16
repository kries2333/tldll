#pragma once
// 隐藏模块.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include <Windows.h>


/* 所需要的结构体
1. _LDR_DATA_TABLE_ENTRY 链表指向数据
2. _PEB_LDR_DATA 表示其 PEB0x处指向的数据表
3. _LIST_ENTRY 指针指向的链表
*/

typedef struct _LSA_UNICODE_STRING {
    USHORT Length;
    USHORT MaximumLength;
    PWSTR  Buffer;
}
UNICODE_STRING, * PUNICODE_STRING;

typedef struct _PEB_LDR_DATA
{
    DWORD Length; // +0x00
    bool Initialized; // +0x04
    PVOID SsHandle; // +0x08
    LIST_ENTRY InLoadOrderModuleList; // +0x0c
    LIST_ENTRY InMemoryOrderModuleList; // +0x14
    LIST_ENTRY InInitializationOrderModuleList;// +0x1c
} PEB_LDR_DATA, * PPEB_LDR_DATA; // +0x24

typedef struct _LDR_MODULE
{
    LIST_ENTRY          InLoadOrderModuleList;
    LIST_ENTRY          InMemoryOrderModuleList;
    LIST_ENTRY          InInitializationOrderModuleList;
    void* BaseAddress;
    void* EntryPoint;
    ULONG               SizeOfImage;
    UNICODE_STRING   FullDllName;
    UNICODE_STRING      BaseDllName;
    ULONG               Flags;
    SHORT               LoadCount;
    SHORT               TlsIndex;
    HANDLE              SectionHandle;
    ULONG               CheckSum;
    ULONG               TimeDateStamp;
} LDR_MODULE, * PLDR_MODULE;

//typedef struct _IMAGE_DOS_HEADER {      // DOS .EXE header
//    WORD   e_magic;                     // Magic number
//    WORD   e_cblp;                      // Bytes on last page of file
//    WORD   e_cp;                        // Pages in file
//    WORD   e_crlc;                      // Relocations
//    WORD   e_cparhdr;                   // Size of header in paragraphs
//    WORD   e_minalloc;                  // Minimum extra paragraphs needed
//    WORD   e_maxalloc;                  // Maximum extra paragraphs needed
//    WORD   e_ss;                        // Initial (relative) SS value
//    WORD   e_sp;                        // Initial SP value
//    WORD   e_csum;                      // Checksum
//    WORD   e_ip;                        // Initial IP value
//    WORD   e_cs;                        // Initial (relative) CS value
//    WORD   e_lfarlc;                    // File address of relocation table
//    WORD   e_ovno;                      // Overlay number
//    WORD   e_res[4];                    // Reserved words
//    WORD   e_oemid;                     // OEM identifier (for e_oeminfo)
//    WORD   e_oeminfo;                   // OEM information; e_oemid specific
//    WORD   e_res2[10];                  // Reserved words
//    LONG   e_lfanew;                    // File address of new exe header
//} IMAGE_DOS_HEADER, * PIMAGE_DOS_HEADER;
//
//typedef struct _IMAGE_NT_HEADERS {
//    DWORD Signature;
//    IMAGE_FILE_HEADER FileHeader;
//    IMAGE_OPTIONAL_HEADER32 OptionalHeader;
//} IMAGE_NT_HEADERS32, * PIMAGE_NT_HEADERS32;

//所谓模块句柄，即该模块的入口地址
void hide_module(char* szDllName);

void clean_pe(char* szDllName);