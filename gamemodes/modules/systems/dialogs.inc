#include <hooks>

new PlayerDialog[MAX_PLAYERS] = {-1, ...};

enum
{
	DIALOG_NONE = -1,
	DIALOG_INVALID = 0,
	DIALOG_REGISTER = 1,
	DIALOG_LOGIN = 2,
	DIALOG_ADMIN_AUTH = 100,
	DIALOG_APANEL = 101,
	DIALOG_ADMIN_LIST = 102
}

hook OnPlayerConnect(playerid)
{
	PlayerDialog[playerid] = -1;
	return HOOK_CONTINUE;
}

stock ShowPlayerDialogEx(playerid, dialogid, DIALOG_STYLE:style, const caption[], const info[], const button1[], const button2[])
{
	static str[100];
	str = "{FF9900}";
	strcat(str,caption);
	ShowPlayerDialog(playerid, dialogid, style, str, info, button1, button2);

	if(PlayerDialog[playerid] == DIALOG_NONE) PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
	PlayerDialog[playerid] = dialogid;
	return 1;
}