stock J_Kick(playerid)
{
    if(IsPlayerNPC(playerid)) return 0;
	if(PI[playerid][pAdmin] > 5) return 0;
	Kick(playerid);
	return 1;
}