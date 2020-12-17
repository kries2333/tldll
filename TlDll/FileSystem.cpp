#include "pch.h"
#include "FileSystem.h"

//写日志文件函数
void WriteLog(CString& Cstr)
{
	//try
	//{
	//	//设置文件的打开参数
	//	CStdioFile outFile(pFileSystem->logFile.c_str(), CFile::modeNoTruncate | CFile::modeCreate | CFile::modeWrite | CFile::typeText);
	//	//////////////////////////////////////////////////////////////////////////

	//	CFileStatus fileStatus;
	//	if (outFile.GetStatus(pFileSystem->logFile.c_str(), fileStatus))
	//	{
	//		if (fileStatus.m_size > 40960)
	//			outFile.SetLength(0);
	//	}

	//	CTime tt = CTime::GetCurrentTime();
	//	//作为Log文件，经常要给每条Log打时间戳，时间格式可自由定义，
	//	//这里的格式如：2010-June-10 Thursday, 15:58:12
	//	Cstr = tt.Format(_T("[%Y-%B-%d %A, %H:%M:%S]")) + Cstr + _T("\n");
	//	//在文件末尾插入新纪录
	//	outFile.SeekToEnd();
	//	outFile.WriteString(Cstr);
	//	outFile.Close();

	//}
	//catch (CFileException* fx)
	//{//文件异常
	//	fx->Delete();
	//}
};

CFileSystem::CFileSystem()
{
}


CFileSystem::~CFileSystem()
{
}

void CFileSystem::FileInitial()
{
	////////////////////脚本目录下的文件//////////////////////////////////////
	//Script目录

	Script = Module + _T("script\\");
}

void CFileSystem::MyGetFilePath(HMODULE hModule)
{//获取dll路径和登录脚本文件路径


	TCHAR* szBuffer = new TCHAR[MAX_PATH * 2];
	::GetModuleFileName(hModule, szBuffer, MAX_PATH * 2);
	if (!_tcscmp(szBuffer, _T("")))
	{
		//获取模块路径
		//dbgPrint(_T("GetModuleFileName Error %d"), GetLastError());
	}
	//查找字符串最左侧的'\'字符
	TCHAR* lpPath = _tcsrchr(szBuffer, _T('\\'));
	::ZeroMemory(++lpPath, 50);//将模块名字清除

	Module = szBuffer;
	////存放帐号的文件
	//Account = szBuffer;
	//Account += ACCOUNT_FILE_NAME;
	////用户登录文件路径--START
	//Login = szBuffer;
	//Login += LOGIN_FILE_NAME;
	////用户登录文件路径---END
	Script = Module + _T("script\\");
	////_tprintf(_T("%s\n"), Item.c_str());

	delete[]szBuffer;
	szBuffer = NULL;
}