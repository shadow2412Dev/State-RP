#include <open.mp>
#include <a_mysql>
#include <streamer>
#include <Pawn.CMD>
#include <mxINI>
#include <foreach>
#include <sscanf2>
#include <MD5>

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

public OnPlayerCommandReceived(playerid, cmd[], params[], flags)
{
	return true;
}

public OnPlayerSpawn(playerid)
{
	SetPlayerMoney(playerid, PI[playerid][pMoney]);
	return 1;
}

public OnPlayerClickMap(playerid, Float:fX, Float:fY, Float:fZ)
{
	if(PI[playerid][pAdmin] > ADM_NONE)
 	{
  		if(GetPlayerState(playerid) == PLAYER_STATE_DRIVER)
  		{
	   		new tmpcar = GetPlayerVehicleID(playerid);
	   		J_SetVehiclePos(tmpcar, fX, fY, fZ+2);
  		}
  		else J_SetPlayerPos(playerid, fX, fY, fZ+1);
  		SetPlayerVirtualWorld(playerid,0);
  		SetPlayerInterior(playerid,0);
 	}
	return 1;
}