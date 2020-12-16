#include "pch.h"
#include "Function.h"
#include "Message.h"
#include "AsmMonster.h"
#include "AsmRole.h"
#include "Me.h"
#include "AsmTask.h"
#include "AsmGroundParcel.h"
#include "AsmItem.h"
#include "HPInit.h"
#include "AsmPet.h"
#include "AsmStoreItem.h"
#include "AsmTeam.h"
#include <map>
#include <TlHelp32.h>
using namespace std;

extern CMe* g_pMe;
extern CMessage* g_pMsg;
extern CAsmRole* g_pAsmRole;
extern CAsmTask* g_pAsmTask;
extern CAsmMonster* g_pAsmMonster;
extern CUser* g_pUser;
extern CAsmSkill *g_pAsmSkill;
extern CAsmGroundParcel* g_pGroundParcel;
extern CAsmItem* g_pAsmItem;
extern CAsmPet* g_pAsmPet;
extern CAsmStoreItem* g_pAsmStoreItem;
extern CAsmTeam* g_pAsmTeam;

extern CHPInit* g_pHPInit;
extern CCriticalSection* g_pCriticalSection;
DWORD g_GameExeBase = 0;

struct TFunScene
{
	char szName[20];
	int nSceneId;
};

TFunScene gScene_Array[]//��ͼ��������
{
	{"����", 0},
	{ "����", 1 },
	{ "����", 2 },
	{ "��ɽ", 3 },
	{ "̫�� ", 4 },
	{ "����", 5 },
	{ "����ɽ", 6 },
	{ "����", 7 },
	{ "�ػ�", 8 },
	{ "������", 9 },
	{ "ؤ���ܶ�", 0xA },
	{ "������", 0xB },
	{ "�䵱ɽ", 0xC },
	{ "������", 0xD },
	{ "�貨��", 0xE },
	{ "����ɽ", 0xF },
	{ "���޺�", 0x10 },
	{ "��ɽ", 0x11 },
	{ "����", 0x12 },
	{ "����", 0x1E },
	{ "����", 24 },
	{ "��԰", 0x7B },
	{ "Сľ����", 122},
	{ "���ض�һ��", 166},
	{ "Ħ�¶�", 213},
};

TAsmMap role_curmap()
{
	TAsmMap tcurmap;
	tcurmap.bool_ret = TRUE;
	tcurmap.nSceneId =g_pMsg->msg_getnumber("g_GetValue = GetSceneID()");
	tcurmap.szName = g_pMsg->msg_getstring("mycurmapname", "mycurmapname = GetCurrentSceneName()").c_str();
	if (tcurmap.szName == "����")
		tcurmap.nSceneId = 0;
	return tcurmap;
}


HMODULE GetProcessModuleHandle(DWORD pid, CONST TCHAR* moduleName) {	// ���� PID ��ģ��������Ҫд��׺���磺".dll"������ȡģ����ڵ�ַ��
	MODULEENTRY32 moduleEntry;
	HANDLE handle = NULL;
	handle = ::CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid); //  ��ȡ���̿����а�����th32ProcessID��ָ���Ľ��̵����е�ģ�顣
	if (!handle) {
		CloseHandle(handle);
		return NULL;
	}
	ZeroMemory(&moduleEntry, sizeof(MODULEENTRY32));
	moduleEntry.dwSize = sizeof(MODULEENTRY32);
	if (!Module32First(handle, &moduleEntry)) {
		CloseHandle(handle);
		return NULL;
	}
	do {
		if (_tcscmp(moduleEntry.szModule, moduleName) == 0) { return moduleEntry.hModule; }
	} while (Module32Next(handle, &moduleEntry));
	CloseHandle(handle);
	return 0;
}

DWORD GetGameProcessId(CString processName)
{
	DWORD ProcessId = 0;

	HANDLE hSnapshot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	PROCESSENTRY32 pe;
	pe.dwSize = sizeof(pe);

	BOOL bOk = Process32First(hSnapshot, &pe);

	for (; bOk; bOk = Process32Next(hSnapshot, &pe))
	{
		if (processName == pe.szExeFile)
		{
			ProcessId = pe.th32ProcessID;
		}
	}

	::CloseHandle(hSnapshot);

	return ProcessId;
}

DWORD GetGameBase()
{
	DWORD dwProcessId = GetGameProcessId("Game.exe");
	HMODULE base = GetProcessModuleHandle(dwProcessId, "Game.exe");
	return (DWORD)base;
}

bool SplitCStringArr(CString expression, CString delimiter, CStringArray* resultArr)
{
	int pos = 0;
	int pre_pos = 0;
	if (expression.IsEmpty()) {
		return false;
	}
	/*pos = expression.Find(delimiter, (pos + 1));*/
	while (true)
	{
		pos = expression.Find(delimiter, 0);
		if (-1 == pos) {
			if (!expression.IsEmpty()) {
				(*resultArr).Add(expression);
				TRACE("�ָ���:%s\n", expression);
			}
			break;
		}
		else {
			if (!expression.IsEmpty()) {
				(*resultArr).Add(expression.Mid(pre_pos, (pos - pre_pos)));
				TRACE("�ָ���:%s\n", expression.Mid(pre_pos, (pos - pre_pos)));
				expression.Delete(0, pos - pre_pos + 1);
			}
		}
	}
	return true;
}

//��������
BOOL CFunction::FUN_Questions()
{
	g_pHPInit->MySendGameInfo("��ʼ��������");
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	map<CString, CString> Quiz;
	Quiz.insert(pair<CString, CString>("��֪����һ������ʲô��ʱ�򣬾Ϳ���ȥ�鿴�ɽ�������ô��֪����δ򿪿ɽ�������", "����������Ϳ��Կ����ɽ�������"));
	Quiz.insert(pair< CString, CString>("�������ľŴ����ɶ������������ˣ���֪���ĸ����ɲ����ھŴ�������", "��ɽ��"));
	Quiz.insert(pair< CString, CString>("���֪������Ƕ��ſɵ������", "����ɽ"));
	Quiz.insert(pair< CString, CString>("���ڿ���ʹ������ӽǹ��ͷ羰�ˣ���ô��֪����������Դ����������", "ϵͳѡ���е���ʾ����"));
	Quiz.insert(pair< CString, CString>("�㾭��������վ�����ڸ���֮�䣬��ô��֪��ʹ����վ�շ���", "ȥ�Ķ����շ�"));
	Quiz.insert(pair< CString, CString>("���֪����������ſɵ������", "����"));
	Quiz.insert(pair< CString, CString>("���з����������Ϸ�����ת����Ϸ������", "��ס����Ҽ��϶����"));
	Quiz.insert(pair< CString, CString>("Я�����������������Ƶģ�����ͨ����������ȼ�����Ԫ������������Я��λ����η�����������ޣ�", "�㿪���޽��棬�������"));
	Quiz.insert(pair< CString, CString>("��֪��������½ǵĹ�������ĵڶ���ͼ���ʹ�ÿ�ݼ�Alt+S�򿪵���ʲô������", "���ܽ���"));
	Quiz.insert(pair< CString, CString>("��Ȼ�������ˡ������˲��������磬��֪������С˵������˭��", "��ӹ"));
	Quiz.insert(pair< CString, CString>("��·���ۣ�������Ϸ�п����Զ����ߣ������ɵģ���֪������������", "������������沢��ס"));
	Quiz.insert(pair< CString, CString>("��˵��ͨ���Ž�δ������ô�⡶�����˲����Ĺ����Ƿ������ĸ�������", "�γ�"));
	Quiz.insert(pair< CString, CString>("��������ĺð��֣���ô���˵����Ļ���½ǹ������ĵ�����ͼ�꣬��������ʲô��ݼ������޽����أ�", "Alt+X"));
	Quiz.insert(pair< CString, CString>("��Ҫ�������飬��������˵�����Ͻ�С��ͼ����һ����N���İ�ť����ʲô����", "�ǰ���תȡ���򱱵��ӽǰ�"));
	Quiz.insert(pair< CString, CString>("���ǵ�������ʹ�á������еĵ��߰ɣ�", "�򿪱������Ҽ������Ʒ"));
	Quiz.insert(pair< CString, CString>("��û����������˵�����ɣ���֪������˵��һ�����������", "����Ļ���½ǵ�Ц������"));
	Quiz.insert(pair< CString, CString>("��ϸ������������Ļ��ʲô�ط���������ĵȼ���", "���Ͻ��ҵ�ͷ���Ա���һ������"));
	Quiz.insert(pair< CString, CString>("���ڽ���Ư������ʲô���üǵã�����ǵ��������ϰ���˭��", "����"));
	Quiz.insert(pair< CString, CString>("��֪����������ǰ�����صĴ������վ�ϰ���˭��", "�޷��"));
	Quiz.insert(pair< CString, CString>("���ǵõ�һ�����㷢�����������ָ����˭��", "����ʦ"));
	Quiz.insert(pair< CString, CString>("���޵ĵȼ��ǿ����������ģ���֪����ô�������������޵ĵȼ���", "����һ���ȥ���"));
	Quiz.insert(pair< CString, CString>("��Ļ���Ͻ�ͷ���Աߵĺ�ɫ������ʾʲô��˼����", "�ҵ�Ѫ����������Ϊ���ʱ���Ҿ�ȥ�ظ���"));
	Quiz.insert(pair< CString, CString>("���������˲�����������ɱ�֣�����ʲô�ô���", "������⾭��"));
	Quiz.insert(pair< CString, CString>("�ò��������Լ�����ô�����İɣ����ƽ�ɫ�ƶ���Ҫ�������ĸ�������", "���"));
	Quiz.insert(pair< CString, CString>("�Ҽ����������ң������˵����������Ϊ���ѡ������ô����", "�ӶԷ�Ϊ����"));
	Quiz.insert(pair< CString, CString>("��֪����Ҫӵ��һ�����������Ҫ��Щ�����أ�", "ֻҪװ������Ϳ���"));
	Quiz.insert(pair< CString, CString>("һ�������Ĺ��ܣ����ǲ鿴�������еĵ�ͼ����֪�������鿴��", "��TAB��"));
	Quiz.insert(pair< CString, CString>("���������˲���Ϸ�У����ӵ�����������Ͻ�С��ͼ�ϻ�����ʾ��������ʲô�����ָ������Ҫ�ҵ����أ�", "��ɫСԲ��"));
	Quiz.insert(pair< CString, CString>("������ʱ�����롰#���֡�����ʲô��", "���������ӷ�"));
	Quiz.insert(pair< CString, CString>("���롰#G���֡����ֵ���ɫ����ʲô��ɫ��", "��ɫ"));
	Quiz.insert(pair< CString, CString>("��Ϸ��С��ͼ��������Ļ��ʲôλ�ã�", "��Ļ���Ͻ�"));
	Quiz.insert(pair< CString, CString>("���Ҳ��һ������ʹ�õļ��ܣ���ôʹ��Alt+S�򿪼���������������ҵ��أ�", "��ͨ����"));
	Quiz.insert(pair< CString, CString>("���潭������֪������ɫû�У�֪�������������е�������ʲô��", "����"));

	CString Text;
	CString Question;
	CString Answer;
	map<CString, CString>::iterator iter;

Bagin:
	Text = g_pMsg->msg_getstring("MyMonCanCommitString", "setmetatable(_G, {__index = Quiz_Env});MyMonCanCommitString = Quiz_Button_1:GetText()").c_str();
	if ("��ʼ����" == Text) {
		g_pMsg->msg_dostring("setmetatable(_G, {__index = Quiz_Env});Quiz_Button_Clicked(1);");
		Sleep(500);
	}

	Question = g_pMsg->msg_getstring("MyMonCanCommitString", "setmetatable(_G, {__index = Quiz_Env});MyMonCanCommitString = Quiz_Text:GetText()").c_str();
	Sleep(500);
	for (iter = Quiz.begin(); iter != Quiz.end(); iter++)
	{
		//dbgPrint("��ǰ���⣺%s,�𰸣�%s", iter->first, iter->second);
		if (Question.Find(iter->first) > -1) {
			Answer = iter->second;
		}
	}
	if (Question.Find("����") > -1)
	{
		g_pMsg->msg_dostring("setmetatable(_G, {__index = Quiz_Env});Quiz_Button_Clicked(1);");
		Sleep(500);
		goto Bagin;
	}
	dbgPrint("��ǰ���⣺%s,�𰸣�%s", Question, Answer);
	Text = g_pMsg->msg_getstring("MyMonCanCommitString", "setmetatable(_G, {__index = Quiz_Env});MyMonCanCommitString = Quiz_Button_1:GetText()").c_str();
	Sleep(500);
	if (Answer == Text)
	{
		g_pMsg->msg_dostring("setmetatable(_G, {__index = Quiz_Env});Quiz_Button_Clicked(1);");
		Sleep(500);
	}
	Text = g_pMsg->msg_getstring("MyMonCanCommitString", "setmetatable(_G, {__index = Quiz_Env});MyMonCanCommitString = Quiz_Button_2:GetText()").c_str();
	Sleep(500);
	if (Answer == Text)
	{
		g_pMsg->msg_dostring("setmetatable(_G, {__index = Quiz_Env});Quiz_Button_Clicked(2);");
		Sleep(500);
	}

	Text = g_pMsg->msg_getstring("MyMonCanCommitString", "setmetatable(_G, {__index = Quiz_Env});MyMonCanCommitString = Quiz_Button_3:GetText()").c_str();
	Sleep(500);
	if (Answer == Text)
	{
		g_pMsg->msg_dostring("setmetatable(_G, {__index = Quiz_Env});Quiz_Button_Clicked(3);");
		Sleep(500);
	}
	g_pHPInit->MySendGameInfo("������������");
	return TRUE;
}

BOOL CFunction::FUN_IsQuitRelative()
{
	CString text = g_pMsg->msg_getstring("MyMonCanCommitString", "setmetatable(_G, {__index = QuitRelative_Env}); MyMonCanCommitString = QuitRelative_Text:GetText()").c_str();
	dbgPrint("FUN_IsQuitRelative = %s", text);
	if (text.Find("������������") != -1)
	{
		return TRUE;
	}
	return FALSE;
}

// �������
//�жϵ�ǰ����
BOOL CFunction::FUN_GetPlayerMission(_tstring taskName)
{
	dbgPrint("FUN_GetPlayerMission taskName = %s", taskName.c_str());
	TAsmTask tAsmTask;
	auto ATask = g_pAsmTask->AsmGetTaskData();//��ȡ��ɫ�ܱ߷�Χ�ڵĹ���
	for (auto t : ATask)
	{
		if (((CString)t.szName).Find(taskName.c_str(), 0) != -1) {
			dbgPrint("FUN_GetPlayerMission �ҵ�����");
			return TRUE;
		}
	}
	return FALSE;
}

float CFunction::FUN_MaximumRange(TAsmSkill tAsmSkill)
{
	float fRange = 0;
	if (tAsmSkill.fMax == -1)//����Χ��Ⱥ�弼��
	{
		fRange = tAsmSkill.fAngle;
	}
	else
	{
		fRange = tAsmSkill.fMax;
	}

	return fRange;
}



//BOOL CFunction::FUN_RunToTarget(float fx, float fy, float dis)//���ͼѰ·
//{
//	bool bMov = false;
//	DWORD dwWaitTime = GetTickCount64();//��һ�μ�¼ʱ�䣬�����ʱ�Ƿ��ܶ�״̬
//	DWORD dwEndTime = GetTickCount64();//��һ�μ�¼ʱ�䣬�����ʱ�Ƿ��ܶ�״̬
//	float fOldX = -1, fOldY = -1;
//	float fCurDistance = FUN_GetDistance(fx, fy);
//	if (fCurDistance > 30)
//	{
//		g_pMsg->CallInOutRide(1);
//	}
//
//	while (g_pMe->bRun)
//	{
//		DWORD dwCurTime = GetTickCount64();
//		
//		if ((dwCurTime - dwWaitTime) > (180 * 1000)) //���30�뻹û�����˳�����Ѱ·
//		{
//			dbgPrint("��ͼ��Ѱ·3����û�ߵ�Ŀ�꣬������������δ֪�ص�");
//			return FALSE;
//		}
//		TAsmMap role_map = role_curmap();
//		if (role_map.szName == (CString)"�ظ�")	//�ж��Ƿ��ڵظ���
//		{
//			FUN_DeathResurrection();
//			return TRUE;
//		}
//
//		TAsmRoleInfo tRoleInfo = g_pAsmRole->GetRoleInfo();
//		if (tRoleInfo.nState != 2)
//		{
//			if (!bMov)
//			{
//				g_pMsg->msg_dostring("AutoRunToTarget(%d, %d)", (int)fx, (int)fy);
//				Sleep(1000);
//				FUN_AutoMove();
//				bMov = true;
//				Sleep(1000);
//				auto APos = g_pAsmRole->GetPos();
//				fOldX = APos.fx;
//				fOldY = APos.fy;
//			}
//		}
//		dbgPrint("Ѱ·��ʱ%d", (dwCurTime - dwEndTime));
//		if ((dwCurTime - dwEndTime) > (3 * 1000)) //���30�뻹û�����˳�����Ѱ·
//		{
//			auto APos = g_pAsmRole->GetPos();
//			if ((APos.fx <= fOldX || APos.fx >= fOldX) &&
//				(APos.fy <= fOldY || APos.fy >= fOldY))
//			{
//				bMov = false;
//				fOldX = APos.fx;
//				fOldY = APos.fy;
//			}
//			dwEndTime = GetTickCount64();
//		}
//
//		fCurDistance = FUN_GetDistance(fx, fy);
//		CString strTemp;
//		strTemp.Format("��ͼ���ƶ� ����Ŀ��=%f", fCurDistance);
//		g_pHPInit->MySendGameInfo(strTemp);
//		if (fCurDistance < dis)//��ǰ�ľ���С��2���Բ���Ŀ����
//		{
//			dbgPrint("����Ŀ�ĵ�");
//			g_pMsg->CallInOutRide(0);
//			return TRUE;
//		}
//		Sleep(1000);
//	}
//	return FALSE;
//}

BOOL CFunction::FUN_RunToTargetEx(float fx, float fy, int SceneId, float dis)//���ͼѰ·
{
	bool bMov = false;
	TAsmMap tAsmMap = role_curmap();
	float fOldX = -1, fOldY = -1;
	DWORD dwWaitTime = GetTickCount64();//��һ�μ�¼ʱ�䣬�����ʱ�Ƿ��ܶ�״̬
	DWORD dwEndTime = GetTickCount64();//��һ�μ�¼ʱ�䣬�����ʱ�Ƿ��ܶ�״̬

	while (g_pMe->bRun)
	{

		DWORD dwCurTime = GetTickCount64();
		if ((dwCurTime - dwWaitTime) > (5 * 60 * 1000)) //���10���ӻ�û�����˳�����Ѱ·
		{
			dbgPrint("��ͼѰ·10����û�ߵ�Ŀ�꣬������������δ֪�ص�");
			return FALSE;
		}

		if (tAsmMap.szName == (CString)"�ظ�")	//�ж��Ƿ��ڵظ���
		{
			FUN_DeathResurrection();
			return TRUE;
		}

		TAsmRoleInfo tRoleInfo = g_pAsmRole->GetRoleInfo();
		if (tRoleInfo.nState != 2)
		{
			if (!bMov)
			{
				if (SceneId == tAsmMap.nSceneId || SceneId == -1)
				{
					float fCurDistance = FUN_GetDistance(fx, fy);
					if (fCurDistance > 30)
					{
						g_pMsg->CallInOutRide(1);
					}

					g_pMsg->msg_dostring("AutoRunToTarget(%d, %d)", (int)fx, (int)fy);
				}
				else if (SceneId != tAsmMap.nSceneId)
				{
					dbgPrint("��ʼ��ͼѰ· %f, %f, %d", fx, fy, SceneId);
					g_pMsg->CallInOutRide(1);

					g_pMsg->msg_dostring("AutoRunToTargetEx(%d, %d, tonumber(%d))", (int)fx, (int)fy, SceneId);
				}
				Sleep(1000);
				FUN_AutoMove();
				bMov = true;
				auto APos = g_pAsmRole->GetPos();
				fOldX = APos.fx;
				fOldY = APos.fy;
			}
		}
		//dbgPrint("Ѱ·��ʱ%d", (dwCurTime - dwEndTime));
		if ((dwCurTime - dwEndTime) > (5 * 1000)) //���30�뻹û�����˳�����Ѱ·
		{
			auto APos = g_pAsmRole->GetPos();
			if ((APos.fx <= fOldX || APos.fx >= fOldX) &&
				(APos.fy <= fOldY || APos.fy >= fOldY))
			{
				bMov = false;
				fOldX = APos.fx;
				fOldY = APos.fy;
			}
			dwEndTime = GetTickCount64();
		}

		float fCurDistance = FUN_GetDistance(fx, fy);
		CString strTemp;
		strTemp.Format("��%s(%d)�ƶ���%s(%d) ����Ŀ��=%f", tAsmMap.szName, tAsmMap.nSceneId, CString(FUN_GetSceneName(SceneId).c_str()), SceneId, fCurDistance);
		g_pHPInit->MySendGameInfo(strTemp);
		if (fCurDistance < dis)//��ǰ�ľ���С��2���Բ���Ŀ����
		{
			dbgPrint("����Ŀ�ĵ�");
			//g_pMsg->CallInOutRide(0);
			return TRUE;
		}

		Sleep(1000);
	}
	return FALSE;
}

float CFunction::FUN_GetDistance(float x2, float y2)//��ȡ��Ŀ��ľ���
{
	auto APos = g_pAsmRole->GetPos();
	if (APos.fx <= 0 || APos.fy <= 0)
	{
		return 1000;
	}
	//��ȡ���������쳣��־(APos, return 0.0f)
	float fdistance = sqrt(pow(APos.fx - x2, 2) + pow(APos.fy - y2, 2));

	return fdistance;
}

int  CFunction::FUN_GetSceneID(const char* szSceneName)//ͨ�����ƻ�ȡ��ͼ����ID
{
	size_t Count = sizeof(gScene_Array) / sizeof(gScene_Array[0]);
	for (size_t i = 0; i < Count; i++)
	{
		if (strcmp(gScene_Array[i].szName, szSceneName) == 0)//������ȫƥ��
		{
			return gScene_Array[i].nSceneId;
		}
	}

	return -1;//û�оͷ���-1
}

_tstring CFunction::FUN_GetSceneName(int nSceneId)	//ͨ��ID��ȡ������
{
	size_t Count = sizeof(gScene_Array) / sizeof(gScene_Array[0]);
	for (size_t i = 0; i < Count; i++)
	{
		if (gScene_Array[i].nSceneId == nSceneId)//������ȫƥ��
		{
			return _tstring(gScene_Array[i].szName);
		}
	}

	return "";//û�оͷ��ؿ�
}

TAsmMonster CFunction::FUN_GetCatchMonsterByName(_tstring MonsterName)
{
	TAsmMonster tAsmMonster;
	float fdistance = 0.0f;
	auto AMonster = g_pAsmMonster->AsmGetMonsterData();//��ȡ��ɫ�ܱ߷�Χ�ڵĹ���
	for (auto m : AMonster)
	{
		dbgPrint("m.szName=%s nAttack=%dm m.nCatch=%d", m.szName, m.nAttack, m.nCatch);
		if (CString(m.szName).Find(MonsterName.c_str(), 0) != -1)
		{
			if (m.nAttack == 1 && m.nCatch == -1)
			{
				dbgPrint("ץ������");
				if (FUN_MinDistanceObject(&fdistance, m.fMonsterX, m.fMonsterY))//�������Ĺ���
					tAsmMonster = m;
			}
		}
	}

	return tAsmMonster;
}

TAsmMonster CFunction::FUN_GetMonsterByName(_tstring MonsterName, int nMonsterType, int nKillType)//���ƻ�ȡ�����Լ�����Ĺ���
{
	TAsmMonster tAsmMonster;
	float fdistance = 0.0f;
	auto AMonster = g_pAsmMonster->AsmGetMonsterData();//��ȡ��ɫ�ܱ߷�Χ�ڵĹ���
	for (auto m : AMonster)
	{
		if (((CString)m.szName).Find(MonsterName.c_str(), 0) != -1)
		{
			if (m.nAttack == 0)
			{
				switch (nMonsterType)
				{
				case 0://����ɱ��
				{
					if (FUN_MinDistanceObject(&fdistance, m.fMonsterX, m.fMonsterY))//�������Ĺ���
					{
						tAsmMonster = m;
					}
					break;
				}
				default:
					break;
				}
			}
		}
		Sleep(100);
	}
	return tAsmMonster;
}

void CFunction::FUN_MisKillMonsterByName(_tstring MonsterName)
{
	TAsmSkill tAsmSkill;
	int nMenPaiId = g_pAsmRole->GetMenPaiForId();
	if (nMenPaiId == 8)	//��ң��
	{
		tAsmSkill.nSkillId = 521;
		tAsmSkill.fMax = 15.0;
	}
	else {
		tAsmSkill.nSkillId = 0;
		tAsmSkill.fMax = 2.0;
	}
	//�����ս���
	g_pAsmPet->PetGoFight(0);

	auto AMonster = g_pAsmMonster->AsmGetMonsterData();//��ȡ��ɫ�ܱ߷�Χ�ڵĹ���
	for (auto m : AMonster)
	{
		if (((CString)m.szName).Find(MonsterName.c_str()) != -1)
		{
			if (m.nAttack == 0)
			{
				for (size_t i = 0; i < 60; i++)	//ѭ��60�δ��1���ӣ������û����͵�����ѡ��
				{
					float nHp = g_pAsmMonster->GetHp(m);
					if (nHp <= 0) {
						return;
					}
					FUN_UseMovAttackSkill(tAsmSkill, m);
					CString strTemp;
					strTemp.Format("Ŀ�����=%s ʣ��Ѫ��=%f", m.szName, nHp);
					g_pHPInit->MySendGameInfo(strTemp);
					dbgPrint(strTemp);
					if (g_pAsmRole->GetHpPercent() <= 0) {
						dbgPrint("��ɫ����");
						return;
					}

					Sleep(500);
				}
			}
		}
		Sleep(100);
	}	
}

bool  CFunction::FUN_MinDistanceObject(float* fdistance, float fx, float fy)	//true�������
{
	//��ǰ�����Ƿ�������Ĺ�
	float fnext = FUN_GetDistance(fx, fy);

	if (*fdistance == 0.0f)//��һ�β���
	{
		*fdistance = fnext;
		return true;
	}
	else if (*fdistance > fnext)//����һ�����Ƚ�
	{
		*fdistance = fnext;
		return true;
	}

	return false;
}

//�����ܹ���
void CFunction::FUN_UseMovAttackSkill(TAsmSkill tAsmSkill, TAsmMonster tAsmMonster)//��ʼ�ͷŹ�������
{
	if (FUN_MovToMonster(tAsmMonster, tAsmSkill))//�����������
	{
		g_pAsmSkill->AsmUseSkillCall(tAsmMonster.nMonsterId, tAsmSkill.nSkillId);//ʹ�ô˼���
	}
}

bool CFunction::FUN_MovToMonster(TAsmMonster tAsmMonster, TAsmSkill tAsmSkill)//�ߵ�������̷�Χ��
{
	float fr = FUN_MaximumRange(tAsmSkill);//��ȡ���
	if (FUN_GetDistance(tAsmMonster.fMonsterX, tAsmMonster.fMonsterY) < fr + 2)//��̷�Χ����
	{
		return true;
	}

	FUN_RunTo(tAsmMonster.fMonsterX, tAsmMonster.fMonsterY, 30);

	for (int i = 0; i < 10 && g_pMe->bRun; i++)
	{
		if (FUN_GetDistance(tAsmMonster.fMonsterX, tAsmMonster.fMonsterY) < fr + 2)//��̷�Χ��
			return true;
		Sleep(300);
	}

	return false;//û���ߵ������
}

bool CFunction::FUN_IsMonsterDie(TAsmMonster* ptAsmMonster)//falseΪ����
{
	try
	{
		auto AMonster = g_pAsmMonster->AsmGetMonsterData();//��ȡ��ɫ�ܱ߷�Χ�ڵĹ���
		for (auto m : AMonster)
		{
			if ((*ptAsmMonster).nMonsterId == m.nMonsterId)
			{
				if (m.fMonsterHp > 0)
				{
					*ptAsmMonster = m;//ȡ�ù��ﵱǰ������,��ʱ�򣬻ᳬ�����룬Ҫȡ��������Ϣ�ж�
					return true;
				}
			}
		}
	}
	catch (...)
	{
		dbgPrint(__FUNCTION__);
	}
	return false;
}
//ץ��
bool CFunction::FUN_MisUseSkillCatchMonster(_tstring monsterName)
{
	_tstring strTemp = "����ץ��" + monsterName;
	g_pHPInit->MySendGameInfo(strTemp.c_str());
	TAsmSkill tAsmSkill;
	tAsmSkill.nSkillId = 1;
	TAsmMonster tAsmMonster = FUN_GetCatchMonsterByName(monsterName);
	if (tAsmMonster.fMonsterX == 0 || tAsmMonster.fMonsterY == 0)
	{
		dbgPrint("δ�ҵ�����");
		return false;
	}
	
	dbgPrint("�ƶ� fMonsterX=%f, fMonsterY=%f", tAsmMonster.fMonsterX, tAsmMonster.fMonsterY);
	if (FUN_MovToMonster(tAsmMonster, tAsmSkill))//�����������
	{
		dbgPrint("���ﷶΧ tAsmMonster.szName=%s nType=%d, fMonsterX=%f, fMonsterY=%f", tAsmMonster.szName, tAsmMonster.nType, tAsmMonster.fMonsterX, tAsmMonster.fMonsterY);
		g_pAsmSkill->AsmUseSkillCall(tAsmMonster.nMonsterId, tAsmSkill.nSkillId);//ʹ�ô˼���
		Sleep(20000);//�ӳ�20��
		return true;
	}

	return false;
}

int CFunction::FUN_GetItemIdxForName(_tstring strName)
{
	for (int i = 0; i < 100; i++)
	{
		CString itemName = g_pMsg->msg_getstring("ItemName", " ItemName = PlayerPackage:GetBagItemName(%d)", i).c_str();
		if (itemName.Find(strName.c_str(), 0) != -1) {
			return i;
		}
	}
	return -1;
}

int CFunction::FUN_GetBagItemBlankNum()
{
	int nCount = 0;
	for (int i = 0; i < 100; i++)
	{
		CString itemName = g_pMsg->msg_getstring("ItemName", " ItemName = PlayerPackage:GetBagItemName(%d)", i).c_str();
		if (itemName != "") {
			nCount++;
		}
	}
	return nCount;
}

bool CFunction::FUN_IsAvailableTask(_tstring taskName)
{
	_tstring strInfo = g_pMsg->msg_getstring("MyMonOutString", "MyMonOutString = MissionOutlinetext(\"%s\")", taskName.c_str());
	
	if (!strInfo.empty())
	{
		dbgPrint("FUN_IsAvailableTask %s �ڿɽ������б���", taskName.c_str());
		return TRUE;
	}
	dbgPrint("FUN_IsAvailableTask %s ���ڿɽ������б���", taskName.c_str());
	return FALSE;
}

VString CFunction::FUN_GetTaskInfo(_tstring taskName)
{
	VString tempStr;
	_tstring strInfo = g_pMsg->msg_getstring("GetTaskInfoString", "GetTaskInfoString = MissionTaskInfo(\"%s\")", taskName.c_str());
	dbgPrint("FUN_GetTaskInfo = %s", strInfo.c_str());
	if (!strInfo.empty())
	{
		tempStr = stringIntercept(strInfo, "{", 9, "}");
	}
	return tempStr;
}

_tstring CFunction::FUN_GetMissionTaskKillMonsterInfo(_tstring taskName)
{
	_tstring strInfo = g_pMsg->msg_getstring("GetMonsterInfo", "GetMonsterInfo = MissionTaskKillMonsterInfo(\"%s\")", taskName.c_str());
	dbgPrint("FUN_GetMissionTaskKillMonsterInfo = %s", strInfo.c_str());
	return strInfo;
}

_tstring CFunction::FUN_GetAvailableTaskInfo(_tstring taskName)
{
	_tstring strInfo = g_pMsg->msg_getstring("MyMonOutString", "MyMonOutString = MissionOutlinetext(\"%s\")", taskName.c_str());
	dbgPrint("FUN_GetAvailableTaskInfo taskName = %s  strInfo=%s", taskName.c_str(), strInfo.c_str());
	return strInfo;
}

_tstring CFunction::FUN_GetMissionFinishInfo(_tstring taskName)
{
	_tstring strInfo = g_pMsg->msg_getstring("MissionFinishString", "MissionFinishString = GetMissionFinishInfo(\"%s\")", taskName.c_str());
	dbgPrint("FUN_GetMissionFinishInfo taskName = %s  strInfo=%s", taskName.c_str(), strInfo.c_str());
	return strInfo;
}

void CFunction::FUN_Levelup()
{
	dbgPrint("FUN_Levelup");
	g_pMsg->msg_dostring("setmetatable(_G, {__index = ActionSkill_Env}); SelfEquip_AskLevelup();");
}

bool CFunction::FUN_IsSkillName(_tstring taskName)
{
	return g_pAsmSkill->AsmHaveMasterSkill(taskName);
}

bool CFunction::FUN_SkillUseIDPos(int nSkillId, float x, float y)
{
	//�ж� ��������״̬���ܴ�֡�
	g_pMsg->CallInOutRide(0);
	g_pAsmSkill->AsmUseSkillCallByPoint(nSkillId, x, y);
	return 1;
}

// �ѽ��б����Ƿ��д�����
bool CFunction::FUN_IsTask(_tstring taskName)
{
	_tstring finishInfo = FUN_GetMissionFinishInfo(taskName);
	if (!finishInfo.empty())
	{
		auto  vString = UserSubMonsterName(finishInfo, _T('|'));
		if (vString.size() >= 6)
		{
			if (stoi(vString[0]) != -1) {
				dbgPrint("%s���ѽ������б���", taskName.c_str());
				return TRUE;
			}
		}
	}
	dbgPrint("%s�����ѽ������б���", taskName.c_str());
	return FALSE;
}

int CFunction::FUN_IsTaskSuccess(_tstring taskName)
{
	int finished = -1;

	// ��ȡ��ɺ��ύ�������Ϣ
	_tstring finishInfo = FUN_GetMissionFinishInfo(taskName);
	if (!finishInfo.empty())
	{
		auto  vString = UserSubMonsterName(finishInfo, _T('|'));
		if (vString.size() >= 6)
		{
			finished = stoi(vString[0]);
			return finished;
		}
	}

	return finished;
}

bool CFunction::FUN_TaskAccep(_tstring taskName, _tstring npcName, _tstring sceneName, float x, float y)
{
	if (!g_pMe->bRun)
	{
		return false;
	}
	CString strTemp;
	strTemp.Format("��ȡ����:%s", taskName.c_str());
	dbgPrint(strTemp);
	g_pHPInit->MySendGameInfo(strTemp);

	int sceneId = FUN_GetSceneID(sceneName.c_str());

	for (int i = 0; i < 5; i++)
	{
		if (CString(npcName.c_str()).Find("���") != -1)
		{
			x = 199;
			y = 68;
		}
		if (FUN_RunToTargetEx(x, y, sceneId))
		{
			g_pMsg->SetAutoRunTargetNPCName(npcName.c_str()); //�ƶ���NPC

			if (g_pMsg->IsNpcDialog())
			{
				g_pMsg->msg_dostring("ClickMission(\"%s\")", taskName.c_str());//msg��lua��Ϣ����
				Sleep(500);

				g_pMsg->MissionQuestAccept_Clicked();
				Sleep(200);
				return TRUE;
			}
			else {
				dbgPrint("�Ի���û��ʾ");
			}
		}
		Sleep(1000);
	}

	return FALSE;
}

// ִ���������ʹ�ý�ȡ��չ�����Ժϲ��¡�
bool CFunction::FUN_TaskAccepEx(_tstring taskName)
{
	if (!g_pMe->bRun)
	{
		return false;
	}
	//���������
	FUN_Levelup();

	dbgPrint("FUN_TaskAccepEx=%s", taskName.c_str());

	//�ж��Ƿ����
	//���ж��Ƿ��ڿɽ��б���
	_tstring strInfo = FUN_GetAvailableTaskInfo(taskName);
	if (!strInfo.empty())
	{
		_tstring tmp_npcName;
		_tstring tmp_sceneName;
		float	tmp_posX = 0.0;
		float	tmp_posY = 0.0;

		auto  vString = UserSubMonsterName(strInfo, _T('|'));
		if (vString.size() >= 4)
		{
			tmp_npcName = vString[0];
			tmp_sceneName = vString[1];
			tmp_posX = stof(vString[2]);
			tmp_posY = stof(vString[3]);

			dbgPrint("FUN_TaskAccepEx �ڿɽ��б��ȡ������ ������=%s tmp_npcName=%s tmp_sceneName=%s tmp_posX=%f tmp_posY=%f",
				taskName.c_str(), tmp_npcName.c_str(), tmp_sceneName.c_str(), tmp_posX, tmp_posY);

			FUN_TaskAccep(taskName, tmp_npcName, tmp_sceneName, tmp_posX, tmp_posY);	// ������������ִ��
			return TRUE;
		
		}
	}

	return FALSE;
}

bool CFunction::FUN_ExecuteTask(_tstring taskName)
{
	if (!g_pMe->bRun)
	{
		return false;
	}

	FUN_TaskAccepEx(taskName);	//��ȡ����

	return FUN_TaskSuccessEx(taskName, 1);	//ִ������
}

//��Ҫ��ȷ�������Ѿ���ȡ
bool CFunction::FUN_ExecuteTaskKill(_tstring taskName, int nType, int nMonsterType, _tstring monsterName, bool bPickUp)
{
	if (!g_pMe->bRun)
	{
		return false;
	}

	CString strTemp;
	strTemp.Format("ִ��ɱ������:%s", taskName.c_str());
	g_pHPInit->MySendGameInfo(strTemp);

	_tstring tmp_npcName;
	_tstring tmp_sceneName;
	int		tmp_sceneId;
	float	tmp_posX = 0.0;
	float	tmp_posY = 0.0;
	int		finished = -1;

	//�ж��������û��ȡ�ͽ�ȡ
	FUN_TaskAccepEx(taskName);	//��ȡ����

	// ��ȡ��ɺ��ύ�������Ϣ
	_tstring finishInfo = FUN_GetMissionFinishInfo(taskName);
	if (!finishInfo.empty())
	{
		auto  vString = UserSubMonsterName(finishInfo, _T('|'));
		if (vString.size() >= 6)
		{
			finished = stoi(vString[0]);
			tmp_npcName = vString[1];
			tmp_sceneName = vString[2];
			tmp_sceneId = stoi(vString[3]);
			tmp_posX = stoi(vString[4]);
			tmp_posY = stoi(vString[5]);
		}
	}
	if (finished == -1)	//����-1��ʾ�ѽ������б���û������
	{
		dbgPrint("δִ��ɱ������");
		return FALSE;
	}

	if (finished == 1)
	{
		//����ò���е����⣬����Ի���npc��һ���������еĵ�һ���ڵ�
		FUN_TaskSuccessEx(taskName, 1);
		return TRUE;
	}

	//��ȡ��Ҫɱ�ù���
	if (monsterName.empty())
	{
		_tstring monsterInfos = FUN_GetMissionTaskKillMonsterInfo(taskName);
		if (!monsterInfos.empty())
		{
			auto  vString = UserSubMonsterName(monsterInfos, _T('|'));
			if (vString.size() > 0)
			{
				monsterName = vString[0];
			}
		}
	}
	dbgPrint("ִ��ɱ��--finished=%d monsterName=%s ���npc=%s, ��ɳ���=%s, �������x=%f,y=%f",
		finished, monsterName.c_str(), tmp_npcName.c_str(), tmp_sceneName.c_str(), tmp_posX, tmp_posY);

	// ��ȡ��Ҫɱ�ֵõط�
	VString taskInfos = FUN_GetTaskInfo(taskName);
	if (taskInfos.size() < 1)
	{
		return FALSE;
	}
	if (taskInfos.size() >= nType)
	{
		_tstring taskInfo = taskInfos[nType - 1];

		dbgPrint("taskInfo=%s", taskInfo.c_str());
		auto vString = UserSubMonsterName(taskInfo, _T(','));
		if (vString.size() > 3)
		{
			tmp_posX = stof(vString[0]);
			tmp_posY = stof(vString[1]);
			tmp_sceneId = stoi(vString[2]);

			dbgPrint("ִ��ɱ��--��Ҫɱ�Ĺ���=%s, ����=%d, ����x=%f, y=%f",
				monsterName.c_str(), tmp_sceneId, tmp_posX, tmp_posY);

			float fCurDistance = 1.0;
			bool bMov = true;
			while (bMov)
			{
				if (!g_pMe->bRun)
				{
					return false;
				}
				if (FUN_RunToTargetEx(tmp_posX, tmp_posY, tmp_sceneId))
				{
					float fCurDistance = FUN_GetDistance(tmp_posX, tmp_posY);//��¼��ǰ�ľ���
					if (fCurDistance < 10)
					{
						bMov = false;
						break;
					}
				}
				Sleep(1000);
			}

			//����Ŀ�ĵ�,��ʼ���
			dbgPrint("��ʼ���");
			while (true)
			{
				if (!g_pMe->bRun)
				{
					return false;
				}

				if (g_pAsmRole->GetHpPercent() <= 0) {
					dbgPrint("��ɫ����");
					FUN_DeathResurrection();
					return true;
				}

				if (FUN_IsTaskSuccess(taskName) == 1)
				{
					//�����ж�������
					FUN_TaskSuccessEx(taskName, 1);	// ��һ�����ޣ�������Ҫȡ���ĸ����� ���һ������
					return TRUE;
				}

				FUN_MisKillMonsterByName(monsterName);
				if (bPickUp) //�Ƿ�ʰȡ
				{
					Sleep(1000);
					FUN_PickUp();
				}
				Sleep(100);
			}
		}
	}

	return FALSE;
}

void CFunction::FUN_TaskSuccess(_tstring taskName, _tstring npcName, _tstring sceneName, float x, float y)
{
	if (!g_pMe->bRun)
	{
		return;
	}

	CString strTemp;
	strTemp.Format("�������:%s", taskName.c_str());
	g_pHPInit->MySendGameInfo(strTemp);

	int sceneId = FUN_GetSceneID(sceneName.c_str());

	for (int i = 0; i < 5; i++)
	{
		if (CString(npcName.c_str()).Find("���") != -1)
		{
			x = 199;
			y = 68;
		}
		if (FUN_RunToTargetEx(x, y, sceneId))
		{
			g_pMsg->SetAutoRunTargetNPCName(npcName.c_str()); //�ƶ���NPC

			if (g_pMsg->IsNpcDialog())
			{
				g_pMsg->msg_dostring("ClickMission(\"%s\")", taskName.c_str());//msg��lua��Ϣ����
				Sleep(500);

				g_pMsg->QuestFrameMissionContinue();	//NPC�Ի���ִ�м���
				Sleep(200);

				g_pMsg->QuestFrameMissionComplete(0);
				Sleep(200);

				g_pMsg->MissionQuestAccept_Clicked();
				Sleep(200);
				return;
			}
		}
		Sleep(1000);
	}

}

//����������NPC��ִ��nType �ǻ�ȡ���NPC��ʽ��ͬ �����ڵڼ������꣬��1��ʼ
bool CFunction::FUN_TaskSuccessEx(_tstring taskName, int nType)
{
	if (!g_pMe->bRun)
	{
		return false;
	}

	_tstring taskInfo = "";
	_tstring tmp_npcName;
	_tstring tmp_sceneName;
	int		tmp_sceneId = -1;
	float	tmp_posX;
	float	tmp_posY;
	int		finished = -1;

	// ��ȡ��ɺ��ύ�������Ϣ
	_tstring finishInfo = FUN_GetMissionFinishInfo(taskName);
	if (!finishInfo.empty())
	{
		auto  vString = UserSubMonsterName(finishInfo, _T('|'));
		if (vString.size() >= 6)
		{
			finished = stoi(vString[0]);
			tmp_npcName = vString[1];
			tmp_sceneName = vString[2];
			tmp_sceneId = stoi(vString[3]);
			tmp_posX = stoi(vString[4]);
			tmp_posY = stoi(vString[5]);
			if (finished == -1)
			{
				return FALSE;
			}
			FUN_TaskSuccess(taskName, tmp_npcName, tmp_sceneName, tmp_posX, tmp_posY);
			return TRUE;
		}
	}
	return FALSE;
}

void CFunction::FUN_SetAttackMonsterPoints(CString szPoints)
{
	_tstring szTemp = szPoints;
	auto  vString = UserSubMonsterName(szTemp, _T('|'));
	for (auto v : vString)
	{
		auto temp = UserSubMonsterName(v, _T(','));
		if (temp.size() == 2)
		{
			TUserPos tUserPos;
			tUserPos.nPosX = stoi(temp[0].c_str());
			tUserPos.nPosY = stoi(temp[1].c_str());
			g_pUser->vtKillPos.push_back(tUserPos);
		}
	}
}

void CFunction::FUN_AttackMonster(_tstring monsterName, _tstring sceneName, float x, float y)
{
	if (!g_pMe->bRun)
	{
		return;
	}
	int sceneId = FUN_GetSceneID(sceneName.c_str());
	if (FUN_RunToTargetEx(x, y, sceneId)) {
		TAsmMonster tAsmMonster = FUN_GetMonsterByName(monsterName.c_str(), 0, 1);
		if (tAsmMonster.nMonsterId != -1)
		{
			dbgPrint("FUN_KillMonsterByNameID %d", tAsmMonster.nMonsterId);
			FUN_UseSkillKillMonster(tAsmMonster);//��ʼ���ܴ��;
		}
	}
}

//��������
void CFunction::FUN_JoinMenPai(_tstring szMenPaiName)
{
	if (!g_pMe->bRun)
	{
		return;
	}

	dbgPrint("FUN_JoinMenPai");
	int menpai = g_pAsmRole->GetMenPaiForId();
	dbgPrint("FUN_JoinMenPai ��ǰ����=%d, ��������=%s", menpai, szMenPaiName.c_str());
	if (menpai == 9) //�ж��Ƿ���������
	{
		_tstring taskName = "�Ŵ�����";
		while (g_pMe->bRun) {
			if (FUN_IsTaskSuccess(taskName) == 1)
			{
				FUN_TaskSuccessEx(taskName, 1);
				return;
			}

			g_pMsg->CallInOutRide(1);

			int sceneId = FUN_GetSceneID("�貨��");
			float tmpPosX = 124;
			float tmpPosY = 144;
			if (FUN_RunToTargetEx(tmpPosX, tmpPosY, sceneId))
			{
				g_pMsg->SetAutoRunTargetNPCName("���Ǻ�"); //�ƶ���NPC

				g_pMsg->msg_dostring("ClickMission('��������')");//msg��lua��Ϣ����
				Sleep(1000);

				_tstring szTemp = "��ȷ��Ҫ����" + szMenPaiName;
				g_pMsg->msg_dostring("ClickMission(\"%s\")", szTemp.c_str());//msg��lua��Ϣ����
				Sleep(500);

				g_pMsg->QuestFrameMissionContinue();
				Sleep(200);

				g_pMsg->QuestFrameMissionComplete(0);
				Sleep(200);

				g_pMsg->MissionQuestAccept_Clicked();
				Sleep(200);
			}

			Sleep(500);
		}
	}
}

void CFunction::FUN_PickUp()
{
	VAsmGroundParcel vers = g_pGroundParcel->AsmGetGroundParcelData();
	for (TAsmGroundParcel vParcel : vers)
	{
		g_pGroundParcel->AsmOpenGroundParcelById(vParcel.nParcelId);
		Sleep(1000);
		g_pMsg->msg_dostring("setmetatable(_G, { __index = ActionSkill_Env }); LootPacket_Collect_Clicked();");//msg��lua��Ϣ����
	}
}

//ִ���Զ��ƶ��Ի���
int CFunction::FUN_AutoMove()
{
	if (!g_pMe->bRun)
	{
		return 0;
	}

	// �ر�����ָ��
	g_pMsg->msg_dostring("setmetatable(_G, { __index = FreshManIntro_Env }); FreshManIntro_Quit_Clicked();");
	Sleep(200);
	
	CString strTemp = "";
	strTemp = g_pMsg->msg_getstring("strTemp", "strTemp = getMessageBoxText();").c_str();
	dbgPrint("GetMessageBoxText strTemp=%s", strTemp);
	if (!strTemp.IsEmpty())
	{
		if (strTemp.Find("�Զ�Ѱ·") != -1)
		{
			g_pMsg->CallInOutRide(1);

			g_pMsg->msg_dostring("setmetatable(_G, {__index = MessageBox_Self_Env}); StartAutoMove();this:Hide()");
			Sleep(200);
			return 101;
		}
	}

	strTemp = g_pMsg->msg_getstring("strTemp", "strTemp = getAcceptBoxText();").c_str();
	dbgPrint("AcceptBox_Text strTemp=%s", strTemp);
	if (!strTemp.IsEmpty())
	{
		if (strTemp.Find("��ɱ��") != -1)
		{
			g_pMsg->msg_dostring("setmetatable(_G, { __index = AcceptBox_Env }); AcceptBox_Cancel_Clicked(1);");
			Sleep(200);
			g_pMsg->msg_dostring("setmetatable(_G, { __index = AcceptBox_Env }); AcceptBox_OnHide();");
			Sleep(200);
			return 0;
		}
	}
	return 0;
}

void CFunction::FUN_SkillUseID(int nSkillId)
{
	g_pMsg->CallInOutRide(0);
	Sleep(500);
	g_pAsmSkill->AsmUseSkillCall(-1, nSkillId);//ʹ�ô˼���
}

BOOL CFunction::FUN_RunTo(float fx, float fy, int nTime)//Ѱ·ָ��ʱ��Sleep(10)
{
	if (FUN_GetDistance(fx, fy) < 3)//�жϾ�����
		return TRUE;

	g_pMsg->msg_dostring("AutoRunToTarget(%d, %d)", (int)fx, (int)fy);
	for (int i = 0; i < nTime; i++)
	{
		if (FUN_GetDistance(fx, fy) < 3)//�жϾ�����
			return TRUE;
		Sleep(500);
	}
	return FALSE;
}

//��������
void CFunction::FUN_DeathResurrection()
{
	if (g_pAsmRole->GetHpPercent() > 0.1) {
		return;
	}

	g_pHPInit->MySendGameInfo("��ɫ����");
	for (int  i = 0; i < 6; i++)
	{
		int nTemp = g_pMsg->msg_getnumber("g_GetValue = IsDeathDialog()");
		Sleep(100);

		if (nTemp)
		{
			g_pMsg->msg_dostring("setmetatable(_G, { __index = Relive_Env }); Relive_Out_Fool();");
			Sleep(1000);

			g_pMsg->msg_dostring("setmetatable(_G, {__index = Relive_Env}); Relive_Out_Ghost();");
			Sleep(1000);
		}
		Sleep(100);
	}
	g_pHPInit->MySendGameInfo("��ɫ������");
	for (int i = 0; i < 6; i++) 
	{
		TAsmMap tMap = role_curmap();
		dbgPrint("��ɫ������ ��ɫ��" + tMap.szName);
		if (tMap.szName == "�ظ�")
		{
			dbgPrint("��ɫ�Ѹ���");
			for (int i = 0; i < 5; i++)
			{
				if (FUN_RunTo(16, 26, 10))
				{
					g_pMsg->SetAutoRunTargetNPCName("����");

					if (g_pMsg->IsNpcDialog())
					{
						g_pMsg->msg_dostring("ClickMission(\"����\")"); //���͵�����
						Sleep(5000);
					}
				}
				Sleep(1000);
			}

		}
		else if (tMap.szName == "����")
		{
			dbgPrint("��ɫ���͵�����");
			for (int i = 0; i < 5; i++)
			{
				if (FUN_RunTo(278, 152, 10))
				{
					g_pMsg->SetAutoRunTargetNPCName("��չԪ");
					if (g_pMsg->IsNpcDialog())
					{
						g_pHPInit->MySendGameInfo("��ɫ�ָ���Ѫ");
						g_pMsg->msg_dostring("ClickMission(\"����\")");
						Sleep(1000);

						g_pMsg->msg_dostring("ClickMission(\"��\")");
						Sleep(1000);

						g_pMsg->msg_dostring("ClickMission(\"�����޻ָ���Ѫ\")"); //���͵�����
						g_pHPInit->MySendGameInfo("��ɫ�������Ѫ����");
						Sleep(1000);
						return;
					}
					return;
				}
				Sleep(1000);
			}
		}
		Sleep(1000);
	}
}

//�Զ���ҩ
void CFunction::FUN_AutoBuy(CString itemNames, CString sceneName, int nPosX, int nPosY, CString npcName, CString _talkName)
{
	dbgPrint("FUN_AutoBuy itemNames=%s, _talkName=%s", itemNames, _talkName);
	int nSceneId = FUN_GetSceneID(sceneName);

	if (FUN_RunToTargetEx(nPosX, nPosY, nSceneId)) {

		g_pMsg->SetAutoRunTargetNPCName(npcName); //�ƶ���NPC
		Sleep(1000);
		//if (g_pMsg->IsNpcDialog())
		{
			if (_talkName == "")
			{
				
				_tstring szT = itemNames;
				auto  szTemps = UserSubMonsterName(szT, _T('|'));
				for (auto temp : szTemps)
				{
					auto items = UserSubMonsterName(temp, _T('='));
					if (items.size() == 2)
					{
						g_pAsmStoreItem->BuyRoleDrugByName(npcName, items[0].c_str(), stoi(items[1].c_str()));
					}
				}
			} 
			else if (CString("����������Ʒ").Find(_talkName) != -1)
			{
				g_pMsg->msg_dostring("ClickMission(\"%s\")", _talkName);//msg��lua��Ϣ����
				Sleep(2000);

				_tstring szT = itemNames;
				auto  szTemps = UserSubMonsterName(szT, _T('|'));
				for (auto temp : szTemps)
				{
					auto items = UserSubMonsterName(temp, _T('='));
					if (items.size() == 2)
					{
						g_pAsmStoreItem->BuyPetDrugByName(npcName, items[0].c_str(), stoi(items[1].c_str()));
					}
				}
			}
		}
	}
}

//����Ʒ
void CFunction::FUN_AutoSell(CString sceneName, int nPosX, int nPosY, CString npcName, CString _talkName)
{
	dbgPrint("FUN_AutoSell _talkName=%s", _talkName);
	int nSceneId = FUN_GetSceneID(sceneName);

	if (FUN_RunToTargetEx(nPosX, nPosY, nSceneId)) {

		g_pMsg->SetAutoRunTargetNPCName(npcName); //�ƶ���NPC
		if (g_pMsg->IsNpcDialog())
		{
			g_pMsg->msg_dostring("ClickMission(\"%s\")", _talkName);//msg��lua��Ϣ����
			Sleep(2000);
		}
	}
}

//�Զ�����
void CFunction::FUN_AutoStorage(_tstring sceneName, int nPosX, int nPosY, _tstring npcName, _tstring _talkName)
{
	//"����,93,178,�ܻ��,������"

	int nSceneId = FUN_GetSceneID(sceneName.c_str());

	if (FUN_RunToTargetEx(nPosX, nPosY, nSceneId)) {

		g_pMsg->SetAutoRunTargetNPCName(npcName.c_str()); //�ƶ���NPC
		if (g_pMsg->IsNpcDialog())
		{
			g_pMsg->msg_dostring("ClickMission(\"%s\")", _talkName);//msg��lua��Ϣ����
			Sleep(2000);
		}
	}
}

//�Զ��ָ�Ѫ��������ʱֻ������
void CFunction::FUN_AutoRoleStatus(int nPercentage)
{
	if (!g_pAsmRole->IsHpAndMp(nPercentage))
	{
		TAsmMap tMap = role_curmap();
		if (tMap.szName == "����") {
			for (int i = 0; i < 5; i++)
			{
				if (FUN_RunTo(84, 162, 60))
				{
					g_pMsg->SetAutoRunTargetNPCName("ƽɳ��");
					if (g_pMsg->IsNpcDialog())
					{
						g_pHPInit->MySendGameInfo("��ɫ�ָ���Ѫ");
						g_pMsg->msg_dostring("ClickMission(\"����\")");
						Sleep(1000);

						g_pMsg->msg_dostring("ClickMission(\"��\")");
						Sleep(1000);

						g_pMsg->msg_dostring("ClickMission(\"�����޻ָ���Ѫ\")"); //���͵�����
						g_pHPInit->MySendGameInfo("��ɫ�������Ѫ����");
						Sleep(1000);
						return;
					}
					return;
				}
				Sleep(1000);
			}
		}
	}
}


/////////////////////////////////////////////////////////
//������ӿ�
////////////////////////////////////////////////////////
BOOL CFunction::FUN_KillMonsterByName(VUserMonsterName vm_UserMonsterName)
{
	for (auto Name : vm_UserMonsterName)
	{
		TAsmMonster tAsmMonster = FUN_GetMonsterByName(Name.c_str(), 0, 1);
		if (tAsmMonster.nMonsterId != -1)
		{
			dbgPrint("FUN_KillMonsterByNameID %d", tAsmMonster.nMonsterId);
			FUN_UseSkillKillMonster(tAsmMonster);//��ʼ���ܴ��;
			return TRUE;
		}
	}

	return FALSE;
}

void CFunction::FUN_UseSkillKillMonster(TAsmMonster tAsmMonster)
{
	while (g_pMe->bRun)
	{
		auto ARoleInfo = g_pAsmRole->GetRoleInfo();
		if (ARoleInfo.nState == 9)
		{
			return;
		}
		for (auto Skill : g_pUser->vUserSkill)
		{
			FUN_UseAttackSkill(Skill, tAsmMonster);//�������ͼ�������
			if (FUN_IsMonsterDie(&tAsmMonster) == false || !g_pMe->bRun)//falseΪ����
			{
				//dbgPrint(_T("�����ѱ�����"));
				return;
			}
		}
		Sleep(100);
	}
}

void CFunction::FUN_UseAttackSkill(TAsmSkill tAsmSkill/*CString SkillName*/, TAsmMonster tAsmMonster)//ʹ�ù�������
{
	auto ARoleInfo = g_pAsmRole->GetRoleInfo();
	if (ARoleInfo.nState != 7)
	{
		FUN_UseMovAttackSkill(tAsmSkill, tAsmMonster);//ʹ�ù�������
	}
}

void CFunction::FUN_RoleHMProtection(CString szLp, CString szTypeName, int Per, CString szYaoNames)
{
	dbgPrint("FUN_RoleHMProtection szTypeName=%s Per=%d szYaoNames=%s", szTypeName, Per, szYaoNames);
	if (szLp == "�ٻظ�")
	{
		if (szTypeName == "Ѫ��")
		{
			_tstring szTemp = szYaoNames;
			auto  vString = UserSubMonsterName(szTemp, _T('|'));
			for (auto v : vString)
			{
				g_pUser->tHighProtect.vYaoName.push_back(v.c_str());
			}
			g_pUser->tHighProtect.nHpPer = Per;
		}
		else if (szTypeName == "����")
		{
			_tstring szTemp = szYaoNames;
			auto  vString = UserSubMonsterName(szTemp, _T('|'));
			for (auto v : vString)
			{
				g_pUser->tHighProtect.vYaoName.push_back(v.c_str());
			}
			g_pUser->tHighProtect.nMpPer = Per;
		}
	}
	else if (szLp == "���ظ�")
	{
		if (szTypeName == "Ѫ��")
		{
			_tstring szTemp = szYaoNames;
			auto  vString = UserSubMonsterName(szTemp, _T('|'));
			for (auto v : vString)
			{
				g_pUser->tLowProtect.vYaoName.push_back(v.c_str());
			}
			g_pUser->tLowProtect.nHpPer = Per;
		}
		else if (szTypeName == "����")
		{
			_tstring szTemp = szYaoNames;
			auto  vString = UserSubMonsterName(szTemp, _T('|'));
			for (auto v : vString)
			{
				g_pUser->tLowProtect.vYaoName.push_back(v.c_str());
			}
			g_pUser->tLowProtect.nMpPer = Per;
		}
	}
	else
	{

	}
}

void CFunction::FUN_PetHMProtection(CString szTypeName, int Per, CString szYaoNames)
{
	dbgPrint("FUN_PetHMProtection szTypeName=%s Per=%d szYaoNames=%s", szTypeName, Per, szYaoNames);
	if (szTypeName == "Ѫ��")
	{
		_tstring szTemp = szYaoNames;
		auto  vString = UserSubMonsterName(szTemp, _T('|'));
		for (auto v : vString)
		{
			g_pUser->tPetProtect.vYaoName.push_back(v.c_str());
		}
		g_pUser->tPetProtect.nHpPer = Per;
	}
}

BOOL CFunction::FUN_AutoTeam(int nNum)
{
	g_pHPInit->MySendGameInfo("�Զ����");
	int timeOut = 0;
	CString temp;
	CString text;
	CString roleName;
	CStringArray teamArr;		//��Ա����
	while (timeOut < 600) 
	{
		teamArr.RemoveAll();
		temp = g_pMsg->mGameInfo->Team;
		dbgPrint("��� temp=%s", temp);
		//if ("" == temp) {
		//	return FALSE;		//�����ԱΪ��
		//} 

		SplitCStringArr(temp, "|", &teamArr);
		TAsmRoleInfo role = g_pAsmRole->GetRoleInfo();
		roleName = role.szName;
		dbgPrint("��ǰ�����Ա:%s", temp);
		if (teamArr.GetCount() >= 2) {
			if (teamArr.GetAt(0) == roleName) {
				//�Ƕӳ� ��Ҫ�����Ա

				for (int i = 1; i < teamArr.GetSize(); i++)
				{
					text.Format("����:%s", teamArr.GetAt(i));
					g_pHPInit->MySendGameInfo(text);
					g_pAsmTeam->AsmInvaiteTeam(teamArr.GetAt(i));	//��������
					Sleep(3000);
				}
			}
			else {
				//��Ա�ȴ��������
				if (g_pAsmTeam->GetInvite()) {
					Sleep(1000);
					g_pHPInit->MySendGameInfo("��������� �������");
					g_pAsmTeam->TeamJoin();
					Sleep(2000);
				}
				else {
					Sleep(2000);
				}
			}
		}
		else {
			dbgPrint("��������̫��");
		}

		if (FUN_CheckTeam(nNum)) {
			g_pHPInit->MySendGameInfo("������");
			return TRUE;
			//������
		}

		timeOut++;
		Sleep(2000);
	}
	return FALSE;
}

BOOL CFunction::FUN_CheckTeam(int nNum)
{
	CString temp;
	CString roleName;
	CStringArray teamArr;		//��Ա����
	int checkCount = 0;			//�����Ա���
	temp = g_pMsg->mGameInfo->Team;
	if ("" == temp) {
		return 0;		//�����ԱΪ��
	}

	SplitCStringArr(temp, "|", &teamArr);
	TAsmRoleInfo role = g_pAsmRole->GetRoleInfo();
	g_pAsmTeam->GetTeamInfo();

	if (teamArr.GetAt(0) == roleName) {
		//�Ƕӳ���Ҫ����Ա�Ƿ���
		for (auto itor = g_pAsmTeam->m_TeamArr.begin(); itor != g_pAsmTeam->m_TeamArr.end(); itor++)
		{
			for (int i = 1; i < teamArr.GetSize(); i++)
			{
				if (teamArr.GetAt(i) == itor->szName) {
					checkCount++;
				}
			}
		}
	}
	else {
		//��Ա��Ҫ���ӳ��ǲ�����ȷ��
		for (auto itor = g_pAsmTeam->m_TeamArr.begin(); itor != g_pAsmTeam->m_TeamArr.end(); itor++)
		{
			if (teamArr.GetAt(0) == itor->szName) {
				return 1;
			}
		}
	}

	if (checkCount >= nNum) {
		return 1;	//��Ա����
	}

	return 0;
}