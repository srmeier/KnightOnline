if nEventID == 35001 then
	pUser:SelectMsg(35001, 2901, 35012, 2902, 35032, 2903, 35052, 2904, 35072, 2905, 35092, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
	return;
elseif nEventID == 35012 then
local lvl = pUser:GetLevel();
if lvl >= 1 and lvl < 10 then
	pUser:NpcSay(35002);
	return;
end
local state = pUser:SearchQuest(30);
if state == 1 then
local state = pUser:SearchQuest(30);
if state == 1 then
local count = pUser:HowMuchItem(379048000);
if count >= 10 and count < 9999 then
	pUser:SelectMsg(35007, 2914, 35030, 2915, 35029, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
end
local count = pUser:HowMuchItem(379048000);
if count >= 0 and count < 9 then
	pUser:NpcSay(35006);
	return;
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
elseif nEventID == 35013 then
local lvl = pUser:GetLevel();
if lvl >= 1 and lvl < 10 then
	pUser:NpcSay(35002);
	return;
end
elseif nEventID == 35014 then
local state = pUser:SearchQuest(30);
if state == 1 then
local state = pUser:SearchQuest(30);
if state == 1 then
local count = pUser:HowMuchItem(379048000);
if count >= 10 and count < 9999 then
	pUser:SelectMsg(35007, 2914, 35030, 2915, 35029, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
end
local count = pUser:HowMuchItem(379048000);
if count >= 0 and count < 9 then
	pUser:NpcSay(35006);
	return;
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
elseif nEventID == 35015 then
local state = pUser:SearchQuest(30);
if state == 1 then
local state = pUser:SearchQuest(30);
if state == 1 then
local count = pUser:HowMuchItem(379048000);
if count >= 10 and count < 9999 then
	pUser:SelectMsg(35007, 2914, 35030, 2915, 35029, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
end
local count = pUser:HowMuchItem(379048000);
if count >= 0 and count < 9 then
	pUser:NpcSay(35006);
	return;
end
end
end
elseif nEventID == 35016 then
local state = pUser:SearchQuest(30);
if state == 0 then
local state = pUser:SearchQuest(30);
if state == 0 then
	pUser:SelectMsg(35004, 2911, 35024, 2912, 35029, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
end
end
elseif nEventID == 35017 then
local state = pUser:SearchQuest(30);
if state == 2 then
local state = pUser:SearchQuest(30);
if state == 2 then
	pUser:SelectMsg(35003, 2913, 35024, 2912, 35029, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
end
end
elseif nEventID == 35022 then
local state = pUser:SearchQuest(30);
if state == 2 then
	pUser:SelectMsg(35003, 2913, 35024, 2912, 35029, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
end
elseif nEventID == 35023 then
local state = pUser:SearchQuest(30);
if state == 0 then
	pUser:SelectMsg(35004, 2911, 35024, 2912, 35029, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
end
elseif nEventID == 35024 then
local state = pUser:SearchQuest(30);
if state == 0 then
	pUser:NpcSay(35005);
	pUser:GiveItem(379048000, 10);
	pUser:SaveEvent(30, 1);
end
elseif nEventID == 35025 then
local state = pUser:SearchQuest(30);
if state == 1 then
local count = pUser:HowMuchItem(379048000);
if count >= 10 and count < 9999 then
	pUser:SelectMsg(35007, 2914, 35030, 2915, 35029, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
end
local count = pUser:HowMuchItem(379048000);
if count >= 0 and count < 9 then
	pUser:NpcSay(35006);
	return;
end
end
elseif nEventID == 35026 then
local count = pUser:HowMuchItem(379048000);
if count >= 10 and count < 9999 then
	pUser:SelectMsg(35007, 2914, 35030, 2915, 35029, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
end
elseif nEventID == 35027 then
local count = pUser:HowMuchItem(379048000);
if count >= 0 and count < 9 then
	pUser:NpcSay(35006);
	return;
end
elseif nEventID == 35029 then
	pUser:NpcSay(35009);
elseif nEventID == 35030 then
local state = pUser:SearchQuest(30);
if state == 1 then
local count = pUser:HowMuchItem(379048000);
if count >= 10 and count < 9999 then
	pUser:RobItem(379048000, 10);
	pUser:ExpChange(500);
	pUser:SaveEvent(30, 2);
	pUser:NpcSay(35008);
end
end
else
	pUser:SendDebugString("Unprocessed event id("..nEventID..")");
end
