if nEventID == 1100 then
	pUser:SelectMsg(1100, 1207, 1209, 1208, 1210, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 1102 then
	local state = pUser:SearchQuest(63);
	if state == 2 then
	pUser:NpcSay(1102, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
elseif nEventID == 1103 then
	local state = pUser:SearchQuest(63);
	if state == 1 then
	if pUser:CheckExistItem(910084000, 1) then
	pUser:NpcSay(1103, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:GiveItem(910084000, 1);
	pUser:NpcSay(1103, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
elseif nEventID == 1202 then
	local state = pUser:SearchQuest(64);
	if state == 2 then
	pUser:NpcSay(1102, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
elseif nEventID == 1203 then
	local state = pUser:SearchQuest(64);
	if state == 1 then
	if pUser:CheckExistItem(910085000, 1) then
	pUser:NpcSay(1203, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:GiveItem(910085000, 1);
	pUser:NpcSay(1203, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
elseif nEventID == 1204 then
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_NATION'.");
	if false then -- unknown logic command (CHECK_NATION)
	pUser:NpcSay(1105, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:NpcSay(1106, -1, -1, -1, -1, -1, -1, -1);
	do return; end
elseif nEventID == 1209 then
	local state = pUser:SearchQuest(63);
	if state == 2 then
	pUser:NpcSay(1102, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	local state = pUser:SearchQuest(63);
	if state == 1 then
	if pUser:CheckExistItem(910084000, 1) then
	pUser:NpcSay(1103, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:GiveItem(910084000, 1);
	pUser:NpcSay(1103, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_NATION'.");
	if false then -- unknown logic command (CHECK_NATION)
	pUser:NpcSay(1105, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:NpcSay(1106, -1, -1, -1, -1, -1, -1, -1);
	do return; end
elseif nEventID == 1210 then
	local state = pUser:SearchQuest(64);
	if state == 2 then
	pUser:NpcSay(1102, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	local state = pUser:SearchQuest(64);
	if state == 1 then
	if pUser:CheckExistItem(910085000, 1) then
	pUser:NpcSay(1203, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:GiveItem(910085000, 1);
	pUser:NpcSay(1203, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_NATION'.");
	if false then -- unknown logic command (CHECK_NATION)
	pUser:NpcSay(1105, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:NpcSay(1106, -1, -1, -1, -1, -1, -1, -1);
	do return; end
else
	pUser:SendDebugString("Unprocessed event id ("..nEventID..")");
end
