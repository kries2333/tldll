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

TFunScene gScene_Array[]//地图场景数组
{
	{"洛阳", 0},
	{ "苏州", 1 },
	{ "大理", 2 },
	{ "嵩山", 3 },
	{ "太湖 ", 4 },
	{ "镜湖", 5 },
	{ "无量山", 6 },
	{ "剑阁", 7 },
	{ "敦煌", 8 },
	{ "少林寺", 9 },
	{ "丐帮总舵", 0xA },
	{ "光明殿", 0xB },
	{ "武当山", 0xC },
	{ "天龙寺", 0xD },
	{ "凌波洞", 0xE },
	{ "峨嵋山", 0xF },
	{ "星宿海", 0x10 },
	{ "天山", 0x11 },
	{ "雁南", 0x12 },
	{ "西湖", 0x1E },
	{ "洱海", 24 },
	{ "后花园", 0x7B },
	{ "小木人巷", 122},
	{ "宝藏洞一层", 166},
	{ "摩崖洞", 213},
};

TAsmMap role_curmap()
{
	TAsmMap tcurmap;
	tcurmap.bool_ret = TRUE;
	tcurmap.nSceneId =g_pMsg->msg_getnumber("g_GetValue = GetSceneID()");
	tcurmap.szName = g_pMsg->msg_getstring("mycurmapname", "mycurmapname = GetCurrentSceneName()").c_str();
	if (tcurmap.szName == "洛阳")
		tcurmap.nSceneId = 0;
	return tcurmap;
}


HMODULE GetProcessModuleHandle(DWORD pid, CONST TCHAR* moduleName) {	// 根据 PID 、模块名（需要写后缀，如：".dll"），获取模块入口地址。
	MODULEENTRY32 moduleEntry;
	HANDLE handle = NULL;
	handle = ::CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid); //  获取进程快照中包含在th32ProcessID中指定的进程的所有的模块。
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
				TRACE("分割结果:%s\n", expression);
			}
			break;
		}
		else {
			if (!expression.IsEmpty()) {
				(*resultArr).Add(expression.Mid(pre_pos, (pos - pre_pos)));
				TRACE("分割结果:%s\n", expression.Mid(pre_pos, (pos - pre_pos)));
				expression.Delete(0, pos - pre_pos + 1);
			}
		}
	}
	return true;
}

//江湖答题
BOOL CFunction::FUN_Questions()
{
	g_pHPInit->MySendGameInfo("开始江湖答题");
	// TODO: 在此添加控件通知处理程序代码
	map<CString, CString> Quiz;
	Quiz.insert(pair<CString, CString>("想知道下一步该做什么的时候，就可以去查看可接任务，那么你知道如何打开可接任务吗？", "点击任务界面就可以看到可接任务了"));
	Quiz.insert(pair< CString, CString>("最有名的九大门派都派人来大理了，你知道哪个门派不属于九大门派吗？", "华山派"));
	Quiz.insert(pair< CString, CString>("你可知出大理城东门可到达哪里？", "无量山"));
	Quiz.insert(pair< CString, CString>("现在可以使用天空视角观赏风景了，那么你知道在哪里可以打开这个功能吗？", "系统选项中的显示设置"));
	Quiz.insert(pair< CString, CString>("你经常会用驿站来往于各地之间，那么你知道使用驿站收费吗？", "去哪都不收费"));
	Quiz.insert(pair< CString, CString>("你可知出大理城西门可到达哪里？", "剑阁"));
	Quiz.insert(pair< CString, CString>("你有发现在这个游戏里，怎样转动游戏画面吗？", "按住鼠标右键拖动鼠标"));
	Quiz.insert(pair< CString, CString>("携带珍兽是有数量限制的，可以通过提升自身等级或购买元宝道具来增加携带位，如何放生多余的珍兽？", "点开珍兽界面，点击放生"));
	Quiz.insert(pair< CString, CString>("你知道点击左下角的功能栏里的第二个图标或使用快捷键Alt+S打开的是什么界面吗？", "技能界面"));
	Quiz.insert(pair< CString, CString>("既然你来到了《天龙八部》的世界，你知道它的小说作者是谁吗？", "金庸"));
	Quiz.insert(pair< CString, CString>("走路很累，不过游戏中可以自动行走，很轻松的，你知道怎样操作吗？", "鼠标左键点击地面并按住"));
	Quiz.insert(pair< CString, CString>("听说你通晓古今未来，那么这《天龙八部》的故事是发生于哪个朝代？", "宋朝"));
	Quiz.insert(pair< CString, CString>("珍兽是你的好帮手，那么除了点击屏幕左下角功能栏的第三个图标，还可以用什么快捷键打开珍兽界面呢？", "Alt+X"));
	Quiz.insert(pair< CString, CString>("不要现在试验，你能立刻说出右上角小地图里有一个“N”的按钮是做什么的吗？", "是帮我转取到向北的视角吧"));
	Quiz.insert(pair< CString, CString>("还记得怎样“使用”背包中的道具吧？", "打开背包，右键点击物品"));
	Quiz.insert(pair< CString, CString>("还没有在这世界说过话吧，你知道怎样说出一个表情符号吗？", "按屏幕左下角的笑脸符号"));
	Quiz.insert(pair< CString, CString>("仔细看看，现在屏幕上什么地方标明了你的等级？", "左上角我的头像旁边有一个数字"));
	Quiz.insert(pair< CString, CString>("人在江湖漂，见到什么都该记得，大理城的武器店老板是谁？", "蒲良"));
	Quiz.insert(pair< CString, CString>("你知道经常送你前往各地的大理城驿站老板是谁吗？", "崔逢九"));
	Quiz.insert(pair< CString, CString>("还记得第一个给你发布任务的新手指导是谁吗？", "赵天师"));
	Quiz.insert(pair< CString, CString>("珍兽的等级是可以逐渐提升的，你知道怎么样可以提升珍兽的等级吗？", "带它一起出去打怪"));
	Quiz.insert(pair< CString, CString>("屏幕左上角头像旁边的红色横条表示什么意思啊？", "我的血条啊，长度为零的时候我就去地府了"));
	Quiz.insert(pair< CString, CString>("在新天龙八部中与好友组队杀怪，会有什么好处？", "给予额外经验"));
	Quiz.insert(pair< CString, CString>("该不会忘记自己是怎么走来的吧，控制角色移动需要点击鼠标哪个按键？", "左键"));
	Quiz.insert(pair< CString, CString>("右键点击其他玩家，弹出菜单，点击“加为好友”后会怎么样？", "加对方为好友"));
	Quiz.insert(pair< CString, CString>("你知道想要拥有一个坐骑代步需要哪些东西呢？", "只要装备坐骑就可以"));
	Quiz.insert(pair< CString, CString>("一项体贴的功能，就是查看整个城市的地图，你知道怎样查看吗？", "按TAB键"));
	Quiz.insert(pair< CString, CString>("在新天龙八部游戏中，当接到任务后，在右上角小地图上会有提示，请问是什么标记是指向任务要找的人呢？", "蓝色小圆点"));
	Quiz.insert(pair< CString, CString>("在聊天时，输入“#数字”代表什么？", "插入表情包子符"));
	Quiz.insert(pair< CString, CString>("输入“#G文字”文字的颜色会变成什么颜色？", "绿色"));
	Quiz.insert(pair< CString, CString>("游戏的小地图出现在屏幕的什么位置？", "屏幕右上角"));
	Quiz.insert(pair< CString, CString>("骑乘也是一个可以使用的技能，那么使用Alt+S打开技能栏后会在哪里找到呢？", "普通技能"));
	Quiz.insert(pair< CString, CString>("初涉江湖，不知你进入角色没有，知道脚下这座城市的名字是什么吗？", "大理"));

	CString Text;
	CString Question;
	CString Answer;
	map<CString, CString>::iterator iter;

Bagin:
	Text = g_pMsg->msg_getstring("MyMonCanCommitString", "setmetatable(_G, {__index = Quiz_Env});MyMonCanCommitString = Quiz_Button_1:GetText()").c_str();
	if ("开始答题" == Text) {
		g_pMsg->msg_dostring("setmetatable(_G, {__index = Quiz_Env});Quiz_Button_Clicked(1);");
		Sleep(500);
	}

	Question = g_pMsg->msg_getstring("MyMonCanCommitString", "setmetatable(_G, {__index = Quiz_Env});MyMonCanCommitString = Quiz_Text:GetText()").c_str();
	Sleep(500);
	for (iter = Quiz.begin(); iter != Quiz.end(); iter++)
	{
		//dbgPrint("当前问题：%s,答案：%s", iter->first, iter->second);
		if (Question.Find(iter->first) > -1) {
			Answer = iter->second;
		}
	}
	if (Question.Find("错误") > -1)
	{
		g_pMsg->msg_dostring("setmetatable(_G, {__index = Quiz_Env});Quiz_Button_Clicked(1);");
		Sleep(500);
		goto Bagin;
	}
	dbgPrint("当前问题：%s,答案：%s", Question, Answer);
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
	g_pHPInit->MySendGameInfo("结束江湖答题");
	return TRUE;
}

BOOL CFunction::FUN_IsQuitRelative()
{
	CString text = g_pMsg->msg_getstring("MyMonCanCommitString", "setmetatable(_G, {__index = QuitRelative_Env}); MyMonCanCommitString = QuitRelative_Text:GetText()").c_str();
	dbgPrint("FUN_IsQuitRelative = %s", text);
	if (text.Find("服务器的连接") != -1)
	{
		return TRUE;
	}
	return FALSE;
}

// 任务相关
//判断当前任务
BOOL CFunction::FUN_GetPlayerMission(_tstring taskName)
{
	dbgPrint("FUN_GetPlayerMission taskName = %s", taskName.c_str());
	TAsmTask tAsmTask;
	auto ATask = g_pAsmTask->AsmGetTaskData();//获取角色周边范围内的怪物
	for (auto t : ATask)
	{
		if (((CString)t.szName).Find(taskName.c_str(), 0) != -1) {
			dbgPrint("FUN_GetPlayerMission 找到任务");
			return TRUE;
		}
	}
	return FALSE;
}

float CFunction::FUN_MaximumRange(TAsmSkill tAsmSkill)
{
	float fRange = 0;
	if (tAsmSkill.fMax == -1)//自身范围的群体技能
	{
		fRange = tAsmSkill.fAngle;
	}
	else
	{
		fRange = tAsmSkill.fMax;
	}

	return fRange;
}



//BOOL CFunction::FUN_RunToTarget(float fx, float fy, float dis)//跨地图寻路
//{
//	bool bMov = false;
//	DWORD dwWaitTime = GetTickCount64();//第一次记录时间，人物此时是非跑动状态
//	DWORD dwEndTime = GetTickCount64();//第一次记录时间，人物此时是非跑动状态
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
//		if ((dwCurTime - dwWaitTime) > (180 * 1000)) //如果30秒还没到达退出重新寻路
//		{
//			dbgPrint("地图内寻路3分钟没走到目标，可能是走向了未知地点");
//			return FALSE;
//		}
//		TAsmMap role_map = role_curmap();
//		if (role_map.szName == (CString)"地府")	//判断是否在地府。
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
//		dbgPrint("寻路计时%d", (dwCurTime - dwEndTime));
//		if ((dwCurTime - dwEndTime) > (3 * 1000)) //如果30秒还没到达退出重新寻路
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
//		strTemp.Format("地图内移动 距离目标=%f", fCurDistance);
//		g_pHPInit->MySendGameInfo(strTemp);
//		if (fCurDistance < dis)//当前的距离小于2可以操作目标了
//		{
//			dbgPrint("到达目的地");
//			g_pMsg->CallInOutRide(0);
//			return TRUE;
//		}
//		Sleep(1000);
//	}
//	return FALSE;
//}

BOOL CFunction::FUN_RunToTargetEx(float fx, float fy, int SceneId, float dis)//跨地图寻路
{
	bool bMov = false;
	TAsmMap tAsmMap = role_curmap();
	float fOldX = -1, fOldY = -1;
	DWORD dwWaitTime = GetTickCount64();//第一次记录时间，人物此时是非跑动状态
	DWORD dwEndTime = GetTickCount64();//第一次记录时间，人物此时是非跑动状态

	while (g_pMe->bRun)
	{

		DWORD dwCurTime = GetTickCount64();
		if ((dwCurTime - dwWaitTime) > (5 * 60 * 1000)) //如果10分钟还没到达退出重新寻路
		{
			dbgPrint("跨图寻路10分钟没走到目标，可能是走向了未知地点");
			return FALSE;
		}

		if (tAsmMap.szName == (CString)"地府")	//判断是否在地府。
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
					dbgPrint("开始跨图寻路 %f, %f, %d", fx, fy, SceneId);
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
		//dbgPrint("寻路计时%d", (dwCurTime - dwEndTime));
		if ((dwCurTime - dwEndTime) > (5 * 1000)) //如果30秒还没到达退出重新寻路
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
		strTemp.Format("从%s(%d)移动到%s(%d) 距离目标=%f", tAsmMap.szName, tAsmMap.nSceneId, CString(FUN_GetSceneName(SceneId).c_str()), SceneId, fCurDistance);
		g_pHPInit->MySendGameInfo(strTemp);
		if (fCurDistance < dis)//当前的距离小于2可以操作目标了
		{
			dbgPrint("到达目的地");
			//g_pMsg->CallInOutRide(0);
			return TRUE;
		}

		Sleep(1000);
	}
	return FALSE;
}

float CFunction::FUN_GetDistance(float x2, float y2)//获取与目标的距离
{
	auto APos = g_pAsmRole->GetPos();
	if (APos.fx <= 0 || APos.fy <= 0)
	{
		return 1000;
	}
	//获取人物坐标异常日志(APos, return 0.0f)
	float fdistance = sqrt(pow(APos.fx - x2, 2) + pow(APos.fy - y2, 2));

	return fdistance;
}

int  CFunction::FUN_GetSceneID(const char* szSceneName)//通过名称获取地图场景ID
{
	size_t Count = sizeof(gScene_Array) / sizeof(gScene_Array[0]);
	for (size_t i = 0; i < Count; i++)
	{
		if (strcmp(gScene_Array[i].szName, szSceneName) == 0)//名称完全匹配
		{
			return gScene_Array[i].nSceneId;
		}
	}

	return -1;//没有就返回-1
}

_tstring CFunction::FUN_GetSceneName(int nSceneId)	//通过ID获取场景名
{
	size_t Count = sizeof(gScene_Array) / sizeof(gScene_Array[0]);
	for (size_t i = 0; i < Count; i++)
	{
		if (gScene_Array[i].nSceneId == nSceneId)//名称完全匹配
		{
			return _tstring(gScene_Array[i].szName);
		}
	}

	return "";//没有就返回空
}

TAsmMonster CFunction::FUN_GetCatchMonsterByName(_tstring MonsterName)
{
	TAsmMonster tAsmMonster;
	float fdistance = 0.0f;
	auto AMonster = g_pAsmMonster->AsmGetMonsterData();//获取角色周边范围内的怪物
	for (auto m : AMonster)
	{
		dbgPrint("m.szName=%s nAttack=%dm m.nCatch=%d", m.szName, m.nAttack, m.nCatch);
		if (CString(m.szName).Find(MonsterName.c_str(), 0) != -1)
		{
			if (m.nAttack == 1 && m.nCatch == -1)
			{
				dbgPrint("抓捕怪物");
				if (FUN_MinDistanceObject(&fdistance, m.fMonsterX, m.fMonsterY))//最近距离的怪物
					tAsmMonster = m;
			}
		}
	}

	return tAsmMonster;
}

TAsmMonster CFunction::FUN_GetMonsterByName(_tstring MonsterName, int nMonsterType, int nKillType)//名称获取距离自己最近的怪物
{
	TAsmMonster tAsmMonster;
	float fdistance = 0.0f;
	auto AMonster = g_pAsmMonster->AsmGetMonsterData();//获取角色周边范围内的怪物
	for (auto m : AMonster)
	{
		if (((CString)m.szName).Find(MonsterName.c_str(), 0) != -1)
		{
			if (m.nAttack == 0)
			{
				switch (nMonsterType)
				{
				case 0://正常杀怪
				{
					if (FUN_MinDistanceObject(&fdistance, m.fMonsterX, m.fMonsterY))//最近距离的怪物
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
	if (nMenPaiId == 8)	//逍遥派
	{
		tAsmSkill.nSkillId = 521;
		tAsmSkill.fMax = 15.0;
	}
	else {
		tAsmSkill.nSkillId = 0;
		tAsmSkill.fMax = 2.0;
	}
	//宠物出战打怪
	g_pAsmPet->PetGoFight(0);

	auto AMonster = g_pAsmMonster->AsmGetMonsterData();//获取角色周边范围内的怪物
	for (auto m : AMonster)
	{
		if (((CString)m.szName).Find(MonsterName.c_str()) != -1)
		{
			if (m.nAttack == 0)
			{
				for (size_t i = 0; i < 60; i++)	//循环60次大概1分钟，如果还没怪物就得重新选择
				{
					float nHp = g_pAsmMonster->GetHp(m);
					if (nHp <= 0) {
						return;
					}
					FUN_UseMovAttackSkill(tAsmSkill, m);
					CString strTemp;
					strTemp.Format("目标怪物=%s 剩余血量=%f", m.szName, nHp);
					g_pHPInit->MySendGameInfo(strTemp);
					dbgPrint(strTemp);
					if (g_pAsmRole->GetHpPercent() <= 0) {
						dbgPrint("角色死亡");
						return;
					}

					Sleep(500);
				}
			}
		}
		Sleep(100);
	}	
}

bool  CFunction::FUN_MinDistanceObject(float* fdistance, float fx, float fy)	//true是最近的
{
	//当前怪物是否是最近的怪
	float fnext = FUN_GetDistance(fx, fy);

	if (*fdistance == 0.0f)//第一次查找
	{
		*fdistance = fnext;
		return true;
	}
	else if (*fdistance > fnext)//和下一个做比较
	{
		*fdistance = fnext;
		return true;
	}

	return false;
}

//单技能攻击
void CFunction::FUN_UseMovAttackSkill(TAsmSkill tAsmSkill, TAsmMonster tAsmMonster)//开始释放攻击技能
{
	if (FUN_MovToMonster(tAsmMonster, tAsmSkill))//怪物在射程内
	{
		g_pAsmSkill->AsmUseSkillCall(tAsmMonster.nMonsterId, tAsmSkill.nSkillId);//使用此技能
	}
}

bool CFunction::FUN_MovToMonster(TAsmMonster tAsmMonster, TAsmSkill tAsmSkill)//走到技能射程范围内
{
	float fr = FUN_MaximumRange(tAsmSkill);//获取射程
	if (FUN_GetDistance(tAsmMonster.fMonsterX, tAsmMonster.fMonsterY) < fr + 2)//射程范围以内
	{
		return true;
	}

	FUN_RunTo(tAsmMonster.fMonsterX, tAsmMonster.fMonsterY, 30);

	for (int i = 0; i < 10 && g_pMe->bRun; i++)
	{
		if (FUN_GetDistance(tAsmMonster.fMonsterX, tAsmMonster.fMonsterY) < fr + 2)//射程范围内
			return true;
		Sleep(300);
	}

	return false;//没有走到射程内
}

bool CFunction::FUN_IsMonsterDie(TAsmMonster* ptAsmMonster)//false为死亡
{
	try
	{
		auto AMonster = g_pAsmMonster->AsmGetMonsterData();//获取角色周边范围内的怪物
		for (auto m : AMonster)
		{
			if ((*ptAsmMonster).nMonsterId == m.nMonsterId)
			{
				if (m.fMonsterHp > 0)
				{
					*ptAsmMonster = m;//取得怪物当前的数据,有时候，会超出距离，要取得最新信息判断
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
//抓捕
bool CFunction::FUN_MisUseSkillCatchMonster(_tstring monsterName)
{
	_tstring strTemp = "正在抓捕" + monsterName;
	g_pHPInit->MySendGameInfo(strTemp.c_str());
	TAsmSkill tAsmSkill;
	tAsmSkill.nSkillId = 1;
	TAsmMonster tAsmMonster = FUN_GetCatchMonsterByName(monsterName);
	if (tAsmMonster.fMonsterX == 0 || tAsmMonster.fMonsterY == 0)
	{
		dbgPrint("未找到怪物");
		return false;
	}
	
	dbgPrint("移动 fMonsterX=%f, fMonsterY=%f", tAsmMonster.fMonsterX, tAsmMonster.fMonsterY);
	if (FUN_MovToMonster(tAsmMonster, tAsmSkill))//怪物在射程内
	{
		dbgPrint("到达范围 tAsmMonster.szName=%s nType=%d, fMonsterX=%f, fMonsterY=%f", tAsmMonster.szName, tAsmMonster.nType, tAsmMonster.fMonsterX, tAsmMonster.fMonsterY);
		g_pAsmSkill->AsmUseSkillCall(tAsmMonster.nMonsterId, tAsmSkill.nSkillId);//使用此技能
		Sleep(20000);//延迟20秒
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
		dbgPrint("FUN_IsAvailableTask %s 在可接任务列表里", taskName.c_str());
		return TRUE;
	}
	dbgPrint("FUN_IsAvailableTask %s 不在可接任务列表里", taskName.c_str());
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
	//判断 如果是骑乘状态不能打怪。
	g_pMsg->CallInOutRide(0);
	g_pAsmSkill->AsmUseSkillCallByPoint(nSkillId, x, y);
	return 1;
}

// 已接列表中是否有此任务
bool CFunction::FUN_IsTask(_tstring taskName)
{
	_tstring finishInfo = FUN_GetMissionFinishInfo(taskName);
	if (!finishInfo.empty())
	{
		auto  vString = UserSubMonsterName(finishInfo, _T('|'));
		if (vString.size() >= 6)
		{
			if (stoi(vString[0]) != -1) {
				dbgPrint("%s在已接任务列表中", taskName.c_str());
				return TRUE;
			}
		}
	}
	dbgPrint("%s不在已接任务列表中", taskName.c_str());
	return FALSE;
}

int CFunction::FUN_IsTaskSuccess(_tstring taskName)
{
	int finished = -1;

	// 获取完成后提交任务得信息
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
	strTemp.Format("接取任务:%s", taskName.c_str());
	dbgPrint(strTemp);
	g_pHPInit->MySendGameInfo(strTemp);

	int sceneId = FUN_GetSceneID(sceneName.c_str());

	for (int i = 0; i < 5; i++)
	{
		if (CString(npcName.c_str()).Find("柴进") != -1)
		{
			x = 199;
			y = 68;
		}
		if (FUN_RunToTargetEx(x, y, sceneId))
		{
			g_pMsg->SetAutoRunTargetNPCName(npcName.c_str()); //移动到NPC

			if (g_pMsg->IsNpcDialog())
			{
				g_pMsg->msg_dostring("ClickMission(\"%s\")", taskName.c_str());//msg的lua消息函数
				Sleep(500);

				g_pMsg->MissionQuestAccept_Clicked();
				Sleep(200);
				return TRUE;
			}
			else {
				dbgPrint("对话框没显示");
			}
		}
		Sleep(1000);
	}

	return FALSE;
}

// 执行任务可以使用接取扩展，可以合并下。
bool CFunction::FUN_TaskAccepEx(_tstring taskName)
{
	if (!g_pMe->bRun)
	{
		return false;
	}
	//点击下升级
	FUN_Levelup();

	dbgPrint("FUN_TaskAccepEx=%s", taskName.c_str());

	//判断是否完成
	//先判断是否在可接列表里
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

			dbgPrint("FUN_TaskAccepEx 在可接列表获取到任务 任务名=%s tmp_npcName=%s tmp_sceneName=%s tmp_posX=%f tmp_posY=%f",
				taskName.c_str(), tmp_npcName.c_str(), tmp_sceneName.c_str(), tmp_posX, tmp_posY);

			FUN_TaskAccep(taskName, tmp_npcName, tmp_sceneName, tmp_posX, tmp_posY);	// 接受任务后继续执行
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

	FUN_TaskAccepEx(taskName);	//接取任务

	return FUN_TaskSuccessEx(taskName, 1);	//执行任务
}

//需要先确定任务已经接取
bool CFunction::FUN_ExecuteTaskKill(_tstring taskName, int nType, int nMonsterType, _tstring monsterName, bool bPickUp)
{
	if (!g_pMe->bRun)
	{
		return false;
	}

	CString strTemp;
	strTemp.Format("执行杀怪任务:%s", taskName.c_str());
	g_pHPInit->MySendGameInfo(strTemp);

	_tstring tmp_npcName;
	_tstring tmp_sceneName;
	int		tmp_sceneId;
	float	tmp_posX = 0.0;
	float	tmp_posY = 0.0;
	int		finished = -1;

	//判断任务如果没接取就接取
	FUN_TaskAccepEx(taskName);	//接取任务

	// 获取完成后提交任务得信息
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
	if (finished == -1)	//等于-1表示已接任务列表里没有任务
	{
		dbgPrint("未执行杀怪任务");
		return FALSE;
	}

	if (finished == 1)
	{
		//这里貌似有点问题，后面对话的npc不一定是任务中的第一个节点
		FUN_TaskSuccessEx(taskName, 1);
		return TRUE;
	}

	//获取需要杀得怪物
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
	dbgPrint("执行杀怪--finished=%d monsterName=%s 完成npc=%s, 完成场景=%s, 完成坐标x=%f,y=%f",
		finished, monsterName.c_str(), tmp_npcName.c_str(), tmp_sceneName.c_str(), tmp_posX, tmp_posY);

	// 获取需要杀怪得地方
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

			dbgPrint("执行杀怪--需要杀的怪物=%s, 场景=%d, 坐标x=%f, y=%f",
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
					float fCurDistance = FUN_GetDistance(tmp_posX, tmp_posY);//记录当前的距离
					if (fCurDistance < 10)
					{
						bMov = false;
						break;
					}
				}
				Sleep(1000);
			}

			//到达目的地,开始打怪
			dbgPrint("开始打怪");
			while (true)
			{
				if (!g_pMe->bRun)
				{
					return false;
				}

				if (g_pAsmRole->GetHpPercent() <= 0) {
					dbgPrint("角色死亡");
					FUN_DeathResurrection();
					return true;
				}

				if (FUN_IsTaskSuccess(taskName) == 1)
				{
					//这里判断有问题
					FUN_TaskSuccessEx(taskName, 1);	// 第一次珍兽，这里需要取第四个坐标 最后一个坐标
					return TRUE;
				}

				FUN_MisKillMonsterByName(monsterName);
				if (bPickUp) //是否拾取
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
	strTemp.Format("完成任务:%s", taskName.c_str());
	g_pHPInit->MySendGameInfo(strTemp);

	int sceneId = FUN_GetSceneID(sceneName.c_str());

	for (int i = 0; i < 5; i++)
	{
		if (CString(npcName.c_str()).Find("柴进") != -1)
		{
			x = 199;
			y = 68;
		}
		if (FUN_RunToTargetEx(x, y, sceneId))
		{
			g_pMsg->SetAutoRunTargetNPCName(npcName.c_str()); //移动到NPC

			if (g_pMsg->IsNpcDialog())
			{
				g_pMsg->msg_dostring("ClickMission(\"%s\")", taskName.c_str());//msg的lua消息函数
				Sleep(500);

				g_pMsg->QuestFrameMissionContinue();	//NPC对话中执行继续
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

//完成任务后找NPC回执，nType 是获取完成NPC方式不同 就是在第几个坐标，从1开始
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

	// 获取完成后提交任务得信息
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
			FUN_UseSkillKillMonster(tAsmMonster);//开始技能打怪;
		}
	}
}

//加入门派
void CFunction::FUN_JoinMenPai(_tstring szMenPaiName)
{
	if (!g_pMe->bRun)
	{
		return;
	}

	dbgPrint("FUN_JoinMenPai");
	int menpai = g_pAsmRole->GetMenPaiForId();
	dbgPrint("FUN_JoinMenPai 当前门派=%d, 加入门派=%s", menpai, szMenPaiName.c_str());
	if (menpai == 9) //判断是否是无门派
	{
		_tstring taskName = "九大门派";
		while (g_pMe->bRun) {
			if (FUN_IsTaskSuccess(taskName) == 1)
			{
				FUN_TaskSuccessEx(taskName, 1);
				return;
			}

			g_pMsg->CallInOutRide(1);

			int sceneId = FUN_GetSceneID("凌波洞");
			float tmpPosX = 124;
			float tmpPosY = 144;
			if (FUN_RunToTargetEx(tmpPosX, tmpPosY, sceneId))
			{
				g_pMsg->SetAutoRunTargetNPCName("苏星河"); //移动到NPC

				g_pMsg->msg_dostring("ClickMission('加入门派')");//msg的lua消息函数
				Sleep(1000);

				_tstring szTemp = "我确定要拜入" + szMenPaiName;
				g_pMsg->msg_dostring("ClickMission(\"%s\")", szTemp.c_str());//msg的lua消息函数
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
		g_pMsg->msg_dostring("setmetatable(_G, { __index = ActionSkill_Env }); LootPacket_Collect_Clicked();");//msg的lua消息函数
	}
}

//执行自动移动对话框
int CFunction::FUN_AutoMove()
{
	if (!g_pMe->bRun)
	{
		return 0;
	}

	// 关闭新手指导
	g_pMsg->msg_dostring("setmetatable(_G, { __index = FreshManIntro_Env }); FreshManIntro_Quit_Clicked();");
	Sleep(200);
	
	CString strTemp = "";
	strTemp = g_pMsg->msg_getstring("strTemp", "strTemp = getMessageBoxText();").c_str();
	dbgPrint("GetMessageBoxText strTemp=%s", strTemp);
	if (!strTemp.IsEmpty())
	{
		if (strTemp.Find("自动寻路") != -1)
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
		if (strTemp.Find("加杀气") != -1)
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
	g_pAsmSkill->AsmUseSkillCall(-1, nSkillId);//使用此技能
}

BOOL CFunction::FUN_RunTo(float fx, float fy, int nTime)//寻路指定时间Sleep(10)
{
	if (FUN_GetDistance(fx, fy) < 3)//判断距离判
		return TRUE;

	g_pMsg->msg_dostring("AutoRunToTarget(%d, %d)", (int)fx, (int)fy);
	for (int i = 0; i < nTime; i++)
	{
		if (FUN_GetDistance(fx, fy) < 3)//判断距离判
			return TRUE;
		Sleep(500);
	}
	return FALSE;
}

//死亡复活
void CFunction::FUN_DeathResurrection()
{
	if (g_pAsmRole->GetHpPercent() > 0.1) {
		return;
	}

	g_pHPInit->MySendGameInfo("角色死亡");
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
	g_pHPInit->MySendGameInfo("角色复活中");
	for (int i = 0; i < 6; i++) 
	{
		TAsmMap tMap = role_curmap();
		dbgPrint("角色复活中 角色在" + tMap.szName);
		if (tMap.szName == "地府")
		{
			dbgPrint("角色已复活");
			for (int i = 0; i < 5; i++)
			{
				if (FUN_RunTo(16, 26, 10))
				{
					g_pMsg->SetAutoRunTargetNPCName("孟婆");

					if (g_pMsg->IsNpcDialog())
					{
						g_pMsg->msg_dostring("ClickMission(\"大理\")"); //传送到大理
						Sleep(5000);
					}
				}
				Sleep(1000);
			}

		}
		else if (tMap.szName == "大理")
		{
			dbgPrint("角色传送到大理");
			for (int i = 0; i < 5; i++)
			{
				if (FUN_RunTo(278, 152, 10))
				{
					g_pMsg->SetAutoRunTargetNPCName("白展元");
					if (g_pMsg->IsNpcDialog())
					{
						g_pHPInit->MySendGameInfo("角色恢复气血");
						g_pMsg->msg_dostring("ClickMission(\"治疗\")");
						Sleep(1000);

						g_pMsg->msg_dostring("ClickMission(\"是\")");
						Sleep(1000);

						g_pMsg->msg_dostring("ClickMission(\"给珍兽恢复气血\")"); //传送到大理
						g_pHPInit->MySendGameInfo("角色已完成满血复活");
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

//自动买药
void CFunction::FUN_AutoBuy(CString itemNames, CString sceneName, int nPosX, int nPosY, CString npcName, CString _talkName)
{
	dbgPrint("FUN_AutoBuy itemNames=%s, _talkName=%s", itemNames, _talkName);
	int nSceneId = FUN_GetSceneID(sceneName);

	if (FUN_RunToTargetEx(nPosX, nPosY, nSceneId)) {

		g_pMsg->SetAutoRunTargetNPCName(npcName); //移动到NPC
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
			else if (CString("购买珍兽用品").Find(_talkName) != -1)
			{
				g_pMsg->msg_dostring("ClickMission(\"%s\")", _talkName);//msg的lua消息函数
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

//卖物品
void CFunction::FUN_AutoSell(CString sceneName, int nPosX, int nPosY, CString npcName, CString _talkName)
{
	dbgPrint("FUN_AutoSell _talkName=%s", _talkName);
	int nSceneId = FUN_GetSceneID(sceneName);

	if (FUN_RunToTargetEx(nPosX, nPosY, nSceneId)) {

		g_pMsg->SetAutoRunTargetNPCName(npcName); //移动到NPC
		if (g_pMsg->IsNpcDialog())
		{
			g_pMsg->msg_dostring("ClickMission(\"%s\")", _talkName);//msg的lua消息函数
			Sleep(2000);
		}
	}
}

//自动存贮
void CFunction::FUN_AutoStorage(_tstring sceneName, int nPosX, int nPosY, _tstring npcName, _tstring _talkName)
{
	//"苏州,93,178,曹伙计,打开银行"

	int nSceneId = FUN_GetSceneID(sceneName.c_str());

	if (FUN_RunToTargetEx(nPosX, nPosY, nSceneId)) {

		g_pMsg->SetAutoRunTargetNPCName(npcName.c_str()); //移动到NPC
		if (g_pMsg->IsNpcDialog())
		{
			g_pMsg->msg_dostring("ClickMission(\"%s\")", _talkName);//msg的lua消息函数
			Sleep(2000);
		}
	}
}

//自动恢复血和气，暂时只有苏州
void CFunction::FUN_AutoRoleStatus(int nPercentage)
{
	if (!g_pAsmRole->IsHpAndMp(nPercentage))
	{
		TAsmMap tMap = role_curmap();
		if (tMap.szName == "苏州") {
			for (int i = 0; i < 5; i++)
			{
				if (FUN_RunTo(84, 162, 60))
				{
					g_pMsg->SetAutoRunTargetNPCName("平沙雁");
					if (g_pMsg->IsNpcDialog())
					{
						g_pHPInit->MySendGameInfo("角色恢复气血");
						g_pMsg->msg_dostring("ClickMission(\"治疗\")");
						Sleep(1000);

						g_pMsg->msg_dostring("ClickMission(\"是\")");
						Sleep(1000);

						g_pMsg->msg_dostring("ClickMission(\"给珍兽恢复气血\")"); //传送到大理
						g_pHPInit->MySendGameInfo("角色已完成满血复活");
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
//不对外接口
////////////////////////////////////////////////////////
BOOL CFunction::FUN_KillMonsterByName(VUserMonsterName vm_UserMonsterName)
{
	for (auto Name : vm_UserMonsterName)
	{
		TAsmMonster tAsmMonster = FUN_GetMonsterByName(Name.c_str(), 0, 1);
		if (tAsmMonster.nMonsterId != -1)
		{
			dbgPrint("FUN_KillMonsterByNameID %d", tAsmMonster.nMonsterId);
			FUN_UseSkillKillMonster(tAsmMonster);//开始技能打怪;
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
			FUN_UseAttackSkill(Skill, tAsmMonster);//多种类型技能区分
			if (FUN_IsMonsterDie(&tAsmMonster) == false || !g_pMe->bRun)//false为死亡
			{
				//dbgPrint(_T("怪物已被消灭"));
				return;
			}
		}
		Sleep(100);
	}
}

void CFunction::FUN_UseAttackSkill(TAsmSkill tAsmSkill/*CString SkillName*/, TAsmMonster tAsmMonster)//使用攻击技能
{
	auto ARoleInfo = g_pAsmRole->GetRoleInfo();
	if (ARoleInfo.nState != 7)
	{
		FUN_UseMovAttackSkill(tAsmSkill, tAsmMonster);//使用攻击技能
	}
}

void CFunction::FUN_RoleHMProtection(CString szLp, CString szTypeName, int Per, CString szYaoNames)
{
	dbgPrint("FUN_RoleHMProtection szTypeName=%s Per=%d szYaoNames=%s", szTypeName, Per, szYaoNames);
	if (szLp == "速回复")
	{
		if (szTypeName == "血量")
		{
			_tstring szTemp = szYaoNames;
			auto  vString = UserSubMonsterName(szTemp, _T('|'));
			for (auto v : vString)
			{
				g_pUser->tHighProtect.vYaoName.push_back(v.c_str());
			}
			g_pUser->tHighProtect.nHpPer = Per;
		}
		else if (szTypeName == "蓝量")
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
	else if (szLp == "慢回复")
	{
		if (szTypeName == "血量")
		{
			_tstring szTemp = szYaoNames;
			auto  vString = UserSubMonsterName(szTemp, _T('|'));
			for (auto v : vString)
			{
				g_pUser->tLowProtect.vYaoName.push_back(v.c_str());
			}
			g_pUser->tLowProtect.nHpPer = Per;
		}
		else if (szTypeName == "蓝量")
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
	if (szTypeName == "血量")
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
	g_pHPInit->MySendGameInfo("自动组队");
	int timeOut = 0;
	CString temp;
	CString text;
	CString roleName;
	CStringArray teamArr;		//队员数组
	while (timeOut < 600) 
	{
		teamArr.RemoveAll();
		temp = g_pMsg->mGameInfo->Team;
		dbgPrint("组队 temp=%s", temp);
		//if ("" == temp) {
		//	return FALSE;		//队伍成员为空
		//} 

		SplitCStringArr(temp, "|", &teamArr);
		TAsmRoleInfo role = g_pAsmRole->GetRoleInfo();
		roleName = role.szName;
		dbgPrint("当前队伍成员:%s", temp);
		if (teamArr.GetCount() >= 2) {
			if (teamArr.GetAt(0) == roleName) {
				//是队长 需要邀请队员

				for (int i = 1; i < teamArr.GetSize(); i++)
				{
					text.Format("邀请:%s", teamArr.GetAt(i));
					g_pHPInit->MySendGameInfo(text);
					g_pAsmTeam->AsmInvaiteTeam(teamArr.GetAt(i));	//发出邀请
					Sleep(3000);
				}
			}
			else {
				//队员等待组队邀请
				if (g_pAsmTeam->GetInvite()) {
					Sleep(1000);
					g_pHPInit->MySendGameInfo("有组队邀请 进入队伍");
					g_pAsmTeam->TeamJoin();
					Sleep(2000);
				}
				else {
					Sleep(2000);
				}
			}
		}
		else {
			dbgPrint("队伍人数太少");
		}

		if (FUN_CheckTeam(nNum)) {
			g_pHPInit->MySendGameInfo("组队完成");
			return TRUE;
			//组队完成
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
	CStringArray teamArr;		//队员数组
	int checkCount = 0;			//队伍成员检查
	temp = g_pMsg->mGameInfo->Team;
	if ("" == temp) {
		return 0;		//队伍成员为空
	}

	SplitCStringArr(temp, "|", &teamArr);
	TAsmRoleInfo role = g_pAsmRole->GetRoleInfo();
	g_pAsmTeam->GetTeamInfo();

	if (teamArr.GetAt(0) == roleName) {
		//是队长需要检查队员是否到齐
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
		//队员需要检查队长是不是正确的
		for (auto itor = g_pAsmTeam->m_TeamArr.begin(); itor != g_pAsmTeam->m_TeamArr.end(); itor++)
		{
			if (teamArr.GetAt(0) == itor->szName) {
				return 1;
			}
		}
	}

	if (checkCount >= nNum) {
		return 1;	//人员到齐
	}

	return 0;
}