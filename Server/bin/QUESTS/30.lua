if nEventID == 22001 then
	pUser:SelectMsg(51000, 50007, 22010, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0);
elseif nEventID == 22010 then
	local lvl = pUser:GetLevel();
	if lvl >= 75 and lvl <= 80 then
	pUser:ZoneChange(71, 140, 146);
	end
else
	pUser:SendDebugString("Unprocessed event id ("..nEventID..")");
end
