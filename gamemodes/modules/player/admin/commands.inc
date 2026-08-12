CMD:apanel(playerid)
{
	if(PI[playerid][pAdmin] < ADM_MODER) return 0;

	if(ADuty[playerid] == false)
	{
		ShowPlayerAdminAuth(playerid);
	}
	return 1;
}

CMD:a(playerid, params[])
{
	if(PI[playerid][pAdmin] < ADM_MODER) return 0;
    if(ADuty[playerid] != true) return SendClientMessage(playerid,COLOR_GREY,NO_DUTY_TEXT);
    new result[150], string[200];
    if(sscanf(params,"s[150]",result)) return SendClientMessage(playerid,COLOR_RED,"Используйте: /a [текст]");
	format(string,sizeof(string),"[A] %s[%i]: %s",PN(playerid),playerid,result);
    SendAdminsMessage(COLOR_ACHAT,string,1);
	return 1;
}