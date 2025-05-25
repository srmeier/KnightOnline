if nEventID == 22001 then
	pUser:SelectMsg(40600, 50004, 22010, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
else
	pUser:SendDebugString("Unprocessed event id ("..nEventID..")");
end
