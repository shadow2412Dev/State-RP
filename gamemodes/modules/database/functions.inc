#include <hooks>

hook OnGameModeInit()
{
	SetGameModeText("Loading...");

	new GetFile = ini_openFile("config.ini");
	ini_getString(GetFile, "host", MYSQL_HOST);
	ini_getString(GetFile, "user", MYSQL_USER);
	ini_getString(GetFile, "password", MYSQL_PASS);
	ini_getString(GetFile, "database", MYSQL_DB);
	ini_getString(GetFile, "server_name", ModeCity);
	ini_getInteger(GetFile, "is_test_server", isTestServer);
	ini_closeFile(GetFile);

	mysql_global_options(DUPLICATE_CONNECTIONS, true);
	mysql_log(ERROR | WARNING);

	mysql = mysql_connect(MYSQL_HOST, MYSQL_USER, MYSQL_PASS, MYSQL_DB);
	mysql_set_charset("cp1251", mysql);
	
	ManualVehicleEngineAndLights();
   	EnableStuntBonusForAll(false);
	DisableInteriorEnterExits();
	LimitPlayerMarkerRadius(70.0);
	SetNameTagDrawDistance(25.0);
   	ShowPlayerMarkers(PLAYER_MARKERS_MODE:2);

	if(isTestServer)
	{
		format(global_str, sizeof(global_str), "name State RP | Test (%s)", BRANCH_NAME);
		SendRconCommand(global_str);
	}
	else
	{
		format(global_str, sizeof(global_str), "name State Role Play | %s", ModeCity);
		SendRconCommand(global_str);
	}

	gUnixTime = gettime();
	SetTimer("OnSecondTimer", 1000, true);

	SetWeather(GlobalWeather);
	SetGameModeText(ModeText);

	mysql_tquery(mysql, "UPDATE `accounts` SET `Online_status` = 1001` WHERE `Online_status` != 1001");
	return HOOK_CONTINUE;
}