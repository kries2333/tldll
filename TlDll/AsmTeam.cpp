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
    dbgPrint("�������");
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
    dbgPrint("Call��ַ����%x�� Ecx����%x�� ������ݣ���%x��", TeamCall, TeamEcx, TeamData);
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
        //	+ 0x0 �̶���ֵһ������
        //	+ 0x4 �̶���ֵΪ0
        //	+ 0x8 �̶���ֵΪ0
        //	+ 0xC �̶���ֵΪ - 1
        //	+ 0x10λ�� ������ID
        //	+ 0x14λ�� �Ǳ����������ֳ���
        //  + 0x15λ�� �Ǳ������˵�����
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
        //	dbgPrint("�������:%02X", Data[i]);
        //}
        _asm {//�������
            mov ecx,[TeamEcx]
            push TeamData1
            mov eax,TeamCall
            call eax
        }

        VirtualFree(Data, 40, MEM_COMMIT);
    }
    catch (const std::exception&)
    {
        dbgPrint("����쳣");
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

        int teamCount = g_pMsg->msg_getnumber("g_GetValue = DataPool:GetTeamMemberCount();");//��ȡ��������

        if (teamCount <= 0) {
            return; //û�ж���ֱ���˳�
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
        dbgPrint("�����Ϣ��ȡʧ��");
    }


    return;
}

void CAsmTeam::TeamJoin()
{
    //setmetatable(_G, {__index = Team_Frame_Env}); Team_Button_Frame5_Click();
    //���������
    //g_pMsg->msg_dostring("setmetatable(_G, {__index = Team_Frame_Env}); Team_Button_Frame5_Click();");//�������
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

    dbgPrint("�������Call��ַ����%x�� Ecx����%x�� ������ݣ���%x�� �����ţ���%x��", TeamCall, TeamEcx, TeamData, RoleInfo.nRoleCode);
    if (TeamCall == 0 || TeamEcx == 0 || TeamData == 0) return;

    try
    {
        BYTE* Data = (BYTE*)VirtualAlloc(NULL, 32, MEM_COMMIT, PAGE_READWRITE);
        if (NULL == Data) {
            return;
        }
        ZeroMemory(Data, 32);
        //00FDEBFC  00D39C98  Game.00D39C98 ����
        //00FDEC00  00000000 �̶�0
        //00FDEC04  00000000 �̶�0
        //00FDEC08  FFFFFFFF �̶� - 1
        //00FDEC0C  00000001 �̶�1
        //00FDEC10  0D1E33BF ������
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
        	dbgPrint("�������:%02X", Data[i]);
        }
        _asm {//�������
            mov ecx,TeamEcx
            push TeamData1
            mov eax,TeamCall
            call eax
        }

        VirtualFree(Data, 24, MEM_COMMIT);
    }
    catch (const std::exception&)
    {
        dbgPrint("�������ʧ��");
    }

}

void CAsmTeam::TeamLeave()
{
    g_pMsg->msg_dostring("setmetatable(_G, { __index = Team_Frame_Env }); Team_Button_Frame4_Click();"); //�뿪����
}

void CAsmTeam::TeamFollow()
{
    g_pMsg->msg_dostring("setmetatable(_G, { __index = Team_Frame_Env }); Team_Button_Team_Follow_Click();"); //����������
    
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
        dbgPrint("�����Ϣ��ȡʧ��");
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
        dbgPrint("�����Ϣ��ȡʧ��");
    }
}
