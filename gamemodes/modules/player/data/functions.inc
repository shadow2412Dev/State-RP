#include <hooks>

hook OnPlayerConnect(playerid)
{
	for(new i; i < 100; i ++) SCM(playerid, -1, !" ");
	SCM(playerid, COLOR_RED, !"Добро пожаловать на State Role Play!");
	PlayerPlaySound(playerid, 1062, 0.0, 0.0, 0.0);

	ClearPlayerInfo(playerid);
	GetPlayerName(playerid, PI[playerid][pNickName], MAX_PLAYER_NAME);
	GetPlayerIp(playerid, PI[playerid][pLastIP], 16);
	SetPlayerVirtualWorld(playerid, playerid+1);
	SetPlayerWeather(playerid, GlobalWeather);

	mysql_escape_string(PI[playerid][pNickName], PI[playerid][pNickName], MAX_PLAYER_NAME);

	mysql_format(mysql, global_str, 150, "SELECT `ID`, `Password` FROM accounts WHERE NickName = '%e' LIMIT 1", PI[playerid][pNickName]);
    mysql_tquery(mysql, global_str, "OnPlayerConnected", "i", playerid);

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
		ShowPlayerRegisterDialog(playerid);
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

	ShowPlayerLoginDialog(playerid);
	return 1;
}

stock ShowPlayerLoginDialog(playerid)
{
	DisableAllRaceCPs(playerid);
	
	f(String256, sizeof(String256), "\
		{FFFFFF}Добро пожаловать на {EBA225}State Role Play{FFFFFF}\n\n\
		Введите свой пароль\n\
		{FFFFFF}Попыток для ввода пароля: {28910B}%d", PI[playerid][pPasswordAttempts]);

	ShowPlayerDialogEx(playerid, DIALOG_LOGIN, DIALOG_STYLE_PASSWORD, "Авторизация", 
		String256, "Принять", "");

	return 1;
}

stock ShowPlayerRegisterDialog(playerid)
{
	switch(PI[playerid][pRegisterState])
	{
		case 0: ShowPlayerGrandRegisterDialog(playerid);
		case 1:
		{
			ShowPlayerDialogEx(playerid, DIALOG_REGISTER, DIALOG_STYLE_LIST, "{E88813}[2/4] Выберите Ваш пол", "Мужской\nЖенский", "Принять", "Выход");
		}
	}
}

stock ShowPlayerGrandRegisterDialog(playerid)
{
	f(String512, sizeof(String512), "\
		{FFFFFF}Добро пожаловать, {205CA2}%s{FFFFFF}\n\n\
		Этот аккаунт {FF6347}не зарегистрирован {FFFFFF}на нашем сервере.\n\
		Для регистрации введите пароль.\n\
		Он будет использоваться для авторизации на нашем сервере.\n\n\
		{BF2F2F}\tПримечания:\n\
		\t- Длина пароля от 6 до 30 символов\n\
		\t- Пароль должен состоять из букв и цифр\n\
		\t- Пароль чувствителен к регистру\n", PN(playerid));

	ShowPlayerDialogEx(playerid, DIALOG_REGISTER, DIALOG_STYLE_INPUT, "{E88813}[1/4] Пароль", String512, "Принять", "Выход");
	return 1;
}

hook OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	if(dialogid == DIALOG_REGISTER)
	{
		switch(PI[playerid][pRegisterState])
		{
			case 0:
			{
				if(strlen(inputtext) < 6) return ShowPlayerGrandRegisterDialog(playerid);
				else if(!CheckPassword(inputtext)) return ShowPlayerGrandRegisterDialog(playerid);

				PI[playerid][pRegisterState]++;
				f(PI[playerid][pPassword], 64, "%s", MD5_Hash(inputtext));
				ShowPlayerRegisterDialog(playerid);
			}
			case 1:
			{
				PI[playerid][pSex] = listitem;

				f(String512, sizeof(String512), "SELECT `ID` FROM `accounts` WHERE `NickName` = BINARY('%s') LIMIT 1", PN(playerid));
				mysql_pquery(mysql, String512, "OnPlayerRegister", "i", playerid);
			}
		}
	}
	else if(dialogid == DIALOG_LOGIN)
	{
		if(!strlen(inputtext)) return ShowPlayerLoginDialog(playerid);
		if(!GetString(PI[playerid][pPassword], MD5_Hash(inputtext)))
		{
			PI[playerid][pPasswordAttempts]--;
			if(PI[playerid][pPasswordAttempts] <= 0)
			{
				J_Kick(playerid);
				return 1;
			}
			return ShowPlayerLoginDialog(playerid);
		}
		else
		{
			LoginPlayer(playerid);
		}
	}

	return HOOK_CONTINUE;
}

stock LoginPlayer(playerid)
{
	format(String256, sizeof(String256), "SELECT * FROM `Accounts` WHERE `ID` = %i LIMIT 1", PI[playerid][pID]);
	mysql_pquery(mysql, String256, "OnPlayerLogin", "ii", playerid, PI[playerid][pID]);
	return 1;
}

func OnPlayerLogin(playerid, acc_id)
{
	if(PI[playerid][pID] != acc_id) return J_Kick(playerid);

	new rows;
	cache_get_row_count(rows);

	if(!rows) return J_Kick(playerid);


	cache_get_value_name(0, "RegIP", PI[playerid][pRegIP], 16);
	cache_get_value_name(0, "OldIP", PI[playerid][pIP], 16);
	cache_get_value_name(0, "Referal", PI[playerid][pReferal], 24);
	cache_get_value_name(0, "Mail", PI[playerid][pMail], 50);
	cache_get_value_name_int(0, "Level", PI[playerid][pLevel]);
	cache_get_value_name_int(0, "Exp", PI[playerid][pExp]);
	cache_get_value_name_int(0, "Sex", PI[playerid][pSex]);
	cache_get_value_name_int(0, "Admin", PI[playerid][pAdmin]);
	cache_get_value_name_int(0, "Money", PI[playerid][pMoney]);
	cache_get_value_name_int(0, "Bank", PI[playerid][pBank]);
	cache_get_value_name_int(0, "Skin", PI[playerid][pSkin]);

	f(global_str, 128, "~n~~n~~n~~n~~n~~n~~w~Welcome~n~~b~%s", PN(playerid));
	GameTextForPlayer(playerid, global_str, 5000, 6);

	SendHelp(playerid, "На сервере есть инвентарь, используйте клавишу Y для работы с ним.");
 	SendHelp(playerid, "Вы можете задать вопрос в нашу техническую поддержку /report.");

	PreloadAllAnimLibs(playerid);
	return PlayerSpawn(playerid);
}

func OnPlayerRegister(playerid)
{
	new rows;
	cache_get_row_count(rows);

	if(rows) return J_Kick(playerid);

	f(String512, sizeof(String512), "INSERT INTO `accounts` \
		(`NickName`, `Password`, `RegIP`, `OldIP`, `Referal`, `Sex`) VALUES \
		('%s', '%s', '%s', '%s', '%s', %i)", 
		PN(playerid), PI[playerid][pPassword], PI[playerid][pLastIP], PI[playerid][pLastIP], PI[playerid][pReferal], PI[playerid][pSex]);
	mysql_tquery(mysql, String512, "OnPlayerRegisterMySQL", "i", playerid);
	return 1;
}

func OnPlayerRegisterMySQL(playerid)
{
	PI[playerid][pID] = cache_insert_id();

	SCM(playerid, COLOR_STATE, !"[Подсказка] {FFFFFF}Благодарим вас за регистрацию на нашем сервере");
	SCM(playerid, COLOR_STATE, !"[Подсказка] {FFFFFF}Сейчас вам желательно добраться до мэрии и получить паспорт");

	foreach(Player, i) {
		
		if(PI[i][pAdmin]) 
			SCMF(i, -1, "Приветствуем нового игрока нашего сервера: {FF9900}%s {FFFFFF}(ID: %i) {cccccc}IP: %s",PN(playerid), playerid, PI[i][pIP]);
				
	}

	LoginPlayer(playerid);
	return 1;
}

func PlayerSpawn(playerid)
{
	if (IsPlayerNPC(playerid)) return 1;
    if (IsPlayerInAnyVehicle(playerid))
	{
	    new Float:X, Float:Y, Float:Z;
	    GetPlayerPos(playerid, X, Y, Z);
		SetPlayerPos(playerid, X ,Y, Z);
	}

 	SettingSpawn(playerid);
	/* if pTemp[playerid][SPECTPLAYER] == true *then SpecPl(playerid, false);
	else  */
	SpawnPlayer(playerid);
	return 1;
}

stock SettingSpawn(playerid)
{
    if (IsPlayerNPC(playerid)) return 1;
	new skin = GetSkinOfPlayer(playerid);
	SetSpawnInfoEx(playerid, skin, 167.5974,-109.2371,1.5501,272.6516);
	SetPlayerInterior(playerid, 0);
	SetPlayerVirtualWorld(playerid, 0);
	return 1;
}

stock GetSkinOfPlayer(playerid)
{
	new skin = PI[playerid][pSkin];
	return skin;
}

stock SetSpawnInfoEx(playerid, skin, Float:x, Float:y, Float:z, Float:a)
{
    return SetSpawnInfo(playerid, 255, skin, x, y, z-0.4, a, WEAPON:0, 0, WEAPON:0, 0, WEAPON:0, 0);
}