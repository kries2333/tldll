#pragma once
#include <malloc.h>
#include <memory.h>
#include <windows.h>

typedef enum _SYSTEM_INFORMATION_CLASS
{
	SystemBasicInformation = 0,          // 0
	SystemProcessorInformation,          // 1
	SystemPerformanceInformation,        // 2
	SystemTimeOfDayInformation,          // 3
	SystemNotImplemented1,            // 4
	SystemProcessesAndThreadsInformation = 5,  // 5
	SystemCallCounts,              // 6
	SystemConfigurationInformation,        // 7
	SystemProcessorTimes,            // 8
	SystemGlobalFlag,              // 9
	SystemNotImplemented2,            // 10
	SystemModuleInformation,          // 11
	SystemLockInformation,            // 12
	SystemNotImplemented3,            // 13
	SystemNotImplemented4,            // 14
	SystemNotImplemented5,            // 15
	SystemHandleInformation,          // 16
	SystemObjectInformation,          // 17
	SystemPagefileInformation,          // 18
	SystemInstructionEmulationCounts,      // 19
	SystemInvalidInfoClass1,          // 20
	SystemCacheInformation,            // 21
	SystemPoolTagInformation,          // 22
	SystemProcessorStatistics,          // 23
	SystemDpcInformation,            // 24
	SystemNotImplemented6,            // 25
	SystemLoadImage,              // 26
	SystemUnloadImage,              // 27
	SystemTimeAdjustment,            // 28
	SystemNotImplemented7,            // 29
	SystemNotImplemented8,            // 30
	SystemNotImplemented9,            // 31
	SystemCrashDumpInformation,          // 32
	SystemExceptionInformation,          // 33
	SystemCrashDumpStateInformation,      // 34
	SystemKernelDebuggerInformation,      // 35
	SystemContextSwitchInformation,        // 36
	SystemRegistryQuotaInformation,        // 37
	SystemLoadAndCallImage = 38,        // 38
	SystemPrioritySeparation,          // 39
	SystemNotImplemented10,            // 40
	SystemNotImplemented11,            // 41
	SystemInvalidInfoClass2,          // 42
	SystemInvalidInfoClass3,          // 43
	SystemTimeZoneInformation,          // 44
	SystemLookasideInformation,          // 45
	SystemSetTimeSlipEvent,            // 46
	SystemCreateSession,            // 47
	SystemDeleteSession,            // 48
	SystemInvalidInfoClass4,          // 49
	SystemRangeStartInformation,        // 50
	SystemVerifierInformation,          // 51
	SystemAddVerifier,              // 52
	SystemSessionProcessesInformation      // 53
} SYSTEM_INFORMATION_CLASS;

typedef enum _OBJECT_INFORMATION_CLASS
{
	ObjectBasicInformation = 0,
	ObjectNameInformation = 1,
	ObjectTypeInformation = 2,
} OBJECT_INFORMATION_CLASS;

typedef struct _SYSTEM_HANDLE_INFORMATION
{
	ULONG ProcessId;                  //进程标识符
	UCHAR ObjectTypeNumber;           //打开的对象的类型
	UCHAR Flags;                   //句柄属性标志
	USHORT Handle;                 //句柄数值,在进程打开的句柄中唯一标识某个句柄         
	PVOID Object;                  //句柄对应的EPROCESS的地址
	ACCESS_MASK GrantedAccess;          //句柄对象的访问权限
} SYSTEM_HANDLE_INFORMATION, * PSYSTEM_HANDLE_INFORMATION;


typedef struct _SYSTEM_HANDLE_INFORMATION_EX
{
	ULONG NumberOfHandles;            //句柄数目
	SYSTEM_HANDLE_INFORMATION Information[1];
} SYSTEM_HANDLE_INFORMATION_EX, * PSYSTEM_HANDLE_INFORMATION_EX;

typedef struct _UNICODE_STRING
{
	USHORT Length;
	USHORT MaximumLength;
	PWSTR  Buffer;
} UNICODE_STRING, * PUNICODE_STRING;

typedef struct _OBJECT_NAME_INFORMATION
{
	UNICODE_STRING  Name;

} OBJECT_NAME_INFORMATION, * POBJECT_NAME_INFORMATION;

typedef LONG NTSTATUS;

#define NT_SUCCESS(Status)    (((NTSTATUS)(Status)) >= 0)

typedef NTSTATUS(WINAPI* ZWQUERYSYSTEMINFORMATION)(DWORD, PVOID, DWORD, PDWORD);
typedef NTSTATUS(WINAPI* ZWQUERYOBJECT)(IN HANDLE OPTIONAL, IN OBJECT_INFORMATION_CLASS, OUT PVOID OPTIONAL, IN ULONG, OUT PULONG OPTIONAL);

