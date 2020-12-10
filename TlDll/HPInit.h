#pragma once
class CHPInit
{
public:
	void MySendGameInfo(const char* message);

	bool LoginPassWord(const char* UserName, const char* UserKey, const char* other);//输入帐号密码

	UINT Login_Thread();    //登录线程函数
};

