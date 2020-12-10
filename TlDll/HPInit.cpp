#include "pch.h"
#include "HPInit.h"
#include "Message.h"
#include "Main.h"
#include "CControlsCommon.h"
#include "Function.h"
#include <iostream>
using namespace std;

extern CMessage* g_pMsg;
extern CMain* g_pMain;

void CHPInit::MySendGameInfo(const char* message)
{
	//dbgPrint("写入地址:%x 内容:%x", g_pMsg->mGameInfo, message);

	strcpy_s(g_pMsg->mGameInfo->RoleState, message);
}

UINT CHPInit::Login_Thread()    //登录线程函数
{
	MySendGameInfo("开始登录游戏");
	CControlsCommon CControlsCommon;
	CString Account;
	CString Password;
	CString Server;
	CString retStr;
	CString LuaScript;
	//获取账号
	Account = g_pMsg->mGameInfo->Account;
	Password = g_pMsg->mGameInfo->Password;
	Server = g_pMsg->mGameInfo->Server;
	LuaScript = g_pMsg->mGameInfo->LuaScript;
	dbgPrint("LuaScript=%s", LuaScript);

	if (g_pMsg->IsWindowShow("MiniMap")) {
		return  1;//游戏界面，已经登录成功)
	}

	for (INT i = 0; i < 30; i++)
	{
		g_pMsg->msg_dostring("setmetatable(_G, {__index = HuoDongRiCheng_Env});HuoDongRiCheng_next_click();");
		Sleep(1000);
		if (g_pMsg->IsWindowShow("LoginSelectServer"))//进入选择大区界面
		{
			Sleep(4000);
			g_pMsg->SelectServer(Server);//选择大区
			Sleep(1000);
			break;
		}
		Sleep(2000);
	}
	/************************************************************************/
	/*     开始登录操作                                      */
	/************************************************************************/
	Sleep(2000);
	MySendGameInfo("正在输入账号密码并进入");
	//输入账号，密码，确定进入游戏
	LoginPassWord(Account, Password, "手机号码登录");//输入帐号，选择后缀
	Sleep(2000);
Login:
	g_pMsg->msg_dostring("setmetatable(_G, { __index = LoginLogOn_Env }); LogOn_CheckAccount();");
	Sleep(2000);
	bool isSelectRole = g_pMsg->IsWindowShow("LoginSelectRole");//选择角色
	bool isCreateRole = g_pMsg->IsWindowShow("LoginCreateRole");//创建角色
	bool isUserPwdError = false;  // CControlsCommon.isVisiableControl("SelectServerQuest_LostPassword");//账号密码错误
	bool isFangChenMi_MiBao = CControlsCommon.isVisiableControl("FangChenMi_Frame_1");//防沉迷FangChenMi_Frame_1
	bool isInputMiBao = CControlsCommon.isVisiableControl("Pswprotect_Function_Frame");//密保输入界面
	bool isLoginException = false;//_CCLoginState.IsExceptionUI();//登录异常
	retStr = g_pMsg->msg_getstring("MyMonCanCommitString", "setmetatable(_G, {__index = LoginSelectServerQuest_Env});if this:IsVisible() then MyMonCanCommitString = SelectServerQuest_InfoWindow:GetText() else MyMonCanCommitString = \"-1\" end;").c_str();
	if (retStr.Find("密码是否输入正确") > -1) {
		isUserPwdError = true;
	}
	if (retStr.Find("连接服务器超时") > -1) {
		isLoginException = true;
	}
	bool isPaiDui = g_pMsg->IsQueueUp(retStr);//排队
	bool isPaiDuiMan = g_pMsg->IsQueueUpFull();//排队满


	bool isRungMiBao = false;
	int TimeSleep = 0;//240秒超时
	while (isSelectRole == false && isCreateRole == false && TimeSleep < 240)
	{
		//这里判断下是否存在防沉迷界面，如果存在，点击按钮
		if (isFangChenMi_MiBao)
		{
			g_pMsg->msg_dostring("setmetatable(_G, {__index = FangChenMi_Env});FangChenMi_Accept_Clicked();");
		}
		//if (isInputMiBao)
		//{
		//	//取出密保未知,发送到服务端
		//	CString _MiBaoText1;
		//	CString _MiBaoText2;
		//	CString _MiBaoText3;
		//	//g_pMsg->GetMiBaoText(_MiBaoText1, _MiBaoText2, _MiBaoText3);
		//	//if (!_MiBaoText1.IsEmpty() && !_MiBaoText2.IsEmpty() && !_MiBaoText3.IsEmpty())
		//	//{
		//	//	if (isRungMiBao == false)
		//	//	{
		//	//		isRungMiBao = true;
		//	//		GetMibao(_MiBaoText1, _MiBaoText1);
		//	//		GetMibao(_MiBaoText2, _MiBaoText2);
		//	//		GetMibao(_MiBaoText3, _MiBaoText3);
		//	//	}
		//	//}
		//	//设置全局变量为正在输入密保，然后等待返回结果，如果输入完在存在，再次设置
		//}
		//if (isPaiDui || isPaiDuiMan)
		//{
		if (isPaiDui)
		{
			MySendGameInfo("正在排队");
			break;
		}
		else if (isPaiDuiMan)
		{
			MySendGameInfo("在线排队人数已满,登录结束");
			return 0;
		}
		if (isUserPwdError)
		{
			MySendGameInfo("账号密码错误");
			return 0;
		}
		if (isLoginException)
		{
			MySendGameInfo("错误,无法连接");
			return 0;
		}
		Sleep(1000);
		TimeSleep++;
		isPaiDui = g_pMsg->IsQueueUp(retStr);//排队
		isPaiDuiMan = g_pMsg->IsQueueUpFull();//排队满
		//isUserPwdError = CControlsCommon.isVisiableControl("SelectServerQuest_LostPassword");//账号密码错误
		retStr = g_pMsg->msg_getstring("MyMonCanCommitString", "setmetatable(_G, {__index = LoginSelectServerQuest_Env});if this:IsVisible() then MyMonCanCommitString = SelectServerQuest_InfoWindow:GetText() else MyMonCanCommitString = \"-1\" end;").c_str();
		if (retStr.Find("密码是否输入正确") > -1) {
			isUserPwdError = true;
		}
		if (retStr.Find("连接服务器超时") > -1) {
			isLoginException = true;
		}
		if (retStr.Find("已有人物在") > -1) {
			//关闭按钮 好像有个lua调用
			g_pMsg->telua_dostring("setmetatable(_G, {__index = LoginSelectServerQuest_Env}); SelectServerQuest_Bn1Click();");
			goto Login;
		}
		if (retStr.Find("过于频繁") > -1) {
			//关闭按钮 好像有个lua调用
			Sleep(5000);
			g_pMsg->telua_dostring("setmetatable(_G, {__index = LoginSelectServerQuest_Env}); SelectServerQuest_Bn1Click();");
			goto Login;
		}
		isInputMiBao = CControlsCommon.isVisiableControl("Pswprotect_Function_Frame");//密保输入界面
		isFangChenMi_MiBao = CControlsCommon.isVisiableControl("FangChenMi_Frame_1");//防沉迷FangChenMi_Frame_1
		isSelectRole = g_pMsg->IsWindowShow("LoginSelectRole");//选择角色
		isCreateRole = g_pMsg->IsWindowShow("LoginCreateRole");//创建角色
		//isLoginException = _CCLoginState.IsExceptionUI();//登录异常
		dbgPrint("循环判断登录异常");
	}
	dbgPrint("开始选择角色");

	while (isPaiDui)//一直等吧
	{
		Sleep(1000);
		isPaiDui = g_pMsg->IsQueueUp(retStr);//排队
	}
	Sleep(1000);//留出界面跳转的时间

	if (isSelectRole == false && isCreateRole == false)
	{
		//超时了
		//发送超时消息
		MySendGameInfo("进入选或者创建角色界面超时");
		return 0;
	}
	else
	{
		bool isGameUI = g_pMsg->IsWindowShow("MiniMap");//游戏界面，已经登录成功
		int TimeSleepEX = 0;//120秒超时
		while (isGameUI == false && TimeSleepEX < 120)
		{
			isSelectRole = g_pMsg->IsWindowShow("LoginSelectRole");//选择角色
			isCreateRole = g_pMsg->IsWindowShow("LoginCreateRole");//创建角色
			if (isSelectRole)//进入到选择界面
			{
				//根据索引选择角色
				MySendGameInfo("选择角色并进入");
				g_pMsg->msg_dostring("setmetatable(_G, { __index = SelectRole_Env });SelectRole_DoubleClicked(tonumber(%d));", 0);//选择角色
				Sleep(1000);
				g_pMsg->msg_dostring("setmetatable(_G, { __index = ActionSkill_Env }); SelectRole_EnterGame();");//选择角色后的确定
				Sleep(1000);
				g_pMsg->msg_dostring("setmetatable(_G, { __index = LogonList_Env }); LogOnlist_Choose_Click();");//选择线路
				Sleep(5000);

					//等待进入游戏,超时2分钟(输入数字验证的，交由控制台来完成了,等待进入游戏就好了，如果一直未进入，则提示登录超时)
			}
			else if (isCreateRole)//目前没有角色，需要创建
			{
				MySendGameInfo("当前无角色请勾选创建");
				return 0;
		
			//	MySendGameInfo("创建角色");
			//	g_pMsg->msg_dostring("setmetatable(_G, {__index = LoginSelectServerQuest_Env}); SelectServerQuest_Bn1Click();");//选择角色后的确定
			//	Sleep(500);
			//	//填写角色名，等待进入游戏界面
			//Renamed:
			//	g_pMsg->SetNameText("梦想家黑鸡");
			//	Sleep(1000);
			//	g_pMsg->msg_dostring("setmetatable(_G, {__index = LoginSelectServerQuest_Env}); SelectServerQuest_Bn1Click();");//选择角色后的确定
			//	Sleep(500);
			//	g_pMsg->msg_dostring("setmetatable(_G, {__index = LoginCreateRole_Env}); CreateRole_BnSelFace2();");//选择头像
			//	Sleep(500);
			//	g_pMsg->msg_dostring("setmetatable(_G, {__index = LoginCreateRole_Env}); CreateRole_BnClickCreateRole();");//确定创建
			//	Sleep(1500);
			//	retStr = g_pMsg->msg_getstring("MyMonCanCommitString", "setmetatable(_G, {__index = LoginSelectServerQuest_Env});if this:IsVisible() then MyMonCanCommitString = SelectServerQuest_InfoWindow:GetText() else MyMonCanCommitString = \"-1\" end;").c_str();
			//	if (retStr.Find("名字已经存在") > -1 || retStr.Find("言辞") > -1) {
			//		g_pMsg->msg_dostring("setmetatable(_G, {__index = LoginCreateRole_Env}); CreateRole_Name:SetText(\"\")");//选择角色后的确定
			//		Sleep(500);
			//		g_pMsg->msg_dostring("setmetatable(_G, {__index = LoginSelectServerQuest_Env}); SelectServerQuest_Bn1Click();");//选择角色后的确定
			//		goto Renamed;  //需要重新生成名字 
			//	}

			}
			//这里判断下是否存在防沉迷界面，如果存在，点击按钮
			Sleep(1000);
			TimeSleepEX = TimeSleepEX + 1;
			isGameUI = g_pMsg->IsWindowShow("MiniMap");//游戏界面，已经登录成功
		}
		if (!isGameUI)
		{
			MySendGameInfo("进入游戏超时");
		}
		else
		{
			MySendGameInfo("已登录");
		}
	}
	return 0;
}


bool CHPInit::LoginPassWord(const char* UserName, const char* UserKey, const char* other)//输入帐号密码
{
	char* TailName[] = {
		"@changyou.com",
		"@game.sohu.com",
		"@Sohu.com",
		"@chinaren.com",
		"@sogou.com",
		"@17173.com",
		"手机号码登录",
		"输入其他账号后缀"
	};
	int nTail = -1;
	for (size_t i = 0; i < 8; i++)
	{
		if ((string)TailName[i] == (string)other)
		{
			nTail = i;
			break;
		}
	}
	if (nTail != -1 && !((string)UserName).empty() && ((string)UserKey).length() != 0)
	{
		g_pMsg->msg_dostring("LoginPassWord(\"%s\",%d)", UserName, nTail);
		for (size_t i = NULL; i < strlen(UserKey); i++)
		{
			::SendMessage(g_pMsg->GamehWnd, WM_IME_CHAR, (WPARAM)UserKey[i], 1);
			Sleep(10);
		}
		return true;
	}


	return false;
}