if nEventID == 22001 then
	pUser:SelectMsg(22001, 22001, 22010, 22002, 22020, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 22010 then
	pUser:ZoneChange(21, 450, 300);
elseif nEventID == 22020 then
	pUser:NpcSay(22020, -1, -1, -1, -1, -1, -1, -1);
else
	pUser:SendDebugString("Unprocessed event id ("..nEventID..")");
end
