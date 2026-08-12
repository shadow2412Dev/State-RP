#include <hooks>

new PlayerDialog[MAX_PLAYERS] = {-1, ...};

hook OnPlayerConnect(playerid)
{
	PlayerDialog[playerid] = -1;
	return HOOK_CONTINUE;
}

hook OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	if(PlayerDialog[playerid] != dialogid)
	{
		PlayerDialog[playerid] = -1;
		return 0;
	}

	PlayerDialog[playerid] = -1;

	for(new i; i<strlen(inputtext); i++)
	{
	    switch(inputtext[i])
	    {
	        case 'A'..'Z', 'a'..'z', 'À'..'ß', 'à'..'ÿ', '0'..'9', ' ', '>', '<', '(', ')', '/', '+', '-','_', '?', '!', '.', ',', '@':continue;
	        default: inputtext[i]=' ';
	    }
	}
	if(strlen(inputtext)>0) mysql_escape_string(inputtext, inputtext, 256);

	PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);

	return 1;
}

stock ShowPlayerDialogEx(playerid, dialogid, DIALOG_STYLE:style, const caption[], const info[], const button1[], const button2[])
{
	static str[100];
	str = "{BFBBBA}";
	strcat(str,caption);
	ShowPlayerDialog(playerid, dialogid, style, str, info, button1, button2);
	PlayerDialog[playerid] = dialogid;
	return 1;
}