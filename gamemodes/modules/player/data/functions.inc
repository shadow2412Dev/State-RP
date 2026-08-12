#include <hooks>

hook OnPlayerConnect(playerid)
{
	for(new i; i < 100; i ++) SCM(playerid, -1, !" ");
	SCM(playerid, COLOR_RED, !"Добро пожаловать на State Role Play!");
	PlayerPlaySound(playerid, 1062, 0.0, 0.0, 0.0);

	ClearPlayerInfo(playerid);
	GetPlayerName(playerid, PI[playerid][pNickName], MAX_PLAYER_NAME);
	GetPlayerIp(playerid, PI[playerid][pIP], 16);
	SetPlayerVirtualWorld(playerid, playerid+1);
	SetPlayerWeather(playerid, GlobalWeather);

	mysql_escape_string(PI[playerid][pNickName], PI[playerid][pNickName], MAX_PLAYER_NAME);

	mysql_format(mysql, global_str, 150, "SELECT `ID`, `Password` FROM accounts WHERE NickName = '%e' LIMIT 1", PI[playerid][pNickName]);
    mysql_tquery(mysql, global_str, "OnPlayerConnected", "i", playerid);

	PI[playerid][pAdmin] = 7;
	return HOOK_CONTINUE;
}

stock ClearPlayerInfo(playerid)
{
	PI[playerid] = PI_EOS;
	return 1;
}

func OnPlayerConnected(playerid)
{
	new rows;
	cache_get_row_count(rows);

	if(!rows)
	{

	}
	else
	{
		cache_get_value_name_int(0, "ID", PI[playerid][pID]);
		cache_get_value_name(0, "Password", PI[playerid][pPassword], 64);

		new string[256];
		format(string, sizeof(string), "SELECT * FROM `BanNames` WHERE BINARY `Name` = '%s' LIMIT 1", PN(playerid));
		mysql_pquery(mysql, string, "OnPlayerCheckBanned", "ii", playerid, PI[playerid][pID]);
	}
}

func OnPlayerCheckBanned(playerid, acc_id)
{
	if(PI[playerid][pID] != acc_id)
	{
		J_Kick(playerid);
		return 1;
	}

	new rows;
	cache_get_row_count(rows);

	if(rows)
	{
		new NickName[MAX_PLAYER_NAME], Admin[MAX_PLAYER_NAME], reason[100], seconds, date[64], unban_date[64];

		cache_get_value_name(0, "Name", NickName, MAX_PLAYER_NAME);
		cache_get_value_name(0, "BanAdmin", Admin, MAX_PLAYER_NAME);
		cache_get_value_name(0, "BanReason", reason, 100);
		cache_get_value_name_int(0, "BanTime", seconds);
		cache_get_value_name(0, "BanDate", date, 64);

		new tima[20], dima;
		new diff = seconds - gUnixTime;

		if (diff >= 86400)
		{
			tima = "дня(ей)";
			dima = floatround(float(diff) / 86400.0, floatround_ceil);
		}
		else
		{
			tima = "час(ов)";
			dima = floatround(float(diff) / 3600.0, floatround_ceil);
		}

		if(gUnixTime<seconds)
		{
			FormatUnixTime(seconds, unban_date);

			format(String512,sizeof(String512),"\
			{FFFFFF}Ваш игровой аккаунт: {FF0000}%s {FFFFFF}заблокирован!\n\n\
			Администратор: {CCCCCC}%s\n\
			{FFFFFF}Причина: {CCCCCC}%s\n\
			{FFFFFF}Дата блокировки: {CCCCCC}%s\n\
			{FFFFFF}Дата разблокировки: {CCCCCC}%s\n\
			{FFFFFF}До разблокировки еще {CCCCCC}%i %s.\n\n\n\
			{FFFFFF}Если вы не согласны с блокировкой аккаунта,\n\
			вы можете подать заявку на разблокировку на форуме: %s",
			NickName, Admin, reason, date, unban_date, dima,tima, ModeSite);
			ShowPlayerDialogEx(playerid,0,DIALOG_STYLE_MSGBOX,"Этот аккаунт заблокирован!",String512,"Ок","");
			J_Kick(playerid);
			return true;
		}else{
			format(String256, sizeof(String256), "DELETE FROM `BanNames` WHERE `Name` = '%s' LIMIT 1", PN(playerid));
			mysql_pquery(mysql, String256);
		}
	}

	return 1;
}