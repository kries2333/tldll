#include "pch.h"
#include "HPInit.h"
#include "Message.h"
#include "Me.h"
#include "CControlsCommon.h"
#include "Function.h"
#include <iostream>
using namespace std;

extern CMessage* g_pMsg;
extern CMe* g_pMe;

void CHPInit::MySendGameInfo(const char* message)
{
	//dbgPrint("д���ַ:%x ����:%x", g_pMsg->mGameInfo, message);

	strcpy_s(g_pMsg->mGameInfo->RoleState, message);
}

UINT CHPInit::Login_Thread()    //��¼�̺߳���
{
	MySendGameInfo("��ʼ��¼��Ϸ");
	CControlsCommon CControlsCommon;
	CString Account;
	CString Password;
	CString Server;
	CString retStr;
	CString LuaScript;
	//��ȡ�˺�
	Account = g_pMsg->mGameInfo->Account;
	Password = g_pMsg->mGameInfo->Password;
	Server = g_pMsg->mGameInfo->Server;
	LuaScript = g_pMsg->mGameInfo->LuaScript;
	dbgPrint("LuaScript=%s", LuaScript);

	if (g_pMsg->IsWindowShow("MiniMap")) {
		return  1;//��Ϸ���棬�Ѿ���¼�ɹ�)
	}

	for (INT i = 0; i < 30; i++)
	{
		g_pMsg->msg_dostring("setmetatable(_G, {__index = HuoDongRiCheng_Env});HuoDongRiCheng_next_click();");
		Sleep(1000);
		if (g_pMsg->IsWindowShow("LoginSelectServer"))//����ѡ���������
		{
			Sleep(4000);
			g_pMsg->SelectServer(Server);//ѡ�����
			Sleep(1000);
			break;
		}
		Sleep(2000);
	}
	/************************************************************************/
	/*     ��ʼ��¼����                                      */
	/************************************************************************/
	Sleep(2000);
	MySendGameInfo("���������˺����벢����");
	//�����˺ţ����룬ȷ��������Ϸ
	LoginPassWord(Account, Password, "�ֻ������¼");//�����ʺţ�ѡ���׺
	Sleep(2000);
Login:
	g_pMsg->msg_dostring("setmetatable(_G, { __index = LoginLogOn_Env }); LogOn_CheckAccount();");
	Sleep(2000);
	bool isSelectRole = g_pMsg->IsWindowShow("LoginSelectRole");//ѡ���ɫ
	bool isCreateRole = g_pMsg->IsWindowShow("LoginCreateRole");//������ɫ
	bool isUserPwdError = false;  // CControlsCommon.isVisiableControl("SelectServerQuest_LostPassword");//�˺��������
	bool isFangChenMi_MiBao = CControlsCommon.isVisiableControl("FangChenMi_Frame_1");//������FangChenMi_Frame_1
	bool isInputMiBao = CControlsCommon.isVisiableControl("Pswprotect_Function_Frame");//�ܱ��������
	bool isLoginException = false;//_CCLoginState.IsExceptionUI();//��¼�쳣
	retStr = g_pMsg->msg_getstring("MyMonCanCommitString", "setmetatable(_G, {__index = LoginSelectServerQuest_Env});if this:IsVisible() then MyMonCanCommitString = SelectServerQuest_InfoWindow:GetText() else MyMonCanCommitString = \"-1\" end;").c_str();
	if (retStr.Find("�����Ƿ�������ȷ") > -1) {
		isUserPwdError = true;
	}
	if (retStr.Find("���ӷ�������ʱ") > -1) {
		isLoginException = true;
	}
	bool isPaiDui = g_pMsg->IsQueueUp(retStr);//�Ŷ�
	bool isPaiDuiMan = g_pMsg->IsQueueUpFull();//�Ŷ���


	bool isRungMiBao = false;
	int TimeSleep = 0;//240�볬ʱ
	while (isSelectRole == false && isCreateRole == false && TimeSleep < 240)
	{
		//�����ж����Ƿ���ڷ����Խ��棬������ڣ������ť
		if (isFangChenMi_MiBao)
		{
			g_pMsg->msg_dostring("setmetatable(_G, {__index = FangChenMi_Env});FangChenMi_Accept_Clicked();");
		}
		//if (isInputMiBao)
		//{
		//	//ȡ���ܱ�δ֪,���͵������
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
		//	//����ȫ�ֱ���Ϊ���������ܱ���Ȼ��ȴ����ؽ��������������ڴ��ڣ��ٴ�����
		//}
		//if (isPaiDui || isPaiDuiMan)
		//{
		if (isPaiDui)
		{
			MySendGameInfo("�����Ŷ�");
			break;
		}
		else if (isPaiDuiMan)
		{
			MySendGameInfo("�����Ŷ���������,��¼����");
			return 0;
		}
		if (isUserPwdError)
		{
			MySendGameInfo("�˺��������");
			return 0;
		}
		if (isLoginException)
		{
			MySendGameInfo("����,�޷�����");
			return 0;
		}
		Sleep(1000);
		TimeSleep++;
		isPaiDui = g_pMsg->IsQueueUp(retStr);//�Ŷ�
		isPaiDuiMan = g_pMsg->IsQueueUpFull();//�Ŷ���
		//isUserPwdError = CControlsCommon.isVisiableControl("SelectServerQuest_LostPassword");//�˺��������
		retStr = g_pMsg->msg_getstring("MyMonCanCommitString", "setmetatable(_G, {__index = LoginSelectServerQuest_Env});if this:IsVisible() then MyMonCanCommitString = SelectServerQuest_InfoWindow:GetText() else MyMonCanCommitString = \"-1\" end;").c_str();
		if (retStr.Find("�����Ƿ�������ȷ") > -1) {
			isUserPwdError = true;
		}
		if (retStr.Find("���ӷ�������ʱ") > -1) {
			isLoginException = true;
		}
		if (retStr.Find("����������") > -1) {
			//�رհ�ť �����и�lua����
			g_pMsg->telua_dostring("setmetatable(_G, {__index = LoginSelectServerQuest_Env}); SelectServerQuest_Bn1Click();");
			goto Login;
		}
		if (retStr.Find("����Ƶ��") > -1) {
			//�رհ�ť �����и�lua����
			Sleep(5000);
			g_pMsg->telua_dostring("setmetatable(_G, {__index = LoginSelectServerQuest_Env}); SelectServerQuest_Bn1Click();");
			goto Login;
		}
		isInputMiBao = CControlsCommon.isVisiableControl("Pswprotect_Function_Frame");//�ܱ��������
		isFangChenMi_MiBao = CControlsCommon.isVisiableControl("FangChenMi_Frame_1");//������FangChenMi_Frame_1
		isSelectRole = g_pMsg->IsWindowShow("LoginSelectRole");//ѡ���ɫ
		isCreateRole = g_pMsg->IsWindowShow("LoginCreateRole");//������ɫ
		//isLoginException = _CCLoginState.IsExceptionUI();//��¼�쳣
		dbgPrint("ѭ���жϵ�¼�쳣");
	}
	dbgPrint("��ʼѡ���ɫ");

	while (isPaiDui)//һֱ�Ȱ�
	{
		Sleep(1000);
		isPaiDui = g_pMsg->IsQueueUp(retStr);//�Ŷ�
	}
	Sleep(1000);//����������ת��ʱ��

	if (isSelectRole == false && isCreateRole == false)
	{
		//��ʱ��
		//���ͳ�ʱ��Ϣ
		MySendGameInfo("����ѡ���ߴ�����ɫ���泬ʱ");
		return 0;
	}
	else
	{
		bool isGameUI = g_pMsg->IsWindowShow("MiniMap");//��Ϸ���棬�Ѿ���¼�ɹ�
		int TimeSleepEX = 0;//120�볬ʱ
		while (isGameUI == false && TimeSleepEX < 120)
		{
			isSelectRole = g_pMsg->IsWindowShow("LoginSelectRole");//ѡ���ɫ
			isCreateRole = g_pMsg->IsWindowShow("LoginCreateRole");//������ɫ
			if (isSelectRole)//���뵽ѡ�����
			{
				//��������ѡ���ɫ
				MySendGameInfo("ѡ���ɫ������");
				g_pMsg->msg_dostring("setmetatable(_G, { __index = SelectRole_Env });SelectRole_DoubleClicked(tonumber(%d));", 0);//ѡ���ɫ
				Sleep(1000);
				g_pMsg->msg_dostring("setmetatable(_G, { __index = ActionSkill_Env }); SelectRole_EnterGame();");//ѡ���ɫ���ȷ��
				Sleep(1000);
				g_pMsg->msg_dostring("setmetatable(_G, { __index = LogonList_Env }); LogOnlist_Choose_Click();");//ѡ����·
				Sleep(5000);

					//�ȴ�������Ϸ,��ʱ2����(����������֤�ģ����ɿ���̨�������,�ȴ�������Ϸ�ͺ��ˣ����һֱδ���룬����ʾ��¼��ʱ)
			}
			else if (isCreateRole)//Ŀǰû�н�ɫ����Ҫ����
			{
				MySendGameInfo("���ֶ�������ɫ�����");
				Sleep(2000);
				TimeSleepEX = 0;
		
			//	MySendGameInfo("������ɫ");
			//	g_pMsg->msg_dostring("setmetatable(_G, {__index = LoginSelectServerQuest_Env}); SelectServerQuest_Bn1Click();");//ѡ���ɫ���ȷ��
			//	Sleep(500);
			//	//��д��ɫ�����ȴ�������Ϸ����
			//Renamed:
			//	g_pMsg->SetNameText("����Һڼ�");
			//	Sleep(1000);
			//	g_pMsg->msg_dostring("setmetatable(_G, {__index = LoginSelectServerQuest_Env}); SelectServerQuest_Bn1Click();");//ѡ���ɫ���ȷ��
			//	Sleep(500);
			//	g_pMsg->msg_dostring("setmetatable(_G, {__index = LoginCreateRole_Env}); CreateRole_BnSelFace2();");//ѡ��ͷ��
			//	Sleep(500);
			//	g_pMsg->msg_dostring("setmetatable(_G, {__index = LoginCreateRole_Env}); CreateRole_BnClickCreateRole();");//ȷ������
			//	Sleep(1500);
			//	retStr = g_pMsg->msg_getstring("MyMonCanCommitString", "setmetatable(_G, {__index = LoginSelectServerQuest_Env});if this:IsVisible() then MyMonCanCommitString = SelectServerQuest_InfoWindow:GetText() else MyMonCanCommitString = \"-1\" end;").c_str();
			//	if (retStr.Find("�����Ѿ�����") > -1 || retStr.Find("�Դ�") > -1) {
			//		g_pMsg->msg_dostring("setmetatable(_G, {__index = LoginCreateRole_Env}); CreateRole_Name:SetText(\"\")");//ѡ���ɫ���ȷ��
			//		Sleep(500);
			//		g_pMsg->msg_dostring("setmetatable(_G, {__index = LoginSelectServerQuest_Env}); SelectServerQuest_Bn1Click();");//ѡ���ɫ���ȷ��
			//		goto Renamed;  //��Ҫ������������ 
			//	}

			}
			//�����ж����Ƿ���ڷ����Խ��棬������ڣ������ť
			Sleep(1000);
			TimeSleepEX = TimeSleepEX + 1;
			isGameUI = g_pMsg->IsWindowShow("MiniMap");//��Ϸ���棬�Ѿ���¼�ɹ�
		}
		if (!isGameUI)
		{
			MySendGameInfo("������Ϸ��ʱ");
		}
		else
		{
			MySendGameInfo("�ѵ�¼");
			return 1;
		}
	}
	return 0;
}


bool CHPInit::LoginPassWord(const char* UserName, const char* UserKey, const char* other)//�����ʺ�����
{
	char* TailName[] = {
		"@changyou.com",
		"@game.sohu.com",
		"@Sohu.com",
		"@chinaren.com",
		"@sogou.com",
		"@17173.com",
		"�ֻ������¼",
		"���������˺ź�׺"
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