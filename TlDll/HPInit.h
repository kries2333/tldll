#pragma once
class CHPInit
{
public:
	void MySendGameInfo(const char* message);

	bool LoginPassWord(const char* UserName, const char* UserKey, const char* other);//�����ʺ�����

	UINT Login_Thread();    //��¼�̺߳���
};

