if nEventID == 22001 then
	pUser:SelectMsg(51000, 50000, 22010, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 22010 then
	pUser:ZoneChange(21, 306, 352);
else
	pUser:SendDebugString("Unprocessed event id ("..nEventID..")");
end
