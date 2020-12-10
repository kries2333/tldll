#pragma once
#include <iostream>
#include <vector>
#include "pch.h"
using namespace std;

#define  IsReadAddrInt(addr) !IsBadReadPtr(addr,sizeof(unsigned int))//四字节判断
#define  IsReadAddrByte(addr) !IsBadReadPtr(addr,sizeof(byte))//一字节判断
#define  IsReadAddrString(addr) !IsBadStringPtr(addr,sizeof(DWORD))//是否是字符串指针判断

typedef struct _Controls {
	TCHAR	tControlName[150];			//控件名字
	TCHAR	tControlType[150];			//控件类型
	DWORD	tControlBase;				//控件地址
	DWORD	tParentNode;				//父控件ID
	DWORD	tControlPointLeftX;			//左上角X坐标
	DWORD	tControlPointLeftY;			//左上角Y坐标
	DWORD	tControlPointRightX;		//右上角X坐标
	DWORD	tControlPointRightY;		//右上角Y坐标
}CONTROLS, * PCONTROLS;

class CControlsCommon
{
public:
	void InitControlBase();

	void ControlsRecursion(DWORD Node, int ParentId); //递归获取所有显示控件

	bool isVisiableControl(CString strControlName);


public:
	vector<CONTROLS> UiArrObj;			//控件数组对象
};

