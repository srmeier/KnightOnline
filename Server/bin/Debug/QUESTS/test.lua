-- Testint script

-- nEventID
-- bSelectedReward
-- pNpc
-- pUser

if nEventID == 35001 then

	-- display the main menu
	pUser:SelectMsg(35001, 2901, 35012, 2902, 35032, 2903, 35052, 2904, 35072, 2905, 35092);

elseif nEventID == 35012 then

	-- worm extermination
	local lvl = pUser:GetLevel();
	if lvl >= 1 and lvl < 10 then
		-- check that the player is in the quest range (this may have been moved to DB)
		pUser:NpcSay(35002);
	else
		-- EVTs would set the bEventStatus for each phase of the quest
		-- instead of doing that the DB does that and returns nEventTriggerIndex which is the entry point for bEventStatus
		local nEventTriggerIndex = pUser:SearchQuest();

		if nEventTriggerIndex == 35023 then -- bEventStatus = 0 (quest start)

			pUser:SelectMsg(35004, 2911, 35024, 2912, 35029);

		elseif nEventTriggerIndex == 35025 then -- bEventStatus = 1 (quest is in process)

			if pUser:CheckExistItem(379048000, 10) then
				-- player has more than 10 silk bundles
				pUser:SelectMsg(35007, 2914, 35030, 2915, 35029);
			else
				-- player has less than 10 silk bundles
				pUser:NpcSay(35006);
			end

		elseif nEventTriggerIndex == 35022 then -- bEventStatus = 2 (quest is done)

			pUser:SelectMsg(35003, 2913, 35024, 2912, 35029);

		end
	end

elseif nEventID == 35032 then

	pUser:SendDebugString("Second menu selected...");

elseif nEventID == 35052 then

	pUser:SendDebugString("Third menu selected...");

elseif nEventID == 35072 then

	pUser:SendDebugString("Fouth menu selected...");

elseif nEventID == 35092 then

	pUser:SendDebugString("Fifth menu selected...");

elseif nEventID == 35024 then

	-- the player has started the worm extermination quest
	pUser:NpcSay(35005);
	pUser:GiveItem(379048000, 10); -- temp for testing...
	pUser:SaveEvent(3); -- the actual SQL index for the next bEventStatus entry

elseif nEventID == 35029 then

	-- player declines the quest for now
	pUser:NpcSay(35009);

elseif nEventID == 35030 then

	-- quest is done - grab the silk, give the points, and set the state
	pUser:RobItem(379048000, 10);
	pUser:ExpChange(500);
	pUser:SaveEvent(4); -- bEventStatus = 2
	pUser:NpcSay(35008);

else

	-- debugging for missing nEventID
	pUser:SendDebugString("Unprocessed event id ("..nEventID..")");

end
