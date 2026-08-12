#include <open.mp>
#include <a_mysql>
#include <streamer>
#include <Pawn.CMD>
#include <mxINI>

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