#pragma once
#include <iostream>
#include <vector>
#include "pch.h"
using namespace std;

#define  IsReadAddrInt(addr) !IsBadReadPtr(addr,sizeof(unsigned int))//���ֽ��ж�
#define  IsReadAddrByte(addr) !IsBadReadPtr(addr,sizeof(byte))//һ�ֽ��ж�
#define  IsReadAddrString(addr) !IsBadStringPtr(addr,sizeof(DWORD))//�Ƿ����ַ���ָ���ж�

typedef struct _Controls {
	TCHAR	tControlName[150];			//�ؼ�����
	TCHAR	tControlType[150];			//�ؼ�����
	DWORD	tControlBase;				//�ؼ���ַ
	DWORD	tParentNode;				//���ؼ�ID
	DWORD	tControlPointLeftX;			//���Ͻ�X����
	DWORD	tControlPointLeftY;			//���Ͻ�Y����
	DWORD	tControlPointRightX;		//���Ͻ�X����
	DWORD	tControlPointRightY;		//���Ͻ�Y����
}CONTROLS, * PCONTROLS;

class CControlsCommon
{
public:
	void InitControlBase();

	void ControlsRecursion(DWORD Node, int ParentId); //�ݹ��ȡ������ʾ�ؼ�

	bool isVisiableControl(CString strControlName);


public:
	vector<CONTROLS> UiArrObj;			//�ؼ��������
};

