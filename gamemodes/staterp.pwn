#include <open.mp>
#include <a_mysql>
#include <streamer>
#include <Pawn.CMD>
#include <mxINI>
#include <foreach>
#include <sscanf2>

#define MAX_PLAYERS 1000
#define MAX_PLAYER_NAME 24

main()
{
    print("---------- State Role Play loaded ----------");
}

#include "../gamemodes/modules/connector.inc"

public OnGameModeInit()
{
    return 1;
}

public OnPlayerConnect(playerid)
{
	return 1;
}

public OnPlayerDisconnect(playerid, reason)
{
	return 1;
}

func OnSecondTimer()
{
	gUnixTime++;
	return 1;
}

public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
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
	        case '%', '~', '\'', '`': inputtext[i]=' ';
	    }
	}
	if(strlen(inputtext)>0) mysql_escape_string(inputtext, inputtext, 256);

	PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
	return 1;
}