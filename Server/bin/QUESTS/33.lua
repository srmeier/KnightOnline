if nEventID == 22001 then
	pUser:SelectMsg(53000, 20018, 22010, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 22010 then
	pUser:RobItem(989200000, 1);
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
elseif nEventID == 42001 then
	local count = pUser:HowMuchItem(389025000);
	if count >= 1 and count <= 9999 then
	pUser:SelectMsg(40071, 40075, 42075, 40009, 42009, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
	local count = pUser:HowMuchItem(389025000);
	if count >= 0 and count <= 0 then
	pUser:SelectMsg(40008, 40010, 42010, 40041, 40041, 40090, 42090, 40076, 42076, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
elseif nEventID == 42009 then
	pUser:SelectMsg(40008, 40010, 42010, 40041, 40041, 40090, 42090, 40076, 42076, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 42010 then
	pUser:ZoneChange(33, 62, 226);
elseif nEventID == 42015 then
	local count = pUser:HowMuchItem(379080000);
	if count >= 0 and count <= 0 then
	pUser:NpcSay(40015, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
elseif nEventID == 42035 then
	pUser:SendDebugString("Unknown EXEC command 'ROB_ALLITEM_PARTY'."); -- unknown execute command (ROB_ALLITEM_PARTY)
	do return; end
	pUser:SendDebugString("Unknown EXEC command 'ZONE_CHANGE_PARTY'."); -- unknown execute command (ZONE_CHANGE_PARTY)
	do return; end
elseif nEventID == 42075 then
	pUser:RobItem(379111000, 1);
	pUser:SendDebugString("Unknown EXEC command 'ZONE_CHANGE_PARTY'."); -- unknown execute command (ZONE_CHANGE_PARTY)
	do return; end
elseif nEventID == 42076 then
	pUser:ZoneChange(32, 50, 74);
elseif nEventID == 42090 then
	pUser:SelectMsg(40090, 40099, 42099, 40076, 42076, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 42095 then
elseif nEventID == 42099 then
	pUser:ZoneChange(30, 500, 250);
elseif nEventID == 42101 then
	local count = pUser:HowMuchItem(389025000);
	if count >= 1 and count <= 9999 then
	pUser:SelectMsg(40071, 40075, 42175, 40009, 42109, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
	local count = pUser:HowMuchItem(389025000);
	if count >= 0 and count <= 0 then
	pUser:SelectMsg(40108, 40010, 42110, 40041, 40041, 40090, 42190, 40076, 42176, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
elseif nEventID == 42109 then
	pUser:SelectMsg(40108, 40010, 42110, 40041, 40041, 40090, 42190, 40076, 42176, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 42110 then
	pUser:ZoneChange(33, 62, 351);
elseif nEventID == 42115 then
	local count = pUser:HowMuchItem(379080000);
	if count >= 0 and count <= 0 then
	pUser:NpcSay(40015, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
elseif nEventID == 42135 then
	pUser:SendDebugString("Unknown EXEC command 'ROB_ALLITEM_PARTY'."); -- unknown execute command (ROB_ALLITEM_PARTY)
	do return; end
	pUser:SendDebugString("Unknown EXEC command 'ZONE_CHANGE_PARTY'."); -- unknown execute command (ZONE_CHANGE_PARTY)
	do return; end
elseif nEventID == 42175 then
	pUser:RobItem(379111000, 1);
	pUser:SendDebugString("Unknown EXEC command 'ZONE_CHANGE_PARTY'."); -- unknown execute command (ZONE_CHANGE_PARTY)
	do return; end
elseif nEventID == 42176 then
	pUser:ZoneChange(32, 50, 74);
elseif nEventID == 42190 then
	pUser:SelectMsg(40090, 40095, 42195, 40099, 42199, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 42195 then
	pUser:ZoneChange(33, 62, 91);
elseif nEventID == 42199 then
	pUser:ZoneChange(30, 500, 250);
elseif nEventID == 42201 then
	local count = pUser:HowMuchItem(389025000);
	if count >= 1 and count <= 9999 then
	pUser:SelectMsg(40071, 40075, 42275, 40009, 42209, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
	local count = pUser:HowMuchItem(389025000);
	if count >= 0 and count <= 0 then
	pUser:SelectMsg(40208, 40010, 42210, 40041, 40041, 40090, 42290, 40076, 42276, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
elseif nEventID == 42209 then
	pUser:SelectMsg(40208, 40010, 42210, 40041, 40041, 40090, 42290, 40076, 42276, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 42210 then
	pUser:ZoneChange(33, 62, 477);
elseif nEventID == 42215 then
	local count = pUser:HowMuchItem(379080000);
	if count >= 0 and count <= 0 then
	pUser:NpcSay(40015, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
elseif nEventID == 42235 then
	pUser:SendDebugString("Unknown EXEC command 'ROB_ALLITEM_PARTY'."); -- unknown execute command (ROB_ALLITEM_PARTY)
	do return; end
	pUser:SendDebugString("Unknown EXEC command 'ZONE_CHANGE_PARTY'."); -- unknown execute command (ZONE_CHANGE_PARTY)
	do return; end
elseif nEventID == 42275 then
	pUser:RobItem(379111000, 1);
	pUser:SendDebugString("Unknown EXEC command 'ZONE_CHANGE_PARTY'."); -- unknown execute command (ZONE_CHANGE_PARTY)
	do return; end
elseif nEventID == 42276 then
	pUser:ZoneChange(32, 50, 74);
elseif nEventID == 42290 then
	pUser:SelectMsg(40090, 40095, 42295, 40099, 42299, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 42295 then
	pUser:ZoneChange(33, 62, 226);
elseif nEventID == 42299 then
	pUser:ZoneChange(30, 500, 250);
elseif nEventID == 42301 then
	local count = pUser:HowMuchItem(389025000);
	if count >= 1 and count <= 9999 then
	pUser:SelectMsg(40071, 40075, 42375, 40009, 42309, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
	local count = pUser:HowMuchItem(389025000);
	if count >= 0 and count <= 0 then
	pUser:SelectMsg(40308, 40010, 42310, 40041, 40041, 40090, 42390, 40076, 42376, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
elseif nEventID == 42309 then
	pUser:SelectMsg(40308, 40010, 42310, 40041, 40041, 40090, 42390, 40076, 42376, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 42310 then
	pUser:ZoneChange(33, 189, 94);
elseif nEventID == 42315 then
	local count = pUser:HowMuchItem(379080000);
	if count >= 0 and count <= 0 then
	pUser:NpcSay(40015, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
elseif nEventID == 42335 then
	pUser:SendDebugString("Unknown EXEC command 'ROB_ALLITEM_PARTY'."); -- unknown execute command (ROB_ALLITEM_PARTY)
	do return; end
	pUser:SendDebugString("Unknown EXEC command 'ZONE_CHANGE_PARTY'."); -- unknown execute command (ZONE_CHANGE_PARTY)
	do return; end
elseif nEventID == 42375 then
	pUser:RobItem(379111000, 1);
	pUser:SendDebugString("Unknown EXEC command 'ZONE_CHANGE_PARTY'."); -- unknown execute command (ZONE_CHANGE_PARTY)
	do return; end
elseif nEventID == 42376 then
	pUser:ZoneChange(32, 50, 74);
elseif nEventID == 42390 then
	pUser:SelectMsg(40090, 40095, 42395, 40099, 42399, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 42395 then
	pUser:ZoneChange(33, 62, 351);
elseif nEventID == 42399 then
	pUser:ZoneChange(30, 500, 250);
elseif nEventID == 42401 then
	local count = pUser:HowMuchItem(389025000);
	if count >= 1 and count <= 9999 then
	pUser:SelectMsg(40071, 40075, 42475, 40009, 42409, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
	local count = pUser:HowMuchItem(389025000);
	if count >= 0 and count <= 0 then
	pUser:SelectMsg(40408, 40010, 42410, 40041, 40041, 40090, 42490, 40076, 42476, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
elseif nEventID == 42409 then
	pUser:SelectMsg(40408, 40010, 42410, 40041, 40041, 40090, 42490, 40076, 42476, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 42410 then
	pUser:ZoneChange(33, 189, 225);
elseif nEventID == 42415 then
	local count = pUser:HowMuchItem(379080000);
	if count >= 0 and count <= 0 then
	pUser:NpcSay(40015, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
elseif nEventID == 42435 then
	pUser:SendDebugString("Unknown EXEC command 'ROB_ALLITEM_PARTY'."); -- unknown execute command (ROB_ALLITEM_PARTY)
	do return; end
	pUser:SendDebugString("Unknown EXEC command 'ZONE_CHANGE_PARTY'."); -- unknown execute command (ZONE_CHANGE_PARTY)
	do return; end
elseif nEventID == 42475 then
	pUser:RobItem(379111000, 1);
	pUser:SendDebugString("Unknown EXEC command 'ZONE_CHANGE_PARTY'."); -- unknown execute command (ZONE_CHANGE_PARTY)
	do return; end
elseif nEventID == 42476 then
	pUser:ZoneChange(32, 50, 74);
elseif nEventID == 42490 then
	pUser:SelectMsg(40090, 40095, 42495, 40099, 42499, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 42495 then
	pUser:ZoneChange(33, 62, 477);
elseif nEventID == 42499 then
	pUser:ZoneChange(30, 500, 250);
elseif nEventID == 42501 then
	local count = pUser:HowMuchItem(389025000);
	if count >= 1 and count <= 9999 then
	pUser:SelectMsg(40071, 40075, 42575, 40009, 42509, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
	local count = pUser:HowMuchItem(389025000);
	if count >= 0 and count <= 0 then
	pUser:SelectMsg(40508, 40010, 42510, 40041, 40041, 40090, 42590, 40076, 42576, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
elseif nEventID == 42509 then
	pUser:SelectMsg(40508, 40010, 42510, 40041, 40041, 40090, 42590, 40076, 42576, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 42510 then
	pUser:ZoneChange(33, 189, 348);
elseif nEventID == 42515 then
	local count = pUser:HowMuchItem(379080000);
	if count >= 0 and count <= 0 then
	pUser:NpcSay(40015, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
elseif nEventID == 42535 then
	pUser:SendDebugString("Unknown EXEC command 'ROB_ALLITEM_PARTY'."); -- unknown execute command (ROB_ALLITEM_PARTY)
	do return; end
	pUser:SendDebugString("Unknown EXEC command 'ZONE_CHANGE_PARTY'."); -- unknown execute command (ZONE_CHANGE_PARTY)
	do return; end
elseif nEventID == 42575 then
	pUser:RobItem(379111000, 1);
	pUser:SendDebugString("Unknown EXEC command 'ZONE_CHANGE_PARTY'."); -- unknown execute command (ZONE_CHANGE_PARTY)
	do return; end
elseif nEventID == 42576 then
	pUser:ZoneChange(32, 50, 74);
elseif nEventID == 42590 then
	pUser:SelectMsg(40090, 40095, 42595, 40099, 42599, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 42595 then
	pUser:ZoneChange(33, 189, 94);
elseif nEventID == 42599 then
	pUser:ZoneChange(30, 500, 250);
elseif nEventID == 42601 then
	local count = pUser:HowMuchItem(389025000);
	if count >= 1 and count <= 9999 then
	pUser:SelectMsg(40071, 40075, 42675, 40009, 42609, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
	local count = pUser:HowMuchItem(389025000);
	if count >= 0 and count <= 0 then
	pUser:SelectMsg(40608, 40010, 42610, 40041, 40041, 40090, 42690, 40076, 42676, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
elseif nEventID == 42609 then
	pUser:SelectMsg(40608, 40010, 42610, 40041, 40041, 40090, 42690, 40076, 42676, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 42610 then
	pUser:ZoneChange(33, 189, 458);
elseif nEventID == 42615 then
	local count = pUser:HowMuchItem(379080000);
	if count >= 0 and count <= 0 then
	pUser:NpcSay(40015, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
elseif nEventID == 42635 then
	pUser:SendDebugString("Unknown EXEC command 'ROB_ALLITEM_PARTY'."); -- unknown execute command (ROB_ALLITEM_PARTY)
	do return; end
	pUser:SendDebugString("Unknown EXEC command 'ZONE_CHANGE_PARTY'."); -- unknown execute command (ZONE_CHANGE_PARTY)
	do return; end
elseif nEventID == 42675 then
	pUser:RobItem(379111000, 1);
	pUser:SendDebugString("Unknown EXEC command 'ZONE_CHANGE_PARTY'."); -- unknown execute command (ZONE_CHANGE_PARTY)
	do return; end
elseif nEventID == 42676 then
	pUser:ZoneChange(32, 50, 74);
elseif nEventID == 42690 then
	pUser:SelectMsg(40090, 40095, 42695, 40099, 42699, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 42695 then
	pUser:ZoneChange(33, 189, 225);
elseif nEventID == 42699 then
	pUser:ZoneChange(30, 500, 250);
elseif nEventID == 42701 then
	local count = pUser:HowMuchItem(389025000);
	if count >= 1 and count <= 9999 then
	pUser:SelectMsg(40071, 40075, 42775, 40009, 42709, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
	local count = pUser:HowMuchItem(389025000);
	if count >= 0 and count <= 0 then
	pUser:SelectMsg(40708, 40010, 42710, 40041, 40041, 40090, 42790, 40076, 42776, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
elseif nEventID == 42709 then
	pUser:SelectMsg(40708, 40010, 42710, 40041, 40041, 40090, 42790, 40076, 42776, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 42710 then
	pUser:ZoneChange(33, 315, 90);
elseif nEventID == 42715 then
	local count = pUser:HowMuchItem(379080000);
	if count >= 0 and count <= 0 then
	pUser:NpcSay(40015, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
elseif nEventID == 42735 then
	pUser:SendDebugString("Unknown EXEC command 'ROB_ALLITEM_PARTY'."); -- unknown execute command (ROB_ALLITEM_PARTY)
	do return; end
	pUser:SendDebugString("Unknown EXEC command 'ZONE_CHANGE_PARTY'."); -- unknown execute command (ZONE_CHANGE_PARTY)
	do return; end
elseif nEventID == 42775 then
	pUser:RobItem(379111000, 1);
	pUser:SendDebugString("Unknown EXEC command 'ZONE_CHANGE_PARTY'."); -- unknown execute command (ZONE_CHANGE_PARTY)
	do return; end
elseif nEventID == 42776 then
	pUser:ZoneChange(32, 50, 74);
elseif nEventID == 42790 then
	pUser:SelectMsg(40090, 40095, 42795, 40099, 42799, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 42795 then
	pUser:ZoneChange(33, 189, 348);
elseif nEventID == 42799 then
	pUser:ZoneChange(30, 500, 250);
elseif nEventID == 42801 then
	local count = pUser:HowMuchItem(389025000);
	if count >= 1 and count <= 9999 then
	pUser:SelectMsg(40071, 40075, 42875, 40009, 42809, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
	local count = pUser:HowMuchItem(389025000);
	if count >= 0 and count <= 0 then
	pUser:SelectMsg(40808, 40010, 42810, 40041, 40041, 40090, 42890, 40076, 42876, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
elseif nEventID == 42809 then
	pUser:SelectMsg(40808, 40010, 42810, 40041, 40041, 40090, 42890, 40076, 42876, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 42810 then
	pUser:ZoneChange(33, 315, 225);
elseif nEventID == 42815 then
	local count = pUser:HowMuchItem(379080000);
	if count >= 0 and count <= 0 then
	pUser:NpcSay(40015, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
elseif nEventID == 42835 then
	pUser:SendDebugString("Unknown EXEC command 'ROB_ALLITEM_PARTY'."); -- unknown execute command (ROB_ALLITEM_PARTY)
	do return; end
	pUser:SendDebugString("Unknown EXEC command 'ZONE_CHANGE_PARTY'."); -- unknown execute command (ZONE_CHANGE_PARTY)
	do return; end
elseif nEventID == 42875 then
	pUser:RobItem(379111000, 1);
	pUser:SendDebugString("Unknown EXEC command 'ZONE_CHANGE_PARTY'."); -- unknown execute command (ZONE_CHANGE_PARTY)
	do return; end
elseif nEventID == 42876 then
	pUser:ZoneChange(32, 50, 74);
elseif nEventID == 42890 then
	pUser:SelectMsg(40090, 40095, 42895, 40099, 42899, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 42895 then
	pUser:ZoneChange(33, 189, 458);
elseif nEventID == 42899 then
	pUser:ZoneChange(30, 500, 250);
elseif nEventID == 42901 then
	local count = pUser:HowMuchItem(389025000);
	if count >= 1 and count <= 9999 then
	pUser:SelectMsg(40071, 40075, 42975, 40009, 42909, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
	local count = pUser:HowMuchItem(389025000);
	if count >= 0 and count <= 0 then
	pUser:SelectMsg(40908, 40010, 42910, 40041, 40041, 40090, 42990, 40076, 42976, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
elseif nEventID == 42909 then
	pUser:SelectMsg(40908, 40010, 42910, 40041, 40041, 40090, 42990, 40076, 42976, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 42910 then
	pUser:ZoneChange(33, 315, 351);
elseif nEventID == 42915 then
	local count = pUser:HowMuchItem(379080000);
	if count >= 0 and count <= 0 then
	pUser:NpcSay(40015, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
elseif nEventID == 42935 then
	pUser:SendDebugString("Unknown EXEC command 'ROB_ALLITEM_PARTY'."); -- unknown execute command (ROB_ALLITEM_PARTY)
	do return; end
	pUser:SendDebugString("Unknown EXEC command 'ZONE_CHANGE_PARTY'."); -- unknown execute command (ZONE_CHANGE_PARTY)
	do return; end
elseif nEventID == 42975 then
	pUser:RobItem(379111000, 1);
	pUser:SendDebugString("Unknown EXEC command 'ZONE_CHANGE_PARTY'."); -- unknown execute command (ZONE_CHANGE_PARTY)
	do return; end
elseif nEventID == 42976 then
	pUser:ZoneChange(32, 50, 74);
elseif nEventID == 42990 then
	pUser:SelectMsg(40090, 40095, 42995, 40099, 42999, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 42995 then
	pUser:ZoneChange(33, 315, 90);
elseif nEventID == 42999 then
	pUser:ZoneChange(30, 500, 250);
elseif nEventID == 43001 then
	local count = pUser:HowMuchItem(389025000);
	if count >= 1 and count <= 9999 then
	pUser:SelectMsg(40071, 40075, 43075, 40009, 43009, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
	local count = pUser:HowMuchItem(389025000);
	if count >= 0 and count <= 0 then
	pUser:SelectMsg(41008, 40010, 43010, 40041, 40041, 40090, 43090, 40076, 43076, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
elseif nEventID == 43009 then
	pUser:SelectMsg(41008, 40010, 43010, 40041, 40041, 40090, 43090, 40076, 43076, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 43010 then
	local count = pUser:HowMuchItem(379080000);
	if count >= 0 and count <= 0 then
	pUser:NpcSay(40015, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown EXEC command 'ROB_ALLITEM_PARTY'."); -- unknown execute command (ROB_ALLITEM_PARTY)
	do return; end
	pUser:SendDebugString("Unknown EXEC command 'ZONE_CHANGE_PARTY'."); -- unknown execute command (ZONE_CHANGE_PARTY)
	do return; end
elseif nEventID == 43036 then
	pUser:ZoneChange(33, 315, 475);
elseif nEventID == 43075 then
	pUser:RobItem(379111000, 1);
	pUser:SendDebugString("Unknown EXEC command 'ZONE_CHANGE_PARTY'."); -- unknown execute command (ZONE_CHANGE_PARTY)
	do return; end
elseif nEventID == 43076 then
	pUser:ZoneChange(32, 50, 74);
elseif nEventID == 43090 then
	pUser:SelectMsg(40090, 40095, 43095, 40099, 43099, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 43095 then
	pUser:ZoneChange(33, 315, 225);
elseif nEventID == 43099 then
	pUser:ZoneChange(30, 500, 250);
elseif nEventID == 43101 then
	local count = pUser:HowMuchItem(389025000);
	if count >= 1 and count <= 9999 then
	pUser:SelectMsg(40071, 40075, 43175, 40009, 43109, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
	local count = pUser:HowMuchItem(389025000);
	if count >= 0 and count <= 0 then
	pUser:SelectMsg(41108, 40010, 43110, 40041, 40041, 40090, 43190, 40076, 43176, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
elseif nEventID == 43109 then
	pUser:SelectMsg(41108, 40010, 43110, 40041, 40041, 40090, 43190, 40076, 43176, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 43110 then
	pUser:ZoneChange(33, 444, 102);
elseif nEventID == 43115 then
	local count = pUser:HowMuchItem(379080000);
	if count >= 0 and count <= 0 then
	pUser:NpcSay(40015, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
elseif nEventID == 43135 then
	pUser:SendDebugString("Unknown EXEC command 'ROB_ALLITEM_PARTY'."); -- unknown execute command (ROB_ALLITEM_PARTY)
	do return; end
	pUser:SendDebugString("Unknown EXEC command 'ZONE_CHANGE_PARTY'."); -- unknown execute command (ZONE_CHANGE_PARTY)
	do return; end
elseif nEventID == 43175 then
	pUser:RobItem(379111000, 1);
	pUser:SendDebugString("Unknown EXEC command 'ZONE_CHANGE_PARTY'."); -- unknown execute command (ZONE_CHANGE_PARTY)
	do return; end
elseif nEventID == 43176 then
	pUser:ZoneChange(32, 50, 74);
elseif nEventID == 43190 then
	pUser:SelectMsg(40090, 40095, 43195, 40099, 43199, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 43195 then
	pUser:ZoneChange(33, 315, 351);
elseif nEventID == 43199 then
	pUser:ZoneChange(30, 500, 250);
elseif nEventID == 43201 then
	local count = pUser:HowMuchItem(389025000);
	if count >= 1 and count <= 9999 then
	pUser:SelectMsg(40071, 40075, 43275, 40009, 43209, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
	local count = pUser:HowMuchItem(389025000);
	if count >= 0 and count <= 0 then
	pUser:SelectMsg(41208, 40010, 43210, 40041, 40041, 40090, 43290, 40076, 43276, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
elseif nEventID == 43209 then
	pUser:SelectMsg(41208, 40010, 43210, 40041, 40041, 40090, 43290, 40076, 43276, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 43210 then
	local count = pUser:HowMuchItem(379080000);
	if count >= 0 and count <= 0 then
	pUser:NpcSay(40015, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown EXEC command 'ROB_ALLITEM_PARTY'."); -- unknown execute command (ROB_ALLITEM_PARTY)
	do return; end
	pUser:SendDebugString("Unknown EXEC command 'ZONE_CHANGE_PARTY'."); -- unknown execute command (ZONE_CHANGE_PARTY)
	do return; end
elseif nEventID == 43236 then
	pUser:ZoneChange(33, 444, 224);
elseif nEventID == 43275 then
	pUser:RobItem(379111000, 1);
	pUser:SendDebugString("Unknown EXEC command 'ZONE_CHANGE_PARTY'."); -- unknown execute command (ZONE_CHANGE_PARTY)
	do return; end
elseif nEventID == 43276 then
	pUser:ZoneChange(32, 50, 74);
elseif nEventID == 43290 then
	pUser:SelectMsg(40090, 40095, 43295, 40099, 43299, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 43295 then
	pUser:ZoneChange(33, 315, 475);
elseif nEventID == 43299 then
	pUser:ZoneChange(30, 500, 250);
elseif nEventID == 43301 then
	local count = pUser:HowMuchItem(389025000);
	if count >= 1 and count <= 9999 then
	pUser:SelectMsg(40071, 40075, 43375, 40009, 43309, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
	local count = pUser:HowMuchItem(389025000);
	if count >= 0 and count <= 0 then
	pUser:SelectMsg(41308, 40010, 43310, 40041, 40041, 40090, 43390, 40076, 43376, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
elseif nEventID == 43309 then
	pUser:SelectMsg(41308, 40010, 43310, 40041, 40041, 40090, 43390, 40076, 43376, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 43310 then
	local count = pUser:HowMuchItem(379080000);
	if count >= 0 and count <= 0 then
	pUser:NpcSay(40015, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown EXEC command 'ROB_ALLITEM_PARTY'."); -- unknown execute command (ROB_ALLITEM_PARTY)
	do return; end
	pUser:SendDebugString("Unknown EXEC command 'ZONE_CHANGE_PARTY'."); -- unknown execute command (ZONE_CHANGE_PARTY)
	do return; end
elseif nEventID == 43336 then
	pUser:ZoneChange(33, 444, 224);
elseif nEventID == 43375 then
	pUser:RobItem(379111000, 1);
	pUser:SendDebugString("Unknown EXEC command 'ZONE_CHANGE_PARTY'."); -- unknown execute command (ZONE_CHANGE_PARTY)
	do return; end
elseif nEventID == 43376 then
	pUser:ZoneChange(32, 50, 74);
elseif nEventID == 43390 then
	pUser:SelectMsg(40090, 40095, 43395, 40099, 43399, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 43395 then
	pUser:ZoneChange(33, 444, 102);
elseif nEventID == 43399 then
	pUser:ZoneChange(30, 500, 250);
elseif nEventID == 43401 then
	local count = pUser:HowMuchItem(389025000);
	if count >= 1 and count <= 9999 then
	pUser:SelectMsg(40071, 40075, 43475, 40009, 43409, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
	local count = pUser:HowMuchItem(389025000);
	if count >= 0 and count <= 0 then
	pUser:SelectMsg(41408, 40010, 43410, 40041, 40041, 40090, 43490, 40076, 43476, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
elseif nEventID == 43409 then
	pUser:SelectMsg(41408, 40010, 43410, 40041, 40041, 40090, 43490, 40076, 43476, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 43410 then
	local count = pUser:HowMuchItem(379080000);
	if count >= 0 and count <= 0 then
	pUser:NpcSay(40015, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown EXEC command 'ROB_ALLITEM_PARTY'."); -- unknown execute command (ROB_ALLITEM_PARTY)
	do return; end
	pUser:SendDebugString("Unknown EXEC command 'ZONE_CHANGE_PARTY'."); -- unknown execute command (ZONE_CHANGE_PARTY)
	do return; end
elseif nEventID == 43436 then
	pUser:ZoneChange(33, 444, 458);
elseif nEventID == 43475 then
	pUser:RobItem(379111000, 1);
	pUser:SendDebugString("Unknown EXEC command 'ZONE_CHANGE_PARTY'."); -- unknown execute command (ZONE_CHANGE_PARTY)
	do return; end
elseif nEventID == 43476 then
	pUser:ZoneChange(32, 50, 74);
elseif nEventID == 43490 then
	pUser:SelectMsg(40090, 40095, 43495, 40099, 43499, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 43495 then
	pUser:ZoneChange(33, 444, 224);
elseif nEventID == 43499 then
	pUser:ZoneChange(30, 500, 250);
elseif nEventID == 43501 then
	local count = pUser:HowMuchItem(389025000);
	if count >= 1 and count <= 9999 then
	pUser:SelectMsg(40071, 40075, 43575, 40009, 43509, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
	local count = pUser:HowMuchItem(389025000);
	if count >= 0 and count <= 0 then
	pUser:SelectMsg(41508, 40090, 43590, 40076, 43576, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
elseif nEventID == 43509 then
	pUser:SelectMsg(41508, 40090, 43590, 40076, 43576, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 43510 then
	pUser:ZoneChange(33, 444, 458);
elseif nEventID == 43515 then
	local count = pUser:HowMuchItem(379080000);
	if count >= 0 and count <= 0 then
	pUser:NpcSay(40015, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
elseif nEventID == 43535 then
	pUser:SendDebugString("Unknown EXEC command 'ROB_ALLITEM_PARTY'."); -- unknown execute command (ROB_ALLITEM_PARTY)
	do return; end
	pUser:SendDebugString("Unknown EXEC command 'ZONE_CHANGE_PARTY'."); -- unknown execute command (ZONE_CHANGE_PARTY)
	do return; end
elseif nEventID == 43575 then
	pUser:RobItem(379111000, 1);
	pUser:SendDebugString("Unknown EXEC command 'ZONE_CHANGE_PARTY'."); -- unknown execute command (ZONE_CHANGE_PARTY)
	do return; end
elseif nEventID == 43576 then
	pUser:ZoneChange(32, 50, 74);
elseif nEventID == 43590 then
	pUser:SelectMsg(40090, 40099, 43599, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 43595 then
	pUser:ZoneChange(33, 444, 224);
elseif nEventID == 43599 then
	pUser:ZoneChange(30, 500, 250);
else
	pUser:SendDebugString("Unprocessed event id ("..nEventID..")");
end
