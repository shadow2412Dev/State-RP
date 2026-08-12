stock J_Kick(playerid)
{
    if(IsPlayerNPC(playerid)) return 0;
	if(PI[playerid][pAdmin] > 5) return 0;
	Kick(playerid);
	return 1;
}

stock GetString(const param1[],const param2[])
{
	return !strcmp(param1, param2, false);
}