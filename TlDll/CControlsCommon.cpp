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
	DWORD StartBase = 0;			//��ʼ��ַ
	DWORD EndBase = 0;				//������ַ
	DWORD ContextObj = 0;			//���ݶ���
	CONTROLS tControlsObj;			//�ؼ�����
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
			//dbgPrint("���ɼ� ��ǰ�ڵ��ַ:%x,��ʼ�ڵ��ַ:%x,�����ڵ��ַ:%x, ���ڵ��ʶ:%d, �ڵ�����:��%s��, �ڵ�����:��%s��", Node, StartBase, EndBase, ParentId, tControlsObj.tControlType, tControlsObj.tControlName);

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
							//dbgPrint("ȡָ�� ��ǰ�ڵ��ַ:%x,��ʼ�ڵ��ַ:%x,�����ڵ��ַ:%x, ���ڵ��ʶ:%d, �ڵ�����:��%s��, �ڵ�����:��%s��", Node, StartBase, EndBase, ParentId, tControlsObj.tControlType, tControlsObj.tControlName);

							lstrcpy(tControlsObj.tControlName, (LPSTR) * (DWORD*)(Node + 0x1EC));
						}
						else {
							//dbgPrint("ȡ�ַ��� ��ǰ�ڵ��ַ:%x,��ʼ�ڵ��ַ:%x,�����ڵ��ַ:%x, ���ڵ��ʶ:%d, �ڵ�����:��%s��, �ڵ�����:��%s��", Node, StartBase, EndBase, ParentId, tControlsObj.tControlType, tControlsObj.tControlName);

							lstrcpy(tControlsObj.tControlName, (LPSTR)(DWORD*)(Node + 0x1EC));
						}
						UiArrObj.push_back(tControlsObj);	//��ӿɼ��ؼ�
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
		dbgPrint("�ؼ��ڴ����");
	}
}

bool CControlsCommon::isVisiableControl(CString strControlName)
{
	dbgPrint("�жϿؼ��Ƿ���ʾ");
	UiArrObj.clear();
	InitControlBase();	//��ʼ����ַ ������������ʾ�ؼ�
	CString tempControlName;
	vector<CONTROLS>::iterator iter;
	for (iter = UiArrObj.begin(); iter != UiArrObj.end(); iter++) //�������пؼ�
	{
		tempControlName = iter->tControlName;
		//dbgPrint("��ǰ�ؼ�����:%s", tempControlName);
		if (tempControlName == strControlName) {
			return true;
		}
	}
	return false;
}

