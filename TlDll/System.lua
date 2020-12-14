function SelectServer_my(Namestr)
    local iCurAreaCount = GameProduceLogin:GetServerAreaCount();
    for index = 0, iCurAreaCount - 1 do
        local strAreaName = GameProduceLogin:GetServerAreaName(index);
        iLoginServerCount = GameProduceLogin:GetAreaLoginServerCount(index);
        for i = 0, iLoginServerCount - 1 do
            ServerName,
            ServerStatus,
            RecommendLevel,
            IsNew
            , netPro,
            ServerPrizeLevel,
            ServerVersion
            = GameProduceLogin:GetAreaLoginServerInfo(index, i);
            if Namestr == ServerName then
                GameProduceLogin:SelectLoginServer(index, i, 3);
            end
        end
    end
end

function LoginPassWord(username, mail)
    local TailName ={
    [0] = "@changyou.com",
    "@game.sohu.com",
    "@Sohu.com",
    "@chinaren.com",
    "@sogou.com",
    "@17173.com",
    "手机号码登录",
    "输入其他账号后缀"
    }
	setmetatable(_G, { __index = LoginLogOn_Env });
    -- if this:IsVisible() then
       -- LogOn_LogonMailID_Active()
      -- LogOn_Region:SetCurrentSelect(tonumber(mail));
    -- end
	-- LogOn_ID:SetText("");
	-- LogOn_PassWord:SetText("");
	
    setmetatable(_G, { __index = LoginLogOn_Env });
    if this:IsVisible() then
        LogOn_LogonID_Active();
        LogOn_ID:SetText(tostring(username));
    end
    setmetatable(_G, { __index = LoginLogOn_Env });
    if this:IsVisible() then
        LogOn_LogonPassWord_Active();
        -- LogOn_PassWord:SetText("");
    end
end

function myIsWindowShow(str)

    if (IsWindowShow(str)) then
        -- 地面包裹
        return 1
    end
    return 0
end

function IsNpcDialog()
	setmetatable(_G, {__index = Quest_Env});
	if this:IsVisible() then 
		return 1
	end
	return 0
end

function IsDeathDialog()
	setmetatable(_G, {__index = Relive_Env});
	if this:IsVisible() then 
		return 1
	end
	return 0
end

function IsQuizDialog()
	setmetatable(_G, {__index = Quiz_Env});
	if this:IsVisible() then 
		return 1
	end
	return 0
end

-- 返回可接列表数据，ncp名字|scene|PosX|PosY|任务名称
function MissionOutlinetext(Mission)
    -- 任务是否可接，可接返回任务名字
    local j;
    local i;
    CollectMissionOutline()
    for i = 1, 300 do
        -- 现任务类型一共200种
        local DeployNum = GetMissionOutlineNum(i)
        if DeployNum > 0 then
            for j = 1, DeployNum do
				local MissionLevel, MinLevel, MaxLevel, strNpcName, strNpcPos, strScene, strMissionName, PosX, PosY, SceneID  = GetMissionOutlineInfo( i, j );
                strMissionName = string.gsub(strMissionName, "%s", "");
                strMissionName = string.gsub(strMissionName, "%-", "");
                strMissionName = string.gsub(strMissionName, "%(", "");
                strMissionName = string.gsub(strMissionName, "%)", "");
                strMissionName = string.gsub(strMissionName, "（", "");
                strMissionName = string.gsub(strMissionName, "）", "");
                strMissionName = string.gsub(strMissionName, "#r", "");
                strMissionName = string.gsub(strMissionName, "#G", "");
                if Mission == strMissionName then
                    return strNpcName .. "|" .. strScene .. "|" .. PosX .. "|" .. PosY .. "|" .. strMissionName;
                end
            end
        end
    end
    return "";
end

function GetAvailableMissionList()
    -- 任务是否可接，可接返回任务名字
    local j;
    local i;
	local szTemp = "";
    CollectMissionOutline()
    for i = 1, 300 do
        -- 现任务类型一共200种
        local DeployNum = GetMissionOutlineNum(i)
        if DeployNum > 0 then
            for j = 1, DeployNum do
				local MissionLevel, MinLevel, MaxLevel, strNpcName, strNpcPos, strScene, strMissionName, PosX, PosY, SceneID  = GetMissionOutlineInfo( i, j );
                strMissionName = string.gsub(strMissionName, "%s", "");
                strMissionName = string.gsub(strMissionName, "%-", "");
                strMissionName = string.gsub(strMissionName, "%(", "");
                strMissionName = string.gsub(strMissionName, "%)", "");
                strMissionName = string.gsub(strMissionName, "#r", "");
                strMissionName = string.gsub(strMissionName, "#G", "");
                strMissionName = string.gsub(strMissionName, "（", "");
                strMissionName = string.gsub(strMissionName, "）", "");
				szTemp = szTemp .. "|" .. strMissionName
            end
        end
    end
    return szTemp;
end

-- 获取需要杀死得怪物与数量
function MissionTaskKillMonsterInfo(Mission)
	local info = "";
	for i = 1, 20 do
		if (DataPool:GetPlayerMission_InUse(i - 1) == 1) then
			local strInfo, strDesc = DataPool:GetPlayerMission_Memo(i - 1);
			if strInfo == Mission then
				local nNPCName, nNum = DataPool:GetPlayerMissionDemand_NPC(0,0,i-1);
				return nNPCName .. "|" .. nNum
			end
		end
	end
	return ""
end

-- 获取已接任务信息
function MissionTaskInfo(Mission)
	local info = "";
	for j = 1, 200 do
		for i = 1, 20 do
			if (DataPool:GetPlayerMission_InUse(i - 1) == 1) then
				local MissionKind = DataPool:GetPlayerMission_Kind(i - 1);
				if (MissionKind == j) then
					local strInfo, strDesc = DataPool:GetPlayerMission_Memo(i - 1);
					if strInfo == Mission then
						s = string.find(strDesc, "{");
						l = string.find(strDesc, "}");
						if s ~= nil and l ~= nil then
							str = string.sub(strDesc, s + 1, l - 1);
							getstr = GetDictionaryString(str);
							return getstr
						end
					end
				end
			end
		end
	end
	return "";
end

-- 获取任务完成情况,是否完成，完成后提交npc， 场景，坐标
function GetMissionFinishInfo(Mission)
	local Mission_Variable = 0;
	local strFinishNPC, strFinishSceneName, nFinishSceneID, nFinishX, nFinishY;
	local info = "";
	
	for i = 1, 20 do
		if (DataPool:GetPlayerMission_InUse(i - 1) == 1) then
			local strInfo, strDesc = DataPool:GetPlayerMission_Memo(i - 1);
			if strInfo == Mission then
				Mission_Variable = DataPool:GetPlayerMission_Variable(i-1, 0)
				if Mission == '苏飞的世界' then	--特殊处理
					strFinishNPC = '苏飞'
					strFinishSceneName = '西湖'
					nFinishSceneID = 30;
					nFinishX = 164;
					nFinishY = 262;
					
					return Mission_Variable .. "|".. strFinishNPC .. "|" .. strFinishSceneName .. "|" .. nFinishSceneID .. "|" .. nFinishX .. "|" .. nFinishY
				else
					--完成后
					strFinishNPC, strFinishSceneName, nFinishSceneID, nFinishX, nFinishY = DataPool:GetMissionFinishInfo(i - 1);
					return Mission_Variable .. "|".. strFinishNPC .. "|" .. strFinishSceneName .. "|" .. nFinishSceneID .. "|" .. nFinishX .. "|" .. nFinishY
				end
			end
		end
	end
	return "-1|-1|-1|-1|-1|-1"
end

function GetNPCMissionList()
    local nEventListNum = DataPool:GetNPCEventList_Num();
    local szTemp = "";
    for i = 1, nEventListNum do

        local strType, strState, strScriptId, strExtra, strTemp = DataPool:GetNPCEventList_Item(i - 1);

        if strType ~= "text" then

            local pos1 = string.find(strTemp, "#{");
            if pos1 == 1 then
                strTemp = string.sub(strTemp, 3, -2);
                strTemp = GetDictionaryString(strTemp);
            end
            strTemp = string.gsub(strTemp, "%s", "");
            strTemp = string.gsub(strTemp, "%-", "");
            strTemp = string.gsub(strTemp, "%(", "");
            strTemp = string.gsub(strTemp, "%)", "");
            strTemp = string.gsub(strTemp, "#r", "");
            strTemp = string.gsub(strTemp, "#G", "");
            strTemp = string.gsub(strTemp, "（", "");
            strTemp = string.gsub(strTemp, "）", "");
            szTemp = szTemp .. "|" .. strTemp
			PushDebugMessage(tostring(szTemp));
        end
    end
    return szTemp
end

function ClickMission(Mission)
	local nEventListNum = DataPool:GetNPCEventList_Num();
    local nVal = 0;
    for i = 1, nEventListNum do

        local strType, strState, strScriptId, strExtra, strTemp = DataPool:GetNPCEventList_Item(i - 1);
		local pos1 = string.find(strTemp, "#{");
		if pos1 == 1 then
			strTemp = string.sub(strTemp, 3, -2);
			strTemp = GetDictionaryString(strTemp);
		end
			
        if strType ~= "text" then
            strTemp = string.gsub(strTemp, "%s", "");
            strTemp = string.gsub(strTemp, "%-", "");
            strTemp = string.gsub(strTemp, "%(", "");
            strTemp = string.gsub(strTemp, "%)", "");
            strTemp = string.gsub(strTemp, "#r", "");
            strTemp = string.gsub(strTemp, "#G", "");
            strTemp = string.gsub(strTemp, "（", "");
            strTemp = string.gsub(strTemp, "）", "");
			if strTemp == Mission then
                QuestFrameOptionClicked(tonumber(-1), tonumber(strScriptId), tonumber(strExtra));
                return 1;
            end
        end
    end
end

-- 取任务信息
function mymis(TD)
    local j;
    local i;
    local s;
    local l;
    local Mission_Variable = 0;
    for j = 1, 200 do
        for i = 1, 20 do
            if (DataPool:GetPlayerMission_InUse(i - 1) == 1) then
                local MissionKind = DataPool:GetPlayerMission_Kind(i - 1);
                if (MissionKind == j) then
                    local strBT = DataPool:GetMissionInfo_Kind(j);
                    local strInfo, strDesc = DataPool:GetPlayerMission_Memo(i - 1);
                    local nMissionLevel = DataPool:GetPlayerMission_Level(i - 1);
                    local Mission_VariableX = DataPool:GetPlayerMission_Variable(i - 1, 0);
                    local nScriptId = DataPool:GetPlayerMission_Display(i - 1, 7);
                    local nRound = DataPool:GetPlayerMission_Display(i - 1, 3);

                    if (nRound >= 0) then
                        Mission_Variable = DataPool:GetPlayerMission_DataRound(nRound);
                    end
                    --  特殊处理

                    if (nScriptId == 610013 or nScriptId == 610014) then
                        strDesc = QuestLog_GuildInteractiveMission(i - 1);
                    end

                    if (DataPool:GetPlayerMission_Display(i - 1, 7) == 500510) then
                        strDesc = QuestLog_GetSCMTarget(i - 1);
                    end

                    -- 生育系统成长任务特写
                    if (DataPool:GetPlayerMission_Display(i - 1, 7) == 890500) then
                        strDesc = QuestLog_GetInfantTarget(i - 1);
                    end

                    if (DataPool:GetPlayerMission_Display(i - 1, 7) == 889265) then
                        strDesc = QuestLog_GetHLXQJTarget(i - 1);
                    end
                    -- Q3打卡活动
                    if (DataPool:GetPlayerMission_Display(i - 1, 7) == 890612) then
                        strDesc = QuestLog_GetDAKATarget(i - 1);
                    end
                    -- 诗歌荟萃之对歌
                    if (DataPool:GetPlayerMission_Display(i - 1, 7) == 051024) then
                        strDesc = QuestLog_GetSGHCDGTarget(i - 1);
                    end
                    -- 诗歌荟萃之对诗
                    if (DataPool:GetPlayerMission_Display(i - 1, 7) == 051025) then
                        strDesc = QuestLog_GetSGHCDSTarget(i - 1);
                    end
                    -- 子女系统属性培养——作画任务
                    if (DataPool:GetPlayerMission_Display(i - 1, 7) == 890512) then
                        strDesc = QuestLog_GetDrawTarget(i - 1);
                    end
                    -- 子女系统属性培养——礼仪任务
                    if (DataPool:GetPlayerMission_Display(i - 1, 7) == 890513) then
                        strDesc = QuestLog_GetMannerNPC(i - 1);
                    end

                    local ss = string.find(strInfo, "{");
                    local ls = string.find(strInfo, "}");
                    if ss ~= nil and ls ~= nil then
                        local strs = string.sub(strInfo, ss + 1, ls - 1);
                        strInfo = GetDictionaryString(strs);
                    end;
                    local nks = 0
                    if strInfo == TD then
                        strDesc2 = strDesc

                        local nks = 0;
                        repeat
                            s = string.find(strDesc, "{");
                            l = string.find(strDesc, "}");
                            if s ~= nil and l ~= nil then
                                str = string.sub(strDesc, s + 1, l - 1);
                                getstr = GetDictionaryString(str);
                                TsT = string.find(str, "INFOAIM");
                                -- PushDebugMessage(tostring(TT));--判断是不是人物取人物坐标
                                if TsT ~= nil then
                                    getstr = getstr .. "<" .. str .. ">"
                                end
                                strDesc = string.sub(strDesc, 0, s - 2) .. getstr .. string.sub(strDesc, l + 1, l + 500);
                            end
                            nks = nks + 1
                            if nks > 10 then
                                s = nil
                            end
                        until (s == nil)
                        if strDesc == "" then
                            return(tostring(strInfo) .. "|" .. tostring(strDesc2) .. "|" .. tostring(Mission_VariableX) .. "|" .. tostring(i - 1) .. "|" .. tostring(nScriptId) .. "|" .. tostring(strBT) .. "|" .. tostring(Mission_Variable));
                        else
                            return(tostring(strInfo) .. "|" .. tostring(strDesc) .. "|" .. tostring(Mission_VariableX) .. "|" .. tostring(i - 1) .. "|" .. tostring(nScriptId) .. "|" .. tostring(strBT) .. "|" .. tostring(Mission_Variable));
                        end
                    end
                end
            end
        end
    end
    return("-1|-1|-1|-1|-1|-1|-1");
end

-- 点击任务对话
function myclickmis(nam)
    PushDebugMessage(nam)
    setmetatable(_G, { __index = Quest_Env });
    if this:IsVisible() then
    else
        return 0;
    end
    if string.len(nam) == 1 then
        nam = tonumber(nam);
        local strType, strState, strScriptId, strExtra, strTemp = DataPool:GetNPCEventList_Item(nam - 1);
        QuestFrameOptionClicked(tonumber(-1), tonumber(strScriptId), tonumber(strExtra));
        return 1;
    end

    local nEventListNum = DataPool:GetNPCEventList_Num();
    local nVal = 0;
    for i = 1, nEventListNum do

        local strType, strState, strScriptId, strExtra, strTemp = DataPool:GetNPCEventList_Item(i - 1);

        if strType ~= "text" then

            local pos1 = string.find(strTemp, "#{");
            if pos1 == 1 then
                strTemp = string.sub(strTemp, 3, -2);
                strTemp = GetDictionaryString(strTemp);
            end

            strTemp = string.gsub(strTemp, "%s", "");
            strTemp = string.gsub(strTemp, "%-", "");
            strTemp = string.gsub(strTemp, "%(", "");
            strTemp = string.gsub(strTemp, "%)", "");
            strTemp = string.gsub(strTemp, "#r", "");
            strTemp = string.gsub(strTemp, "#G", "");
			strTemp = string.gsub(strTemp, "（", "");
            strTemp = string.gsub(strTemp, "）", "");

            if nam == "炼金任务" then
                if strTemp == nam then
                    QuestFrameOptionClicked(tonumber(-1), tonumber(strScriptId), tonumber(strExtra));
                    return 1;
                end
            else
                if string.find(strTemp, nam) ~= nil then
                    QuestFrameOptionClicked(tonumber(-1), tonumber(strScriptId), tonumber(strExtra));
                    return 1;
                end
            end

        end
    end

    return 0;
end

function getMessageBoxText()
    setmetatable(_G, {__index = MessageBox_Self_Env});
    if this:IsVisible() then 
        local tmp = MessageBox_Self_Text:GetText();
	    return tmp;
    end
    return "";
end

function getAcceptBoxText()
    setmetatable(_G, {__index = AcceptBox_Env});
    if this:IsVisible() then 
        local tmp = AcceptBox_Text:GetText();
	    return tmp;
    end
    return "";
end

function GetBagItemNum(name)
	local count = 0;
	for i=1, 100 do
		local itemName = PlayerPackage:GetBagItemName(i);
		if itemName == name then
			count = count + PlayerPackage:GetBagItemNum(i);
		end
	end
	return count;
end