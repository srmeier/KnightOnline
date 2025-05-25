if nEventID == 22001 then
	pUser:SelectMsg(22001, 22001, 22010, 22002, 22020, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 22010 then
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_NATION'.");
	if false then -- unknown logic command (CHECK_NATION)
	pUser:ZoneChange(1, 354, 1610);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_NATION'.");
	if false then -- unknown logic command (CHECK_NATION)
	pUser:ZoneChange(2, 1670, 370);
	do return; end
	end
elseif nEventID == 22020 then
	pUser:NpcSay(22020, -1, -1, -1, -1, -1, -1, -1);
else
	pUser:SendDebugString("Unprocessed event id ("..nEventID..")");
end
