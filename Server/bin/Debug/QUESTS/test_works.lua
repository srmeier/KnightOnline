-- Testing script

-- nEventID
-- bSelectedReward
-- pNpc
-- pUser

if nEventID == 35001 then

	-- display the main menu
	pUser:SelectMsg(35001, 2901, 35012, 2902, 35032, 2903, 35052, 2904, 35072, 2905, 35092);

elseif nEventID == 35012 then

	-- WORM EXTERMINATION

	local lvl = pUser:GetLevel();

	if lvl >= 1 and lvl < 10 then
		pUser:NpcSay(35002);
	else
		local nEventTriggerIndex = pUser:SearchQuest(30);

		if nEventTriggerIndex == 0 then --35023 then -- 0
			pUser:SelectMsg(35004, 2911, 35024, 2912, 35029);
		elseif nEventTriggerIndex == 1 then --35025 then -- 1
			if pUser:CheckExistItem(379048000, 10) then
				pUser:SelectMsg(35007, 2914, 35030, 2915, 35029);
			else
				pUser:NpcSay(35006);
			end
		elseif nEventTriggerIndex == 2 then --35022 then -- 2
			pUser:SelectMsg(35003, 2913, 35024, 2912, 35029);
		end
	end

elseif nEventID == 35032 then

	-- GAVOLT EXTERMINATION

	local lvl = pUser:GetLevel();
	
	if lvl >= 1 and lvl < 10 then
		pUser:NpcSay(35002);
	else
		local nEventTriggerIndex = pUser:SearchQuest(31);

		if nEventTriggerIndex == 0 then --35043 then -- 0
			pUser:SelectMsg(35011, 2911, 35044, 2912, 35049);
		elseif nEventTriggerIndex == 1 then --35045 then -- 1
			if pUser:CheckExistItem(379043000, 5) then
				pUser:SelectMsg(35014, 2914, 35050, 2915, 35049);
			else
				pUser:NpcSay(35013);
			end
		elseif nEventTriggerIndex == 2 then --35042 then -- 2
			pUser:SelectMsg(35010, 2913, 35044, 2912, 35049);
		end
	end

elseif nEventID == 35052 then

	-- COLLECTING SILAN BONES

	local lvl = pUser:GetLevel();

	if lvl >=1 and lvl < 10 then
		pUser:NpcSay(35002);
	else
		local nEventTriggerIndex = pUser:SearchQuest(32);

		if nEventTriggerIndex == 0 then --35063 then -- 0
			pUser:SelectMsg(35018, 2911, 35064, 2912, 35069);
		elseif nEventTriggerIndex == 1 then --35065 then -- 1
			if pUser:CheckExistItem(379077000, 10) then
				pUser:SelectMsg(35021, 2914, 35070, 2915, 35069);
			else
				pUser:NpcSay(35020);
			end
		elseif nEventTriggerIndex == 2 then --35062 then -- 2
			pUser:SelectMsg(35018, 2911, 35064, 2912, 35069);
		end
	end

elseif nEventID == 35072 then

	pUser:SendDebugString("Fouth menu selected...");

elseif nEventID == 35092 then

	pUser:SendDebugString("Fifth menu selected...");

elseif nEventID == 35024 then

	pUser:NpcSay(35005);
	pUser:GiveItem(379048000, 10); -- temp for testing
	pUser:SaveEvent(3); -- 1

elseif nEventID == 35029 then

	pUser:NpcSay(35009);

elseif nEventID == 35030 then

	pUser:RobItem(379048000, 10);
	pUser:ExpChange(500);
	pUser:SaveEvent(4); -- 2
	pUser:NpcSay(35008);

elseif nEventID == 35044 then

	pUser:NpcSay(35012);
	pUser:GiveItem(379043000, 5); -- temp for testing
	pUser:SaveEvent(6); -- 1

elseif nEventID == 35049 then

	pUser:NpcSay(35016);

elseif nEventID == 35050 then

	pUser:RobItem(379043000, 5);
	pUser:ExpChange(700);
	pUser:GiveItem(389062000, 5);
	pUser:SaveEvent(7); -- 2
	pUser:NpcSay(35015);

elseif nEventID == 35064 then

	pUser:NpcSay(35019);
	pUser:GiveItem(379077000, 10); -- temp for testing
	pUser:SaveEvent(9); -- 1

elseif nEventID == 35069 then

	pUser:NpcSay(35023);

elseif nEventID == 35070 then

	pUser:RobItem(379077000, 10);
	pUser:ExpChange(1000);
	pUser:GoldGain(50000);
	pUser:SaveEvent(10); -- 2
	pUser:NpcSay(35022);

else

	-- notice for missing nEventID
	pUser:SendDebugString("Unprocessed event id ("..nEventID..")");

end
