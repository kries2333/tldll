#include "pch.h"
#include "AsmTeam.h"
#include "AsmTeam.h"
#include "Function.h"
#include "Message.h"
#include "Base.h"
#include "AsmRole.h"

extern CMessage* g_pMsg;
extern CAsmRole* g_pAsmRole;

void CAsmTeam::AsmInvaiteTeam(CString Name)
{
    dbgPrint("邀请组队");
    if (Name.IsEmpty()) return;
    auto RoleInfo = g_pAsmRole->GetRoleInfo();

    if (g_GameExeBase == 0) return;

    DWORD TeamCall;
    DWORD TeamEcx;
    DWORD TeamData;
    DWORD TeamData1;
    int TeamData2;
    TeamCall = (DWORD)(g_GameExeBase + TEAM_CALL);
    TeamEcx = (DWORD)(g_GameExeBase + TEAM_ECX);
    TeamEcx = *(DWORD*)(TeamEcx);
    TeamData = (DWORD)(g_GameExeBase + TEAM_DATA1);
    dbgPrint("Call地址：【%x】 Ecx：【%x】 封包数据：【%x】", TeamCall, TeamEcx, TeamData);
    if (TeamCall == 0 || TeamEcx == 0 || TeamData == 0) return;
    try
    {
        BYTE* Data = (BYTE*)VirtualAlloc(NULL, 40, MEM_COMMIT, PAGE_READWRITE);
        if (NULL == Data) {
            return;
        }
        BYTE NameLen;
        NameLen = (BYTE)strlen(Name.GetBuffer());
        ZeroMemory(Data, 40);
        //	+ 0x0 固定赋值一个对象
        //	+ 0x4 固定赋值为0
        //	+ 0x8 固定赋值为0
        //	+ 0xC 固定赋值为 - 1
        //	+ 0x10位置 是人物ID
        //	+ 0x14位置 是被邀请人名字长度
        //  + 0x15位置 是被邀请人的名字
        memcpy(Data, &TeamData, 4);
        TeamData2 = 0;
        memcpy(Data + 0x4, &TeamData2, 4);
        TeamData2 = 0;
        memcpy(Data + 0x8, &TeamData2, 4);
        TeamData2 = -1;
        memcpy(Data + 0xC, &TeamData2, 4);
        memcpy(Data + 0x10, &RoleInfo.nRoleID, 4);
        memcpy(Data + 0x14, &NameLen, 1);
        memcpy(Data + 0x15, Name.GetBuffer(), NameLen);
        TeamData1 = (DWORD)Data;
        //for (size_t i = 0; i < 40; i++)
        //{
        //	dbgPrint("封包数据:%02X", Data[i]);
        //}
        _asm {//邀请组队
            mov ecx,[TeamEcx]
            push TeamData1
            mov eax,TeamCall
            call eax
        }

        VirtualFree(Data, 40, MEM_COMMIT);
    }
    catch (const std::exception&)
    {
        dbgPrint("组包异常");
        return;
    }
}

void CAsmTeam::GetTeamInfo()
{
    DWORD Data;
    DWORD Obj;
    TAsmTempInfo tempInfo;
    if (g_GameExeBase == 0) return;

    try
    {
        Obj = *(DWORD*)(g_GameExeBase + TEAM_INFO);
        Obj = *(DWORD*)(Obj + 0xBC);

        int teamCount = g_pMsg->msg_getnumber("g_GetValue = DataPool:GetTeamMemberCount();");//获取所有数量

        if (teamCount <= 0) {
            return; //没有队伍直接退出
        }

        for (int i = 0; i < teamCount; i++)
        {
            Data = *(DWORD*)(Obj + i*0x4);
            if (IsBadReadPtr((char*)(Data + 0x10), 4) == 0)
                tempInfo.szName = (char*)(Data + 0x10);
            if (IsBadReadPtr((int*)(Data + 0x4), 4) == 0)
                tempInfo.nRoleID = *(int*)(Data + 0x4);
            if (IsBadReadPtr((int*)(Data + 0x38), 4) == 0)
                tempInfo.nLevel = *(int*)(Data + 0x38);
            if (IsBadReadPtr((int*)(Data + 0x30), 4) == 0)
                tempInfo.nMenpai = *(int*)(Data + 0x30);
            if (IsBadReadPtr((float*)(Data + 0x40), 4) == 0)
                tempInfo.nPosX = *(float*)(Data + 0x40);
            if (IsBadReadPtr((float*)(Data + 0x44), 4) == 0)
                tempInfo.nPosY = *(float*)(Data + 0x44);
            if (IsBadReadPtr((int*)(Data + 0x48), 4) == 0)
                tempInfo.nHP = *(int*)(Data + 0x48);
            if (IsBadReadPtr((int*)(Data + 0x4C), 4) == 0)
                tempInfo.nMPMax = *(int*)(Data + 0x4C);
            if (IsBadReadPtr((int*)(Data + 0x50), 4) == 0)
                tempInfo.nMP = *(int*)(Data + 0x50);
            if (IsBadReadPtr((int*)(Data + 0x54), 4) == 0)
                tempInfo.nMPMax = *(int*)(Data + 0x54);
            m_TeamArr.push_back(tempInfo);
        }
    }
    catch (const std::exception&)
    {
        dbgPrint("组队信息获取失败");
    }


    return;
}

void CAsmTeam::TeamJoin()
{
    //setmetatable(_G, {__index = Team_Frame_Env}); Team_Button_Frame5_Click();
    //加入队伍封包
    //g_pMsg->msg_dostring("setmetatable(_G, {__index = Team_Frame_Env}); Team_Button_Frame5_Click();");//加入队伍
    auto RoleInfo = g_pAsmRole->GetRoleInfo();

    if (g_GameExeBase == 0) return;
     
    DWORD TeamCall;
    DWORD TeamEcx;
    DWORD TeamData;
    DWORD TeamData1;
    int TeamData2;
    TeamCall = (DWORD)(g_GameExeBase + TEAM_CALL);
    TeamEcx = (DWORD)(g_GameExeBase + TEAM_ECX);
    TeamEcx = *(DWORD*)(TeamEcx);
    TeamData = (DWORD)(g_GameExeBase + TEAM_JOIN_DATA);

    dbgPrint("进入队伍Call地址：【%x】 Ecx：【%x】 封包数据：【%x】 人物编号：【%x】", TeamCall, TeamEcx, TeamData, RoleInfo.nRoleCode);
    if (TeamCall == 0 || TeamEcx == 0 || TeamData == 0) return;

    try
    {
        BYTE* Data = (BYTE*)VirtualAlloc(NULL, 32, MEM_COMMIT, PAGE_READWRITE);
        if (NULL == Data) {
            return;
        }
        ZeroMemory(Data, 32);
        //00FDEBFC  00D39C98  Game.00D39C98 对象
        //00FDEC00  00000000 固定0
        //00FDEC04  00000000 固定0
        //00FDEC08  FFFFFFFF 固定 - 1
        //00FDEC0C  00000001 固定1
        //00FDEC10  0D1E33BF 人物编号
        memcpy(Data, &TeamData, 4);
        TeamData2 = 0;
        memcpy(Data + 0x4, &TeamData2, 4);
        TeamData2 = 0;
        memcpy(Data + 0x8, &TeamData2, 4);
        TeamData2 = -1;
        memcpy(Data + 0xC, &TeamData2, 4);
        TeamData2 = 1;
        memcpy(Data + 0x10, &TeamData2, 4);
        TeamData2 = RoleInfo.nRoleCode - 1;
        memcpy(Data + 0x14, &TeamData2, 4);

        TeamData1 = (DWORD)Data;
        for (size_t i = 0; i < 32; i++)
        {
        	dbgPrint("封包数据:%02X", Data[i]);
        }
        _asm {//邀请组队
            mov ecx,TeamEcx
            push TeamData1
            mov eax,TeamCall
            call eax
        }

        VirtualFree(Data, 24, MEM_COMMIT);
    }
    catch (const std::exception&)
    {
        dbgPrint("加入队伍失败");
    }

}

void CAsmTeam::TeamLeave()
{
    g_pMsg->msg_dostring("setmetatable(_G, { __index = Team_Frame_Env }); Team_Button_Frame4_Click();"); //离开队伍
}

void CAsmTeam::TeamFollow()
{
    g_pMsg->msg_dostring("setmetatable(_G, { __index = Team_Frame_Env }); Team_Button_Team_Follow_Click();"); //发起队伍跟随
    
}

bool CAsmTeam::GetTeamExist()
{
    DWORD Data;
    DWORD Obj;
    TAsmTempInfo tempInfo;
    if (g_GameExeBase == 0) return false;
    try
    {
        Obj = *(DWORD*)(g_GameExeBase + TEAM_STATUS);
        Data = *(DWORD*)(Obj + 0xAC);
        if (1 == Data)
            return true;
        else
            return false;

    }
    catch (const std::exception&)
    {
        dbgPrint("组队信息获取失败");
    }
}

bool CAsmTeam::GetInvite()
{
    DWORD Data;
    DWORD Obj;
    TAsmTempInfo tempInfo;
    if (g_GameExeBase == 0) return false;

    try
    {
        Obj = *(DWORD*)(g_GameExeBase + TEAM_INFO);
        Data = *(DWORD*)(Obj + 0xFC);
        if (1 == Data) 
            return true;
        else
            return false;

    }
    catch (const std::exception&)
    {
        dbgPrint("组队信息获取失败");
    }
}
