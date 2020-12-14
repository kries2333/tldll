#include "pch.h"
#include "User.h"
#include "CGUI.h"
#include "AsmSkill.h"
#include "Function.h"
#include "AsmRole.h"

extern CGUI *g_pUI;
extern CAsmSkill* g_pAsmSkill;
extern CAsmRole* g_pAsmRole;

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
VAsmSkill CUser::UserGetSkill()//��ȡ����
{
	int nCount = g_pUI->m_Page2.m_SkillList.GetItemCount();
	VAsmSkill l_AsmSkill;

	int nMenPai = g_pAsmRole->GetMenPaiForId();
	TAsmSkill nSkill;
	if (nMenPai == 8) {
		nSkill.szName = "��Ӣ��";
		nSkill.nSkillId = 521;
		nSkill.fMax = 15.0;
		l_AsmSkill.push_back(nSkill);
	}
	else if (nMenPai == 4)
	{
		nSkill.szName = "��������";
		nSkill.nSkillId = 401;
		nSkill.fMax = 15.0;
		l_AsmSkill.push_back(nSkill);
	}
	else
	{

	}
	return l_AsmSkill;
}