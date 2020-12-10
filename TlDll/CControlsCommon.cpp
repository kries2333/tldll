#include "pch.h"
#include "CControlsCommon.h"
#include "FileSystem.h"


void CControlsCommon::InitControlBase()
{
	DWORD ControlBase = 0;
	ControlBase = (DWORD)GetModuleHandleA("CEGUIBase.dll");
	ControlBase = *(DWORD*)(ControlBase + 0x220078);
	ControlBase = *(DWORD*)(ControlBase + 0x28);
	ControlsRecursion(ControlBase, 1);
}

void CControlsCommon::ControlsRecursion(DWORD Node, int ParentId)
{
	DWORD StartBase = 0;			//起始地址
	DWORD EndBase = 0;				//结束地址
	DWORD ContextObj = 0;			//内容对象
	CONTROLS tControlsObj;			//控件对象
	char* content[256] = { 0 };
	char* str;
	char isVisible;
	StartBase = *(DWORD*)(Node + 20);
	EndBase = *(DWORD*)(Node + 24);

	try
	{
		memset(&tControlsObj, 0, sizeof(CONTROLS));

		tControlsObj.tControlBase = Node;
		tControlsObj.tParentNode = ParentId;

		if (Node == 0xcccccccc) {
			return;
		}

		if (1 == *(char*)(Node + 0x119)) {
			//dbgPrint("不可见 当前节点地址:%x,起始节点地址:%x,结束节点地址:%x, 父节点标识:%d, 节点类型:【%s】, 节点名字:【%s】", Node, StartBase, EndBase, ParentId, tControlsObj.tControlType, tControlsObj.tControlName);

			if (StartBase > 0 && EndBase > 0 && StartBase != 0xcccccccc) {
				if (IsReadAddrInt((PVOID)StartBase) && IsReadAddrInt((PVOID)EndBase)) {
					while (StartBase < EndBase)
					{
						DWORD SubBaseAddr = *(DWORD*)(StartBase);

						if (IsReadAddrString((LPSTR) * (DWORD*)(Node + 0x1D4))) {
							lstrcpy(tControlsObj.tControlType, (LPSTR) * (DWORD*)(Node + 0x1D4));
						}
						else {
							lstrcpy(tControlsObj.tControlType, (LPSTR)(DWORD*)(Node + 0x1D4));
						}

						if (IsReadAddrString((LPSTR) * (DWORD*)(Node + 0x1EC))) {
							//dbgPrint("取指针 当前节点地址:%x,起始节点地址:%x,结束节点地址:%x, 父节点标识:%d, 节点类型:【%s】, 节点名字:【%s】", Node, StartBase, EndBase, ParentId, tControlsObj.tControlType, tControlsObj.tControlName);

							lstrcpy(tControlsObj.tControlName, (LPSTR) * (DWORD*)(Node + 0x1EC));
						}
						else {
							//dbgPrint("取字符串 当前节点地址:%x,起始节点地址:%x,结束节点地址:%x, 父节点标识:%d, 节点类型:【%s】, 节点名字:【%s】", Node, StartBase, EndBase, ParentId, tControlsObj.tControlType, tControlsObj.tControlName);

							lstrcpy(tControlsObj.tControlName, (LPSTR)(DWORD*)(Node + 0x1EC));
						}
						UiArrObj.push_back(tControlsObj);	//添加可见控件
						if (IsReadAddrInt((PVOID)SubBaseAddr))
						{
							ControlsRecursion(SubBaseAddr, ParentId + 1);
						}
						StartBase = StartBase + 4;
					}
				}
			}
		}
	}
	catch (const std::exception&)
	{
		dbgPrint("控件内存溢出");
	}
}

bool CControlsCommon::isVisiableControl(CString strControlName)
{
	dbgPrint("判断控件是否显示");
	UiArrObj.clear();
	InitControlBase();	//初始化基址 并遍历所有显示控件
	CString tempControlName;
	vector<CONTROLS>::iterator iter;
	for (iter = UiArrObj.begin(); iter != UiArrObj.end(); iter++) //遍历所有控件
	{
		tempControlName = iter->tControlName;
		//dbgPrint("当前控件名称:%s", tempControlName);
		if (tempControlName == strControlName) {
			return true;
		}
	}
	return false;
}

