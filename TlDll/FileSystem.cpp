#include "pch.h"
#include "FileSystem.h"

extern CFileSystem* pFileSystem;

//д��־�ļ�����
void WriteLog(CString& Cstr)
{
	//try
	//{
	//	//�����ļ��Ĵ򿪲���
	//	CStdioFile outFile(pFileSystem->logFile.c_str(), CFile::modeNoTruncate | CFile::modeCreate | CFile::modeWrite | CFile::typeText);
	//	//////////////////////////////////////////////////////////////////////////

	//	CFileStatus fileStatus;
	//	if (outFile.GetStatus(pFileSystem->logFile.c_str(), fileStatus))
	//	{
	//		if (fileStatus.m_size > 40960)
	//			outFile.SetLength(0);
	//	}

	//	CTime tt = CTime::GetCurrentTime();
	//	//��ΪLog�ļ�������Ҫ��ÿ��Log��ʱ�����ʱ���ʽ�����ɶ��壬
	//	//����ĸ�ʽ�磺2010-June-10 Thursday, 15:58:12
	//	Cstr = tt.Format(_T("[%Y-%B-%d %A, %H:%M:%S]")) + Cstr + _T("\n");
	//	//���ļ�ĩβ�����¼�¼
	//	outFile.SeekToEnd();
	//	outFile.WriteString(Cstr);
	//	outFile.Close();

	//}
	//catch (CFileException* fx)
	//{//�ļ��쳣
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
	////////////////////�ű�Ŀ¼�µ��ļ�//////////////////////////////////////
	//ScriptĿ¼

	Script = Module + _T("script\\");
}

void CFileSystem::MyGetFilePath(HMODULE hModule)
{//��ȡdll·���͵�¼�ű��ļ�·��


	TCHAR* szBuffer = new TCHAR[MAX_PATH * 2];
	::GetModuleFileName(hModule, szBuffer, MAX_PATH * 2);
	if (!_tcscmp(szBuffer, _T("")))
	{//��ȡģ��·��
		//dbgPrint(_T("GetModuleFileName Error %d"), GetLastError());
	}
	//�����ַ���������'\'�ַ�
	TCHAR* lpPath = _tcsrchr(szBuffer, _T('\\'));
	::ZeroMemory(++lpPath, 50);//��ģ���������

	Module = szBuffer;
	////����ʺŵ��ļ�
	//Account = szBuffer;
	//Account += ACCOUNT_FILE_NAME;
	////�û���¼�ļ�·��--START
	//Login = szBuffer;
	//Login += LOGIN_FILE_NAME;
	////�û���¼�ļ�·��---END
	Script = Module + _T("script\\");
	////_tprintf(_T("%s\n"), Item.c_str());

	delete[]szBuffer;
	szBuffer = NULL;
}