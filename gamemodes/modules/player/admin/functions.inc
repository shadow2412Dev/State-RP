#include <hooks>

hook OnPlayerConnect(playerid)
{
	ADuty[playerid] = false;
	return 1;
}

hook OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	if(dialogid == DIALOG_ADMIN_AUTH)
	{
		if(response)
		{
			if(!strlen(inputtext)) return ShowPlayerAdminAuth(playerid);
			if(!GetString(inputtext, AdminPasswords[PI[playerid][pAdmin]])) return J_Kick(playerid);

			new Admin[32];
			switch(PI[playerid][pAdmin])
			{
				case ADM_MODER: Admin = "{6495ED}Младший Модератор";
				case ADM_SUPER_MODER: Admin = "{FFCC00}Модератор";
				case ADM_ADMIN: Admin = "{FFCC00}Старший Модератор";
				case ADM_SUPER_ADMIN: Admin = "{060ea1}Администратор";
				case ADM_VICE_GRAND_ADMIN: Admin = "{9a36b3}Заместитель ГА";
				case ADM_GRAND_ADMIN: Admin = "{009900}Главный Администратор";
				case ADM_GRAND: Admin = "{FF0000}Основатель";
			}

			format(String256,sizeof(String256),"[A] %s {FFFFFF}%s вошел в систему администратирования",Admin,PN(playerid));
			SendAdminsMessage(COLOR_WHITE, String256, 1);
			format(String256,sizeof(String256),"[A] Вы успешно авторизовались как %s",Admin);
			SendClientMessage(playerid,COLOR_WHITE,String256);

			ADuty[playerid] = true;
		}
	}
	else if(dialogid == DIALOG_APANEL)
	{
		if(response)
		{
			switch(listitem)
			{
				case 0: ShowAdminList(playerid);
				case 1: 
				{
					if(PI[playerid][pAdmin] < ADM_SUPER_MODER) return SendClientMessage(playerid, COLOR_GREY,"Ты не админ!");
					if(ADuty[playerid]!= true) return SendClientMessage(playerid,COLOR_GREY,NO_DUTY_TEXT);
					ClearChatToAll();
					format(String256,sizeof(String256),"%s Очистил Чат",PN(playerid));
					SendAdminsMessage(COLOR_LIGHTBLUE,String256);
				}
			}
		}
	}
	else if(dialogid == DIALOG_ADMIN_LIST)
	{
		if(response)
		{
			if(listitem == 15) pTemp[playerid][pDialogList] ++;
			else if(listitem == 16) pTemp[playerid][pDialogList] --;
			ShowAdminList(playerid, pTemp[playerid][pDialogList]);
		}
		else
		{
			callcmd::apanel(playerid);
		}
	}
	return HOOK_CONTINUE;
}

stock ShowAdminList(playerid, list = 0)
{
	f(String256, sizeof(String256), "SELECT `NickName`, `Admin`, `Online_status` FROM `accounts` WHERE `Admin` > 0 ORDER BY `Online_status` DESC LIMIT %i, %i", 15*list, 15*(list + 1));
	mysql_tquery(mysql, String256, "OnAdminList", "ii", playerid, list);
}

func OnAdminList(playerid, list)
{
	pTemp[playerid][pDialogList] = list;

	new rows;
	cache_get_row_count(rows);

	if(!rows) return ShowPlayerDialogEx(playerid, DIALOG_INVALID, DIALOG_STYLE_MSGBOX, "Список админов", "Список админов пуст", "Закрыть", "");

	global_str = "{CCCCCC}№ | Имя\t{CCCCCC}Уровень\t{CCCCCC}Статус\n";

	new index = 15*list + 1;

	new name[MAX_PLAYER_NAME], admin, online_status;
	for(new i = 0; i < rows; i++)
	{
		cache_get_value_name(i, "NickName", name);
		cache_get_value_name_int(i, "Admin", admin);
		cache_get_value_name_int(i, "Online_status", online_status);

		format(String256, sizeof(String256), "%i | %s\t[%i lvl]\t%s\n", index+i, name, admin, online_status == 1001 ? "{FF6347}Offline" : "{00FF00}Online");
		strcat(global_str, String256);
	}

	return ShowPlayerDialogEx(playerid, DIALOG_ADMIN_LIST, DIALOG_STYLE_TABLIST_HEADERS, "Список админов", global_str, "Выбор", "Назад");
}

stock ShowPlayerAdminAuth(playerid)
{
	ShowPlayerDialogEx(playerid, DIALOG_ADMIN_AUTH, DIALOG_STYLE_PASSWORD, "Авторизация", "{FFFFFF}Введите админ-пароль", "Ок", "Отмена");
	return 1;
}

stock SendAdminsMessage(color = -1,const string[] = "",level = 1,noplayerid = -1)
{
	foreach(new i : Player)
	{
	    if(i != noplayerid)
	    {
			if(IsPlayerConnected(i))
			{
				if (PI[i][pAdmin] >= level && ADuty[i]== true) SendClientMessage(i, color, string);
			}
		}
	}
	return true;
}