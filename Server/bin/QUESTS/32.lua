if nEventID == 22001 then
	pUser:SelectMsg(53000, 22070, 22010, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 22010 then
	pUser:RobItem(989218000, 1);
	pUser:SendDebugString("Unknown EXEC command 'ZONE_CHANGE_PARTY'."); -- unknown execute command (ZONE_CHANGE_PARTY)
	do return; end
elseif nEventID == 40001 then
	local count = pUser:HowMuchItem(379111000);
	if count >= 1 and count <= 9999 then
	pUser:SelectMsg(40071, 40075, 40075, 40009, 40009, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
	local count = pUser:HowMuchItem(379111000);
	if count >= 0 and count <= 0 then
	pUser:SelectMsg(40008, 40010, 40010, 40041, 40041, 40090, 40090, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
elseif nEventID == 40009 then
	pUser:SelectMsg(40008, 40010, 40010, 40041, 40041, 40090, 40090, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 40010 then
	pUser:ZoneChange(32, 50, 197);
elseif nEventID == 40015 then
	local count = pUser:HowMuchItem(379080000);
	if count >= 0 and count <= 0 then
	pUser:NpcSay(40015, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
elseif nEventID == 40035 then
	pUser:SendDebugString("Unknown EXEC command 'ROB_ALLITEM_PARTY'."); -- unknown execute command (ROB_ALLITEM_PARTY)
	do return; end
	pUser:SendDebugString("Unknown EXEC command 'ZONE_CHANGE_PARTY'."); -- unknown execute command (ZONE_CHANGE_PARTY)
	do return; end
elseif nEventID == 40041 then
	local count = pUser:HowMuchItem(379089000);
	if count >= 1 and count <= 9999 then
	pUser:SelectMsg(40042, 40061, 40061, 40062, 40062, 40063, 40063, 40064, 40064, 40065, 40065, 40066, 40066, 40067, 40067, 40068, 40068, 40069, 40069, 40060, 40060);
	end
	local count = pUser:HowMuchItem(379089000);
	if count >= 0 and count <= 0 then
	local count = pUser:HowMuchItem(379088000);
	if count >= 1 and count <= 9999 then
	pUser:SelectMsg(40044, 40061, 40061, 40062, 40062, 40063, 40063, 40064, 40064, 40065, 40065, 40066, 40066, 40067, 40067, 40068, 40068, 40060, 40060, -1, -1);
	end
	local count = pUser:HowMuchItem(379088000);
	if count >= 0 and count <= 0 then
	local count = pUser:HowMuchItem(379087000);
	if count >= 1 and count <= 9999 then
	pUser:SelectMsg(40046, 40061, 40061, 40062, 40062, 40063, 40063, 40064, 40064, 40065, 40065, 40066, 40066, 40067, 40067, 40060, 40060, -1, -1, -1, -1);
	end
	local count = pUser:HowMuchItem(379087000);
	if count >= 0 and count <= 0 then
	local count = pUser:HowMuchItem(379086000);
	if count >= 1 and count <= 9999 then
	pUser:SelectMsg(40048, 40061, 40061, 40062, 40062, 40063, 40063, 40064, 40064, 40065, 40065, 40066, 40066, 40060, 40060, -1, -1, -1, -1, -1, -1);
	end
	local count = pUser:HowMuchItem(379086000);
	if count >= 0 and count <= 0 then
	local count = pUser:HowMuchItem(379085000);
	if count >= 1 and count <= 9999 then
	pUser:SelectMsg(40050, 40061, 40061, 40062, 40062, 40063, 40063, 40064, 40064, 40065, 40065, 40060, 40060, -1, -1, -1, -1, -1, -1, -1, -1);
	end
	local count = pUser:HowMuchItem(379085000);
	if count >= 0 and count <= 0 then
	local count = pUser:HowMuchItem(379084000);
	if count >= 1 and count <= 9999 then
	pUser:SelectMsg(40052, 40061, 40061, 40062, 40062, 40063, 40063, 40064, 40064, 40060, 40060, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
	local count = pUser:HowMuchItem(379084000);
	if count >= 0 and count <= 0 then
	local count = pUser:HowMuchItem(379083000);
	if count >= 1 and count <= 9999 then
	pUser:SelectMsg(40054, 40061, 40061, 40062, 40062, 40063, 40063, 40060, 40060, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
	local count = pUser:HowMuchItem(379083000);
	if count >= 0 and count <= 0 then
	local count = pUser:HowMuchItem(379082000);
	if count >= 1 and count <= 9999 then
	pUser:SelectMsg(40056, 40061, 40061, 40062, 40062, 40060, 40060, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
	local count = pUser:HowMuchItem(379082000);
	if count >= 0 and count <= 0 then
	local count = pUser:HowMuchItem(379081000);
	if count >= 1 and count <= 9999 then
	pUser:SelectMsg(40058, 40061, 40061, 40060, 40060, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
	local count = pUser:HowMuchItem(379081000);
	if count >= 0 and count <= 0 then
	pUser:NpcSay(40059, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	end
	end
	end
	end
	end
	end
	end
	end
elseif nEventID == 40061 then
	pUser:RobItem(379081000, 1);
	pUser:SendDebugString("Unknown EXEC command 'ZONE_CHANGE_PARTY'."); -- unknown execute command (ZONE_CHANGE_PARTY)
	do return; end
elseif nEventID == 40062 then
	pUser:RobItem(379082000, 1);
	pUser:SendDebugString("Unknown EXEC command 'ZONE_CHANGE_PARTY'."); -- unknown execute command (ZONE_CHANGE_PARTY)
	do return; end
elseif nEventID == 40063 then
	pUser:RobItem(379083000, 1);
	pUser:SendDebugString("Unknown EXEC command 'ZONE_CHANGE_PARTY'."); -- unknown execute command (ZONE_CHANGE_PARTY)
	do return; end
elseif nEventID == 40064 then
	pUser:RobItem(379084000, 1);
	pUser:SendDebugString("Unknown EXEC command 'ZONE_CHANGE_PARTY'."); -- unknown execute command (ZONE_CHANGE_PARTY)
	do return; end
elseif nEventID == 40065 then
	pUser:RobItem(379085000, 1);
	pUser:SendDebugString("Unknown EXEC command 'ZONE_CHANGE_PARTY'."); -- unknown execute command (ZONE_CHANGE_PARTY)
	do return; end
elseif nEventID == 40066 then
	pUser:RobItem(379086000, 1);
	pUser:SendDebugString("Unknown EXEC command 'ZONE_CHANGE_PARTY'."); -- unknown execute command (ZONE_CHANGE_PARTY)
	do return; end
elseif nEventID == 40067 then
	pUser:RobItem(379087000, 1);
	pUser:SendDebugString("Unknown EXEC command 'ZONE_CHANGE_PARTY'."); -- unknown execute command (ZONE_CHANGE_PARTY)
	do return; end
elseif nEventID == 40068 then
	pUser:RobItem(379088000, 1);
	pUser:SendDebugString("Unknown EXEC command 'ZONE_CHANGE_PARTY'."); -- unknown execute command (ZONE_CHANGE_PARTY)
	do return; end
elseif nEventID == 40069 then
	pUser:RobItem(379089000, 1);
	pUser:SendDebugString("Unknown EXEC command 'ZONE_CHANGE_PARTY'."); -- unknown execute command (ZONE_CHANGE_PARTY)
	do return; end
elseif nEventID == 40075 then
	pUser:RobItem(379111000, 1);
	pUser:SendDebugString("Unknown EXEC command 'ZONE_CHANGE_PARTY'."); -- unknown execute command (ZONE_CHANGE_PARTY)
	do return; end
elseif nEventID == 40090 then
	pUser:SelectMsg(40090, 40099, 40099, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 40095 then
elseif nEventID == 40099 then
	pUser:ZoneChange(30, 500, 250);
elseif nEventID == 40101 then
	local count = pUser:HowMuchItem(379111000);
	if count >= 1 and count <= 9999 then
	pUser:SelectMsg(40071, 40075, 40175, 40009, 40109, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
	local count = pUser:HowMuchItem(379111000);
	if count >= 0 and count <= 0 then
	pUser:SelectMsg(40108, 40010, 40110, 40041, 40041, 40090, 40190, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
elseif nEventID == 40109 then
	pUser:SelectMsg(40108, 40010, 40110, 40041, 40041, 40090, 40190, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 40110 then
	pUser:ZoneChange(32, 50, 316);
elseif nEventID == 40115 then
	local count = pUser:HowMuchItem(379080000);
	if count >= 0 and count <= 0 then
	pUser:NpcSay(40015, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
elseif nEventID == 40135 then
	pUser:SendDebugString("Unknown EXEC command 'ROB_ALLITEM_PARTY'."); -- unknown execute command (ROB_ALLITEM_PARTY)
	do return; end
	pUser:SendDebugString("Unknown EXEC command 'ZONE_CHANGE_PARTY'."); -- unknown execute command (ZONE_CHANGE_PARTY)
	do return; end
elseif nEventID == 40175 then
	pUser:RobItem(379111000, 1);
	pUser:SendDebugString("Unknown EXEC command 'ZONE_CHANGE_PARTY'."); -- unknown execute command (ZONE_CHANGE_PARTY)
	do return; end
elseif nEventID == 40190 then
	pUser:SelectMsg(40090, 40095, 40195, 40099, 40199, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 40195 then
	pUser:ZoneChange(32, 50, 74);
elseif nEventID == 40199 then
	pUser:ZoneChange(30, 500, 250);
elseif nEventID == 40201 then
	local count = pUser:HowMuchItem(379111000);
	if count >= 1 and count <= 9999 then
	pUser:SelectMsg(40071, 40075, 40275, 40009, 40209, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
	local count = pUser:HowMuchItem(379111000);
	if count >= 0 and count <= 0 then
	pUser:SelectMsg(40208, 40010, 40210, 40041, 40041, 40090, 40290, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
elseif nEventID == 40209 then
	pUser:SelectMsg(40208, 40010, 40210, 40041, 40041, 40090, 40290, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 40210 then
	pUser:ZoneChange(32, 50, 437);
elseif nEventID == 40215 then
	local count = pUser:HowMuchItem(379080000);
	if count >= 0 and count <= 0 then
	pUser:NpcSay(40015, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
elseif nEventID == 40235 then
	pUser:SendDebugString("Unknown EXEC command 'ROB_ALLITEM_PARTY'."); -- unknown execute command (ROB_ALLITEM_PARTY)
	do return; end
	pUser:SendDebugString("Unknown EXEC command 'ZONE_CHANGE_PARTY'."); -- unknown execute command (ZONE_CHANGE_PARTY)
	do return; end
elseif nEventID == 40275 then
	pUser:RobItem(379111000, 1);
	pUser:SendDebugString("Unknown EXEC command 'ZONE_CHANGE_PARTY'."); -- unknown execute command (ZONE_CHANGE_PARTY)
	do return; end
elseif nEventID == 40290 then
	pUser:SelectMsg(40090, 40095, 40295, 40099, 40299, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 40295 then
	pUser:ZoneChange(32, 50, 197);
elseif nEventID == 40299 then
	pUser:ZoneChange(30, 500, 250);
elseif nEventID == 40301 then
	local count = pUser:HowMuchItem(379111000);
	if count >= 1 and count <= 9999 then
	pUser:SelectMsg(40071, 40075, 40375, 40009, 40309, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
	local count = pUser:HowMuchItem(379111000);
	if count >= 0 and count <= 0 then
	pUser:SelectMsg(40308, 40010, 40310, 40041, 40041, 40090, 40390, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
elseif nEventID == 40309 then
	pUser:SelectMsg(40308, 40010, 40310, 40041, 40041, 40090, 40390, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 40310 then
	pUser:ZoneChange(32, 150, 118);
elseif nEventID == 40315 then
	local count = pUser:HowMuchItem(379080000);
	if count >= 0 and count <= 0 then
	pUser:NpcSay(40015, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
elseif nEventID == 40335 then
	pUser:SendDebugString("Unknown EXEC command 'ROB_ALLITEM_PARTY'."); -- unknown execute command (ROB_ALLITEM_PARTY)
	do return; end
	pUser:SendDebugString("Unknown EXEC command 'ZONE_CHANGE_PARTY'."); -- unknown execute command (ZONE_CHANGE_PARTY)
	do return; end
elseif nEventID == 40375 then
	pUser:RobItem(379111000, 1);
	pUser:SendDebugString("Unknown EXEC command 'ZONE_CHANGE_PARTY'."); -- unknown execute command (ZONE_CHANGE_PARTY)
	do return; end
elseif nEventID == 40390 then
	pUser:SelectMsg(40090, 40095, 40395, 40099, 40399, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 40395 then
	pUser:ZoneChange(32, 50, 316);
elseif nEventID == 40399 then
	pUser:ZoneChange(30, 500, 250);
elseif nEventID == 40401 then
	local count = pUser:HowMuchItem(379111000);
	if count >= 1 and count <= 9999 then
	pUser:SelectMsg(40071, 40075, 40475, 40009, 40409, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
	local count = pUser:HowMuchItem(379111000);
	if count >= 0 and count <= 0 then
	pUser:SelectMsg(40408, 40010, 40410, 40041, 40041, 40090, 40490, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
elseif nEventID == 40409 then
	pUser:SelectMsg(40408, 40010, 40410, 40041, 40041, 40090, 40490, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 40410 then
	pUser:ZoneChange(32, 150, 237);
elseif nEventID == 40415 then
	local count = pUser:HowMuchItem(379080000);
	if count >= 0 and count <= 0 then
	pUser:NpcSay(40015, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
elseif nEventID == 40435 then
	pUser:SendDebugString("Unknown EXEC command 'ROB_ALLITEM_PARTY'."); -- unknown execute command (ROB_ALLITEM_PARTY)
	do return; end
	pUser:SendDebugString("Unknown EXEC command 'ZONE_CHANGE_PARTY'."); -- unknown execute command (ZONE_CHANGE_PARTY)
	do return; end
elseif nEventID == 40475 then
	pUser:RobItem(379111000, 1);
	pUser:SendDebugString("Unknown EXEC command 'ZONE_CHANGE_PARTY'."); -- unknown execute command (ZONE_CHANGE_PARTY)
	do return; end
elseif nEventID == 40490 then
	pUser:SelectMsg(40090, 40095, 40495, 40099, 40499, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 40495 then
	pUser:ZoneChange(32, 50, 437);
elseif nEventID == 40499 then
	pUser:ZoneChange(30, 500, 250);
elseif nEventID == 40501 then
	local count = pUser:HowMuchItem(379111000);
	if count >= 1 and count <= 9999 then
	pUser:SelectMsg(40071, 40075, 40575, 40009, 40509, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
	local count = pUser:HowMuchItem(379111000);
	if count >= 0 and count <= 0 then
	pUser:SelectMsg(40508, 40010, 40510, 40041, 40041, 40090, 40590, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
elseif nEventID == 40509 then
	pUser:SelectMsg(40508, 40010, 40510, 40041, 40041, 40090, 40590, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 40510 then
	pUser:ZoneChange(32, 150, 353);
elseif nEventID == 40515 then
	local count = pUser:HowMuchItem(379080000);
	if count >= 0 and count <= 0 then
	pUser:NpcSay(40015, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
elseif nEventID == 40535 then
	pUser:SendDebugString("Unknown EXEC command 'ROB_ALLITEM_PARTY'."); -- unknown execute command (ROB_ALLITEM_PARTY)
	do return; end
	pUser:SendDebugString("Unknown EXEC command 'ZONE_CHANGE_PARTY'."); -- unknown execute command (ZONE_CHANGE_PARTY)
	do return; end
elseif nEventID == 40575 then
	pUser:RobItem(379111000, 1);
	pUser:SendDebugString("Unknown EXEC command 'ZONE_CHANGE_PARTY'."); -- unknown execute command (ZONE_CHANGE_PARTY)
	do return; end
elseif nEventID == 40590 then
	pUser:SelectMsg(40090, 40095, 40595, 40099, 40599, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 40595 then
	pUser:ZoneChange(32, 150, 118);
elseif nEventID == 40599 then
	pUser:ZoneChange(30, 500, 250);
elseif nEventID == 40601 then
	local count = pUser:HowMuchItem(379111000);
	if count >= 1 and count <= 9999 then
	pUser:SelectMsg(40071, 40075, 40675, 40009, 40609, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
	local count = pUser:HowMuchItem(379111000);
	if count >= 0 and count <= 0 then
	pUser:SelectMsg(40608, 40010, 40610, 40041, 40041, 40090, 40690, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
elseif nEventID == 40609 then
	pUser:SelectMsg(40608, 40010, 40610, 40041, 40041, 40090, 40690, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 40610 then
	pUser:ZoneChange(32, 150, 473);
elseif nEventID == 40615 then
	local count = pUser:HowMuchItem(379080000);
	if count >= 0 and count <= 0 then
	pUser:NpcSay(40015, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
elseif nEventID == 40635 then
	pUser:SendDebugString("Unknown EXEC command 'ROB_ALLITEM_PARTY'."); -- unknown execute command (ROB_ALLITEM_PARTY)
	do return; end
	pUser:SendDebugString("Unknown EXEC command 'ZONE_CHANGE_PARTY'."); -- unknown execute command (ZONE_CHANGE_PARTY)
	do return; end
elseif nEventID == 40675 then
	pUser:RobItem(379111000, 1);
	pUser:SendDebugString("Unknown EXEC command 'ZONE_CHANGE_PARTY'."); -- unknown execute command (ZONE_CHANGE_PARTY)
	do return; end
elseif nEventID == 40690 then
	pUser:SelectMsg(40090, 40095, 40695, 40099, 40699, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 40695 then
	pUser:ZoneChange(32, 150, 237);
elseif nEventID == 40699 then
	pUser:ZoneChange(30, 500, 250);
elseif nEventID == 40701 then
	local count = pUser:HowMuchItem(379111000);
	if count >= 1 and count <= 9999 then
	pUser:SelectMsg(40071, 40075, 40775, 40009, 40709, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
	local count = pUser:HowMuchItem(379111000);
	if count >= 0 and count <= 0 then
	pUser:SelectMsg(40708, 40010, 40710, 40041, 40041, 40090, 40790, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
elseif nEventID == 40709 then
	pUser:SelectMsg(40708, 40010, 40710, 40041, 40041, 40090, 40790, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 40710 then
	pUser:ZoneChange(32, 250, 77);
elseif nEventID == 40715 then
	local count = pUser:HowMuchItem(379080000);
	if count >= 0 and count <= 0 then
	pUser:NpcSay(40015, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
elseif nEventID == 40735 then
	pUser:SendDebugString("Unknown EXEC command 'ROB_ALLITEM_PARTY'."); -- unknown execute command (ROB_ALLITEM_PARTY)
	do return; end
	pUser:SendDebugString("Unknown EXEC command 'ZONE_CHANGE_PARTY'."); -- unknown execute command (ZONE_CHANGE_PARTY)
	do return; end
elseif nEventID == 40775 then
	pUser:RobItem(379111000, 1);
	pUser:SendDebugString("Unknown EXEC command 'ZONE_CHANGE_PARTY'."); -- unknown execute command (ZONE_CHANGE_PARTY)
	do return; end
elseif nEventID == 40790 then
	pUser:SelectMsg(40090, 40095, 40795, 40099, 40799, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 40795 then
	pUser:ZoneChange(32, 150, 353);
elseif nEventID == 40799 then
	pUser:ZoneChange(30, 500, 250);
elseif nEventID == 40801 then
	local count = pUser:HowMuchItem(379111000);
	if count >= 1 and count <= 9999 then
	pUser:SelectMsg(40071, 40075, 40875, 40009, 40809, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
	local count = pUser:HowMuchItem(379111000);
	if count >= 0 and count <= 0 then
	pUser:SelectMsg(40808, 40010, 40810, 40041, 40041, 40090, 40890, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
elseif nEventID == 40809 then
	pUser:SelectMsg(40808, 40010, 40810, 40041, 40041, 40090, 40890, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 40810 then
	pUser:ZoneChange(32, 250, 192);
elseif nEventID == 40815 then
	local count = pUser:HowMuchItem(379080000);
	if count >= 0 and count <= 0 then
	pUser:NpcSay(40015, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
elseif nEventID == 40835 then
	pUser:SendDebugString("Unknown EXEC command 'ROB_ALLITEM_PARTY'."); -- unknown execute command (ROB_ALLITEM_PARTY)
	do return; end
	pUser:SendDebugString("Unknown EXEC command 'ZONE_CHANGE_PARTY'."); -- unknown execute command (ZONE_CHANGE_PARTY)
	do return; end
elseif nEventID == 40875 then
	pUser:RobItem(379111000, 1);
	pUser:SendDebugString("Unknown EXEC command 'ZONE_CHANGE_PARTY'."); -- unknown execute command (ZONE_CHANGE_PARTY)
	do return; end
elseif nEventID == 40890 then
	pUser:SelectMsg(40090, 40095, 40895, 40099, 40899, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 40895 then
	pUser:ZoneChange(32, 150, 473);
elseif nEventID == 40899 then
	pUser:ZoneChange(30, 500, 250);
elseif nEventID == 40901 then
	local count = pUser:HowMuchItem(379111000);
	if count >= 1 and count <= 9999 then
	pUser:SelectMsg(40071, 40075, 40975, 40009, 40909, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
	local count = pUser:HowMuchItem(379111000);
	if count >= 0 and count <= 0 then
	pUser:SelectMsg(40908, 40010, 40910, 40041, 40041, 40090, 40990, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
elseif nEventID == 40909 then
	pUser:SelectMsg(40908, 40010, 40910, 40041, 40041, 40090, 40990, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 40910 then
	pUser:ZoneChange(32, 250, 315);
elseif nEventID == 40915 then
	local count = pUser:HowMuchItem(379080000);
	if count >= 0 and count <= 0 then
	pUser:NpcSay(40015, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
elseif nEventID == 40935 then
	pUser:SendDebugString("Unknown EXEC command 'ROB_ALLITEM_PARTY'."); -- unknown execute command (ROB_ALLITEM_PARTY)
	do return; end
	pUser:SendDebugString("Unknown EXEC command 'ZONE_CHANGE_PARTY'."); -- unknown execute command (ZONE_CHANGE_PARTY)
	do return; end
elseif nEventID == 40975 then
	pUser:RobItem(379111000, 1);
	pUser:SendDebugString("Unknown EXEC command 'ZONE_CHANGE_PARTY'."); -- unknown execute command (ZONE_CHANGE_PARTY)
	do return; end
elseif nEventID == 40990 then
	pUser:SelectMsg(40090, 40095, 40995, 40099, 40999, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 40995 then
	pUser:ZoneChange(32, 250, 77);
elseif nEventID == 40999 then
	pUser:ZoneChange(30, 500, 250);
elseif nEventID == 41001 then
	local count = pUser:HowMuchItem(379111000);
	if count >= 1 and count <= 9999 then
	pUser:SelectMsg(40071, 40075, 41075, 40009, 41009, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
	local count = pUser:HowMuchItem(379111000);
	if count >= 0 and count <= 0 then
	pUser:SelectMsg(41008, 40010, 41010, 40041, 40041, 40090, 41090, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
elseif nEventID == 41009 then
	pUser:SelectMsg(41008, 40010, 41010, 40041, 40041, 40090, 41090, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 41010 then
	pUser:ZoneChange(32, 250, 435);
elseif nEventID == 41015 then
	local count = pUser:HowMuchItem(379080000);
	if count >= 0 and count <= 0 then
	pUser:NpcSay(40015, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
elseif nEventID == 41035 then
	pUser:SendDebugString("Unknown EXEC command 'ROB_ALLITEM_PARTY'."); -- unknown execute command (ROB_ALLITEM_PARTY)
	do return; end
	pUser:SendDebugString("Unknown EXEC command 'ZONE_CHANGE_PARTY'."); -- unknown execute command (ZONE_CHANGE_PARTY)
	do return; end
elseif nEventID == 41075 then
	pUser:RobItem(379111000, 1);
	pUser:SendDebugString("Unknown EXEC command 'ZONE_CHANGE_PARTY'."); -- unknown execute command (ZONE_CHANGE_PARTY)
	do return; end
elseif nEventID == 41090 then
	pUser:SelectMsg(40090, 40095, 41095, 40099, 41099, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 41095 then
	pUser:ZoneChange(32, 250, 192);
elseif nEventID == 41099 then
	pUser:ZoneChange(30, 500, 250);
elseif nEventID == 41101 then
	local count = pUser:HowMuchItem(379111000);
	if count >= 1 and count <= 9999 then
	pUser:SelectMsg(40071, 40075, 41175, 40009, 41109, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
	local count = pUser:HowMuchItem(379111000);
	if count >= 0 and count <= 0 then
	pUser:SelectMsg(41108, 40010, 41110, 40041, 40041, 40090, 41190, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
elseif nEventID == 41109 then
	pUser:SelectMsg(41108, 40010, 41110, 40041, 40041, 40090, 41190, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 41110 then
	local count = pUser:HowMuchItem(379080000);
	if count >= 0 and count <= 0 then
	pUser:NpcSay(40015, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown EXEC command 'ROB_ALLITEM_PARTY'."); -- unknown execute command (ROB_ALLITEM_PARTY)
	do return; end
	pUser:SendDebugString("Unknown EXEC command 'ZONE_CHANGE_PARTY'."); -- unknown execute command (ZONE_CHANGE_PARTY)
	do return; end
elseif nEventID == 41136 then
	pUser:ZoneChange(32, 350, 117);
elseif nEventID == 41175 then
	pUser:RobItem(379111000, 1);
	pUser:SendDebugString("Unknown EXEC command 'ZONE_CHANGE_PARTY'."); -- unknown execute command (ZONE_CHANGE_PARTY)
	do return; end
elseif nEventID == 41190 then
	pUser:SelectMsg(40090, 40095, 41195, 40099, 41199, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 41195 then
	pUser:ZoneChange(32, 250, 315);
elseif nEventID == 41199 then
	pUser:ZoneChange(30, 500, 250);
elseif nEventID == 41201 then
	local count = pUser:HowMuchItem(379111000);
	if count >= 1 and count <= 9999 then
	pUser:SelectMsg(40071, 40075, 41275, 40009, 41209, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
	local count = pUser:HowMuchItem(379111000);
	if count >= 0 and count <= 0 then
	pUser:SelectMsg(41208, 40010, 41210, 40041, 40041, 40090, 41290, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
elseif nEventID == 41209 then
	pUser:SelectMsg(41208, 40010, 41210, 40041, 40041, 40090, 41290, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 41210 then
	local count = pUser:HowMuchItem(379080000);
	if count >= 0 and count <= 0 then
	pUser:NpcSay(40015, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown EXEC command 'ROB_ALLITEM_PARTY'."); -- unknown execute command (ROB_ALLITEM_PARTY)
	do return; end
	pUser:SendDebugString("Unknown EXEC command 'ZONE_CHANGE_PARTY'."); -- unknown execute command (ZONE_CHANGE_PARTY)
	do return; end
elseif nEventID == 41236 then
	pUser:ZoneChange(32, 350, 235);
elseif nEventID == 41275 then
	pUser:RobItem(379111000, 1);
	pUser:SendDebugString("Unknown EXEC command 'ZONE_CHANGE_PARTY'."); -- unknown execute command (ZONE_CHANGE_PARTY)
	do return; end
elseif nEventID == 41290 then
	pUser:SelectMsg(40090, 40095, 41295, 40099, 41299, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 41295 then
	pUser:ZoneChange(32, 250, 435);
elseif nEventID == 41299 then
	pUser:ZoneChange(30, 500, 250);
elseif nEventID == 41301 then
	local count = pUser:HowMuchItem(379111000);
	if count >= 1 and count <= 9999 then
	pUser:SelectMsg(40071, 40075, 41375, 40009, 41309, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
	local count = pUser:HowMuchItem(379111000);
	if count >= 0 and count <= 0 then
	pUser:SelectMsg(41308, 40010, 41310, 40041, 40041, 40090, 41390, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
elseif nEventID == 41309 then
	pUser:SelectMsg(41308, 40010, 41310, 40041, 40041, 40090, 41390, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 41310 then
	local count = pUser:HowMuchItem(379080000);
	if count >= 0 and count <= 0 then
	pUser:NpcSay(40015, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown EXEC command 'ROB_ALLITEM_PARTY'."); -- unknown execute command (ROB_ALLITEM_PARTY)
	do return; end
	pUser:SendDebugString("Unknown EXEC command 'ZONE_CHANGE_PARTY'."); -- unknown execute command (ZONE_CHANGE_PARTY)
	do return; end
elseif nEventID == 41336 then
	pUser:ZoneChange(32, 350, 356);
elseif nEventID == 41375 then
	pUser:RobItem(379111000, 1);
	pUser:SendDebugString("Unknown EXEC command 'ZONE_CHANGE_PARTY'."); -- unknown execute command (ZONE_CHANGE_PARTY)
	do return; end
elseif nEventID == 41390 then
	pUser:SelectMsg(40090, 40095, 41395, 40099, 41399, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 41395 then
	pUser:ZoneChange(32, 350, 117);
elseif nEventID == 41399 then
	pUser:ZoneChange(30, 500, 250);
elseif nEventID == 41401 then
	local count = pUser:HowMuchItem(379111000);
	if count >= 1 and count <= 9999 then
	pUser:SelectMsg(40071, 40075, 41475, 40009, 41409, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
	local count = pUser:HowMuchItem(379111000);
	if count >= 0 and count <= 0 then
	pUser:SelectMsg(41408, 40010, 41410, 40041, 40041, 40090, 41490, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
elseif nEventID == 41409 then
	pUser:SelectMsg(41408, 40010, 41410, 40041, 40041, 40090, 41490, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 41410 then
	pUser:ZoneChange(32, 350, 475);
elseif nEventID == 41415 then
	local count = pUser:HowMuchItem(379080000);
	if count >= 0 and count <= 0 then
	pUser:NpcSay(40015, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
elseif nEventID == 41435 then
	pUser:SendDebugString("Unknown EXEC command 'ROB_ALLITEM_PARTY'."); -- unknown execute command (ROB_ALLITEM_PARTY)
	do return; end
	pUser:SendDebugString("Unknown EXEC command 'ZONE_CHANGE_PARTY'."); -- unknown execute command (ZONE_CHANGE_PARTY)
	do return; end
elseif nEventID == 41475 then
	pUser:RobItem(379111000, 1);
	pUser:SendDebugString("Unknown EXEC command 'ZONE_CHANGE_PARTY'."); -- unknown execute command (ZONE_CHANGE_PARTY)
	do return; end
elseif nEventID == 41490 then
	pUser:SelectMsg(40090, 40095, 41495, 40099, 41499, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 41495 then
	pUser:ZoneChange(32, 350, 235);
elseif nEventID == 41499 then
	pUser:ZoneChange(30, 500, 250);
elseif nEventID == 41501 then
	local count = pUser:HowMuchItem(379111000);
	if count >= 1 and count <= 9999 then
	pUser:SelectMsg(40071, 40075, 41575, 40009, 41509, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
	local count = pUser:HowMuchItem(379111000);
	if count >= 0 and count <= 0 then
	pUser:SelectMsg(41508, 40010, 41510, 40041, 40041, 40090, 41590, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
elseif nEventID == 41509 then
	pUser:SelectMsg(41508, 40010, 41510, 40041, 40041, 40090, 41590, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 41510 then
	local count = pUser:HowMuchItem(379080000);
	if count >= 0 and count <= 0 then
	pUser:NpcSay(40015, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown EXEC command 'ROB_ALLITEM_PARTY'."); -- unknown execute command (ROB_ALLITEM_PARTY)
	do return; end
	pUser:SendDebugString("Unknown EXEC command 'ZONE_CHANGE_PARTY'."); -- unknown execute command (ZONE_CHANGE_PARTY)
	do return; end
elseif nEventID == 41536 then
	pUser:ZoneChange(32, 450, 76);
elseif nEventID == 41575 then
	pUser:RobItem(379111000, 1);
	pUser:SendDebugString("Unknown EXEC command 'ZONE_CHANGE_PARTY'."); -- unknown execute command (ZONE_CHANGE_PARTY)
	do return; end
elseif nEventID == 41590 then
	pUser:SelectMsg(40090, 40095, 41595, 40099, 41599, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 41595 then
	pUser:ZoneChange(32, 350, 356);
elseif nEventID == 41599 then
	pUser:ZoneChange(30, 500, 250);
elseif nEventID == 41601 then
	local count = pUser:HowMuchItem(379111000);
	if count >= 1 and count <= 9999 then
	pUser:SelectMsg(40071, 40075, 41675, 40009, 41609, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
	local count = pUser:HowMuchItem(379111000);
	if count >= 0 and count <= 0 then
	pUser:SelectMsg(41608, 40010, 41610, 40041, 40041, 40090, 41690, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
elseif nEventID == 41609 then
	pUser:SelectMsg(41608, 40010, 41610, 40041, 40041, 40090, 41690, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 41610 then
	local count = pUser:HowMuchItem(379080000);
	if count >= 0 and count <= 0 then
	pUser:NpcSay(40015, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown EXEC command 'ROB_ALLITEM_PARTY'."); -- unknown execute command (ROB_ALLITEM_PARTY)
	do return; end
	pUser:SendDebugString("Unknown EXEC command 'ZONE_CHANGE_PARTY'."); -- unknown execute command (ZONE_CHANGE_PARTY)
	do return; end
elseif nEventID == 41636 then
	pUser:ZoneChange(32, 450, 195);
elseif nEventID == 41675 then
	pUser:RobItem(379111000, 1);
	pUser:SendDebugString("Unknown EXEC command 'ZONE_CHANGE_PARTY'."); -- unknown execute command (ZONE_CHANGE_PARTY)
	do return; end
elseif nEventID == 41690 then
	pUser:SelectMsg(40090, 40095, 41695, 40099, 41699, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 41695 then
	pUser:ZoneChange(32, 350, 475);
elseif nEventID == 41699 then
	pUser:ZoneChange(30, 500, 250);
elseif nEventID == 41701 then
	local count = pUser:HowMuchItem(379111000);
	if count >= 1 and count <= 9999 then
	pUser:SelectMsg(40071, 40075, 41775, 40009, 41709, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
	local count = pUser:HowMuchItem(379111000);
	if count >= 0 and count <= 0 then
	pUser:SelectMsg(41708, 40010, 41710, 40041, 40041, 40090, 41790, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
elseif nEventID == 41709 then
	pUser:SelectMsg(41708, 40010, 41710, 40041, 40041, 40090, 41790, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 41710 then
	pUser:ZoneChange(32, 450, 316);
elseif nEventID == 41715 then
	local count = pUser:HowMuchItem(379080000);
	if count >= 0 and count <= 0 then
	pUser:NpcSay(40015, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
elseif nEventID == 41735 then
	pUser:SendDebugString("Unknown EXEC command 'ROB_ALLITEM_PARTY'."); -- unknown execute command (ROB_ALLITEM_PARTY)
	do return; end
	pUser:SendDebugString("Unknown EXEC command 'ZONE_CHANGE_PARTY'."); -- unknown execute command (ZONE_CHANGE_PARTY)
	do return; end
elseif nEventID == 41775 then
	pUser:RobItem(379111000, 1);
	pUser:SendDebugString("Unknown EXEC command 'ZONE_CHANGE_PARTY'."); -- unknown execute command (ZONE_CHANGE_PARTY)
	do return; end
elseif nEventID == 41790 then
	pUser:SelectMsg(40090, 40095, 41795, 40099, 41799, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 41795 then
	pUser:ZoneChange(32, 450, 76);
elseif nEventID == 41799 then
	pUser:ZoneChange(30, 500, 250);
elseif nEventID == 41801 then
	local count = pUser:HowMuchItem(379111000);
	if count >= 1 and count <= 9999 then
	pUser:SelectMsg(40071, 40075, 41875, 40009, 41809, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
	local count = pUser:HowMuchItem(379111000);
	if count >= 0 and count <= 0 then
	pUser:SelectMsg(41808, 40010, 41810, 40041, 40041, 40090, 41890, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
elseif nEventID == 41809 then
	pUser:SelectMsg(41808, 40010, 41810, 40041, 40041, 40090, 41890, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 41810 then
	pUser:ZoneChange(32, 450, 432);
elseif nEventID == 41815 then
	local count = pUser:HowMuchItem(379080000);
	if count >= 0 and count <= 0 then
	pUser:NpcSay(40015, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
elseif nEventID == 41835 then
	pUser:SendDebugString("Unknown EXEC command 'ROB_ALLITEM_PARTY'."); -- unknown execute command (ROB_ALLITEM_PARTY)
	do return; end
	pUser:SendDebugString("Unknown EXEC command 'ZONE_CHANGE_PARTY'."); -- unknown execute command (ZONE_CHANGE_PARTY)
	do return; end
elseif nEventID == 41875 then
	pUser:RobItem(379111000, 1);
	pUser:SendDebugString("Unknown EXEC command 'ZONE_CHANGE_PARTY'."); -- unknown execute command (ZONE_CHANGE_PARTY)
	do return; end
elseif nEventID == 41890 then
	pUser:SelectMsg(40090, 40095, 41895, 40099, 41899, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 41895 then
	pUser:ZoneChange(32, 450, 195);
elseif nEventID == 41899 then
	pUser:ZoneChange(30, 500, 250);
elseif nEventID == 41901 then
	local count = pUser:HowMuchItem(379111000);
	if count >= 1 and count <= 9999 then
	pUser:SelectMsg(40071, 40075, 41975, 40009, 41909, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
	local count = pUser:HowMuchItem(379111000);
	if count >= 0 and count <= 0 then
	pUser:SelectMsg(41908, 40090, 41990, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
elseif nEventID == 41909 then
	pUser:SelectMsg(41908, 40090, 41990, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 41910 then
	pUser:ZoneChange(32, 0, 0);
elseif nEventID == 41915 then
	local count = pUser:HowMuchItem(379080000);
	if count >= 0 and count <= 0 then
	pUser:NpcSay(40015, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
elseif nEventID == 41935 then
	pUser:SendDebugString("Unknown EXEC command 'ROB_ALLITEM_PARTY'."); -- unknown execute command (ROB_ALLITEM_PARTY)
	do return; end
	pUser:SendDebugString("Unknown EXEC command 'ZONE_CHANGE_PARTY'."); -- unknown execute command (ZONE_CHANGE_PARTY)
	do return; end
elseif nEventID == 41975 then
	pUser:RobItem(379111000, 1);
	pUser:SendDebugString("Unknown EXEC command 'ZONE_CHANGE_PARTY'."); -- unknown execute command (ZONE_CHANGE_PARTY)
	do return; end
elseif nEventID == 41990 then
	pUser:SelectMsg(40090, 40095, 41995, 40099, 41999, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 41995 then
	pUser:ZoneChange(32, 450, 316);
elseif nEventID == 41999 then
	pUser:ZoneChange(30, 500, 250);
else
	pUser:SendDebugString("Unprocessed event id ("..nEventID..")");
end
