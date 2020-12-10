#pragma once
#include <string>

typedef struct _TAsmTree
{
	_TAsmTree* left;//左子树
	_TAsmTree* root;//树
	_TAsmTree* right;//右子树
}TAsmTree, *PTAsmTree;

#if defined(UNICODE) || defined(_UNICODE)
#define cpp_char wchar_t
#define _tstring std::wstring
#define _to_string(x) std::to_wstring(x)
#else
#define cpp_char char
#define _tstring std::string
#define _to_string(x) std::to_string(x)
#endif

class CFileSystem
{
public:
	CFileSystem();
	~CFileSystem();
public:
	void FileInitial();
	void MyGetFilePath(HMODULE hModule);
public:
	_tstring Module;//动态链接库文件路径
	_tstring Script;//脚本文件目录
};


void WriteLog(CString& Cstr);

template <typename TF, typename... TS>
void DPrint(IN  const TF Format, IN const TS... list)
{
	try
	{
		CString Cstr;
		Cstr.Format(Format, list...);
		Cstr = _T("[WG][dll]") + Cstr;
		::OutputDebugString(Cstr);//调试输出到软件
		WriteLog(Cstr);//写日志
	}
	catch (...)
	{
		//::OutputDebugStringA("[WG][dll]__FUNCTION__");
	}
}

#define def__debug__lib 1
#undef  dbgPrint
#if     def__debug__lib
#define dbgPrint(_x_,...) DPrint(_x_,__VA_ARGS__)
#else
#define dbgPrint( _x_ ,...)
#endif