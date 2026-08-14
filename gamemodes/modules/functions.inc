stock J_Kick(playerid)
{
    if(IsPlayerNPC(playerid)) return 0;
	if(PI[playerid][pAdmin] > 5) return 0;
	Kick(playerid);
	return 1;
}

stock GetString(const param1[],const param2[])
{
	return !strcmp(param1, param2, false);
}

stock DisableAllRaceCPs(playerid)
{
    for(new i = 1;i <= CountDynamicRaceCPs();i ++) TogglePlayerDynamicRaceCP(playerid, i, false);
	return true;
}

stock CheckPassword(const pass[])
{
	for(new i; i < strlen(pass); i ++)
	{
	    if( (pass[i] >= 'a' && pass[i] <= 'z') ||
		(pass[i] >= 'A' && pass[i] <= 'Z') ||
		(pass[i] >= 'а' && pass[i] <= 'я') ||
		(pass[i] >= 'А' && pass[i] <= 'Я') ||
		(pass[i] >= '0' && pass[i] <= '9')  ) continue;
		else return 0;
	}
	return 1;
}

stock PreloadAllAnimLibs(playerid)
{
    PreloadAnimLib(playerid,"AIRPORT");     
    PreloadAnimLib(playerid,"Attractors");  
    PreloadAnimLib(playerid,"BAR"); 
    PreloadAnimLib(playerid,"BASEBALL");    
    PreloadAnimLib(playerid,"BD_FIRE");     
    PreloadAnimLib(playerid,"BEACH");       
    PreloadAnimLib(playerid,"benchpress");  
    PreloadAnimLib(playerid,"BF_injection");
    PreloadAnimLib(playerid,"BIKED");       
    PreloadAnimLib(playerid,"BIKEH"); 
    PreloadAnimLib(playerid,"BIKELEAP");      
    PreloadAnimLib(playerid,"BIKES"); 
    PreloadAnimLib(playerid,"BIKEV"); 
    PreloadAnimLib(playerid,"BIKE_DBZ");      
    PreloadAnimLib(playerid,"BLOWJOBZ");      
    PreloadAnimLib(playerid,"BMX");   
    PreloadAnimLib(playerid,"BOMBER");
    PreloadAnimLib(playerid,"BOX");   
    PreloadAnimLib(playerid,"BSKTBALL");      
    PreloadAnimLib(playerid,"BUDDY"); 
    PreloadAnimLib(playerid,"BUS");   
    PreloadAnimLib(playerid,"CAMERA");
    PreloadAnimLib(playerid,"CAR");   
    PreloadAnimLib(playerid,"CARRY"); 
    PreloadAnimLib(playerid,"CAR_CHAT");      
    PreloadAnimLib(playerid,"CASINO");
    PreloadAnimLib(playerid,"CHAINSAW");      
    PreloadAnimLib(playerid,"CHOPPA");
    PreloadAnimLib(playerid,"CLOTHES");       
    PreloadAnimLib(playerid,"COACH"); 
    PreloadAnimLib(playerid,"COLT45");
    PreloadAnimLib(playerid,"COP_AMBIENT");   
    PreloadAnimLib(playerid,"COP_DVBYZ");     
    PreloadAnimLib(playerid,"CRACK"); 
    PreloadAnimLib(playerid,"CRIB");  
    PreloadAnimLib(playerid,"DAM_JUMP");      
    PreloadAnimLib(playerid,"DANCING");       
    PreloadAnimLib(playerid,"DEALER");
    PreloadAnimLib(playerid,"DILDO"); 
    PreloadAnimLib(playerid,"DODGE"); 
    PreloadAnimLib(playerid,"DOZER"); 
    PreloadAnimLib(playerid,"DRIVEBYS");      
    PreloadAnimLib(playerid,"FAT");   
    PreloadAnimLib(playerid,"FIGHT_B");       
    PreloadAnimLib(playerid,"FIGHT_C");       
    PreloadAnimLib(playerid,"FIGHT_D");       
    PreloadAnimLib(playerid,"FIGHT_E");       
    PreloadAnimLib(playerid,"FINALE");
    PreloadAnimLib(playerid,"FINALE2");       
    PreloadAnimLib(playerid,"FLAME"); 
    PreloadAnimLib(playerid,"Flowers");       
    PreloadAnimLib(playerid,"FOOD");  
    PreloadAnimLib(playerid,"Freeweights");   
    PreloadAnimLib(playerid,"GANGS"); 
    PreloadAnimLib(playerid,"GHANDS");
    PreloadAnimLib(playerid,"GHETTO_DB");     
    PreloadAnimLib(playerid,"goggles");       
    PreloadAnimLib(playerid,"GRAFFITI");      
    PreloadAnimLib(playerid,"GRAVEYARD");     
    PreloadAnimLib(playerid,"GRENADE");       
    PreloadAnimLib(playerid,"GYMNASIUM");     
    PreloadAnimLib(playerid,"HAIRCUTS");      
    PreloadAnimLib(playerid,"HEIST9");
    PreloadAnimLib(playerid,"INT_HOUSE");     
    PreloadAnimLib(playerid,"INT_OFFICE");    
    PreloadAnimLib(playerid,"INT_SHOP");      
    PreloadAnimLib(playerid,"JST_BUISNESS");  
    PreloadAnimLib(playerid,"KART");  
    PreloadAnimLib(playerid,"KISSING");       
    PreloadAnimLib(playerid,"KNIFE"); 
    PreloadAnimLib(playerid,"LAPDAN1");       
    PreloadAnimLib(playerid,"LAPDAN2");       
    PreloadAnimLib(playerid,"LAPDAN3");       
    PreloadAnimLib(playerid,"LOWRIDER");      
    PreloadAnimLib(playerid,"MD_CHASE");      
    PreloadAnimLib(playerid,"MD_END");
    PreloadAnimLib(playerid,"MEDIC"); 
    PreloadAnimLib(playerid,"MISC");  
    PreloadAnimLib(playerid,"MTB");   
    PreloadAnimLib(playerid,"MUSCULAR");      
    PreloadAnimLib(playerid,"NEVADA");
    PreloadAnimLib(playerid,"ON_LOOKERS");    
    PreloadAnimLib(playerid,"OTB");   
    PreloadAnimLib(playerid,"PARACHUTE");     
    PreloadAnimLib(playerid,"PARK");  
    PreloadAnimLib(playerid,"PAULNMAC");      
    PreloadAnimLib(playerid,"ped");   
    PreloadAnimLib(playerid,"PLAYER_DVBYS");  
    PreloadAnimLib(playerid,"PLAYIDLES");     
    PreloadAnimLib(playerid,"POLICE");
    PreloadAnimLib(playerid,"POOL");  
    PreloadAnimLib(playerid,"POOR");  
    PreloadAnimLib(playerid,"PYTHON");
    PreloadAnimLib(playerid,"QUAD");  
    PreloadAnimLib(playerid,"QUAD_DBZ");      
    PreloadAnimLib(playerid,"RAPPING");       
    PreloadAnimLib(playerid,"RIFLE"); 
    PreloadAnimLib(playerid,"RIOT");  
    PreloadAnimLib(playerid,"ROB_BANK");      
    PreloadAnimLib(playerid,"ROCKET");
    PreloadAnimLib(playerid,"RUSTLER");       
    PreloadAnimLib(playerid,"RYDER"); 
    PreloadAnimLib(playerid,"SCRATCHING");    
    PreloadAnimLib(playerid,"SHAMAL");
    PreloadAnimLib(playerid,"SHOP");  
    PreloadAnimLib(playerid,"SHOTGUN");       
    PreloadAnimLib(playerid,"SILENCED");      
    PreloadAnimLib(playerid,"SKATE"); 
    PreloadAnimLib(playerid,"SMOKING");       
    PreloadAnimLib(playerid,"SNIPER");
    PreloadAnimLib(playerid,"SPRAYCAN");      
    PreloadAnimLib(playerid,"STRIP"); 
    PreloadAnimLib(playerid,"SUNBATHE");      
    PreloadAnimLib(playerid,"SWAT");  
    PreloadAnimLib(playerid,"SWEET"); 
    PreloadAnimLib(playerid,"SWIM");  
    PreloadAnimLib(playerid,"SWORD"); 
    PreloadAnimLib(playerid,"TANK");  
    PreloadAnimLib(playerid,"TATTOOS");       
    PreloadAnimLib(playerid,"TEC");   
    PreloadAnimLib(playerid,"TRAIN"); 
    PreloadAnimLib(playerid,"TRUCK"); 
    PreloadAnimLib(playerid,"UZI");   
    PreloadAnimLib(playerid,"VAN");   
    PreloadAnimLib(playerid,"VENDING");       
    PreloadAnimLib(playerid,"VORTEX");
    PreloadAnimLib(playerid,"WAYFARER");      
    PreloadAnimLib(playerid,"WEAPONS");       
    return PreloadAnimLib(playerid,"WUZI");
}

stock PreloadAnimLib(playerid, const animlib[])
	return ApplyAnimation(playerid,animlib,"null",0.0,false,false,false,false,0);

stock SetPlayerMoney(playerid, money)
{
	ResetPlayerMoney(playerid);
	return GivePlayerMoney(playerid, PI[playerid][pMoney] = money);
}

forward J_SetPlayerPos(playerid, Float:X, Float:Y, Float:Z);
public J_SetPlayerPos(playerid, Float:X, Float:Y, Float:Z)
{
    pTemp[playerid][pAntiAir]= 8;

    pTemp[playerid][pPost][0] = floatround(Float:X);
	pTemp[playerid][pPost][1] = floatround(Float:Y);
	pTemp[playerid][pPost][2] = floatround(Float:Z);

    SetPlayerPos(playerid, Float:X,Float:Y,Float:Z);
    Streamer_Update(playerid);
	return true;
}

stock J_SetVehiclePos(vehicleid,Float:X,Float:Y,Float:Z)
{
	if(SetVehiclePos(vehicleid,X,Y,Z))
	{
	    foreach(new i : Player)
		{
		    if(!IsPlayerConnected(i)) continue;
		    if(IsPlayerInVehicle(i,vehicleid))
			{
				pTemp[i][pAntiAir] = 6;
				pTemp[i][pPost][0] = floatround(Float:X);
				pTemp[i][pPost][1] = floatround(Float:Y);
				pTemp[i][pPost][2] = floatround(Float:Z);
			}
		}
	    return true;
    }
    return false;
}

forward f_TogglePlayerControllable( playerid, bool:status);
public f_TogglePlayerControllable( playerid, bool:status)
{
	if(GetPVarInt(playerid, "sob_check") == 1)return 1;
	TogglePlayerControllable( playerid, status);
	return 1;
}

stock ClearChatToAll()
{
	for(new i; i < 20; i ++) SendClientMessageToAll(-1, " ");
	return true;
}


stock SetPlayerSkinEx(playerid,skin)
{
	SetPlayerSkin(playerid,skin);

 	SetPlayerChatBubble(playerid, "Переоделся", COLOR_GREEN, 10.0, 5000);
	return true;
}