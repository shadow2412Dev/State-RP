new str_debug[256];
#define Debug(%1,%2) format(str_debug,144,%2), strins(str_debug, "[DEBUG] ", 0), SendDebugMessage(%1,str_debug)

stock SendDebugMessage(playerid, const message[])
{
    if(isTestServer)
    {
        if(playerid != -1)
        {
            SendClientMessage(playerid, COLOR_RED, message);
        }
        else if(playerid == -1)
        {
            foreach(new i : Player)
            {

                SendClientMessage(i, COLOR_RED, message);
            }
        }
    }
    return true;
}

public OnQueryError(errorid, const error[], const callback[], const query[], MySQL:handle)
{
	Debug(-1, "Внимание! Возникла ошибка в работе с базой данных (0x%04x). Сообщите разработчикам.", errorid);

    return 1;
}