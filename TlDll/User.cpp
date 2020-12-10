#include "pch.h"
#include "User.h"
#include "CGUI.h"
#include "AsmSkill.h"
#include "Function.h"
#include "AsmRole.h"

extern CGUI *g_pUI;
extern CAsmSkill* g_pAsmSkill;
extern CAsmRole* g_pAsmRole;

_tstring Os_stringOferase(_tstring strSource, _tstring strErase)//移除指定字符或者字符串
{
	_tstring strOk = strSource;
	_tstring::size_type pos = 0;

	//查找在字符串中第一个与str中的某个字符匹配的字符，返回它的位置。
	//搜索从index开始，如果没找到就返回string::npos ,我们这里从0开始
	while ((pos = strOk.find_first_of(strErase, 0)) != _tstring::npos)
		strOk.erase(pos, 1);//去掉要抹除的字符或字串

	return strOk;
}

//字符串截取
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

VUserMonsterName UserSubMonsterName(_tstring strSource, TCHAR strErase)//截取怪物名单，存入临时容器返回
{
	VUserMonsterName vUserMonsterName;//临时容器
	_tstring strName = strSource;
	strName = Os_stringOferase(strName, _T("\t\r\n"));//移除这些字符

	if (!strName.empty())
	{
		if (strName[strName.length() - 1] != strErase)//如果最后一个字符不是逗号，就在末尾追加一个
			strName += strErase;

		_tstring::size_type pos = 0; int pos1 = 0;
		while ((pos = strName.find_first_of(strErase, pos1)) != _tstring::npos)//查找","截取字符串
		{
			vUserMonsterName.push_back(strName.substr(pos1, pos - pos1));//将截取的子串放入容器

			if ((strName.size() - 1) == pos)
				break;

			pos1 = pos + 1;
		}
	}

	return vUserMonsterName;
}

void CUser::UserSystemInitial() //初始化用户设置界面
{
	tPuTongSkill.fMax = 2.0;
	tPuTongSkill.nSkillId = 0;
	vUserSkill = UserGetSkill(); //用户技能
	vtKillPos = UserGetPos();//用户获取挂机点总数
}

VUserPos CUser::UserGetPos()//用户获取挂机点总数
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

		vUserPos.push_back(tUserPos);//挂机点存入容器中
	}

	return vUserPos;
}

//////////////////////////////////技能////////////////////////////////////
VAsmSkill CUser::UserGetSkill()//获取技能
{
	int nCount = g_pUI->m_Page2.m_SkillList.GetItemCount();
	VAsmSkill l_AsmSkill;
	//auto  ASkill = g_pAsmSkill->AsmGetSkillData();
	//for (auto s : ASkill)
	//{
	//	//if ((CString)s.szName == "初级隐遁")
	//	//	nYinShenSkillID = s.nSkillId;
	//	//else if ((CString)s.szName == "捕兽")
	//	//	nBuShouSkillID = s.nSkillId;

	//	//if (pAsmSkill->AsmHaveMasterSkill(s.unTree) == 0)//返回0已经学会
	//	//{
	//	for (int i = 0; i < nCount; i++)
	//	{
	//		//用户是否勾选了此技能
	//		if (g_pUI->m_Page2.m_SkillList.GetCheck(i))
	//		{//技能名字
	//			CString SkillName = g_pUI->m_Page2.m_SkillList.GetItemText(i, 0);
	//			if (SkillName == s.szName)
	//			{
	//				l_AsmSkill.push_back(s);
	//			}
	//		}
	//	}
	//	//}
	//}
	int nMenPai = g_pAsmRole->GetMenPaiForId();
	TAsmSkill nSkill;
	if (nMenPai == 8) {
		nSkill.szName = "落英剑";
		nSkill.nSkillId = 521;
		nSkill.fMax = 15.0;
		l_AsmSkill.push_back(nSkill);
	}
	return l_AsmSkill;
}