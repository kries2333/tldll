#include "pch.h"
#include "User.h"
#include "CGUI.h"
#include "AsmSkill.h"
#include "Function.h"
#include "AsmRole.h"
#include "FileSystem.h"

extern CGUI *g_pUI;
extern CAsmSkill* g_pAsmSkill;
extern CAsmRole* g_pAsmRole;
extern CFileSystem* g_pFileSystem;

_tstring Os_stringOferase(_tstring strSource, _tstring strErase)//�Ƴ�ָ���ַ������ַ���
{
	_tstring strOk = strSource;
	_tstring::size_type pos = 0;

	//�������ַ����е�һ����str�е�ĳ���ַ�ƥ����ַ�����������λ�á�
	//������index��ʼ�����û�ҵ��ͷ���string::npos ,���������0��ʼ
	while ((pos = strOk.find_first_of(strErase, 0)) != _tstring::npos)
		strOk.erase(pos, 1);//ȥ��ҪĨ�����ַ����ִ�

	return strOk;
}

//�ַ�����ȡ
VString stringIntercept(_tstring strSource, _tstring strBegin, int offset, _tstring strEnd)
{
	VString tempStr;
	for (int i = 0; i < strSource.length(); i++)
	{
		int nFindStartIndex = strSource.find(strBegin);
		int nFindEndIndex = strSource.find(strEnd);
		if (nFindStartIndex == -1 && nFindEndIndex == -1)
		{
			break;
		}
		_tstring temp = strSource.substr(nFindStartIndex + offset, nFindEndIndex - (nFindStartIndex + offset));
		tempStr.push_back(temp);
		dbgPrint("stringIntercept temp=%s", temp.c_str());
		strSource = strSource.substr(nFindEndIndex + 1, strSource.length() - nFindEndIndex - 1);
	}

	return tempStr;
}

VUserMonsterName UserSubMonsterName(_tstring strSource, TCHAR strErase)//��ȡ����������������ʱ��������
{
	VUserMonsterName vUserMonsterName;//��ʱ����
	_tstring strName = strSource;
	strName = Os_stringOferase(strName, _T("\t\r\n"));//�Ƴ���Щ�ַ�

	if (!strName.empty())
	{
		if (strName[strName.length() - 1] != strErase)//������һ���ַ����Ƕ��ţ�����ĩβ׷��һ��
			strName += strErase;

		_tstring::size_type pos = 0; int pos1 = 0;
		while ((pos = strName.find_first_of(strErase, pos1)) != _tstring::npos)//����","��ȡ�ַ���
		{
			vUserMonsterName.push_back(strName.substr(pos1, pos - pos1));//����ȡ���Ӵ���������

			if ((strName.size() - 1) == pos)
				break;

			pos1 = pos + 1;
		}
	}

	return vUserMonsterName;
}

void CUser::UserSystemInitial() //��ʼ���û����ý���
{
	tPuTongSkill.fMax = 2.0;
	tPuTongSkill.nSkillId = 0;
	vUserSkill = UserGetSkill(); //�û�����
	vtKillPos = UserGetPos();//�û���ȡ�һ�������
}

VUserPos CUser::UserGetPos()//�û���ȡ�һ�������
{
	VUserPos vUserPos;

	int nCount = g_pUI->m_Page1.m_KillPos.GetItemCount();
	for (INT i = 0; i < nCount; i++)
	{
		TUserPos tUserPos;

		CString strTemp;
		strTemp = g_pUI->m_Page1.m_KillPos.GetItemText(i, 1);
		tUserPos.nSceneId = atoi(strTemp);
		strTemp = g_pUI->m_Page1.m_KillPos.GetItemText(i, 2);
		tUserPos.nPosX = atoi(strTemp);
		strTemp = g_pUI->m_Page1.m_KillPos.GetItemText(i, 3);
		tUserPos.nPosY = atoi(strTemp);
		strTemp = g_pUI->m_Page1.m_KillPos.GetItemText(i, 4);
		tUserPos.nPosR = atoi(strTemp);

		vUserPos.push_back(tUserPos);//�һ������������
	}

	return vUserPos;
}

//////////////////////////////////����////////////////////////////////////
VUserSkill CUser::UserGetSkill()//��ȡ����
{
	VAsmSkill l_AsmSkill = g_pAsmSkill->AsmGetSkillData();
	CString szMenPai = g_pAsmRole->GetMenPaiForName();
	dbgPrint("szMenPai = %s", szMenPai);

	CString strPath = CString(g_pFileSystem->Module.c_str()) + "ͨ�ü����ͷ�.ini";
	CString strTags = "ʹ����̬|����ʽ|Ⱥ������|״̬buff|����buff";

	VUserSkill vUserSkill;
	
	CString strTemp;

	_tstring ss = strTags;
	VUserMonsterName  vTags = UserSubMonsterName(ss, _T('|'));
	for (size_t i = 0; i < vTags.size(); i++)
	{
		GetPrivateProfileString(szMenPai, vTags[i].c_str(), "", strTemp.GetBuffer(MAX_PATH), MAX_PATH, strPath);
		_tstring szTemp = strTemp;
		auto  vString = UserSubMonsterName(szTemp, _T('|'));
		dbgPrint("vString = %d", vString.size());
		if (vString.size() == 0)
		{
			TUserSkill tUserSkill;
			tUserSkill.nType = i + 1;
			tUserSkill.nSkillId = -1;
			vUserSkill.push_back(tUserSkill);
		}
		else if (vString.size() == 1) 
		{
			TUserSkill tUserSkill;
			tUserSkill.nType = i + 1;
			for (auto skill : l_AsmSkill)
			{
				if (skill.szName == strTemp)
				{
					tUserSkill.tAsmSkill = skill;
					tUserSkill.nSkillId = skill.nSkillId;
					break;
				}
			}
			vUserSkill.push_back(tUserSkill);
		}
		else
		{
			for (auto v : vString)
			{
				TUserSkill tUserSkill;
				tUserSkill.nType = i + 1;
				for (auto skill : l_AsmSkill)
				{
					if (CString(skill.szName) == CString(v.c_str()))
					{
						tUserSkill.tAsmSkill = skill;
						tUserSkill.nSkillId = skill.nSkillId;
						break;
					}
				}
				vUserSkill.push_back(tUserSkill);
			}
		}
	}
	return vUserSkill;
}