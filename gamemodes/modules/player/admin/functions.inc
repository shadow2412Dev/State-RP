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
				case ADM_MODER: Admin = "{FFCC00}Младший Модератор";
				case ADM_SUPER_MODER: Admin = "{FFCC00}Модератор";
				case ADM_ADMIN: Admin = "{FFCC00}Старший Модератор";
				case ADM_SUPER_ADMIN: Admin = "{009900}Администратор";
				case ADM_VICE_GRAND_ADMIN: Admin = "{009900}Заместитель ГА";
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
	return HOOK_CONTINUE;
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