// TlDll.cpp: 定义 DLL 的初始化例程。
//

#include "pch.h"
#include "framework.h"
#include "TlDll.h"
#include "Main.h"
#include "mutex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CMain* g_pMain;
//
//TODO:  如果此 DLL 相对于 MFC DLL 是动态链接的，
//		则从此 DLL 导出的任何调入
//		MFC 的函数必须将 AFX_MANAGE_STATE 宏添加到
//		该函数的最前面。
//
//		例如: 
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 此处为普通函数体
//		}
//
//		此宏先于任何 MFC 调用
//		出现在每个函数中十分重要。  这意味着
//		它必须作为以下项中的第一个语句:
//		出现，甚至先于所有对象变量声明，
//		这是因为它们的构造函数可能生成 MFC
//		DLL 调用。
//
//		有关其他详细信息，
//		请参阅 MFC 技术说明 33 和 58。
//

// CTlDllApp

BEGIN_MESSAGE_MAP(CTlDllApp, CWinApp)
END_MESSAGE_MAP()


// CTlDllApp 构造

CTlDllApp::CTlDllApp()
{
	// TODO:  在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的 CTlDllApp 对象

CTlDllApp theApp;
bool CloseObjectByName(LPCTSTR pObjectName, int type = 11);

// CTlDllApp 初始化

BOOL CTlDllApp::InitInstance()
{
	CWinApp::InitInstance();
	g_pMain = new CMain();
	g_pMain->hDll = theApp.m_hInstance;
	g_pMain->CreateLogin(1);

	//CloseObjectByName("\\BaseNamedObjects\\DBWinMutex");
	//CloseObjectByName("\\BaseNamedObjects\\__DDravCheckExclMode__");
	//CloseObjectByName("\\BaseNamedObjects\\__DDravExclMode__");
	//CloseObjectByName("\\BaseNamedObjects\\DDrawDriverObjectListMutex");
	//CloseObjectByName("\\BaseNamedObjects\\ShimCacheMutex");
	//CloseObjectByName("\\BaseNamedObjects\\DDrawWindowListMutex");
	//CloseObjectByName("\\BaseNamedObjects\\MtxNPGL");
	//CloseObjectByName("\\BaseNamedObjects\\NL59NPGL");

	return TRUE;
}

bool CloseObjectByName(LPCTSTR pObjectName, int type)
{
	int i;
	ULONG pid;
	ULONG ulSize;
	ULONG* pHandleInfor;
	CHAR pName[2000];
	NTSTATUS ntStatus;
	HMODULE hHanlde;
	POBJECT_NAME_INFORMATION ObjName;
	PSYSTEM_HANDLE_INFORMATION_EX Handles;
	ZWQUERYOBJECT ZwQueryObject;
	ZWQUERYSYSTEMINFORMATION ZwQuerySystemInformation;


	//初始化变量
	ulSize = 0x4000;
	pHandleInfor = NULL;
	ZwQueryObject = NULL;
	ZwQuerySystemInformation = NULL;


	//由于ZwQueryObject和ZwQuerySystemInformation是未导出的函数，需要动态加载Ntdll,dll，然后通过函数GetProcAddress
	//得到它们的函数地址，由于这个dll一般的进程都会在创建的时候加载，所以省略加载，直接获取其模块地址
	hHanlde = GetModuleHandle("ntdll.dll");
	if (NULL == hHanlde)
	{
		//加载Ntdll.dll失败
		return false;
	}

	//获取ZwQuerySystemInformation函数地址  
	ZwQuerySystemInformation = (ZWQUERYSYSTEMINFORMATION)GetProcAddress(hHanlde, "ZwQuerySystemInformation");
	if (NULL == ZwQuerySystemInformation)
	{
		//获取ZwQuerySystemInformation函数地址失败
		return false;
	}

	//获取ZwQueryObject函数地址
	ZwQueryObject = (ZWQUERYOBJECT)GetProcAddress(hHanlde, "ZwQueryObject");
	if (NULL == ZwQueryObject)
	{
		//获取ZwQueryObject函数地址失败
		return false;
	}

	//获取系统所有句柄信息
	do
	{
		//申请内存
		pHandleInfor = (ULONG*)malloc(ulSize);
		if (NULL == pHandleInfor)
		{
			//申请内存失败
			return false;
		}

		ntStatus = ZwQuerySystemInformation(SystemHandleInformation, pHandleInfor, ulSize, NULL);
		if (!NT_SUCCESS(ntStatus))
		{
			//空间不足继续申请。
			free(pHandleInfor);
			ulSize = ulSize * 2;

			//为防止ZwQuerySystemInformation一直失败，程序陷入死循环，当申请的空间超过64M时则返回失败
			if (ulSize > 0x4000000)
			{
				return false;
			}
		}
	} while (!NT_SUCCESS(ntStatus));

	//转换数据结构类型
	Handles = (PSYSTEM_HANDLE_INFORMATION_EX)pHandleInfor;
	if (NULL == Handles)
	{
		return false;
	}

	//获取当前进程pid
	pid = GetCurrentProcessId();

	//申请空间，用于存储对象的名字信息
	ObjName = (POBJECT_NAME_INFORMATION)malloc(0x2000);

	//开始搜索获取的句柄信息，并对句柄对应的对象名进行比较，如果与要求关闭的名字相同，则关闭此句柄
	for (i = 0; i < Handles->NumberOfHandles; i++)
	{
		//对于不是本进程的句柄对象，直接pass掉，如果要实现关闭其它进程的对象，则可以首先根据PID打开这个句柄所在的进程，
		//然后复制此对象，然后进行名字比较，如果相同，则可以通过创建远程线程的方式，关闭掉。
		if (pid != Handles->Information[i].ProcessId)
		{
			continue;
		}
		//获取这个对象的类型


		if (Handles->Information[i].ObjectTypeNumber != type)
		{
			//非互斥内核对象类型
			continue;
		}

		//获取这个对象的名字信息
		ntStatus = ZwQueryObject((HANDLE)Handles->Information[i].Handle, ObjectNameInformation, ObjName, 0x2000, NULL);
		if (!NT_SUCCESS(ntStatus))
		{
			//查询对象失败，进行下一个
			//  continue;
		}

		//将unicode 字串转换为 ansi字串
		WideCharToMultiByte(CP_ACP, 0, ObjName->Name.Buffer, -1, pName, 2000, NULL, NULL);
		// stream=fopen("c:\\fprintf.out","a");  
		// fprintf(stream,"%s\n",pName);
		//	fclose(stream);   


		if (0 == strcmp(pName, pObjectName))
		{
			//找到对应名字的对象，将其关闭

			CloseHandle((HANDLE)Handles->Information[i].Handle);
			//释放申请的空间
			free(Handles);
			free(ObjName);

			return true;
		}

	}

	//释放申请的空间
	free(Handles);
	free(ObjName);

	return false;
}
