if nEventID == 20 then
	pUser:SelectMsg(20, 21, 21, 22, 22, 23, 23, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 21 then
	pUser:NpcSay(21, 22, 23, 24, -1, -1, -1, -1);
	do return; end
elseif nEventID == 22 then
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_MONSTER_CHALLENGE_TIME'.");
	if false then -- unknown logic command (CHECK_MONSTER_CHALLENGE_TIME)
	local lvl = pUser:GetLevel();
	if lvl >= 60 and lvl <= 99 then
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_MONSTER_CHALLENGE_USERCOUNT'.");
	if false then -- unknown logic command (CHECK_MONSTER_CHALLENGE_USERCOUNT)
	pUser:NpcSay(32, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	local coins = pUser:GetCoins();
	if coins >= 0 and coins <= 499999 then
	pUser:NpcSay(34, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:GoldLose(500000);
	pUser:SendDebugString("Unknown EXEC command 'ZONE_CHANGE'."); -- unknown execute command (ZONE_CHANGE)
	do return; end
	end
	pUser:NpcSay(30, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:NpcSay(27, -1, -1, -1, -1, -1, -1, -1);
	do return; end
elseif nEventID == 23 then
	pUser:NpcSay(25, -1, -1, -1, -1, -1, -1, -1);
	do return; end
elseif nEventID == 36 then
	pUser:SelectMsg(36, 21, 37, 22, 38, 23, 39, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 37 then
	pUser:NpcSay(21, 22, 23, 24, -1, -1, -1, -1);
	do return; end
elseif nEventID == 38 then
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_MONSTER_CHALLENGE_TIME'.");
	if false then -- unknown logic command (CHECK_MONSTER_CHALLENGE_TIME)
	local lvl = pUser:GetLevel();
	if lvl >= 46 and lvl <= 59 then
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_MONSTER_CHALLENGE_USERCOUNT'.");
	if false then -- unknown logic command (CHECK_MONSTER_CHALLENGE_USERCOUNT)
	pUser:NpcSay(32, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	local coins = pUser:GetCoins();
	if coins >= 0 and coins <= 99999 then
	pUser:NpcSay(48, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:GoldLose(100000);
	pUser:SendDebugString("Unknown EXEC command 'ZONE_CHANGE'."); -- unknown execute command (ZONE_CHANGE)
	do return; end
	end
	pUser:NpcSay(44, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:NpcSay(41, -1, -1, -1, -1, -1, -1, -1);
	do return; end
elseif nEventID == 39 then
	pUser:NpcSay(25, -1, -1, -1, -1, -1, -1, -1);
	do return; end
elseif nEventID == 50 then
	pUser:SelectMsg(50, 21, 51, 22, 52, 23, 53, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 51 then
	pUser:NpcSay(21, 22, 23, 24, -1, -1, -1, -1);
	do return; end
elseif nEventID == 52 then
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_MONSTER_CHALLENGE_TIME'.");
	if false then -- unknown logic command (CHECK_MONSTER_CHALLENGE_TIME)
	if pUser:CheckClass(6, 8, 10, 12, -1, -1) then
	pUser:NpcSay(58, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_SKILL_TOTAL'.");
	if false then -- unknown logic command (CHECK_SKILL_TOTAL)
	pUser:NpcSay(58, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_STAT_TOTAL'.");
	if false then -- unknown logic command (CHECK_STAT_TOTAL)
	pUser:NpcSay(58, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	local lvl = pUser:GetLevel();
	if lvl >= 46 and lvl <= 59 then
	pUser:NpcSay(58, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	local lvl = pUser:GetLevel();
	if lvl >= 0 and lvl <= 45 then
	pUser:NpcSay(58, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_MONSTER_CHALLENGE_USERCOUNT'.");
	if false then -- unknown logic command (CHECK_MONSTER_CHALLENGE_USERCOUNT)
	pUser:NpcSay(32, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:GoldLose(300000);
	pUser:SendDebugString("Unknown EXEC command 'ZONE_CHANGE'."); -- unknown execute command (ZONE_CHANGE)
	do return; end
	end
	pUser:NpcSay(55, -1, -1, -1, -1, -1, -1, -1);
	do return; end
elseif nEventID == 53 then
	pUser:NpcSay(25, -1, -1, -1, -1, -1, -1, -1);
	do return; end
elseif nEventID == 62 then
	local coins = pUser:GetCoins();
	if coins >= 0 and coins <= 299999 then
	pUser:NpcSay(62, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
elseif nEventID == 70 then
	pUser:SelectMsg(70, 77, 71, 76, 98, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 71 then
	pUser:SelectMsg(71, 73, 73, 74, 74, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 73 then
	local count = pUser:HowMuchItem(379156000);
	if count >= 1 and count <= 9999 then
	if pUser:CheckExistItem(379156000, 0) then
	local lvl = pUser:GetLevel();
	if lvl >= 1 and lvl <= 99 then
	local rand_num = RollDice(sUID, 20);
	if pUser:CheckExistItem(379156000, 0) then
	if rand_num == 1 then
	pUser:NpcSay(78, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 78, 0);
	end
	end
	if pUser:CheckExistItem(379156000, 0) then
	if rand_num == 2 then
	pUser:NpcSay(78, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 79, 0);
	end
	end
	if pUser:CheckExistItem(379156000, 0) then
	if rand_num == 3 then
	pUser:NpcSay(78, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 80, 0);
	end
	end
	if pUser:CheckExistItem(379156000, 0) then
	if rand_num == 4 then
	pUser:NpcSay(78, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 81, 0);
	end
	end
	if pUser:CheckExistItem(379156000, 0) then
	if rand_num == 5 then
	pUser:NpcSay(78, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 82, 0);
	end
	end
	if pUser:CheckExistItem(379156000, 0) then
	if rand_num == 6 then
	pUser:NpcSay(78, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 83, 0);
	end
	end
	if pUser:CheckExistItem(379156000, 0) then
	if rand_num == 7 then
	pUser:NpcSay(78, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 84, 0);
	end
	end
	if pUser:CheckExistItem(379156000, 0) then
	if rand_num == 8 then
	pUser:NpcSay(78, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 85, 0);
	end
	end
	if pUser:CheckExistItem(379156000, 0) then
	if rand_num == 9 then
	pUser:NpcSay(78, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 86, 0);
	end
	end
	if pUser:CheckExistItem(379156000, 0) then
	if rand_num == 10 then
	pUser:NpcSay(78, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 87, 0);
	end
	end
	if pUser:CheckExistItem(379156000, 0) then
	if rand_num == 11 then
	pUser:NpcSay(78, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 88, 0);
	end
	end
	if pUser:CheckExistItem(379156000, 0) then
	if rand_num == 12 then
	pUser:NpcSay(78, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 89, 0);
	end
	end
	if pUser:CheckExistItem(379156000, 0) then
	if rand_num == 13 then
	pUser:NpcSay(78, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 90, 0);
	end
	end
	if pUser:CheckExistItem(379156000, 0) then
	if rand_num == 14 then
	pUser:NpcSay(78, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 91, 0);
	end
	end
	if pUser:CheckExistItem(379156000, 0) then
	if rand_num == 15 then
	pUser:NpcSay(78, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 92, 0);
	end
	end
	if pUser:CheckExistItem(379156000, 0) then
	if rand_num == 16 then
	pUser:NpcSay(78, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 93, 0);
	end
	end
	if pUser:CheckExistItem(379156000, 0) then
	if rand_num == 17 then
	pUser:NpcSay(78, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 94, 0);
	end
	end
	if pUser:CheckExistItem(379156000, 0) then
	if rand_num == 18 then
	pUser:NpcSay(78, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 95, 0);
	end
	end
	if pUser:CheckExistItem(379156000, 0) then
	if rand_num == 19 then
	pUser:NpcSay(78, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 96, 0);
	end
	end
	if pUser:CheckExistItem(379156000, 0) then
	if rand_num == 20 then
	pUser:NpcSay(78, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 97, 0);
	end
	end
	end
	end
	end
	local count = pUser:HowMuchItem(379156000);
	if count >= 0 and count <= 0 then
	pUser:NpcSay(76, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
elseif nEventID == 74 then
	pUser:NpcSay(74, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 98 then
	pUser:SelectMsg(72, 78, 99, 79, 100, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 99 then
	local count = pUser:HowMuchItem(379155000);
	if count >= 1 and count <= 9999 then
	if pUser:CheckExistItem(379155000, 0) then
	local lvl = pUser:GetLevel();
	if lvl >= 1 and lvl <= 99 then
	local rand_num = RollDice(sUID, 20);
	if pUser:CheckExistItem(379155000, 0) then
	if rand_num == 1 then
	pUser:NpcSay(81, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 104, 0);
	end
	end
	if pUser:CheckExistItem(379155000, 0) then
	if rand_num == 2 then
	pUser:NpcSay(81, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 105, 0);
	end
	end
	if pUser:CheckExistItem(379155000, 0) then
	if rand_num == 3 then
	pUser:NpcSay(81, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 106, 0);
	end
	end
	if pUser:CheckExistItem(379155000, 0) then
	if rand_num == 4 then
	pUser:NpcSay(81, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 107, 0);
	end
	end
	if pUser:CheckExistItem(379155000, 0) then
	if rand_num == 5 then
	pUser:NpcSay(81, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 108, 0);
	end
	end
	if pUser:CheckExistItem(379155000, 0) then
	if rand_num == 6 then
	pUser:NpcSay(81, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 109, 0);
	end
	end
	if pUser:CheckExistItem(379155000, 0) then
	if rand_num == 7 then
	pUser:NpcSay(81, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 110, 0);
	end
	end
	if pUser:CheckExistItem(379155000, 0) then
	if rand_num == 8 then
	pUser:NpcSay(81, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 111, 0);
	end
	end
	if pUser:CheckExistItem(379155000, 0) then
	if rand_num == 9 then
	pUser:NpcSay(81, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 112, 0);
	end
	end
	if pUser:CheckExistItem(379155000, 0) then
	if rand_num == 10 then
	pUser:NpcSay(81, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 113, 0);
	end
	end
	if pUser:CheckExistItem(379155000, 0) then
	if rand_num == 11 then
	pUser:NpcSay(81, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 114, 0);
	end
	end
	if pUser:CheckExistItem(379155000, 0) then
	if rand_num == 12 then
	pUser:NpcSay(81, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 115, 0);
	end
	end
	if pUser:CheckExistItem(379155000, 0) then
	if rand_num == 13 then
	pUser:NpcSay(81, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 116, 0);
	end
	end
	if pUser:CheckExistItem(379155000, 0) then
	if rand_num == 14 then
	pUser:NpcSay(81, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 117, 0);
	end
	end
	if pUser:CheckExistItem(379155000, 0) then
	if rand_num == 15 then
	pUser:NpcSay(81, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 118, 0);
	end
	end
	if pUser:CheckExistItem(379155000, 0) then
	if rand_num == 16 then
	pUser:NpcSay(81, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 119, 0);
	end
	end
	if pUser:CheckExistItem(379155000, 0) then
	if rand_num == 17 then
	pUser:NpcSay(81, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 120, 0);
	end
	end
	if pUser:CheckExistItem(379155000, 0) then
	if rand_num == 18 then
	pUser:NpcSay(81, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 121, 0);
	end
	end
	if pUser:CheckExistItem(379155000, 0) then
	if rand_num == 19 then
	pUser:NpcSay(81, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 122, 0);
	end
	end
	if pUser:CheckExistItem(379155000, 0) then
	if rand_num == 20 then
	pUser:NpcSay(81, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 123, 0);
	end
	end
	end
	end
	end
	local count = pUser:HowMuchItem(379155000);
	if count >= 0 and count <= 0 then
	pUser:NpcSay(80, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
elseif nEventID == 100 then
	pUser:NpcSay(79, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 4001 then
	pUser:SelectMsg(4001, 402, 4004, 401, 4003, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 4003 then
	pUser:NpcSay(4002, -1, -1, -1, -1, -1, -1, -1);
	do return; end
elseif nEventID == 4004 then
	pUser:SendDebugString("Unknown EXEC command 'OPEN_EDITBOX'."); -- unknown execute command (OPEN_EDITBOX)
	do return; end
elseif nEventID == 4005 then
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_PPCARD_SERIAL'.");
	if false then -- unknown logic command (CHECK_PPCARD_SERIAL)
	pUser:NpcSay(4004, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	if pUser:EmptySlotCount() == 0 then
	pUser:NpcSay(4009, -1, -1, -1, -1, -1, -1, -1);
	pUser:SendDebugString("Unknown EXEC command 'RETURN;'."); -- unknown execute command (RETURN;)
	do return; end
	end
	pUser:SendDebugString("Unknown EXEC command 'GIVE_PPCARD_ITEM'."); -- unknown execute command (GIVE_PPCARD_ITEM)
	do return; end
	pUser:NpcSay(4005, -1, -1, -1, -1, -1, -1, -1);
	pUser:SendDebugString("Unknown EXEC command 'RETURN;'."); -- unknown execute command (RETURN;)
	do return; end
elseif nEventID == 6001 then
	pUser:SendDebugString("Unhandled LOGIC command 'CHECK_PROMOTION_ELIGIBLE'."); -- unhandled logic command (CHECK_PROMOTION_ELIGIBLE)
	if false then
	if pUser:CheckClass(105, 205, -1, -1, -1, -1) then
	pUser:SelectMsg(6000, 501, 6030, 502, 6040, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
	if pUser:CheckClass(107, 207, -1, -1, -1, -1) then
	pUser:SelectMsg(7000, 501, 6030, 502, 6040, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
	if pUser:CheckClass(109, 209, -1, -1, -1, -1) then
	pUser:SelectMsg(8000, 501, 6030, 502, 6040, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
	if pUser:CheckClass(111, 211, -1, -1, -1, -1) then
	pUser:SelectMsg(9000, 501, 6030, 502, 6040, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
	end
elseif nEventID == 6030 then
	pUser:SendDebugString("Unknown EXEC command 'GIVE_PROMOTION_QUEST'."); -- unknown execute command (GIVE_PROMOTION_QUEST)
	do return; end
elseif nEventID == 6040 then
	pUser:SendDebugString("Unknown EXEC command 'PROMOTE_USER'."); -- unknown execute command (PROMOTE_USER)
	do return; end
	do return; end
elseif nEventID == 7001 then
	pUser:SelectMsg(7050, 7001, 7005, 7821, 7821, 7002, 7240, 7701, 7701, 7551, 7551, 7621, 7621, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 7005 then
	local state = pUser:SearchQuest(5);
	if state == 2 then
	pUser:NpcSay(10110, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'EXIST_COM_EVENT'.");
	if false then -- unknown logic command (EXIST_COM_EVENT)
	local count = pUser:HowMuchItem(379048000);
	if count >= 0 and count <= 4 then
	pUser:NpcSay(10120, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	local count = pUser:HowMuchItem(379048000);
	if count >= 5 and count <= 9999 then
	local state = pUser:SearchQuest(5);
	if state == 1 then
	if pUser:CheckExistItem(379048000, 0) then
	if pUser:CheckClass(1, -1, -1, -1, -1, -1) then
	pUser:ExpChange(50);
	RunCountExchange(sUID, 1, 0);
	local state = pUser:SearchQuest(5);
	if state == 1 then
	pUser:NpcSay(10150, -1, -1, -1, -1, -1, -1, -1);
	pUser:SaveEvent(5, 2);
	do return; end
	end
	end
	end
	end
	local state = pUser:SearchQuest(5);
	if state == 1 then
	if pUser:CheckExistItem(379048000, 0) then
	if pUser:CheckClass(2, -1, -1, -1, -1, -1) then
	pUser:ExpChange(50);
	RunCountExchange(sUID, 2, 0);
	local state = pUser:SearchQuest(5);
	if state == 1 then
	pUser:NpcSay(10150, -1, -1, -1, -1, -1, -1, -1);
	pUser:SaveEvent(5, 2);
	do return; end
	end
	end
	end
	end
	local state = pUser:SearchQuest(5);
	if state == 1 then
	if pUser:CheckExistItem(379048000, 0) then
	if pUser:CheckClass(3, -1, -1, -1, -1, -1) then
	pUser:ExpChange(50);
	RunCountExchange(sUID, 3, 0);
	local state = pUser:SearchQuest(5);
	if state == 1 then
	pUser:NpcSay(10150, -1, -1, -1, -1, -1, -1, -1);
	pUser:SaveEvent(5, 2);
	do return; end
	end
	end
	end
	end
	local state = pUser:SearchQuest(5);
	if state == 1 then
	if pUser:CheckExistItem(379048000, 0) then
	if pUser:CheckClass(4, -1, -1, -1, -1, -1) then
	pUser:ExpChange(50);
	RunCountExchange(sUID, 4, 0);
	local state = pUser:SearchQuest(5);
	if state == 1 then
	pUser:NpcSay(10150, -1, -1, -1, -1, -1, -1, -1);
	pUser:SaveEvent(5, 2);
	do return; end
	end
	end
	end
	end
	end
	end
	pUser:SendDebugString("Unknown LOGIC command 'NOEXIST_COM_EVENT'.");
	if false then -- unknown logic command (NOEXIST_COM_EVENT)
	pUser:SelectMsg(10130, 601, 7070, 602, 7075, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	pUser:SaveEvent(5, 1);
	pUser:SendDebugString("Unknown EXEC command 'SAVE_COM_EVENT'."); -- unknown execute command (SAVE_COM_EVENT)
	do return; end
	end
elseif nEventID == 7070 then
	pUser:NpcSay(10140, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 7240 then
	pUser:SelectMsg(7105, 7101, 7245, 7102, 7251, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 7245 then
	pUser:NpcSay(7110, 7120, 7130, 7140, 7145, -1, -1, -1);
	do return; end
elseif nEventID == 7251 then
	pUser:SelectMsg(7150, 7201, 7261, 7202, 7262, 7203, 7263, 7204, 7264, 7205, 7265, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 7261 then
	local coins = pUser:GetCoins();
	if coins >= 0 and coins <= 99999 then
	pUser:NpcSay(7170, 7220, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	local count = pUser:HowMuchItem(379049000);
	if count >= 0 and count <= 4 then
	pUser:NpcSay(7160, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	local count = pUser:HowMuchItem(379049000);
	if count >= 1 and count <= 9999 then
	if pUser:CheckClass(1, -1, -1, -1, -1, -1) then
	if pUser:CheckExistItem(379049000, 5) then
	pUser:GoldLose(100000);
	RunCountExchange(sUID, 7110, 0);
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7310, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7320, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7330, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7340, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7350, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	end
	end
	if pUser:CheckClass(2, -1, -1, -1, -1, -1) then
	if pUser:CheckExistItem(379049000, 5) then
	pUser:GoldLose(100000);
	RunCountExchange(sUID, 7120, 0);
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7310, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7320, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7330, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7340, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7350, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	end
	end
	if pUser:CheckClass(3, -1, -1, -1, -1, -1) then
	if pUser:CheckExistItem(379049000, 5) then
	pUser:GoldLose(100000);
	RunCountExchange(sUID, 7130, 0);
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7310, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7320, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7330, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7340, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7350, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	end
	end
	if pUser:CheckClass(4, -1, -1, -1, -1, -1) then
	if pUser:CheckExistItem(379049000, 5) then
	pUser:GoldLose(100000);
	RunCountExchange(sUID, 7140, 0);
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7310, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7320, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7330, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7340, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7350, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	end
	end
	end
elseif nEventID == 7262 then
	local coins = pUser:GetCoins();
	if coins >= 0 and coins <= 79999 then
	pUser:NpcSay(7170, 7230, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	local count = pUser:HowMuchItem(379050000);
	if count >= 0 and count <= 4 then
	pUser:NpcSay(7160, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	local count = pUser:HowMuchItem(379050000);
	if count >= 1 and count <= 9999 then
	if pUser:CheckClass(1, -1, -1, -1, -1, -1) then
	if pUser:CheckExistItem(379050000, 5) then
	pUser:GoldLose(80000);
	RunCountExchange(sUID, 7150, 0);
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7310, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7320, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7330, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7340, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7350, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	end
	end
	if pUser:CheckClass(2, -1, -1, -1, -1, -1) then
	if pUser:CheckExistItem(379050000, 5) then
	pUser:GoldLose(80000);
	RunCountExchange(sUID, 7160, 0);
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7310, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7320, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7330, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7340, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7350, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	end
	end
	if pUser:CheckClass(3, -1, -1, -1, -1, -1) then
	if pUser:CheckExistItem(379050000, 5) then
	pUser:GoldLose(80000);
	RunCountExchange(sUID, 7170, 0);
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7310, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7320, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7330, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7340, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7350, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	end
	end
	if pUser:CheckClass(4, -1, -1, -1, -1, -1) then
	if pUser:CheckExistItem(379050000, 5) then
	pUser:GoldLose(80000);
	RunCountExchange(sUID, 7180, 0);
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7310, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7320, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7330, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7340, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7350, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	end
	end
	end
elseif nEventID == 7263 then
	local coins = pUser:GetCoins();
	if coins >= 0 and coins <= 49999 then
	pUser:NpcSay(7170, 7240, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	local count = pUser:HowMuchItem(379051000);
	if count >= 0 and count <= 4 then
	pUser:NpcSay(7160, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	local count = pUser:HowMuchItem(379051000);
	if count >= 1 and count <= 9999 then
	if pUser:CheckClass(1, -1, -1, -1, -1, -1) then
	if pUser:CheckExistItem(379051000, 5) then
	pUser:GoldLose(50000);
	RunCountExchange(sUID, 7190, 0);
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7310, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7320, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7330, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7340, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7350, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	end
	end
	if pUser:CheckClass(2, -1, -1, -1, -1, -1) then
	if pUser:CheckExistItem(379051000, 5) then
	pUser:GoldLose(50000);
	RunCountExchange(sUID, 7200, 0);
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7310, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7320, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7330, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7340, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7350, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	end
	end
	if pUser:CheckClass(3, -1, -1, -1, -1, -1) then
	if pUser:CheckExistItem(379051000, 5) then
	pUser:GoldLose(50000);
	RunCountExchange(sUID, 7210, 0);
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7310, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7320, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7330, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7340, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7350, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	end
	end
	if pUser:CheckClass(4, -1, -1, -1, -1, -1) then
	if pUser:CheckExistItem(379051000, 5) then
	pUser:GoldLose(50000);
	RunCountExchange(sUID, 7220, 0);
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7310, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7320, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7330, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7340, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7350, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	end
	end
	end
elseif nEventID == 7264 then
	local coins = pUser:GetCoins();
	if coins >= 0 and coins <= 39999 then
	pUser:NpcSay(7170, 7250, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	local count = pUser:HowMuchItem(379052000);
	if count >= 0 and count <= 4 then
	pUser:NpcSay(7160, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	local count = pUser:HowMuchItem(379052000);
	if count >= 1 and count <= 9999 then
	if pUser:CheckClass(1, -1, -1, -1, -1, -1) then
	if pUser:CheckExistItem(379052000, 5) then
	pUser:GoldLose(40000);
	RunCountExchange(sUID, 7230, 0);
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7310, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7320, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7330, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7340, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7350, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	end
	end
	if pUser:CheckClass(2, -1, -1, -1, -1, -1) then
	if pUser:CheckExistItem(379052000, 5) then
	pUser:GoldLose(40000);
	RunCountExchange(sUID, 7240, 0);
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7310, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7320, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7330, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7340, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7350, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	end
	end
	if pUser:CheckClass(3, -1, -1, -1, -1, -1) then
	if pUser:CheckExistItem(379052000, 5) then
	pUser:GoldLose(40000);
	RunCountExchange(sUID, 7250, 0);
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7310, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7320, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7330, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7340, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7350, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	end
	end
	if pUser:CheckClass(4, -1, -1, -1, -1, -1) then
	if pUser:CheckExistItem(379052000, 5) then
	pUser:GoldLose(40000);
	RunCountExchange(sUID, 7260, 0);
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7310, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7320, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7330, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7340, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7350, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	end
	end
	end
elseif nEventID == 7265 then
	local coins = pUser:GetCoins();
	if coins >= 0 and coins <= 29999 then
	pUser:NpcSay(7170, 7260, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	local count = pUser:HowMuchItem(379053000);
	if count >= 0 and count <= 4 then
	pUser:NpcSay(7160, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	local count = pUser:HowMuchItem(379053000);
	if count >= 1 and count <= 9999 then
	if pUser:CheckClass(1, -1, -1, -1, -1, -1) then
	if pUser:CheckExistItem(379053000, 5) then
	pUser:GoldLose(30000);
	RunCountExchange(sUID, 7270, 0);
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7310, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7320, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7330, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7340, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7350, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	end
	end
	if pUser:CheckClass(2, -1, -1, -1, -1, -1) then
	if pUser:CheckExistItem(379053000, 5) then
	pUser:GoldLose(30000);
	RunCountExchange(sUID, 7280, 0);
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7310, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7320, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7330, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7340, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7350, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	end
	end
	if pUser:CheckClass(3, -1, -1, -1, -1, -1) then
	if pUser:CheckExistItem(379053000, 5) then
	pUser:GoldLose(30000);
	RunCountExchange(sUID, 7290, 0);
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7310, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7320, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7330, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7340, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7350, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	end
	end
	if pUser:CheckClass(4, -1, -1, -1, -1, -1) then
	if pUser:CheckExistItem(379053000, 5) then
	pUser:GoldLose(30000);
	RunCountExchange(sUID, 7300, 0);
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7310, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7320, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7330, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7340, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7350, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	end
	end
	end
elseif nEventID == 7551 then
	pUser:SelectMsg(7551, 7553, 7553, 7555, 7558, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 7553 then
	pUser:NpcSay(7553, 7554, 7555, 7556, 7557, -1, -1, -1);
	do return; end
elseif nEventID == 7558 then
	pUser:SelectMsg(7558, 7561, 7561, 7562, 7562, 7563, 7563, 7564, 7564, 7565, 7565, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 7561 then
	local coins = pUser:GetCoins();
	if coins >= 0 and coins <= 499999 then
	pUser:NpcSay(7566, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	local count = pUser:HowMuchItem(379076000);
	if count >= 0 and count <= 14 then
	pUser:NpcSay(7571, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	local count = pUser:HowMuchItem(379071000);
	if count >= 0 and count <= 6 then
	pUser:NpcSay(7576, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	if pUser:CheckClass(1, -1, -1, -1, -1, -1) then
	RunCountExchange(sUID, 7591, 0);
	pUser:GoldLose(500000);
	pUser:NpcSay(7591, -1, -1, -1, -1, -1, -1, -1);
	end
	if pUser:CheckClass(2, -1, -1, -1, -1, -1) then
	RunCountExchange(sUID, 7596, 0);
	pUser:GoldLose(500000);
	pUser:NpcSay(7591, -1, -1, -1, -1, -1, -1, -1);
	end
	if pUser:CheckClass(3, -1, -1, -1, -1, -1) then
	RunCountExchange(sUID, 7601, 0);
	pUser:GoldLose(500000);
	pUser:NpcSay(7591, -1, -1, -1, -1, -1, -1, -1);
	end
	if pUser:CheckClass(4, -1, -1, -1, -1, -1) then
	RunCountExchange(sUID, 7606, 0);
	pUser:GoldLose(500000);
	pUser:NpcSay(7591, -1, -1, -1, -1, -1, -1, -1);
	end
elseif nEventID == 7562 then
	local coins = pUser:GetCoins();
	if coins >= 0 and coins <= 499999 then
	pUser:NpcSay(7566, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	local count = pUser:HowMuchItem(379076000);
	if count >= 0 and count <= 11 then
	pUser:NpcSay(7571, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	local count = pUser:HowMuchItem(379071000);
	if count >= 0 and count <= 4 then
	pUser:NpcSay(7577, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	if pUser:CheckClass(1, -1, -1, -1, -1, -1) then
	RunCountExchange(sUID, 7592, 0);
	pUser:GoldLose(500000);
	pUser:NpcSay(7591, -1, -1, -1, -1, -1, -1, -1);
	end
	if pUser:CheckClass(2, -1, -1, -1, -1, -1) then
	RunCountExchange(sUID, 7597, 0);
	pUser:GoldLose(500000);
	pUser:NpcSay(7591, -1, -1, -1, -1, -1, -1, -1);
	end
	if pUser:CheckClass(3, -1, -1, -1, -1, -1) then
	RunCountExchange(sUID, 7602, 0);
	pUser:GoldLose(500000);
	pUser:NpcSay(7591, -1, -1, -1, -1, -1, -1, -1);
	end
	if pUser:CheckClass(4, -1, -1, -1, -1, -1) then
	RunCountExchange(sUID, 7607, 0);
	pUser:GoldLose(500000);
	pUser:NpcSay(7591, -1, -1, -1, -1, -1, -1, -1);
	end
elseif nEventID == 7563 then
	local coins = pUser:GetCoins();
	if coins >= 0 and coins <= 499999 then
	pUser:NpcSay(7566, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	local count = pUser:HowMuchItem(379076000);
	if count >= 0 and count <= 8 then
	pUser:NpcSay(7571, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	local count = pUser:HowMuchItem(379071000);
	if count >= 0 and count <= 3 then
	pUser:NpcSay(7578, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	if pUser:CheckClass(1, -1, -1, -1, -1, -1) then
	RunCountExchange(sUID, 7593, 0);
	pUser:GoldLose(500000);
	pUser:NpcSay(7591, -1, -1, -1, -1, -1, -1, -1);
	end
	if pUser:CheckClass(2, -1, -1, -1, -1, -1) then
	RunCountExchange(sUID, 7598, 0);
	pUser:GoldLose(500000);
	pUser:NpcSay(7591, -1, -1, -1, -1, -1, -1, -1);
	end
	if pUser:CheckClass(3, -1, -1, -1, -1, -1) then
	RunCountExchange(sUID, 7603, 0);
	pUser:GoldLose(500000);
	pUser:NpcSay(7591, -1, -1, -1, -1, -1, -1, -1);
	end
	if pUser:CheckClass(4, -1, -1, -1, -1, -1) then
	RunCountExchange(sUID, 7608, 0);
	pUser:GoldLose(500000);
	pUser:NpcSay(7591, -1, -1, -1, -1, -1, -1, -1);
	end
elseif nEventID == 7564 then
	local coins = pUser:GetCoins();
	if coins >= 0 and coins <= 499999 then
	pUser:NpcSay(7566, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	local count = pUser:HowMuchItem(379076000);
	if count >= 0 and count <= 5 then
	pUser:NpcSay(7571, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	local count = pUser:HowMuchItem(379071000);
	if count >= 0 and count <= 2 then
	pUser:NpcSay(7579, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	if pUser:CheckClass(1, -1, -1, -1, -1, -1) then
	RunCountExchange(sUID, 7594, 0);
	pUser:GoldLose(500000);
	pUser:NpcSay(7591, -1, -1, -1, -1, -1, -1, -1);
	end
	if pUser:CheckClass(2, -1, -1, -1, -1, -1) then
	RunCountExchange(sUID, 7599, 0);
	pUser:GoldLose(500000);
	pUser:NpcSay(7591, -1, -1, -1, -1, -1, -1, -1);
	end
	if pUser:CheckClass(3, -1, -1, -1, -1, -1) then
	RunCountExchange(sUID, 7604, 0);
	pUser:GoldLose(500000);
	pUser:NpcSay(7591, -1, -1, -1, -1, -1, -1, -1);
	end
	if pUser:CheckClass(4, -1, -1, -1, -1, -1) then
	RunCountExchange(sUID, 7609, 0);
	pUser:GoldLose(500000);
	pUser:NpcSay(7591, -1, -1, -1, -1, -1, -1, -1);
	end
elseif nEventID == 7565 then
	local coins = pUser:GetCoins();
	if coins >= 0 and coins <= 499999 then
	pUser:NpcSay(7566, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	local count = pUser:HowMuchItem(379076000);
	if count >= 0 and count <= 2 then
	pUser:NpcSay(7571, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	local count = pUser:HowMuchItem(379071000);
	if count >= 0 and count <= 2 then
	pUser:NpcSay(7580, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	if pUser:CheckClass(1, -1, -1, -1, -1, -1) then
	RunCountExchange(sUID, 7595, 0);
	pUser:GoldLose(500000);
	pUser:NpcSay(7591, -1, -1, -1, -1, -1, -1, -1);
	end
	if pUser:CheckClass(2, -1, -1, -1, -1, -1) then
	RunCountExchange(sUID, 7600, 0);
	pUser:GoldLose(500000);
	pUser:NpcSay(7591, -1, -1, -1, -1, -1, -1, -1);
	end
	if pUser:CheckClass(3, -1, -1, -1, -1, -1) then
	RunCountExchange(sUID, 7605, 0);
	pUser:GoldLose(500000);
	pUser:NpcSay(7591, -1, -1, -1, -1, -1, -1, -1);
	end
	if pUser:CheckClass(4, -1, -1, -1, -1, -1) then
	RunCountExchange(sUID, 7610, 0);
	pUser:GoldLose(500000);
	pUser:NpcSay(7591, -1, -1, -1, -1, -1, -1, -1);
	end
elseif nEventID == 7621 then
	pUser:SelectMsg(7621, 7623, 7623, 7625, 7625, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 7623 then
	pUser:NpcSay(7623, 7624, 7625, 7626, 7627, -1, -1, -1);
	do return; end
elseif nEventID == 7625 then
	local count = pUser:HowMuchItem(379072000);
	if count >= 0 and count <= 4 then
	pUser:NpcSay(7631, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	local coins = pUser:GetCoins();
	if coins >= 0 and coins <= 499999 then
	pUser:NpcSay(7632, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	local count = pUser:HowMuchItem(389075000);
	if count >= 0 and count <= 0 then
	pUser:NpcSay(7633, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SelectMsg(7635, 7641, 7641, 7651, 7651, 7661, 7661, 7671, 7671, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 7641 then
	local count = pUser:HowMuchItem(203011337);
	if count >= 0 and count <= 0 then
	pUser:NpcSay(7642, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	local rand_num = RollDice(sUID, 2);
	if rand_num == 1 then
	pUser:GoldLose(500000);
	RunCountExchange(sUID, 7645, 0);
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7681, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7682, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7683, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7684, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7685, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	end
	if rand_num == 2 then
	pUser:GoldLose(500000);
	RunCountExchange(sUID, 7646, 0);
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7686, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7687, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7688, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7689, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	end
elseif nEventID == 7651 then
	local count = pUser:HowMuchItem(243011338);
	if count >= 0 and count <= 0 then
	pUser:NpcSay(7652, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	local rand_num = RollDice(sUID, 2);
	if rand_num == 1 then
	pUser:GoldLose(500000);
	RunCountExchange(sUID, 7655, 0);
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7681, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7682, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7683, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7684, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7685, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	end
	if rand_num == 2 then
	pUser:GoldLose(500000);
	RunCountExchange(sUID, 7656, 0);
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7686, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7687, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7688, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7689, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	end
elseif nEventID == 7661 then
	local count = pUser:HowMuchItem(263011339);
	if count >= 0 and count <= 0 then
	pUser:NpcSay(7662, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	local rand_num = RollDice(sUID, 2);
	if rand_num == 1 then
	pUser:GoldLose(500000);
	RunCountExchange(sUID, 7665, 0);
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7681, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7682, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7683, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7684, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7685, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	end
	if rand_num == 2 then
	pUser:GoldLose(500000);
	RunCountExchange(sUID, 7666, 0);
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7686, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7687, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7688, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7689, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	end
elseif nEventID == 7671 then
	local count = pUser:HowMuchItem(283011340);
	if count >= 0 and count <= 0 then
	pUser:NpcSay(7672, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	local rand_num = RollDice(sUID, 2);
	if rand_num == 1 then
	pUser:GoldLose(500000);
	RunCountExchange(sUID, 7675, 0);
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7681, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7682, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7683, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7684, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7685, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	end
	if rand_num == 2 then
	pUser:GoldLose(500000);
	RunCountExchange(sUID, 7676, 0);
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7686, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7687, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7688, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7689, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	end
elseif nEventID == 7701 then
	local lvl = pUser:GetLevel();
	if lvl >= 1 and lvl <= 14 then
	pUser:NpcSay(7701, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	local lvl = pUser:GetLevel();
	if lvl >= 15 and lvl <= 90 then
	pUser:SelectMsg(7702, 7702, 7725, 7703, 7730, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
elseif nEventID == 7725 then
	pUser:NpcSay(7703, 7704, 7705, -1, -1, -1, -1, -1);
	do return; end
elseif nEventID == 7730 then
	local coins = pUser:GetCoins();
	if coins >= 0 and coins <= 49999 then
	pUser:NpcSay(7707, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	local count = pUser:HowMuchItem(379077000);
	if count >= 0 and count <= 4 then
	pUser:NpcSay(7708, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	local count = pUser:HowMuchItem(379076000);
	if count >= 0 and count <= 4 then
	pUser:NpcSay(7706, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	if pUser:CheckClass(1, -1, -1, -1, -1, -1) then
	pUser:GoldLose(50000);
	RunCountExchange(sUID, 21301, 0);
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7751, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7752, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7753, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7754, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7755, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	end
	if pUser:CheckClass(2, -1, -1, -1, -1, -1) then
	pUser:GoldLose(50000);
	RunCountExchange(sUID, 21302, 0);
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7751, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7752, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7753, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7754, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7755, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	end
	if pUser:CheckClass(3, -1, -1, -1, -1, -1) then
	pUser:GoldLose(50000);
	RunCountExchange(sUID, 21303, 0);
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7751, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7752, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7753, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7754, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7755, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	end
	if pUser:CheckClass(4, -1, -1, -1, -1, -1) then
	pUser:GoldLose(50000);
	RunCountExchange(sUID, 21304, 0);
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7751, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7752, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7753, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7754, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_ITEMCHANGE_NUM'.");
	if false then -- unknown logic command (CHECK_ITEMCHANGE_NUM)
	pUser:NpcSay(7755, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	end
elseif nEventID == 7801 then
	pUser:GoldLose(1000);
	RunCountExchange(sUID, 7801, 0);
elseif nEventID == 7802 then
	pUser:GoldLose(1000);
	RunCountExchange(sUID, 7802, 0);
elseif nEventID == 7803 then
	pUser:GoldLose(1000);
	RunCountExchange(sUID, 7803, 0);
elseif nEventID == 7804 then
	pUser:GoldLose(1000);
	RunCountExchange(sUID, 7804, 0);
elseif nEventID == 7805 then
	pUser:GoldLose(1000);
	RunCountExchange(sUID, 7805, 0);
elseif nEventID == 7806 then
	pUser:GoldLose(1005);
	RunCountExchange(sUID, 7806, 0);
elseif nEventID == 7807 then
	pUser:GoldLose(1000);
	RunCountExchange(sUID, 7807, 0);
elseif nEventID == 7808 then
	pUser:GoldLose(1000);
	RunCountExchange(sUID, 7808, 0);
elseif nEventID == 7809 then
	pUser:GoldLose(1000);
	RunCountExchange(sUID, 7809, 0);
elseif nEventID == 7810 then
	pUser:GoldLose(1000);
	RunCountExchange(sUID, 7810, 0);
elseif nEventID == 7811 then
	pUser:GoldLose(1000);
	RunCountExchange(sUID, 7811, 0);
elseif nEventID == 7812 then
	pUser:GoldLose(1000);
	RunCountExchange(sUID, 7812, 0);
elseif nEventID == 7813 then
	pUser:GoldLose(1000);
	RunCountExchange(sUID, 7813, 0);
elseif nEventID == 7814 then
	pUser:GoldLose(1000);
	RunCountExchange(sUID, 7814, 0);
elseif nEventID == 7815 then
	pUser:GoldLose(1000);
	RunCountExchange(sUID, 7815, 0);
elseif nEventID == 7816 then
	pUser:GoldLose(1000);
	RunCountExchange(sUID, 7816, 0);
elseif nEventID == 7817 then
	pUser:GoldLose(1000);
	RunCountExchange(sUID, 7817, 0);
elseif nEventID == 7818 then
	pUser:GoldLose(1000);
	RunCountExchange(sUID, 7818, 0);
elseif nEventID == 7819 then
	pUser:GoldLose(1000);
	RunCountExchange(sUID, 7819, 0);
elseif nEventID == 7820 then
	pUser:GoldLose(1000);
	RunCountExchange(sUID, 7820, 0);
elseif nEventID == 7821 then
	pUser:SelectMsg(7821, 7823, 7823, 7825, 7825, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 7823 then
	pUser:NpcSay(7823, 7824, 7825, 7826, 7827, -1, -1, -1);
	do return; end
elseif nEventID == 7825 then
	local coins = pUser:GetCoins();
	if coins >= 0 and coins <= 999 then
	pUser:NpcSay(7832, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	local count = pUser:HowMuchItem(389065000);
	if count >= 0 and count <= 0 then
	pUser:NpcSay(7833, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	if pUser:CheckClass(1, -1, -1, -1, -1, -1) then
	pUser:SelectMsg(7840, 7841, 7841, 7842, 7842, 7843, 7843, 7844, 7844, 7845, 7845, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
	if pUser:CheckClass(2, -1, -1, -1, -1, -1) then
	pUser:SelectMsg(7850, 7851, 7851, 7852, 7852, 7853, 7853, 7854, 7854, 7855, 7855, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
	if pUser:CheckClass(3, -1, -1, -1, -1, -1) then
	pUser:SelectMsg(7860, 7861, 7861, 7862, 7862, 7863, 7863, 7864, 7864, 7865, 7865, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
	if pUser:CheckClass(4, -1, -1, -1, -1, -1) then
	pUser:SelectMsg(7870, 7871, 7871, 7872, 7872, 7873, 7873, 7874, 7874, 7875, 7875, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
elseif nEventID == 7841 then
	local count = pUser:HowMuchItem(201001001);
	if count >= 0 and count <= 0 then
	pUser:NpcSay(7881, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:GoldLose(1000);
	RunCountExchange(sUID, 7801, 0);
elseif nEventID == 7842 then
	local count = pUser:HowMuchItem(201002001);
	if count >= 0 and count <= 0 then
	pUser:NpcSay(7882, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:GoldLose(1000);
	RunCountExchange(sUID, 7802, 0);
elseif nEventID == 7843 then
	local count = pUser:HowMuchItem(201003001);
	if count >= 0 and count <= 0 then
	pUser:NpcSay(7883, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:GoldLose(1000);
	RunCountExchange(sUID, 7803, 0);
elseif nEventID == 7844 then
	local count = pUser:HowMuchItem(201004001);
	if count >= 0 and count <= 0 then
	pUser:NpcSay(7884, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:GoldLose(1000);
	RunCountExchange(sUID, 7804, 0);
elseif nEventID == 7845 then
	local count = pUser:HowMuchItem(201005001);
	if count >= 0 and count <= 0 then
	pUser:NpcSay(7885, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:GoldLose(1000);
	RunCountExchange(sUID, 7805, 0);
elseif nEventID == 7851 then
	local count = pUser:HowMuchItem(241001001);
	if count >= 0 and count <= 0 then
	pUser:NpcSay(7886, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:GoldLose(1005);
	RunCountExchange(sUID, 7806, 0);
elseif nEventID == 7852 then
	local count = pUser:HowMuchItem(241002001);
	if count >= 0 and count <= 0 then
	pUser:NpcSay(7887, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:GoldLose(1000);
	RunCountExchange(sUID, 7807, 0);
elseif nEventID == 7853 then
	local count = pUser:HowMuchItem(241003001);
	if count >= 0 and count <= 0 then
	pUser:NpcSay(7888, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:GoldLose(1000);
	RunCountExchange(sUID, 7808, 0);
elseif nEventID == 7854 then
	local count = pUser:HowMuchItem(241004001);
	if count >= 0 and count <= 0 then
	pUser:NpcSay(7889, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:GoldLose(1000);
	RunCountExchange(sUID, 7809, 0);
elseif nEventID == 7855 then
	local count = pUser:HowMuchItem(241005001);
	if count >= 0 and count <= 0 then
	pUser:NpcSay(7890, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:GoldLose(1000);
	RunCountExchange(sUID, 7810, 0);
elseif nEventID == 7861 then
	local count = pUser:HowMuchItem(261001001);
	if count >= 0 and count <= 0 then
	pUser:NpcSay(7891, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:GoldLose(1000);
	RunCountExchange(sUID, 7811, 0);
elseif nEventID == 7862 then
	local count = pUser:HowMuchItem(261002001);
	if count >= 0 and count <= 0 then
	pUser:NpcSay(7892, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:GoldLose(1000);
	RunCountExchange(sUID, 7812, 0);
elseif nEventID == 7863 then
	local count = pUser:HowMuchItem(261003001);
	if count >= 0 and count <= 0 then
	pUser:NpcSay(7893, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:GoldLose(1000);
	RunCountExchange(sUID, 7813, 0);
elseif nEventID == 7864 then
	local count = pUser:HowMuchItem(261004001);
	if count >= 0 and count <= 0 then
	pUser:NpcSay(7894, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:GoldLose(1000);
	RunCountExchange(sUID, 7814, 0);
elseif nEventID == 7865 then
	local count = pUser:HowMuchItem(261005001);
	if count >= 0 and count <= 0 then
	pUser:NpcSay(7895, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:GoldLose(1000);
	RunCountExchange(sUID, 7815, 0);
elseif nEventID == 7871 then
	local count = pUser:HowMuchItem(281001001);
	if count >= 0 and count <= 0 then
	pUser:NpcSay(7896, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:GoldLose(1000);
	RunCountExchange(sUID, 7816, 0);
elseif nEventID == 7872 then
	local count = pUser:HowMuchItem(281002001);
	if count >= 0 and count <= 0 then
	pUser:NpcSay(7897, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:GoldLose(1000);
	RunCountExchange(sUID, 7817, 0);
elseif nEventID == 7873 then
	local count = pUser:HowMuchItem(281003001);
	if count >= 0 and count <= 0 then
	pUser:NpcSay(7898, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:GoldLose(1000);
	RunCountExchange(sUID, 7818, 0);
elseif nEventID == 7874 then
	local count = pUser:HowMuchItem(281004001);
	if count >= 0 and count <= 0 then
	pUser:NpcSay(7899, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:GoldLose(1000);
	RunCountExchange(sUID, 7819, 0);
elseif nEventID == 7875 then
	pUser:GoldLose(1000);
	RunCountExchange(sUID, 7820, 0);
elseif nEventID == 7900 then
	local count = pUser:HowMuchItem(281005001);
	if count >= 0 and count <= 0 then
	pUser:NpcSay(7900, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
elseif nEventID == 8001 then
	do return; end
	do return; end
elseif nEventID == 8020 then
	do return; end
elseif nEventID == 9001 then
	local state = pUser:SearchQuest(6);
	if state == 1 then
	local lvl = pUser:GetLevel();
	if lvl >= 30 and lvl <= 99 then
	pUser:NpcSay(10240, -1, -1, -1, -1, -1, -1, -1);
	pUser:GiveItem(910013000, 1);
	pUser:SaveEvent(6, 2);
	do return; end
	end
	end
	local state = pUser:SearchQuest(6);
	if state == 1 then
	local lvl = pUser:GetLevel();
	if lvl >= 1 and lvl <= 29 then
	pUser:NpcSay(10230, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	end
	local lvl = pUser:GetLevel();
	if lvl >= 11 and lvl <= 99 then
	pUser:NpcSay(10210, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	local lvl = pUser:GetLevel();
	if lvl >= 1 and lvl <= 11 then
	pUser:SelectMsg(10211, 701, 9050, 702, 9060, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
elseif nEventID == 9050 then
	local state = pUser:SearchQuest(6);
	if state == 0 then
	pUser:NpcSay(10220, -1, -1, -1, -1, -1, -1, -1);
	pUser:SaveEvent(6, 1);
	do return; end
	end
elseif nEventID == 9060 then
	do return; end
elseif nEventID == 9105 then
	local count = pUser:HowMuchItem(910015000);
	if count >= 0 and count <= 0 then
	pUser:NpcSay(10241, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	local count = pUser:HowMuchItem(910015000);
	if count >= 1 and count <= 10 then
	RunCountExchange(sUID, 6, 0);
	pUser:NpcSay(10242, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
elseif nEventID == 15701 then
	local state = pUser:SearchQuest(55);
	if state == 2 then
	pUser:NpcSay(15058, -1, -1, -1, -1, -1, -1, -1);
	end
	local count = pUser:HowMuchItem(379156000);
	if count >= 1 and count <= 99 then
	pUser:RobItem(379156000, 1);
	pUser:GiveItem(110110000, 1);
	pUser:NpcSay(15059, -1, -1, -1, -1, -1, -1, -1);
	end
	pUser:NpcSay(35724, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 20501 then
	local state = pUser:SearchQuest(13);
	if state == 1 then
	pUser:SelectMsg(20907, 20905, 20552, 20906, 20553, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
	local lvl = pUser:GetLevel();
	if lvl >= 1 and lvl <= 19 then
	pUser:NpcSay(20507, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	local state = pUser:SearchQuest(13);
	if state == 2 then
	local state = pUser:SearchQuest(16);
	if state == 1 then
	local count = pUser:HowMuchItem(910019000);
	if count >= 1 and count <= 9999 then
	pUser:SelectMsg(20514, 20508, 20550, 20509, 20546, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
	local count = pUser:HowMuchItem(910019000);
	if count >= 0 and count <= 0 then
	pUser:NpcSay(20512, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	end
	local state = pUser:SearchQuest(16);
	if state == 2 then
	pUser:NpcSay(20516, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	local state = pUser:SearchQuest(16);
	if state == 0 then
	pUser:SelectMsg(20501, 20501, 20540, 20503, 20545, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
	end
	local state = pUser:SearchQuest(13);
	if state == 0 then
	local state = pUser:SearchQuest(16);
	if state == 1 then
	local count = pUser:HowMuchItem(910019000);
	if count >= 1 and count <= 9999 then
	pUser:SelectMsg(20514, 20508, 20550, 20509, 20546, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
	local count = pUser:HowMuchItem(910019000);
	if count >= 0 and count <= 0 then
	pUser:NpcSay(20512, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	end
	local state = pUser:SearchQuest(16);
	if state == 2 then
	pUser:NpcSay(20516, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	local state = pUser:SearchQuest(16);
	if state == 0 then
	pUser:SelectMsg(20501, 20501, 20540, 20503, 20545, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
	end
elseif nEventID == 20540 then
	pUser:SelectMsg(20503, 20504, 20542, 20505, 20541, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 20541 then
	pUser:NpcSay(20504, -1, -1, -1, -1, -1, -1, -1);
	do return; end
elseif nEventID == 20542 then
	local state = pUser:SearchQuest(16);
	if state == 0 then
	pUser:NpcSay(20505, -1, -1, -1, -1, -1, -1, -1);
	pUser:SaveEvent(16, 1);
	do return; end
	end
elseif nEventID == 20545 then
	pUser:NpcSay(20502, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 20546 then
	pUser:NpcSay(20515, -1, -1, -1, -1, -1, -1, -1);
	do return; end
elseif nEventID == 20550 then
	if pUser:EmptySlotCount() == 0 then
	pUser:NpcSay(35653, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	if pUser:EmptySlotCount() > 0 then
	pUser:NpcSay(35653, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	local state = pUser:SearchQuest(16);
	if state == 1 then
	pUser:NpcSay(20516, -1, -1, -1, -1, -1, -1, -1);
	pUser:RobItem(910019000, 1);
	pUser:GiveItem(389013000, 10);
	pUser:GiveItem(389018000, 10);
	pUser:ExpChange(3000);
	pUser:SaveEvent(16, 2);
	do return; end
	end
elseif nEventID == 20552 then
	local count = pUser:HowMuchItem(910018000);
	if count >= 1 and count <= 9999 then
	pUser:NpcSay(20915, -1, -1, -1, -1, -1, -1, -1);
	end
	local count = pUser:HowMuchItem(910018000);
	if count >= 0 and count <= 0 then
	pUser:NpcSay(20909, -1, -1, -1, -1, -1, -1, -1);
	pUser:GiveItem(910018000, 1);
	end
elseif nEventID == 20553 then
	pUser:NpcSay(20516, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 20601 then
	local state = pUser:SearchQuest(15);
	if state == 1 then
	local count = pUser:HowMuchItem(910020000);
	if count >= 1 and count <= 9999 then
	local count = pUser:HowMuchItem(910017000);
	if count >= 1 and count <= 9999 then
	local count = pUser:HowMuchItem(910021000);
	if count >= 1 and count <= 9999 then
	pUser:SelectMsg(20607, 20603, 20650, 20604, 20646, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
	end
	end
	local count = pUser:HowMuchItem(910020000);
	if count >= 0 and count <= 0 then
	pUser:NpcSay(20606, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	local count = pUser:HowMuchItem(910017000);
	if count >= 0 and count <= 0 then
	pUser:NpcSay(20606, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	local count = pUser:HowMuchItem(910021000);
	if count >= 0 and count <= 0 then
	pUser:NpcSay(20606, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	end
	local state = pUser:SearchQuest(15);
	if state == 2 then
	pUser:NpcSay(20604, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	local state = pUser:SearchQuest(15);
	if state == 0 then
	pUser:SelectMsg(20601, 20601, 20640, 20602, 20645, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
elseif nEventID == 20640 then
	local state = pUser:SearchQuest(15);
	if state == 0 then
	pUser:NpcSay(20605, -1, -1, -1, -1, -1, -1, -1);
	pUser:SaveEvent(15, 1);
	do return; end
	end
elseif nEventID == 20645 then
	pUser:NpcSay(20603, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 20646 then
	pUser:NpcSay(20608, -1, -1, -1, -1, -1, -1, -1);
	do return; end
elseif nEventID == 20650 then
	local state = pUser:SearchQuest(15);
	if state == 1 then
	if pUser:CheckExistItem(910020000, 1) then
	if pUser:CheckExistItem(910017000, 1) then
	if pUser:CheckExistItem(910021000, 1) then
	pUser:NpcSay(20609, -1, -1, -1, -1, -1, -1, -1);
	pUser:RobItem(910020000, 1);
	pUser:RobItem(910017000, 1);
	pUser:RobItem(910021000, 1);
	pUser:GiveItem(379016000, 1);
	pUser:ExpChange(3000);
	pUser:GoldGain(100000);
	pUser:SaveEvent(15, 2);
	end
	end
	end
	end
elseif nEventID == 20701 then
	local lvl = pUser:GetLevel();
	if lvl >= 1 and lvl <= 19 then
	pUser:NpcSay(20709, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	local state = pUser:SearchQuest(14);
	if state == 1 then
	local count = pUser:HowMuchItem(910017000);
	if count >= 3 and count <= 9999 then
	pUser:SelectMsg(20711, 20705, 20750, 20706, 20746, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
	local count = pUser:HowMuchItem(910017000);
	if count >= 0 and count <= 2 then
	pUser:NpcSay(20710, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	end
	local state = pUser:SearchQuest(14);
	if state == 2 then
	pUser:NpcSay(20714, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	local state = pUser:SearchQuest(14);
	if state == 0 then
	pUser:NpcSay(20701, 20702, -1, -1, -1, -1, -1, -1);
	do return; end
	end
elseif nEventID == 20714 then
	pUser:SelectMsg(20703, 20701, 20715, 20702, 20716, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 20715 then
	pUser:SelectMsg(20705, 20703, 20740, 20704, 20745, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 20716 then
	pUser:NpcSay(20704, -1, -1, -1, -1, -1, -1, -1);
	do return; end
elseif nEventID == 20740 then
	local state = pUser:SearchQuest(14);
	if state == 0 then
	pUser:NpcSay(20708, -1, -1, -1, -1, -1, -1, -1);
	pUser:SaveEvent(14, 1);
	do return; end
	end
elseif nEventID == 20745 then
	pUser:NpcSay(20704, -1, -1, -1, -1, -1, -1, -1);
	do return; end
elseif nEventID == 20746 then
	pUser:NpcSay(20712, -1, -1, -1, -1, -1, -1, -1);
	do return; end
elseif nEventID == 20750 then
	local state = pUser:SearchQuest(14);
	if state == 1 then
	if pUser:CheckExistItem(910017000, 3) then
	pUser:NpcSay(20713, -1, -1, -1, -1, -1, -1, -1);
	pUser:RobItem(910017000, 3);
	pUser:ExpChange(5000);
	pUser:GiveItem(389013000, 10);
	pUser:GiveItem(389018000, 10);
	pUser:SaveEvent(14, 2);
	do return; end
	end
	end
elseif nEventID == 20801 then
	local lvl = pUser:GetLevel();
	if lvl >= 1 and lvl <= 19 then
	pUser:NpcSay(20809, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	local state = pUser:SearchQuest(12);
	if state == 1 then
	local count = pUser:HowMuchItem(910020000);
	if count >= 5 and count <= 9999 then
	pUser:SelectMsg(20804, 20803, 20850, 20804, 20846, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
	local count = pUser:HowMuchItem(910020000);
	if count >= 0 and count <= 4 then
	pUser:NpcSay(20803, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	end
	local state = pUser:SearchQuest(12);
	if state == 2 then
	pUser:SelectMsg(20801, 20801, 20840, 20802, 20845, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
	local state = pUser:SearchQuest(12);
	if state == 0 then
	pUser:SelectMsg(20807, 20805, 20840, 20806, 20845, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
elseif nEventID == 20840 then
	local state = pUser:SearchQuest(12);
	if state == 0 then
	pUser:NpcSay(20805, -1, -1, -1, -1, -1, -1, -1);
	pUser:SaveEvent(12, 1);
	do return; end
	end
elseif nEventID == 20845 then
	pUser:NpcSay(20806, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 20846 then
	pUser:NpcSay(20808, -1, -1, -1, -1, -1, -1, -1);
	do return; end
elseif nEventID == 20850 then
	local state = pUser:SearchQuest(12);
	if state == 1 then
	if pUser:CheckExistItem(910020000, 5) then
	pUser:NpcSay(20802, -1, -1, -1, -1, -1, -1, -1);
	pUser:RobItem(910020000, 5);
	pUser:ExpChange(1500);
	pUser:GoldLose(10000);
	pUser:SaveEvent(12, 2);
	do return; end
	end
	end
elseif nEventID == 20901 then
	local lvl = pUser:GetLevel();
	if lvl >= 1 and lvl <= 19 then
	pUser:NpcSay(20906, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	local state = pUser:SearchQuest(16);
	if state == 2 then
	local state = pUser:SearchQuest(13);
	if state == 1 then
	local count = pUser:HowMuchItem(910018000);
	if count >= 1 and count <= 9999 then
	pUser:SelectMsg(20910, 20907, 20950, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
	local count = pUser:HowMuchItem(910018000);
	if count >= 0 and count <= 0 then
	pUser:NpcSay(20911, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	end
	local state = pUser:SearchQuest(13);
	if state == 2 then
	pUser:NpcSay(20914, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	local state = pUser:SearchQuest(13);
	if state == 0 then
	pUser:SelectMsg(20901, 20903, 20940, 20904, 20945, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
	end
	local state = pUser:SearchQuest(16);
	if state == 1 then
	pUser:SelectMsg(20508, 20506, 20952, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
	local state = pUser:SearchQuest(16);
	if state == 0 then
	local state = pUser:SearchQuest(13);
	if state == 1 then
	local count = pUser:HowMuchItem(910018000);
	if count >= 1 and count <= 9999 then
	pUser:SelectMsg(20910, 20907, 20950, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
	local count = pUser:HowMuchItem(910018000);
	if count >= 0 and count <= 0 then
	pUser:NpcSay(20911, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	end
	local state = pUser:SearchQuest(13);
	if state == 2 then
	pUser:NpcSay(20914, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	local state = pUser:SearchQuest(13);
	if state == 0 then
	pUser:SelectMsg(20901, 20903, 20940, 20904, 20945, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
	end
elseif nEventID == 20940 then
	pUser:NpcSay(20904, -1, -1, -1, -1, -1, -1, -1);
	pUser:SaveEvent(13, 1);
	do return; end
elseif nEventID == 20945 then
	pUser:NpcSay(20908, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 20950 then
	if pUser:CheckExistItem(910018000, 0) then
	pUser:RobItem(910018000, 1);
	pUser:GoldGain(100000);
	pUser:GiveItem(389076000, 2);
	pUser:SaveEvent(13, 2);
	pUser:NpcSay(20912, -1, -1, -1, -1, -1, -1, -1);
	end
elseif nEventID == 20952 then
	local count = pUser:HowMuchItem(910019000);
	if count >= 1 and count <= 9999 then
	pUser:NpcSay(20517, -1, -1, -1, -1, -1, -1, -1);
	end
	local count = pUser:HowMuchItem(910019000);
	if count >= 0 and count <= 0 then
	pUser:NpcSay(20511, 20513, -1, -1, -1, -1, -1, -1);
	pUser:GiveItem(910019000, 1);
	end
elseif nEventID == 21001 then
	pUser:SelectMsg(21001, 21001, 21101, 21126, 21126, 21002, 21951, 21004, 21151, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 21101 then
	local lvl = pUser:GetLevel();
	if lvl >= 1 and lvl <= 9 then
	pUser:NpcSay(21110, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	if pUser:CheckClass(105, 106, 205, 206, -1, -1) then
	pUser:NpcSay(21115, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	if pUser:CheckClass(107, 108, 207, 208, -1, -1) then
	pUser:NpcSay(21115, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	if pUser:CheckClass(109, 110, 209, 210, -1, -1) then
	pUser:NpcSay(21115, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	if pUser:CheckClass(111, 112, 211, 212, -1, -1) then
	pUser:NpcSay(21115, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SelectMsg(21120, 21122, 21122, 21121, 21121, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 21121 then
	pUser:NpcSay(21121, -1, -1, -1, -1, -1, -1, -1);
	do return; end
elseif nEventID == 21122 then
	local coins = pUser:GetCoins();
	if coins >= 0 and coins <= 2999 then
	pUser:NpcSay(21123, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:GoldLose(3000);
	pUser:PromoteUserNovice();
	pUser:NpcSay(21146, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 21126 then
	local lvl = pUser:GetLevel();
	if lvl >= 1 and lvl <= 9 then
	pUser:NpcSay(21127, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	local state = pUser:SearchQuest(11);
	if state == 2 then
	pUser:NpcSay(21133, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SelectMsg(21128, 21130, 21130, 21135, 21135, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 21129 then
	pUser:NpcSay(21125, 21126, -1, -1, -1, -1, -1, -1);
elseif nEventID == 21130 then	
	if not pUser:CheckExistEvent(11,2) then
	local lvl = pUser:GetLevel();
	if lvl >= 10 and lvl <= 99 then
	 pUser:ZoneChange(51, 150, 150);
	do return; end
	end
	end
elseif nEventID == 21135 then
	local count = pUser:HowMuchItem(910038000);
	if count >= 0 and count <= 4 then
	pUser:NpcSay(21140, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	local state = pUser:SearchQuest(11);
	if state == 0 then
	if pUser:CheckExistItem(910038000, 5) then
	pUser:RobItem(910038000, 5);
	pUser:SaveEvent(11, 2);
	pUser:ExpChange(5000);
	pUser:GoldGain(30000);
	pUser:NpcSay(21145, -1, -1, -1, -1, -1, -1, -1);
	end
	end
elseif nEventID == 21151 then
	pUser:SelectMsg(21151, 21181, 21181, 21151, 21201, 21251, 21251, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 21181 then
	pUser:SelectMsg(21181, 21183, 21183, 21185, 21185, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 21183 then
	pUser:NpcSay(21183, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 21185 then
	local count = pUser:HowMuchItem(379078000);
	if count >= 0 and count <= 4 then
	pUser:NpcSay(21187, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	if pUser:CheckExistItem(379078000, 5) then
	pUser:RobItem(379078000, 5);
	pUser:ExpChange(200);
	pUser:NpcSay(21189, -1, -1, -1, -1, -1, -1, -1);
	end
elseif nEventID == 21201 then
	local lvl = pUser:GetLevel();
	if lvl >= 1 and lvl <= 19 then
	pUser:NpcSay(21213, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	local state = pUser:SearchQuest(7);
	if state == 2 then
	pUser:NpcSay(21218, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SelectMsg(21220, 21225, 21225, 21230, 21230, 21235, 21235, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 21215 then
	local lvl = pUser:GetLevel();
	if lvl >= 24 and lvl <= 250 then
	pUser:NpcSay(21215, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
elseif nEventID == 21225 then
	pUser:NpcSay(21225, 21226, -1, -1, -1, -1, -1, -1);
elseif nEventID == 21230 then
	pUser:SendDebugString("Unknown EXEC command 'ZONE_CHANGE'."); -- unknown execute command (ZONE_CHANGE)
	do return; end
elseif nEventID == 21235 then
	local lvl = pUser:GetLevel();
	if lvl >= 1 and lvl <= 19 then
	pUser:NpcSay(21213, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	local count = pUser:HowMuchItem(910039000);
	if count >= 0 and count <= 9 then
	pUser:NpcSay(21240, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	local state = pUser:SearchQuest(7);
	if state == 0 then
	if pUser:CheckExistItem(910039000, 10) then
	pUser:RobItem(910039000, 10);
	pUser:ExpChange(52000);
	pUser:SaveEvent(7, 2);
	pUser:NpcSay(21246, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	end
elseif nEventID == 21237 then
	local lvl = pUser:GetLevel();
	if lvl >= 24 and lvl <= 250 then
	pUser:NpcSay(21215, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
elseif nEventID == 21251 then
	local lvl = pUser:GetLevel();
	if lvl >= 1 and lvl <= 39 then
	pUser:NpcSay(21263, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	local lvl = pUser:GetLevel();
	if lvl >= 41 and lvl <= 250 then
	pUser:NpcSay(21265, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SelectMsg(21270, 21275, 21275, 21280, 21280, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 21275 then
	pUser:NpcSay(21275, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 21280 then
	pUser:SendDebugString("Unknown EXEC command 'ZONE_CHANGE'."); -- unknown execute command (ZONE_CHANGE)
	do return; end
elseif nEventID == 21281 then
	pUser:SelectMsg(21281, 21283, 21283, 21285, 21285, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 21283 then
	pUser:NpcSay(21283, 21284, -1, -1, -1, -1, -1, -1);
elseif nEventID == 21285 then
	local lvl = pUser:GetLevel();
	if lvl >= 1 and lvl <= 49 then
	pUser:NpcSay(21287, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	local state = pUser:SearchQuest(9);
	if state == 2 then
	pUser:NpcSay(21289, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
elseif nEventID == 21288 then
	local lvl = pUser:GetLevel();
	if lvl >= 54 and lvl <= 250 then
	pUser:NpcSay(21288, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
elseif nEventID == 21951 then
	pUser:SendDebugString("Unknown EXEC command 'STAT_POINT_DISTRIBUTE'."); -- unknown execute command (STAT_POINT_DISTRIBUTE)
	do return; end
elseif nEventID == 21981 then
	pUser:SendDebugString("Unknown EXEC command 'SKILL_POINT_DISTRIBUTE'."); -- unknown execute command (SKILL_POINT_DISTRIBUTE)
	do return; end
elseif nEventID == 22000 then
	pUser:SelectMsg(22003, 22008, 22002, 22003, 22003, 22004, 22004, 22005, 22005, 30134, 35540, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 22002 then
	if pUser:EmptySlotCount() == 0 then
	pUser:NpcSay(35646, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	local count = pUser:HowMuchItem(379180000);
	if count >= 0 and count <= 4 then
	pUser:NpcSay(22012, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	local count = pUser:HowMuchItem(379181000);
	if count >= 0 and count <= 4 then
	pUser:NpcSay(22013, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	local count = pUser:HowMuchItem(379182000);
	if count >= 0 and count <= 4 then
	pUser:NpcSay(22014, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	local count = pUser:HowMuchItem(379183000);
	if count >= 0 and count <= 4 then
	pUser:NpcSay(22015, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	local count = pUser:HowMuchItem(379184000);
	if count >= 0 and count <= 4 then
	pUser:NpcSay(22016, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	do return; end
elseif nEventID == 22003 then
	if pUser:EmptySlotCount() == 0 then
	pUser:NpcSay(35646, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	local count = pUser:HowMuchItem(379180000);
	if count >= 0 and count <= 4 then
	pUser:NpcSay(22012, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	local count = pUser:HowMuchItem(379181000);
	if count >= 0 and count <= 4 then
	pUser:NpcSay(22013, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	local count = pUser:HowMuchItem(379185000);
	if count >= 0 and count <= 4 then
	pUser:NpcSay(22024, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	local count = pUser:HowMuchItem(379186000);
	if count >= 0 and count <= 4 then
	pUser:NpcSay(22025, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	local count = pUser:HowMuchItem(379187000);
	if count >= 0 and count <= 4 then
	pUser:NpcSay(22026, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	do return; end
elseif nEventID == 22004 then
	if pUser:EmptySlotCount() == 0 then
	pUser:NpcSay(35646, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	local count = pUser:HowMuchItem(379180000);
	if count >= 0 and count <= 4 then
	pUser:NpcSay(22012, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	local count = pUser:HowMuchItem(379181000);
	if count >= 0 and count <= 4 then
	pUser:NpcSay(22013, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	local count = pUser:HowMuchItem(379188000);
	if count >= 0 and count <= 4 then
	pUser:NpcSay(22034, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	local count = pUser:HowMuchItem(379189000);
	if count >= 0 and count <= 4 then
	pUser:NpcSay(22035, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	local count = pUser:HowMuchItem(379190000);
	if count >= 0 and count <= 4 then
	pUser:NpcSay(22036, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	do return; end
elseif nEventID == 22005 then
	if pUser:EmptySlotCount() == 0 then
	pUser:NpcSay(35646, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	local count = pUser:HowMuchItem(379180000);
	if count >= 0 and count <= 4 then
	pUser:NpcSay(22012, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	local count = pUser:HowMuchItem(379181000);
	if count >= 0 and count <= 4 then
	pUser:NpcSay(22013, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	local count = pUser:HowMuchItem(379191000);
	if count >= 0 and count <= 4 then
	pUser:NpcSay(22044, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	local count = pUser:HowMuchItem(379192000);
	if count >= 0 and count <= 4 then
	pUser:NpcSay(22045, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	local count = pUser:HowMuchItem(379193000);
	if count >= 0 and count <= 4 then
	pUser:NpcSay(22046, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	do return; end
elseif nEventID == 22050 then
	pUser:SelectMsg(22050, 14022, 22051, 35562, 35562, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 22051 then
	local state = pUser:SearchQuest(89);
	if state == 2 then
	pUser:NpcSay(35469, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	if pUser:EmptySlotCount() == 0 then
	pUser:NpcSay(35646, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	local count = pUser:HowMuchItem(379133000);
	if count >= 5 and count <= 9999 then
	do return; end
	end
	local count = pUser:HowMuchItem(379133000);
	if count >= 0 and count <= 4 then
	pUser:NpcSay(22056, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
elseif nEventID == 30001 then
	pUser:SelectMsg(30001, 30051, 35131, 30101, 30101, 30131, 30131, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 30060 then
	local count = pUser:HowMuchItem(800032000);
	if count >= 0 and count <= 0 then
	pUser:NpcSay(30060, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
elseif nEventID == 30065 then
	local count = pUser:HowMuchItem(800032000);
	if count >= 1 and count <= 9999 then
	pUser:SelectMsg(30065, 30070, 30070, 30075, 30075, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
elseif nEventID == 30070 then
	if pUser:CheckExistItem(800032000, 0) then
	pUser:SendDebugString("Unknown EXEC command 'CHANGE_NAME'."); -- unknown execute command (CHANGE_NAME)
	do return; end
	end
elseif nEventID == 30075 then
	pUser:NpcSay(30075, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 30101 then
	local count = pUser:HowMuchItem(700001000);
	if count >= 0 and count <= 0 then
	pUser:NpcSay(30102, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	local count = pUser:HowMuchItem(700001000);
	if count >= 1 and count <= 9999 then
	pUser:SelectMsg(30110, 30120, 30120, 30125, 30125, 30130, 30130, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
elseif nEventID == 30120 then
	if pUser:CheckExistItem(700001000, 1) then
	pUser:SendDebugString("Unknown EXEC command 'SKILL_POINT_FREE'."); -- unknown execute command (SKILL_POINT_FREE)
	do return; end
	pUser:RobItem(700001000, 1);
	pUser:NpcSay(30121, -1, -1, -1, -1, -1, -1, -1);
	end
elseif nEventID == 30125 then
	if pUser:CheckExistItem(700001000, 1) then
	pUser:SendDebugString("Unknown EXEC command 'STAT_POINT_FREE'."); -- unknown execute command (STAT_POINT_FREE)
	do return; end
	pUser:RobItem(700001000, 1);
	pUser:NpcSay(30122, -1, -1, -1, -1, -1, -1, -1);
	end
elseif nEventID == 30130 then
	pUser:NpcSay(30075, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 30131 then
	pUser:SelectMsg(30131, 30132, 30132, 30134, 30134, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 30132 then
	local lvl = pUser:GetLevel();
	if lvl >= 21 and lvl <= 99 then
	pUser:NpcSay(30135, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	local lvl = pUser:GetLevel();
	if lvl >= 1 and lvl <= 20 then
	local state = pUser:SearchQuest(50);
	if state == 1 then
	pUser:NpcSay(30137, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	local state = pUser:SearchQuest(50);
	if state == 0 then
	pUser:SaveEvent(50, 1);
	pUser:StateChange(8, 1);
	pUser:NpcSay(30138, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	end
elseif nEventID == 30133 then
	local lvl = pUser:GetLevel();
	if lvl >= 1 and lvl <= 40 then
	pUser:NpcSay(30139, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	local lvl = pUser:GetLevel();
	if lvl >= 41 and lvl <= 99 then
	local state = pUser:SearchQuest(50);
	if state == 2 then
	pUser:NpcSay(30141, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	local state = pUser:SearchQuest(30);
	if state == 1 then
	pUser:GoldLose(500000);
	pUser:SaveEvent(30, 2);
	pUser:NpcSay(30141, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	end
elseif nEventID == 30134 then
	pUser:NpcSay(30134, -1, -1, -1, -1, -1, -1, -1);
	do return; end
elseif nEventID == 31001 then
	pUser:SelectMsg(31001, 31021, 31021, 31031, 31031, 31041, 31041, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 31011 then
	pUser:NpcSay(31011, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 31021 then
	pUser:SendDebugString("Unknown EXEC command 'SEND_WEBPAGE_ADDRESS'."); -- unknown execute command (SEND_WEBPAGE_ADDRESS)
	do return; end
elseif nEventID == 31031 then
	pUser:SendDebugString("Unknown EXEC command 'SEND_WEBPAGE_ADDRESS'."); -- unknown execute command (SEND_WEBPAGE_ADDRESS)
	do return; end
elseif nEventID == 31041 then
	pUser:SendDebugString("Unknown EXEC command 'SEND_WEBPAGE_ADDRESS'."); -- unknown execute command (SEND_WEBPAGE_ADDRESS)
	do return; end
elseif nEventID == 31101 then
	pUser:SelectMsg(31101, 31110, 31110, 31173, 31173, 31174, 31174, 31176, 31176, 31175, 31178, 31172, 31182, 31177, 31183, 31180, 31187, 31181, 31188, 31179, 31189);
elseif nEventID == 31110 then
	pUser:NpcSay(31110, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 31171 then
	pUser:SelectMsg(31101, 31172, 31172, 31174, 31175, 31176, 31177, 31173, 31179, 31175, 31180, 31110, 31181, 31177, 31185, 31178, 31184, 31179, 31186, -1, -1);
elseif nEventID == 31172 then
	pUser:NpcSay(31172, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 31173 then
	pUser:NpcSay(31173, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 31174 then
	pUser:NpcSay(31174, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 31175 then
	pUser:NpcSay(31175, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 31176 then
	pUser:NpcSay(31176, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 31177 then
	pUser:NpcSay(31177, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 31178 then
	pUser:NpcSay(31178, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 31179 then
	pUser:NpcSay(31179, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 31180 then
	pUser:NpcSay(31180, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 31182 then
	pUser:NpcSay(31182, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 31183 then
	pUser:NpcSay(31183, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 31184 then
	pUser:NpcSay(31184, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 31185 then
	pUser:NpcSay(31185, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 31186 then
	pUser:NpcSay(31186, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 31187 then
	pUser:NpcSay(31187, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 31188 then
	pUser:NpcSay(31188, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 31189 then
	pUser:NpcSay(31189, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 35001 then
	pUser:SelectMsg(35001, 2901, 35012, 2902, 35032, 2903, 35052, 2904, 35072, 2905, 35092, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	do return; end
elseif nEventID == 35012 then
	local lvl = pUser:GetLevel();
	if lvl >= 1 and lvl <= 10 then
	pUser:NpcSay(35002, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	local state = pUser:SearchQuest(30);
	if state == 1 then
	local state = pUser:SearchQuest(30);
	if state == 1 then
	local count = pUser:HowMuchItem(379048000);
	if count >= 10 and count <= 9999 then
	pUser:SelectMsg(35007, 2914, 35030, 2915, 35029, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
	local count = pUser:HowMuchItem(379048000);
	if count >= 0 and count <= 9 then
	pUser:NpcSay(35006, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	end
	end
	local state = pUser:SearchQuest(30);
	if state == 0 then
	local state = pUser:SearchQuest(30);
	if state == 0 then
	pUser:SelectMsg(35004, 2911, 35024, 2912, 35029, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
	end
	local state = pUser:SearchQuest(30);
	if state == 2 then
	local state = pUser:SearchQuest(30);
	if state == 2 then
	pUser:SelectMsg(35003, 2913, 35024, 2912, 35029, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
	end
elseif nEventID == 35024 then
	local state = pUser:SearchQuest(30);
	if state == 0 then
	pUser:NpcSay(35005, -1, -1, -1, -1, -1, -1, -1);
	pUser:SaveEvent(30, 1);
	end
elseif nEventID == 35029 then
	pUser:NpcSay(35009, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 35030 then
	local state = pUser:SearchQuest(30);
	if state == 1 then
	local count = pUser:HowMuchItem(379048000);
	if count >= 10 and count <= 9999 then
	pUser:RobItem(379048000, 10);
	pUser:ExpChange(500);
	pUser:SaveEvent(30, 2);
	pUser:NpcSay(35008, -1, -1, -1, -1, -1, -1, -1);
	end
	end
elseif nEventID == 35032 then
	local lvl = pUser:GetLevel();
	if lvl >= 1 and lvl <= 15 then
	pUser:NpcSay(35002, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	local state = pUser:SearchQuest(31);
	if state == 1 then
	local state = pUser:SearchQuest(31);
	if state == 1 then
	local count = pUser:HowMuchItem(379043000);
	if count >= 5 and count <= 9999 then
	pUser:SelectMsg(35014, 2914, 35050, 2915, 35049, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
	local count = pUser:HowMuchItem(379043000);
	if count >= 0 and count <= 4 then
	pUser:NpcSay(35013, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	end
	end
	local state = pUser:SearchQuest(31);
	if state == 0 then
	local state = pUser:SearchQuest(31);
	if state == 0 then
	pUser:SelectMsg(35011, 2911, 35044, 2912, 35049, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
	end
	local state = pUser:SearchQuest(31);
	if state == 2 then
	local state = pUser:SearchQuest(31);
	if state == 2 then
	pUser:SelectMsg(35010, 2913, 35044, 2912, 35049, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
	end
elseif nEventID == 35040 then
	local state = pUser:SearchQuest(31);
	if state == 2 then
	local state = pUser:SearchQuest(31);
	if state == 2 then
	pUser:SelectMsg(35010, 2913, 35044, 2912, 35049, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
	end
	local state = pUser:SearchQuest(31);
	if state == 0 then
	pUser:SelectMsg(35011, 2911, 35044, 2912, 35049, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
elseif nEventID == 35044 then
	local state = pUser:SearchQuest(31);
	if state == 0 then
	pUser:NpcSay(35012, -1, -1, -1, -1, -1, -1, -1);
	pUser:SaveEvent(31, 1);
	end
elseif nEventID == 35049 then
	pUser:NpcSay(35016, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 35050 then
	local state = pUser:SearchQuest(31);
	if state == 1 then
	local count = pUser:HowMuchItem(379043000);
	if count >= 5 and count <= 9999 then
	pUser:NpcSay(35015, -1, -1, -1, -1, -1, -1, -1);
	pUser:RobItem(379043000, 5);
	pUser:ExpChange(700);
	pUser:GiveItem(389062000, 5);
	pUser:SaveEvent(31, 2);
	end
	end
elseif nEventID == 35052 then
	local lvl = pUser:GetLevel();
	if lvl >= 1 and lvl <= 19 then
	pUser:NpcSay(35002, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	local state = pUser:SearchQuest(32);
	if state == 1 then
	local state = pUser:SearchQuest(32);
	if state == 1 then
	local count = pUser:HowMuchItem(379077000);
	if count >= 10 and count <= 9999 then
	pUser:SelectMsg(35021, 2914, 35070, 2915, 35069, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
	local count = pUser:HowMuchItem(379077000);
	if count >= 0 and count <= 9 then
	pUser:NpcSay(35020, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	end
	end
	local state = pUser:SearchQuest(32);
	if state == 0 then
	local state = pUser:SearchQuest(32);
	if state == 0 then
	pUser:SelectMsg(35018, 2911, 35064, 2912, 35069, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
	end
	local state = pUser:SearchQuest(32);
	if state == 2 then
	local state = pUser:SearchQuest(32);
	if state == 2 then
	pUser:SelectMsg(35017, 2913, 35064, 2912, 35069, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
	end
elseif nEventID == 35064 then
	local state = pUser:SearchQuest(32);
	if state == 0 then
	pUser:NpcSay(35019, -1, -1, -1, -1, -1, -1, -1);
	pUser:SaveEvent(32, 1);
	end
elseif nEventID == 35069 then
	pUser:NpcSay(35023, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 35070 then
	local state = pUser:SearchQuest(32);
	if state == 1 then
	local count = pUser:HowMuchItem(379077000);
	if count >= 10 and count <= 9999 then
	pUser:NpcSay(35022, -1, -1, -1, -1, -1, -1, -1);
	pUser:RobItem(379077000, 10);
	pUser:ExpChange(1000);
	pUser:GoldGain(50000);
	pUser:SaveEvent(32, 2);
	end
	end
elseif nEventID == 35072 then
	local lvl = pUser:GetLevel();
	if lvl >= 1 and lvl <= 2 then
	pUser:NpcSay(35002, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	local state = pUser:SearchQuest(33);
	if state == 1 then
	local state = pUser:SearchQuest(33);
	if state == 1 then
	local count = pUser:HowMuchItem(910017000);
	if count >= 5 and count <= 9999 then
	pUser:SelectMsg(35028, 2914, 35090, 2915, 35089, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
	local count = pUser:HowMuchItem(910017000);
	if count >= 0 and count <= 4 then
	pUser:NpcSay(35027, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	end
	end
	local state = pUser:SearchQuest(33);
	if state == 0 then
	local state = pUser:SearchQuest(33);
	if state == 0 then
	pUser:SelectMsg(35025, 2911, 35084, 2912, 35089, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
	end
	local state = pUser:SearchQuest(33);
	if state == 2 then
	local state = pUser:SearchQuest(33);
	if state == 2 then
	pUser:SelectMsg(35024, 2913, 35084, 2912, 35089, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
	end
elseif nEventID == 35084 then
	local state = pUser:SearchQuest(33);
	if state == 0 then
	pUser:NpcSay(35026, -1, -1, -1, -1, -1, -1, -1);
	pUser:SaveEvent(33, 1);
	do return; end
	end
elseif nEventID == 35089 then
	pUser:NpcSay(35030, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 35090 then
	local state = pUser:SearchQuest(33);
	if state == 1 then
	local count = pUser:HowMuchItem(910017000);
	if count >= 5 and count <= 9999 then
	pUser:NpcSay(35029, -1, -1, -1, -1, -1, -1, -1);
	pUser:RobItem(910017000, 5);
	pUser:ExpChange(3000);
	pUser:SaveEvent(33, 2);
	end
	end
elseif nEventID == 35092 then
	local lvl = pUser:GetLevel();
	if lvl >= 1 and lvl <= 15 then
	pUser:NpcSay(35002, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	local state = pUser:SearchQuest(34);
	if state == 1 then
	local state = pUser:SearchQuest(34);
	if state == 1 then
	local count = pUser:HowMuchItem(910020000);
	if count >= 10 and count <= 9999 then
	pUser:SelectMsg(35035, 2914, 35110, 2915, 35109, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
	local count = pUser:HowMuchItem(910020000);
	if count >= 0 and count <= 9 then
	pUser:NpcSay(35034, -1, -1, -1, -1, -1, -1, -1);
	end
	end
	end
	local state = pUser:SearchQuest(34);
	if state == 0 then
	local state = pUser:SearchQuest(34);
	if state == 0 then
	pUser:SelectMsg(35032, 2911, 35104, 2912, 35109, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
	end
	local state = pUser:SearchQuest(34);
	if state == 2 then
	local state = pUser:SearchQuest(34);
	if state == 2 then
	pUser:SelectMsg(35031, 2913, 35104, 2912, 35109, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
	end
elseif nEventID == 35104 then
	local state = pUser:SearchQuest(34);
	if state == 0 then
	pUser:NpcSay(35033, -1, -1, -1, -1, -1, -1, -1);
	pUser:SaveEvent(34, 1);
	end
elseif nEventID == 35109 then
	pUser:NpcSay(35037, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 35110 then
	local state = pUser:SearchQuest(34);
	if state == 1 then
	local count = pUser:HowMuchItem(910020000);
	if count >= 10 and count <= 9999 then
	pUser:NpcSay(35036, -1, -1, -1, -1, -1, -1, -1);
	pUser:RobItem(910020000, 10);
	pUser:ExpChange(5000);
	pUser:GiveItem(389063000, 10);
	pUser:SaveEvent(34, 2);
	end
	end
elseif nEventID == 35131 then
	local count = pUser:HowMuchItem(800032000);
	if count >= 0 and count <= 0 then
	pUser:NpcSay(30060, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	local count = pUser:HowMuchItem(800032000);
	if count >= 1 and count <= 9999 then
	pUser:SelectMsg(30065, 30070, 30070, 30075, 30075, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
elseif nEventID == 35150 then
	do return; end
elseif nEventID == 35160 then
	do return; end
elseif nEventID == 35170 then
	do return; end
elseif nEventID == 35201 then
	pUser:SelectMsg(35203, 35232, 35202, 70, 70, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 35202 then
	pUser:SelectMsg(35232, 35235, 35235, 35240, 35240, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 35212 then
	local state = pUser:SearchQuest(44);
	if state == 1 then
	local count = pUser:HowMuchItem(910023000);
	if count >= 10 and count <= 9999 then
	pUser:SelectMsg(35230, 35150, 35150, 35160, 35160, 35170, 35170, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
	local count = pUser:HowMuchItem(910023000);
	if count >= 0 and count <= 9 then
	pUser:NpcSay(35227, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	end
	local state = pUser:SearchQuest(44);
	if state == 0 then
	pUser:SelectMsg(35223, 35224, 35224, 35229, 35229, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
	local state = pUser:SearchQuest(44);
	if state == 2 then
	pUser:NpcSay(35222, -1, -1, -1, -1, -1, -1, -1);
	end
elseif nEventID == 35213 then
	local lvl = pUser:GetLevel();
	if lvl >= 1 and lvl <= 19 then
	pUser:NpcSay(35202, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
elseif nEventID == 35224 then
	local state = pUser:SearchQuest(44);
	if state == 0 then
	pUser:NpcSay(35224, -1, -1, -1, -1, -1, -1, -1);
	pUser:SaveEvent(44, 1);
	end
elseif nEventID == 35235 then
	local count = pUser:HowMuchItem(379106000);
	if count >= 0 and count <= 0 then
	pUser:NpcSay(35245, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	local count = pUser:HowMuchItem(379106000);
	if count >= 1 and count <= 9999 then
	pUser:SelectMsg(35250, 35260, 35260, 35270, 35270, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
elseif nEventID == 35240 then
	pUser:NpcSay(35240, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 35260 then
	if pUser:CheckExistItem(379106000, 0) then
	local lvl = pUser:GetLevel();
	if lvl >= 1 and lvl <= 20 then
	local rand_num = RollDice(sUID, 20);
	if pUser:CheckExistItem(379106000, 0) then
	if rand_num == 1 then
	pUser:NpcSay(35271, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 35271, 0);
	end
	end
	if pUser:CheckExistItem(379106000, 0) then
	if rand_num == 2 then
	pUser:NpcSay(35271, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 35272, 0);
	end
	end
	if pUser:CheckExistItem(379106000, 0) then
	if rand_num == 3 then
	pUser:NpcSay(35271, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 35273, 0);
	end
	end
	if pUser:CheckExistItem(379106000, 0) then
	if rand_num == 4 then
	pUser:NpcSay(35271, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 35274, 0);
	end
	end
	if pUser:CheckExistItem(379106000, 0) then
	if rand_num == 5 then
	pUser:NpcSay(35271, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 35275, 0);
	end
	end
	if pUser:CheckExistItem(379106000, 0) then
	if rand_num == 6 then
	pUser:NpcSay(35271, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 35276, 0);
	end
	end
	if pUser:CheckExistItem(379106000, 0) then
	if rand_num == 7 then
	pUser:NpcSay(35271, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 35277, 0);
	end
	end
	if pUser:CheckExistItem(379106000, 0) then
	if rand_num == 8 then
	pUser:NpcSay(35271, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 35278, 0);
	end
	end
	if pUser:CheckExistItem(379106000, 0) then
	if rand_num == 9 then
	pUser:NpcSay(35271, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 35279, 0);
	end
	end
	if pUser:CheckExistItem(379106000, 0) then
	if rand_num == 10 then
	pUser:NpcSay(35271, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 35280, 0);
	end
	end
	if pUser:CheckExistItem(379106000, 0) then
	if rand_num == 11 then
	pUser:NpcSay(35271, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 35281, 0);
	end
	end
	if pUser:CheckExistItem(379106000, 0) then
	if rand_num == 12 then
	pUser:NpcSay(35271, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 35282, 0);
	end
	end
	if pUser:CheckExistItem(379106000, 0) then
	if rand_num == 13 then
	pUser:NpcSay(35271, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 35283, 0);
	end
	end
	if pUser:CheckExistItem(379106000, 0) then
	if rand_num == 14 then
	pUser:NpcSay(35271, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 35284, 0);
	end
	end
	if pUser:CheckExistItem(379106000, 0) then
	if rand_num == 15 then
	pUser:NpcSay(35271, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 35285, 0);
	end
	end
	if pUser:CheckExistItem(379106000, 0) then
	if rand_num == 16 then
	pUser:NpcSay(35271, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 35286, 0);
	end
	end
	if pUser:CheckExistItem(379106000, 0) then
	if rand_num == 17 then
	pUser:NpcSay(35271, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 35287, 0);
	end
	end
	if pUser:CheckExistItem(379106000, 0) then
	if rand_num == 18 then
	pUser:NpcSay(35271, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 35288, 0);
	end
	end
	if pUser:CheckExistItem(379106000, 0) then
	if rand_num == 19 then
	pUser:NpcSay(35271, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 35289, 0);
	end
	end
	if pUser:CheckExistItem(379106000, 0) then
	if rand_num == 20 then
	pUser:NpcSay(35271, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 35290, 0);
	end
	end
	end
	end
	local lvl = pUser:GetLevel();
	if lvl >= 21 and lvl <= 40 then
	local rand_num = RollDice(sUID, 20);
	if pUser:CheckExistItem(379106000, 0) then
	if rand_num == 1 then
	pUser:NpcSay(35271, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 35271, 0);
	end
	end
	if pUser:CheckExistItem(379106000, 0) then
	if rand_num == 2 then
	pUser:NpcSay(35271, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 35272, 0);
	end
	end
	if pUser:CheckExistItem(379106000, 0) then
	if rand_num == 3 then
	pUser:NpcSay(35271, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 35273, 0);
	end
	end
	if pUser:CheckExistItem(379106000, 0) then
	if rand_num == 4 then
	pUser:NpcSay(35271, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 35274, 0);
	end
	end
	if pUser:CheckExistItem(379106000, 0) then
	if rand_num == 5 then
	pUser:NpcSay(35271, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 35275, 0);
	end
	end
	if pUser:CheckExistItem(379106000, 0) then
	if rand_num == 6 then
	pUser:NpcSay(35271, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 35276, 0);
	end
	end
	if pUser:CheckExistItem(379106000, 0) then
	if rand_num == 7 then
	pUser:NpcSay(35271, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 35277, 0);
	end
	end
	if pUser:CheckExistItem(379106000, 0) then
	if rand_num == 8 then
	pUser:NpcSay(35271, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 35278, 0);
	end
	end
	if pUser:CheckExistItem(379106000, 0) then
	if rand_num == 9 then
	pUser:NpcSay(35271, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 35279, 0);
	end
	end
	if pUser:CheckExistItem(379106000, 0) then
	if rand_num == 10 then
	pUser:NpcSay(35271, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 35280, 0);
	end
	end
	if pUser:CheckExistItem(379106000, 0) then
	if rand_num == 11 then
	pUser:NpcSay(35271, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 35281, 0);
	end
	end
	if pUser:CheckExistItem(379106000, 0) then
	if rand_num == 12 then
	pUser:NpcSay(35271, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 35282, 0);
	end
	end
	if pUser:CheckExistItem(379106000, 0) then
	if rand_num == 13 then
	pUser:NpcSay(35271, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 35283, 0);
	end
	end
	if pUser:CheckExistItem(379106000, 0) then
	if rand_num == 14 then
	pUser:NpcSay(35271, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 35284, 0);
	end
	end
	if pUser:CheckExistItem(379106000, 0) then
	if rand_num == 15 then
	pUser:NpcSay(35271, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 35285, 0);
	end
	end
	if pUser:CheckExistItem(379106000, 0) then
	if rand_num == 16 then
	pUser:NpcSay(35271, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 35286, 0);
	end
	end
	if pUser:CheckExistItem(379106000, 0) then
	if rand_num == 17 then
	pUser:NpcSay(35271, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 35287, 0);
	end
	end
	if pUser:CheckExistItem(379106000, 0) then
	if rand_num == 18 then
	pUser:NpcSay(35271, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 35288, 0);
	end
	end
	if pUser:CheckExistItem(379106000, 0) then
	if rand_num == 19 then
	pUser:NpcSay(35271, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 35289, 0);
	end
	end
	if pUser:CheckExistItem(379106000, 0) then
	if rand_num == 20 then
	pUser:NpcSay(35271, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 35290, 0);
	end
	end
	end
	local lvl = pUser:GetLevel();
	if lvl >= 41 and lvl <= 99 then
	local rand_num = RollDice(sUID, 20);
	if pUser:CheckExistItem(379106000, 0) then
	if rand_num == 1 then
	pUser:NpcSay(35271, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 35271, 0);
	end
	end
	if pUser:CheckExistItem(379106000, 0) then
	if rand_num == 2 then
	pUser:NpcSay(35271, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 35272, 0);
	end
	end
	if pUser:CheckExistItem(379106000, 0) then
	if rand_num == 3 then
	pUser:NpcSay(35271, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 35273, 0);
	end
	end
	if pUser:CheckExistItem(379106000, 0) then
	if rand_num == 4 then
	pUser:NpcSay(35271, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 35274, 0);
	end
	end
	if pUser:CheckExistItem(379106000, 0) then
	if rand_num == 5 then
	pUser:NpcSay(35271, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 35275, 0);
	end
	end
	if pUser:CheckExistItem(379106000, 0) then
	if rand_num == 6 then
	pUser:NpcSay(35271, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 35276, 0);
	end
	end
	if pUser:CheckExistItem(379106000, 0) then
	if rand_num == 7 then
	pUser:NpcSay(35271, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 35277, 0);
	end
	end
	if pUser:CheckExistItem(379106000, 0) then
	if rand_num == 8 then
	pUser:NpcSay(35271, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 35278, 0);
	end
	end
	if pUser:CheckExistItem(379106000, 0) then
	if rand_num == 9 then
	pUser:NpcSay(35271, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 35279, 0);
	end
	end
	if pUser:CheckExistItem(379106000, 0) then
	if rand_num == 10 then
	pUser:NpcSay(35271, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 35280, 0);
	end
	end
	if pUser:CheckExistItem(379106000, 0) then
	if rand_num == 11 then
	pUser:NpcSay(35271, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 35281, 0);
	end
	end
	if pUser:CheckExistItem(379106000, 0) then
	if rand_num == 12 then
	pUser:NpcSay(35271, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 35282, 0);
	end
	end
	if pUser:CheckExistItem(379106000, 0) then
	if rand_num == 13 then
	pUser:NpcSay(35271, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 35283, 0);
	end
	end
	if pUser:CheckExistItem(379106000, 0) then
	if rand_num == 14 then
	pUser:NpcSay(35271, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 35284, 0);
	end
	end
	if pUser:CheckExistItem(379106000, 0) then
	if rand_num == 15 then
	pUser:NpcSay(35271, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 35285, 0);
	end
	end
	if pUser:CheckExistItem(379106000, 0) then
	if rand_num == 16 then
	pUser:NpcSay(35271, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 35286, 0);
	end
	end
	if pUser:CheckExistItem(379106000, 0) then
	if rand_num == 17 then
	pUser:NpcSay(35271, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 35287, 0);
	end
	end
	if pUser:CheckExistItem(379106000, 0) then
	if rand_num == 18 then
	pUser:NpcSay(35271, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 35288, 0);
	end
	end
	if pUser:CheckExistItem(379106000, 0) then
	if rand_num == 19 then
	pUser:NpcSay(35271, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 35289, 0);
	end
	end
	if pUser:CheckExistItem(379106000, 0) then
	if rand_num == 20 then
	pUser:NpcSay(35271, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 35290, 0);
	end
	end
	end
elseif nEventID == 35270 then
	pUser:NpcSay(35270, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 35291 then
	if pUser:CheckExistItem(379106000, 0) then
	if rand_num == 1 then
	pUser:NpcSay(35271, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 35291, 0);
	end
	end
elseif nEventID == 35292 then
	if pUser:CheckExistItem(379106000, 0) then
	if rand_num == 2 then
	pUser:NpcSay(35271, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 35292, 0);
	end
	end
elseif nEventID == 35293 then
	if pUser:CheckExistItem(379106000, 0) then
	if rand_num == 3 then
	pUser:NpcSay(35271, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 35293, 0);
	end
	end
elseif nEventID == 35294 then
	if pUser:CheckExistItem(379106000, 0) then
	if rand_num == 4 then
	pUser:NpcSay(35271, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 35294, 0);
	end
	end
elseif nEventID == 35295 then
	if pUser:CheckExistItem(379106000, 0) then
	if rand_num == 5 then
	pUser:NpcSay(35271, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 35295, 0);
	end
	end
elseif nEventID == 35296 then
	if pUser:CheckExistItem(379106000, 0) then
	if rand_num == 6 then
	pUser:NpcSay(35271, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 35296, 0);
	end
	end
elseif nEventID == 35297 then
	if pUser:CheckExistItem(379106000, 0) then
	if rand_num == 7 then
	pUser:NpcSay(35271, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 35297, 0);
	end
	end
elseif nEventID == 35298 then
	if pUser:CheckExistItem(379106000, 0) then
	if rand_num == 8 then
	pUser:NpcSay(35271, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 35298, 0);
	end
	end
elseif nEventID == 35299 then
	if pUser:CheckExistItem(379106000, 0) then
	if rand_num == 9 then
	pUser:NpcSay(35271, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 35299, 0);
	end
	end
elseif nEventID == 35300 then
	if pUser:CheckExistItem(379106000, 0) then
	if rand_num == 10 then
	pUser:NpcSay(35271, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 35300, 0);
	end
	end
elseif nEventID == 35301 then
	if pUser:CheckExistItem(379106000, 0) then
	if rand_num == 11 then
	pUser:NpcSay(35271, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 35301, 0);
	end
	end
elseif nEventID == 35302 then
	if pUser:CheckExistItem(379106000, 0) then
	if rand_num == 12 then
	pUser:NpcSay(35271, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 35302, 0);
	end
	end
elseif nEventID == 35303 then
	if pUser:CheckExistItem(379106000, 0) then
	if rand_num == 13 then
	pUser:NpcSay(35271, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 35303, 0);
	end
	end
elseif nEventID == 35304 then
	if pUser:CheckExistItem(379106000, 0) then
	if rand_num == 14 then
	pUser:NpcSay(35271, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 35304, 0);
	end
	end
elseif nEventID == 35305 then
	if pUser:CheckExistItem(379106000, 0) then
	if rand_num == 15 then
	pUser:NpcSay(35271, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 35305, 0);
	end
	end
elseif nEventID == 35306 then
	if pUser:CheckExistItem(379106000, 0) then
	if rand_num == 16 then
	pUser:NpcSay(35271, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 35306, 0);
	end
	end
elseif nEventID == 35307 then
	if pUser:CheckExistItem(379106000, 0) then
	if rand_num == 17 then
	pUser:NpcSay(35271, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 35307, 0);
	end
	end
elseif nEventID == 35308 then
	if pUser:CheckExistItem(379106000, 0) then
	if rand_num == 18 then
	pUser:NpcSay(35271, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 35308, 0);
	end
	end
elseif nEventID == 35309 then
	if pUser:CheckExistItem(379106000, 0) then
	if rand_num == 19 then
	pUser:NpcSay(35271, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 35309, 0);
	end
	end
elseif nEventID == 35310 then
	if pUser:CheckExistItem(379106000, 0) then
	if rand_num == 20 then
	pUser:NpcSay(35271, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 35310, 0);
	end
	end
elseif nEventID == 35311 then
	if pUser:CheckExistItem(379106000, 0) then
	if rand_num == 1 then
	pUser:NpcSay(35271, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 35311, 0);
	end
	end
elseif nEventID == 35312 then
	if pUser:CheckExistItem(379106000, 0) then
	if rand_num == 2 then
	pUser:NpcSay(35271, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 35312, 0);
	end
	end
elseif nEventID == 35313 then
	if pUser:CheckExistItem(379106000, 0) then
	if rand_num == 3 then
	pUser:NpcSay(35271, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 35313, 0);
	end
	end
elseif nEventID == 35314 then
	if pUser:CheckExistItem(379106000, 0) then
	if rand_num == 4 then
	pUser:NpcSay(35271, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 35314, 0);
	end
	end
elseif nEventID == 35315 then
	if pUser:CheckExistItem(379106000, 0) then
	if rand_num == 5 then
	pUser:NpcSay(35271, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 35315, 0);
	end
	end
elseif nEventID == 35316 then
	if pUser:CheckExistItem(379106000, 0) then
	if rand_num == 6 then
	pUser:NpcSay(35271, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 35316, 0);
	end
	end
elseif nEventID == 35317 then
	if pUser:CheckExistItem(379106000, 0) then
	if rand_num == 7 then
	pUser:NpcSay(35271, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 35317, 0);
	end
	end
elseif nEventID == 35318 then
	if pUser:CheckExistItem(379106000, 0) then
	if rand_num == 8 then
	pUser:NpcSay(35271, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 35318, 0);
	end
	end
elseif nEventID == 35319 then
	if pUser:CheckExistItem(379106000, 0) then
	if rand_num == 9 then
	pUser:NpcSay(35271, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 35319, 0);
	end
	end
elseif nEventID == 35320 then
	if pUser:CheckExistItem(379106000, 0) then
	if rand_num == 10 then
	pUser:NpcSay(35271, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 35320, 0);
	end
	end
elseif nEventID == 35321 then
	if pUser:CheckExistItem(379106000, 0) then
	if rand_num == 11 then
	pUser:NpcSay(35271, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 35321, 0);
	end
	end
elseif nEventID == 35322 then
	if pUser:CheckExistItem(379106000, 0) then
	if rand_num == 12 then
	pUser:NpcSay(35271, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 35322, 0);
	end
	end
elseif nEventID == 35323 then
	if pUser:CheckExistItem(379106000, 0) then
	if rand_num == 13 then
	pUser:NpcSay(35271, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 35323, 0);
	end
	end
elseif nEventID == 35324 then
	if pUser:CheckExistItem(379106000, 0) then
	if rand_num == 14 then
	pUser:NpcSay(35271, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 35324, 0);
	end
	end
elseif nEventID == 35325 then
	if pUser:CheckExistItem(379106000, 0) then
	if rand_num == 15 then
	pUser:NpcSay(35271, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 35325, 0);
	end
	end
elseif nEventID == 35326 then
	if pUser:CheckExistItem(379106000, 0) then
	if rand_num == 16 then
	pUser:NpcSay(35271, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 35326, 0);
	end
	end
elseif nEventID == 35327 then
	if pUser:CheckExistItem(379106000, 0) then
	if rand_num == 17 then
	pUser:NpcSay(35271, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 35327, 0);
	end
	end
elseif nEventID == 35328 then
	if pUser:CheckExistItem(379106000, 0) then
	if rand_num == 18 then
	pUser:NpcSay(35271, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 35328, 0);
	end
	end
elseif nEventID == 35329 then
	if pUser:CheckExistItem(379106000, 0) then
	if rand_num == 19 then
	pUser:NpcSay(35271, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 35329, 0);
	end
	end
elseif nEventID == 35330 then
	if pUser:CheckExistItem(379106000, 0) then
	if rand_num == 20 then
	pUser:NpcSay(35271, -1, -1, -1, -1, -1, -1, -1);
	RunCountExchange(sUID, 35330, 0);
	end
	end
elseif nEventID == 35331 then
	do return; end
elseif nEventID == 35332 then
	do return; end
elseif nEventID == 35333 then
	do return; end
elseif nEventID == 35334 then
	do return; end
elseif nEventID == 35335 then
	do return; end
elseif nEventID == 35336 then
	do return; end
elseif nEventID == 35337 then
	do return; end
elseif nEventID == 35338 then
	do return; end
elseif nEventID == 35339 then
	do return; end
elseif nEventID == 35340 then
	do return; end
elseif nEventID == 35341 then
	do return; end
elseif nEventID == 35342 then
	do return; end
elseif nEventID == 35343 then
	do return; end
elseif nEventID == 35344 then
	do return; end
elseif nEventID == 35345 then
	do return; end
elseif nEventID == 35346 then
	do return; end
elseif nEventID == 35347 then
	do return; end
elseif nEventID == 35348 then
	do return; end
elseif nEventID == 35349 then
	do return; end
elseif nEventID == 35350 then
	do return; end
elseif nEventID == 35351 then
	pUser:SelectMsg(35351, 35355, 35355, 35360, 35360, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 35355 then
	local count = pUser:HowMuchItem(379095000);
	if count >= 0 and count <= 0 then
	pUser:NpcSay(35365, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	local count = pUser:HowMuchItem(379095000);
	if count >= 1 and count <= 9999 then
	pUser:SelectMsg(35370, 35370, 35390, 35380, 35380, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
elseif nEventID == 35360 then
	pUser:NpcSay(35360, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 35380 then
	pUser:NpcSay(35380, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 35390 then
	local lvl = pUser:GetLevel();
	if lvl >= 1 and lvl <= 99 then
	local rand_num = RollDice(sUID, 20);
	do return; end
	do return; end
	do return; end
	do return; end
	do return; end
	do return; end
	do return; end
	do return; end
	do return; end
	do return; end
	do return; end
	do return; end
	do return; end
	do return; end
	do return; end
	do return; end
	do return; end
	do return; end
	do return; end
	do return; end
	end
elseif nEventID == 35391 then
	do return; end
elseif nEventID == 35392 then
	do return; end
elseif nEventID == 35393 then
	do return; end
elseif nEventID == 35394 then
	do return; end
elseif nEventID == 35395 then
	do return; end
elseif nEventID == 35396 then
	do return; end
elseif nEventID == 35397 then
	do return; end
elseif nEventID == 35398 then
	do return; end
elseif nEventID == 35399 then
	do return; end
elseif nEventID == 35400 then
	do return; end
elseif nEventID == 35401 then
	do return; end
elseif nEventID == 35402 then
	do return; end
elseif nEventID == 35403 then
	do return; end
elseif nEventID == 35404 then
	do return; end
elseif nEventID == 35405 then
	do return; end
elseif nEventID == 35406 then
	do return; end
elseif nEventID == 35407 then
	do return; end
elseif nEventID == 35408 then
	do return; end
elseif nEventID == 35409 then
	do return; end
elseif nEventID == 35410 then
	do return; end
elseif nEventID == 35451 then
	pUser:SelectMsg(35451, 35453, 35453, 35452, 35452, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 35452 then
	pUser:NpcSay(35452, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 35453 then
	local count = pUser:HowMuchItem(379132000);
	if count >= 0 and count <= 4 then
	pUser:NpcSay(35454, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	local count = pUser:HowMuchItem(379132000);
	if count >= 5 and count <= 9999 then
	pUser:SelectMsg(35455, 35456, 35456, 35463, 35457, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	end
elseif nEventID == 35456 then
	local lvl = pUser:GetLevel();
	if lvl >= 1 and lvl <= 20 then
	local rand_num = RollDice(sUID, 20);
	do return; end
	do return; end
	do return; end
	do return; end
	do return; end
	do return; end
	do return; end
	do return; end
	do return; end
	do return; end
	do return; end
	do return; end
	do return; end
	do return; end
	do return; end
	do return; end
	do return; end
	do return; end
	do return; end
	do return; end
	end
	local lvl = pUser:GetLevel();
	if lvl >= 21 and lvl <= 40 then
	local rand_num = RollDice(sUID, 20);
	do return; end
	do return; end
	do return; end
	do return; end
	do return; end
	do return; end
	do return; end
	do return; end
	do return; end
	do return; end
	do return; end
	do return; end
	do return; end
	do return; end
	do return; end
	do return; end
	do return; end
	do return; end
	do return; end
	do return; end
	end
	local lvl = pUser:GetLevel();
	if lvl >= 41 and lvl <= 99 then
	local rand_num = RollDice(sUID, 20);
	do return; end
	do return; end
	do return; end
	do return; end
	do return; end
	do return; end
	do return; end
	do return; end
	do return; end
	do return; end
	do return; end
	do return; end
	do return; end
	do return; end
	do return; end
	do return; end
	do return; end
	do return; end
	do return; end
	do return; end
	end
elseif nEventID == 35457 then
	pUser:NpcSay(35457, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 35461 then
	pUser:SelectMsg(35461, 35467, 35462, 35470, 35463, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 35462 then
	local state = pUser:SearchQuest(45);
	if state == 2 then
	pUser:NpcSay(35469, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	do return; end
	do return; end
elseif nEventID == 35463 then
	pUser:NpcSay(35470, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 35473 then
	do return; end
elseif nEventID == 35480 then
	pUser:SelectMsg(35480, 35510, 35510, 35530, 35530, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 35510 then
	pUser:SelectMsg(35510, 35511, 35511, 35512, 35512, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 35511 then
	local count = pUser:HowMuchItem(910107000);
	if count >= 1 and count <= 9999 then
	pUser:NpcSay(35519, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	local count = pUser:HowMuchItem(910108000);
	if count >= 1 and count <= 9999 then
	pUser:NpcSay(35519, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	local count = pUser:HowMuchItem(910109000);
	if count >= 1 and count <= 9999 then
	pUser:NpcSay(35519, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SelectMsg(35511, 35513, 35513, 35514, 35526, 35515, 35527, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 35512 then
	pUser:NpcSay(35512, -1, -1, -1, -1, -1, -1, -1);
	do return; end
elseif nEventID == 35513 then
	local count = pUser:HowMuchItem(379143000);
	if count >= 0 and count <= 0 then
	pUser:NpcSay(35514, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	local count = pUser:HowMuchItem(379144000);
	if count >= 0 and count <= 0 then
	pUser:NpcSay(35515, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	do return; end
elseif nEventID == 35516 then
	local count = pUser:HowMuchItem(379145000);
	if count >= 0 and count <= 0 then
	pUser:NpcSay(35516, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
elseif nEventID == 35517 then
	local count = pUser:HowMuchItem(379146000);
	if count >= 0 and count <= 0 then
	pUser:NpcSay(35517, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
elseif nEventID == 35524 then
	do return; end
elseif nEventID == 35525 then
	do return; end
elseif nEventID == 35526 then
	local count = pUser:HowMuchItem(379143000);
	if count >= 0 and count <= 0 then
	pUser:NpcSay(35514, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	local count = pUser:HowMuchItem(379144000);
	if count >= 0 and count <= 0 then
	pUser:NpcSay(35515, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	local count = pUser:HowMuchItem(379145000);
	if count >= 0 and count <= 0 then
	pUser:NpcSay(35516, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	do return; end
elseif nEventID == 35527 then
	local count = pUser:HowMuchItem(379143000);
	if count >= 0 and count <= 0 then
	pUser:NpcSay(35514, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	local count = pUser:HowMuchItem(379144000);
	if count >= 0 and count <= 0 then
	pUser:NpcSay(35515, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	local count = pUser:HowMuchItem(379146000);
	if count >= 0 and count <= 0 then
	pUser:NpcSay(35517, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	do return; end
elseif nEventID == 35530 then
	pUser:SelectMsg(35530, 35531, 35531, 35532, 35532, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 35531 then
	local count = pUser:HowMuchItem(910036000);
	if count >= 0 and count <= 0 then
	pUser:NpcSay(35533, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	do return; end
elseif nEventID == 35532 then
	pUser:NpcSay(35532, -1, -1, -1, -1, -1, -1, -1);
	do return; end
elseif nEventID == 35535 then
	pUser:SelectMsg(35535, 35536, 35539, 30134, 35540, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 35536 then
	local count = pUser:HowMuchItem(190250261);
	if count >= 1 and count <= 9999 then
	pUser:GoldLose(500000);
	do return; end
	end
elseif nEventID == 35537 then
	local count = pUser:HowMuchItem(190250261);
	if count >= 0 and count <= 0 then
	pUser:NpcSay(35536, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
elseif nEventID == 35539 then
	local count = pUser:HowMuchItem(190250261);
	if count >= 1 and count <= 9999 then
	pUser:GoldLose(500000);
	do return; end
	end
	local count = pUser:HowMuchItem(190250261);
	if count >= 0 and count <= 0 then
	pUser:NpcSay(35536, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
elseif nEventID == 35540 then
	pUser:NpcSay(35540, -1, -1, -1, -1, -1, -1, -1);
	do return; end
elseif nEventID == 35550 then
	pUser:SendDebugString("Unknown EXEC command 'CHECK_PCBANG_OWNER'."); -- unknown execute command (CHECK_PCBANG_OWNER)
	do return; end
elseif nEventID == 35551 then
	pUser:SendDebugString("Unknown EXEC command 'CHECK_PCBANG_ITEM'."); -- unknown execute command (CHECK_PCBANG_ITEM)
	do return; end
elseif nEventID == 35552 then
	pUser:SendDebugString("Unknown EXEC command 'CHECK_PCBANG_ITEM'."); -- unknown execute command (CHECK_PCBANG_ITEM)
	do return; end
elseif nEventID == 35553 then
	pUser:SendDebugString("Unknown EXEC command 'SHOW_PCBANG_ITEM'."); -- unknown execute command (SHOW_PCBANG_ITEM)
	do return; end
elseif nEventID == 35554 then
	pUser:NpcSay(35556, -1, -1, -1, -1, -1, -1, -1);
	do return; end
elseif nEventID == 35555 then
	pUser:NpcSay(35555, -1, -1, -1, -1, -1, -1, -1);
	do return; end
elseif nEventID == 35556 then
	do return; end
elseif nEventID == 35557 then
	pUser:NpcSay(35557, -1, -1, -1, -1, -1, -1, -1);
	do return; end
elseif nEventID == 35558 then
	do return; end
elseif nEventID == 35559 then
	pUser:NpcSay(35552, -1, -1, -1, -1, -1, -1, -1);
	do return; end
elseif nEventID == 35560 then
	pUser:SelectMsg(35560, 35561, 35561, 35562, 35562, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 35561 then
	pUser:SendDebugString("Unknown EXEC command 'SEND_WEBPAGE_ADDRESS'."); -- unknown execute command (SEND_WEBPAGE_ADDRESS)
	do return; end
elseif nEventID == 35562 then
	do return; end
elseif nEventID == 35563 then
	pUser:SelectMsg(35563, 35564, 35564, 35566, 35566, 35567, 35700, 35703, 35703, 35565, 35565, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 35564 then
	pUser:NpcSay(35564, 35565, 35566, 35567, -1, -1, -1, -1);
	do return; end
elseif nEventID == 35565 then
	pUser:NpcSay(35568, 35569, 35570, -1, -1, -1, -1, -1);
	do return; end
elseif nEventID == 35566 then
	pUser:SelectMsg(35571, 35572, 35572, 35573, 35573, 35574, 35574, 35568, 35563, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 35567 then
	pUser:NpcSay(35591, 35592, 35592, 35593, 35589, 35590, -1, -1);
	do return; end
elseif nEventID == 35570 then
	local lvl = pUser:GetLevel();
	if lvl >= 6 and lvl <= 99 then
	pUser:NpcSay(35704, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	local lvl = pUser:GetLevel();
	if lvl >= 0 and lvl <= 5 then
	pUser:SendDebugString("Unknown EXEC command 'ZONE_CHANGE'."); -- unknown execute command (ZONE_CHANGE)
	do return; end
	end
elseif nEventID == 35572 then
	pUser:NpcSay(35572, 35701, 35575, 35576, -1, -1, -1, -1);
	do return; end
elseif nEventID == 35573 then
	pUser:NpcSay(35577, 35578, 35579, 35580, 35581, 35582, 35583, -1);
	do return; end
elseif nEventID == 35574 then
	pUser:NpcSay(35584, 35585, 35586, 35587, 35588, -1, -1, -1);
	do return; end
elseif nEventID == 35575 then
	pUser:SelectMsg(35551, 35558, 35551, 35552, 35552, 35553, 35553, 35559, 35554, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 35576 then
	pUser:SelectMsg(35719, 35576, 35556, 35559, 35554, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 35577 then
	pUser:SelectMsg(35725, 35577, 35558, 35559, 35554, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 35624 then
	pUser:SelectMsg(35624, 35625, 35625, 35626, 35631, 35627, 35627, 35580, 35628, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 35625 then
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_CASTLE'.");
	if false then -- unknown logic command (CHECK_CASTLE)
	pUser:SendDebugString("Unknown EXEC command 'SEND_WEBPAGE_ADDRESS'."); -- unknown execute command (SEND_WEBPAGE_ADDRESS)
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_NO_CASTLE'.");
	if false then -- unknown logic command (CHECK_NO_CASTLE)
	pUser:NpcSay(35631, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
elseif nEventID == 35627 then
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_CHIEF'.");
	if false then -- unknown logic command (CHECK_CHIEF)
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_CLAN_RANKING'.");
	if false then -- unknown logic command (CHECK_CLAN_RANKING)
	pUser:SendDebugString("Unknown EXEC command 'SEND_WEBPAGE_ADDRESS'."); -- unknown execute command (SEND_WEBPAGE_ADDRESS)
	do return; end
	do return; end
	end
	pUser:NpcSay(35638, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_NO_CHIEF'.");
	if false then -- unknown logic command (CHECK_NO_CHIEF)
	pUser:NpcSay(35633, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
elseif nEventID == 35628 then
	pUser:NpcSay(35628, -1, -1, -1, -1, -1, -1, -1);
	do return; end
elseif nEventID == 35631 then
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_CHIEF'.");
	if false then -- unknown logic command (CHECK_CHIEF)
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_CLAN_RANKING'.");
	if false then -- unknown logic command (CHECK_CLAN_RANKING)
	pUser:NpcSay(35634, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_CLAN_RANKING'.");
	if false then -- unknown logic command (CHECK_CLAN_RANKING)
	pUser:SendDebugString("Unknown EXEC command 'SEND_WEBPAGE_ADDRESS'."); -- unknown execute command (SEND_WEBPAGE_ADDRESS)
	do return; end
	end
	end
	pUser:SendDebugString("Unknown LOGIC command 'CHECK_NO_CHIEF'.");
	if false then -- unknown logic command (CHECK_NO_CHIEF)
	pUser:NpcSay(35633, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
elseif nEventID == 35640 then
	pUser:SelectMsg(35640, 35641, 35641, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 35641 then
	pUser:SendDebugString("Unknown EXEC command 'ZONE_CHANGE'."); -- unknown execute command (ZONE_CHANGE)
	do return; end
elseif nEventID == 35643 then
	pUser:NpcSay(35643, -1, -1, -1, -1, -1, -1, -1);
	do return; end
elseif nEventID == 35662 then
	pUser:SelectMsg(35662, 35663, 35663, 35664, 35664, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 35663 then
	local lvl = pUser:GetLevel();
	if lvl >= 0 and lvl <= 14 then
	pUser:NpcSay(35665, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	local state = pUser:SearchQuest(56);
	if state == 2 then
	pUser:NpcSay(35667, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	local state = pUser:SearchQuest(56);
	if state == 1 then
	pUser:NpcSay(35668, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	if pUser:EmptySlotCount() == 0 then
	pUser:NpcSay(35670, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
	local state = pUser:SearchQuest(56);
	if state == 0 then
	pUser:GiveItem(910080000, 1);
	pUser:SaveEvent(56, 1);
	pUser:NpcSay(35671, -1, -1, -1, -1, -1, -1, -1);
	do return; end
	end
elseif nEventID == 35664 then
	pUser:NpcSay(35647, -1, -1, -1, -1, -1, -1, -1);
	do return; end
elseif nEventID == 35700 then
	pUser:SelectMsg(35700, 35701, 35567, 35702, 35570, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
elseif nEventID == 35703 then
	pUser:NpcSay(35705, -1, -1, -1, -1, -1, -1, -1);
	do return; end
else
	pUser:SendDebugString("Unprocessed event id ("..nEventID..")");
end
