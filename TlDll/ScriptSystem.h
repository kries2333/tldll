#pragma once
#include "MeLua.h"
#include "FileSystem.h"

class CScriptSystem
{
public:
	CScriptSystem() {
		pState = new LuaStateOwner();
		MeLua = new CMeLua();
	};
	~CScriptSystem()
	{
		delete pState;
		delete MeLua;
	};
public:
	void LUA_ScriptSystemInitial();
	void LUA_DoFile(const char* fileName)
	{
		int state = (*pState)->DoFile(fileName);
		dbgPrint("LUA_DoFile fileName=%s state = %d", fileName, state);
	}
private:
	CMeLua* MeLua;//声明对象
	LuaStateOwner* pState;//初始化lua的基本库，才能调用lua函数
};

