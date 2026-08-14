CMD:apanel(playerid)
{
	if(PI[playerid][pAdmin] < ADM_MODER) return 0;

	if(ADuty[playerid] == false)
	{
		ShowPlayerAdminAuth(playerid);
	}
	else
	{
		f(String512, sizeof(String512), "\
		[1] Admins\n\
		[2] Очистить чат");
		ShowPlayerDialogEx(playerid, DIALOG_APANEL, DIALOG_STYLE_LIST, "Админ панель", String512, "Выбор", "Отмена");
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

CMD:setskin(playerid, params[])
{
    if(PI[playerid][pAdmin] < ADM_SUPER_ADMIN) return false;
    if(ADuty[playerid]!= true) return SendClientMessage(playerid,COLOR_GREY,NO_DUTY_TEXT);
    new id,skin,string[150],oll;
    if(sscanf(params,"udd",id,skin,oll)) return SendClientMessage(playerid, COLOR_RED, "Используй: /setskin [id] [0 - 311] [0/1]");
    if(oll<0 || oll > 1) return SendClientMessage(playerid, COLOR_RED, "Используй: /setskin [id] [0 - 311] [0/1]");
    if(oll==1 && PI[playerid][pAdmin]< ADM_GRAND_ADMIN) return SendClientMessage(playerid, COLOR_RED, "Ты не можешь выдать вечный скин!");
    
	format(string,sizeof(string),"Вы дали %s скин %d",PN(id),skin);
	SendClientMessage(playerid,-1,string);

	format(string,sizeof(string),"[A] %s[%d] установил скин [ID: %d] игроку %s[%d]",PN(playerid),playerid,skin,PN(id),id);
	SendAdminsMessage(COLOR_GREY,string,1);

	SetPlayerSkinEx(id,skin);
	f_TogglePlayerControllable(id,true);

	if(oll == 1)
	{
		PI[id][pSkin] = skin;
		f(String144, sizeof(String144), "UPDATE `accounts` SET `Skin` = %i WHERE `ID` = %i", skin, PI[id][pID]);
		mysql_tquery(mysql, String144);

		format(string,sizeof(string),"Администратор %s выдал вам вечный скин.",PN(playerid));
		SendClientMessage(id,COLOR_RED,string);
	}
	else
	{
		format(string,sizeof(string),"Администратор %s выдал вам временный скин.",PN(playerid));
		SendClientMessage(id,COLOR_RED,string);
	}

    return 1;
}

CMD:slap(playerid, params[])
{
    if(PI[playerid][pAdmin] < ADM_MODER) return false;
    if(ADuty[playerid]!=228) return SendClientMessage(playerid,COLOR_GREY,NO_DUTY_TEXT);
    new id;
	if(sscanf(params,"u",id)) return SendClientMessage(playerid, COLOR_RED, "Используй: /slap [id]");
	if(!IsPlayerConnected(id)) return SendClientMessage(playerid, COLOR_RED, "Игрок не законектился еще!");
    new Float:X,Float:Y,Float:Z;
    GetPlayerPos(id,X,Y,Z);
    J_SetPlayerPos(id,X,Y,Z+5);
	new string[300];
    format(string,sizeof(string),"[A] Администратор %s[%d] дал поджопник %s[%d]",PN(playerid),playerid,PN(id),id);
	SendAdminsMessage(COLOR_GREY,string,1);
	aml(1, playerid, Name(id), "подкинул игрока",0 );
	return true;
}