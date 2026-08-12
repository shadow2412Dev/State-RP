#if !defined _hooks_inc
	#define _hooks_inc

/*
	���� y_hooks

	�������������:
	    #include <hooks> // ���������� ������ ���, ���������� ������ ���� ����� ���� ������ ������� ��� ��� � ���� �� �����

	    // ������
	    #include <hooks>
	    hook OnGameModeInit()
	    {
	        return HOOK_CONTINUE;
	    }

	������ HOOK_CONTINUE ���� ���������� ������ ���� �� ����� �� ���� ����
	������ HOOK_STOP_0 ��� HOOK_STOP_1 ���� ���������� ���������� ����������� ����� � ������� 0/1
*/

	#if !defined _samp_included
		#error "Please include a_samp before hooks"
	#endif

	#if !defined HOOKS_MAX_SLOTS
		#define HOOKS_MAX_SLOTS (256)
	#endif

	#if HOOKS_MAX_SLOTS > 256
		#error "HOOKS_MAX_SLOTS macro table supports max 256 slots"
	#endif

	#if !defined HOOKS_MAX_CALLBACKS
		#define HOOKS_MAX_CALLBACKS (64)
	#endif

	#if !defined HOOKS_NAME_SIZE
		#define HOOKS_NAME_SIZE (64)
	#endif

	#define HOOK_CONTINUE (1)
	#define HOOK_STOP_0  (-1)
	#define HOOK_STOP_1  (-2)

	// ��� ����� �������� ���� ��� �� callback � ������ ������ ��������� public-�������.
	static bool:s_HooksCacheLoaded[HOOKS_MAX_CALLBACKS];
	static bool:s_HooksChainLoaded[HOOKS_MAX_CALLBACKS];
	static bool:s_HooksChainExists[HOOKS_MAX_CALLBACKS];
	static s_HooksCacheCount[HOOKS_MAX_CALLBACKS];
	static s_HooksCacheSize;
	static s_HooksCacheLastId = -1;
	static s_HooksCacheCallback[HOOKS_MAX_CALLBACKS][HOOKS_NAME_SIZE];
	static s_HooksCacheName[HOOKS_MAX_CALLBACKS][HOOKS_MAX_SLOTS][HOOKS_NAME_SIZE];
	static s_HooksChainName[HOOKS_MAX_CALLBACKS][HOOKS_NAME_SIZE];

	stock Hooks_GetReturn(value, default_value)
	{
		if (value == HOOK_STOP_0) {
            return 0;
        }

		if (value == HOOK_STOP_1) {
			return 1;
        }
		return default_value;
	}

	stock Hooks_GetCacheId(const callback[])
	{
		if (s_HooksCacheLastId != -1 && strcmp(s_HooksCacheCallback[s_HooksCacheLastId], callback) == 0) {
			return s_HooksCacheLastId;
		}

		for (new i; i < s_HooksCacheSize; i++) {
			if (strcmp(s_HooksCacheCallback[i], callback) != 0) {
				continue;
			}

			s_HooksCacheLastId = i;
			return i;
		}

		if (s_HooksCacheSize >= HOOKS_MAX_CALLBACKS) {
			return -1;
		}

		new cache_id = s_HooksCacheSize;
		s_HooksCacheSize++;

		format(s_HooksCacheCallback[cache_id], HOOKS_NAME_SIZE, "%s", callback);

		s_HooksCacheCount[cache_id] = 0;
		s_HooksCacheLoaded[cache_id] = false;
		s_HooksChainLoaded[cache_id] = false;
		s_HooksChainExists[cache_id] = false;
		s_HooksCacheLastId = cache_id;
		return cache_id;
	}

	stock Hooks_BuildCache(cache_id, const callback[])
	{
		if (cache_id == -1 || s_HooksCacheLoaded[cache_id]) {
			return 0;
		}

		new name[HOOKS_NAME_SIZE], hooks_count;

		for (new i; i < HOOKS_MAX_SLOTS; i++) {
			format(name, sizeof name, "%s@%03d", callback, i);

			if (funcidx(name) == -1) {
				continue;
            }

			format(s_HooksCacheName[cache_id][hooks_count], HOOKS_NAME_SIZE, "%s", name);
			hooks_count++;
		}

		s_HooksCacheCount[cache_id] = hooks_count;
		s_HooksCacheLoaded[cache_id] = true;
		return hooks_count;
	}

	stock bool:Hooks_HasChain(cache_id, const chain[])
	{
		if (cache_id == -1 || chain[0] == EOS) {
			return false;
		}

		if (!s_HooksChainLoaded[cache_id]) {
			format(s_HooksChainName[cache_id], HOOKS_NAME_SIZE, "%s", chain);
			s_HooksChainExists[cache_id] = (funcidx(chain) != -1);
			s_HooksChainLoaded[cache_id] = true;
		}
		return s_HooksChainExists[cache_id];
	}

	stock Hooks_Call0ById(cache_id, const callback[], const spec[], default_value)
	{
		new value;

		if (cache_id != -1) {
			Hooks_BuildCache(cache_id, callback);

			for (new i; i < s_HooksCacheCount[cache_id]; i++) {
				value = CallLocalFunction(s_HooksCacheName[cache_id][i], spec);

				if (value == HOOK_STOP_0 || value == HOOK_STOP_1) {
					return Hooks_GetReturn(value, default_value);
                }
			}
			return default_value;
		}

		new name[HOOKS_NAME_SIZE];

		for (new i; i < HOOKS_MAX_SLOTS; i++) {
			format(name, sizeof name, "%s@%03d", callback, i);

			if (funcidx(name) == -1) {
				continue;
            }

			value = CallLocalFunction(name, spec);

			if (value == HOOK_STOP_0 || value == HOOK_STOP_1) {
				return Hooks_GetReturn(value, default_value);
            }
		}
		return default_value;
	}

	stock Hooks_Call1ById(cache_id, const callback[], const spec[], default_value, a1)
	{
		new value;

		if (cache_id != -1) {
			Hooks_BuildCache(cache_id, callback);

			for (new i; i < s_HooksCacheCount[cache_id]; i++) {
				value = CallLocalFunction(s_HooksCacheName[cache_id][i], spec, a1);

				if (value == HOOK_STOP_0 || value == HOOK_STOP_1) {
					return Hooks_GetReturn(value, default_value);
                }
			}
			return default_value;
		}

		new name[HOOKS_NAME_SIZE];

		for (new i; i < HOOKS_MAX_SLOTS; i++) {
			format(name, sizeof name, "%s@%03d", callback, i);

			if (funcidx(name) == -1) {
				continue;
            }

			value = CallLocalFunction(name, spec, a1);

			if (value == HOOK_STOP_0 || value == HOOK_STOP_1) {
				return Hooks_GetReturn(value, default_value);
            }
		}
		return default_value;
	}

	stock Hooks_Call2ById(cache_id, const callback[], const spec[], default_value, a1, a2)
	{
		new value;

		if (cache_id != -1) {
			Hooks_BuildCache(cache_id, callback);

			for (new i; i < s_HooksCacheCount[cache_id]; i++) {
				value = CallLocalFunction(s_HooksCacheName[cache_id][i], spec, a1, a2);

				if (value == HOOK_STOP_0 || value == HOOK_STOP_1) {
					return Hooks_GetReturn(value, default_value);
                }
			}
			return default_value;
		}

		new name[HOOKS_NAME_SIZE];

		for (new i; i < HOOKS_MAX_SLOTS; i++) {
			format(name, sizeof name, "%s@%03d", callback, i);

			if (funcidx(name) == -1) {
				continue;
            }

			value = CallLocalFunction(name, spec, a1, a2);

			if (value == HOOK_STOP_0 || value == HOOK_STOP_1) {
				return Hooks_GetReturn(value, default_value);
            }
		}
		return default_value;
	}

	stock Hooks_Call3ById(cache_id, const callback[], const spec[], default_value, a1, a2, a3)
	{
		new value;

		if (cache_id != -1) {
			Hooks_BuildCache(cache_id, callback);

			for (new i; i < s_HooksCacheCount[cache_id]; i++) {
				value = CallLocalFunction(s_HooksCacheName[cache_id][i], spec, a1, a2, a3);

				if (value == HOOK_STOP_0 || value == HOOK_STOP_1) {
					return Hooks_GetReturn(value, default_value);
                }
			}
			return default_value;
		}

		new name[HOOKS_NAME_SIZE];

		for (new i; i < HOOKS_MAX_SLOTS; i++) {
			format(name, sizeof name, "%s@%03d", callback, i);

			if (funcidx(name) == -1) {
				continue;
            }

			value = CallLocalFunction(name, spec, a1, a2, a3);

			if (value == HOOK_STOP_0 || value == HOOK_STOP_1) {
				return Hooks_GetReturn(value, default_value);
            }
		}
		return default_value;
	}

	stock Hooks_Call2SById(cache_id, const callback[], const spec[], default_value, a1, a2, const a3[])
	{
		new value;

		if (cache_id != -1) {
			Hooks_BuildCache(cache_id, callback);

			for (new i; i < s_HooksCacheCount[cache_id]; i++) {
				value = CallLocalFunction(s_HooksCacheName[cache_id][i], spec, a1, a2, a3);

				if (value == HOOK_STOP_0 || value == HOOK_STOP_1) {
					return Hooks_GetReturn(value, default_value);
                }
			}
			return default_value;
		}

		new name[HOOKS_NAME_SIZE];

		for (new i; i < HOOKS_MAX_SLOTS; i++) {
			format(name, sizeof name, "%s@%03d", callback, i);

			if (funcidx(name) == -1) {
				continue;
            }

			value = CallLocalFunction(name, spec, a1, a2, a3);

			if (value == HOOK_STOP_0 || value == HOOK_STOP_1) {
				return Hooks_GetReturn(value, default_value);
            }
		}
		return default_value;
	}

	stock Hooks_Call4ById(cache_id, const callback[], const spec[], default_value, a1, a2, a3, a4)
	{
		new value;

		if (cache_id != -1) {
			Hooks_BuildCache(cache_id, callback);

			for (new i; i < s_HooksCacheCount[cache_id]; i++) {
				value = CallLocalFunction(s_HooksCacheName[cache_id][i], spec, a1, a2, a3, a4);

				if (value == HOOK_STOP_0 || value == HOOK_STOP_1) {
					return Hooks_GetReturn(value, default_value);
                }
			}
			return default_value;
		}

		new name[HOOKS_NAME_SIZE];

		for (new i; i < HOOKS_MAX_SLOTS; i++) {
			format(name, sizeof name, "%s@%03d", callback, i);

			if (funcidx(name) == -1) {
				continue;
            }

			value = CallLocalFunction(name, spec, a1, a2, a3, a4);

			if (value == HOOK_STOP_0 || value == HOOK_STOP_1) {
				return Hooks_GetReturn(value, default_value);
            }
		}
		return default_value;
	}

	stock Hooks_Call5ById(cache_id, const callback[], const spec[], default_value, a1, a2, a3, a4, a5)
	{
		new value;

		if (cache_id != -1) {
			Hooks_BuildCache(cache_id, callback);

			for (new i; i < s_HooksCacheCount[cache_id]; i++) {
				value = CallLocalFunction(s_HooksCacheName[cache_id][i], spec, a1, a2, a3, a4, a5);

				if (value == HOOK_STOP_0 || value == HOOK_STOP_1) {
					return Hooks_GetReturn(value, default_value);
                }
			}
			return default_value;
		}

		new name[HOOKS_NAME_SIZE];

		for (new i; i < HOOKS_MAX_SLOTS; i++) {
			format(name, sizeof name, "%s@%03d", callback, i);

			if (funcidx(name) == -1) {
				continue;
            }

			value = CallLocalFunction(name, spec, a1, a2, a3, a4, a5);

			if (value == HOOK_STOP_0 || value == HOOK_STOP_1) {
				return Hooks_GetReturn(value, default_value);
            }
		}
		return default_value;
	}

	stock Hooks_CallDialogById(cache_id, const callback[], const spec[], default_value, a1, a2, a3, a4, const a5[])
	{
		new value;

		if (cache_id != -1) {
			Hooks_BuildCache(cache_id, callback);

			for (new i; i < s_HooksCacheCount[cache_id]; i++) {
				value = CallLocalFunction(s_HooksCacheName[cache_id][i], spec, a1, a2, a3, a4, a5);

				if (value == HOOK_STOP_0 || value == HOOK_STOP_1) {
					return Hooks_GetReturn(value, default_value);
                }
			}
			return default_value;
		}

		new name[HOOKS_NAME_SIZE];

		for (new i; i < HOOKS_MAX_SLOTS; i++) {
			format(name, sizeof name, "%s@%03d", callback, i);

			if (funcidx(name) == -1) {
				continue;
            }

			value = CallLocalFunction(name, spec, a1, a2, a3, a4, a5);

			if (value == HOOK_STOP_0 || value == HOOK_STOP_1) {
				return Hooks_GetReturn(value, default_value);
            }
		}
		return default_value;
	}

	stock Hooks_Call0(const callback[], const spec[], default_value)
	{
		new cache_id = Hooks_GetCacheId(callback);
		return Hooks_Call0ById(cache_id, callback, spec, default_value);
	}

	stock Hooks_Call1(const callback[], const spec[], default_value, a1)
	{
		new cache_id = Hooks_GetCacheId(callback);
		return Hooks_Call1ById(cache_id, callback, spec, default_value, a1);
	}

	stock Hooks_Call2(const callback[], const spec[], default_value, a1, a2)
	{
		new cache_id = Hooks_GetCacheId(callback);
		return Hooks_Call2ById(cache_id, callback, spec, default_value, a1, a2);
	}

	stock Hooks_Call3(const callback[], const spec[], default_value, a1, a2, a3)
	{
		new cache_id = Hooks_GetCacheId(callback);
		return Hooks_Call3ById(cache_id, callback, spec, default_value, a1, a2, a3);
	}

	stock Hooks_Call2S(const callback[], const spec[], default_value, a1, a2, const a3[])
	{
		new cache_id = Hooks_GetCacheId(callback);
		return Hooks_Call2SById(cache_id, callback, spec, default_value, a1, a2, a3);
	}

	stock Hooks_Call4(const callback[], const spec[], default_value, a1, a2, a3, a4)
	{
		new cache_id = Hooks_GetCacheId(callback);
		return Hooks_Call4ById(cache_id, callback, spec, default_value, a1, a2, a3, a4);
	}

	stock Hooks_Call5(const callback[], const spec[], default_value, a1, a2, a3, a4, a5)
	{
		new cache_id = Hooks_GetCacheId(callback);
		return Hooks_Call5ById(cache_id, callback, spec, default_value, a1, a2, a3, a4, a5);
	}

	stock Hooks_CallDialog(const callback[], const spec[], default_value, a1, a2, a3, a4, const a5[])
	{
		new cache_id = Hooks_GetCacheId(callback);
		return Hooks_CallDialogById(cache_id, callback, spec, default_value, a1, a2, a3, a4, a5);
	}

	stock Hooks_Run0(const callback[], const spec[], default_value, const chain[])
	{
		new cache_id = Hooks_GetCacheId(callback);
		new value = default_value;

		if ((cache_id != -1 && Hooks_HasChain(cache_id, chain)) || (cache_id == -1 && funcidx(chain) != -1)) {
			value = CallLocalFunction(chain, spec);

			if (value == 0) {
				return 0;
			}
		}

		return Hooks_Call0ById(cache_id, callback, spec, value);
	}

	stock Hooks_Run1(const callback[], const spec[], default_value, const chain[], a1)
	{
		new cache_id = Hooks_GetCacheId(callback);
		new value = default_value;

		if ((cache_id != -1 && Hooks_HasChain(cache_id, chain)) || (cache_id == -1 && funcidx(chain) != -1)) {
			value = CallLocalFunction(chain, spec, a1);

			if (value == 0) {
				return 0;
			}
		}

		return Hooks_Call1ById(cache_id, callback, spec, value, a1);
	}

	stock Hooks_Run2(const callback[], const spec[], default_value, const chain[], a1, a2)
	{
		new cache_id = Hooks_GetCacheId(callback);
		new value = default_value;

		if ((cache_id != -1 && Hooks_HasChain(cache_id, chain)) || (cache_id == -1 && funcidx(chain) != -1)) {
			value = CallLocalFunction(chain, spec, a1, a2);

			if (value == 0) {
				return 0;
			}
		}

		return Hooks_Call2ById(cache_id, callback, spec, value, a1, a2);
	}

	stock Hooks_Run3(const callback[], const spec[], default_value, const chain[], a1, a2, a3)
	{
		new cache_id = Hooks_GetCacheId(callback);
		new value = default_value;

		if ((cache_id != -1 && Hooks_HasChain(cache_id, chain)) || (cache_id == -1 && funcidx(chain) != -1)) {
			value = CallLocalFunction(chain, spec, a1, a2, a3);

			if (value == 0) {
				return 0;
			}
		}

		return Hooks_Call3ById(cache_id, callback, spec, value, a1, a2, a3);
	}

	stock Hooks_Run4(const callback[], const spec[], default_value, const chain[], a1, a2, a3, a4)
	{
		new cache_id = Hooks_GetCacheId(callback);
		new value = default_value;

		if ((cache_id != -1 && Hooks_HasChain(cache_id, chain)) || (cache_id == -1 && funcidx(chain) != -1)) {
			value = CallLocalFunction(chain, spec, a1, a2, a3, a4);

			if (value == 0) {
				return 0;
			}
		}

		return Hooks_Call4ById(cache_id, callback, spec, value, a1, a2, a3, a4);
	}

	stock Hooks_Run5(const callback[], const spec[], default_value, const chain[], a1, a2, a3, a4, a5)
	{
		new cache_id = Hooks_GetCacheId(callback);
		new value = default_value;

		if ((cache_id != -1 && Hooks_HasChain(cache_id, chain)) || (cache_id == -1 && funcidx(chain) != -1)) {
			value = CallLocalFunction(chain, spec, a1, a2, a3, a4, a5);

			if (value == 0) {
				return 0;
			}
		}

		return Hooks_Call5ById(cache_id, callback, spec, value, a1, a2, a3, a4, a5);
	}

	stock Hooks_Run2S(const callback[], const spec[], default_value, const chain[], a1, a2, const a3[])
	{
		new cache_id = Hooks_GetCacheId(callback);
		new value = default_value;

		if ((cache_id != -1 && Hooks_HasChain(cache_id, chain)) || (cache_id == -1 && funcidx(chain) != -1)) {
			value = CallLocalFunction(chain, spec, a1, a2, a3);

			if (value == 0) {
				return 0;
			}
		}

		return Hooks_Call2SById(cache_id, callback, spec, value, a1, a2, a3);
	}

	public OnGameModeInit()
	{
		return Hooks_Run0("OnGameModeInit", "", true, "Hooks_OnGameModeInit");
	}
	#if defined _ALS_OnGameModeInit
		#undef OnGameModeInit
	#else
		#define _ALS_OnGameModeInit
	#endif
	forward Hooks_OnGameModeInit();
	#define OnGameModeInit Hooks_OnGameModeInit

	public OnGameModeExit()
	{
		return Hooks_Run0("OnGameModeExit", "", true, "Hooks_OnGameModeExit");
	}
	#if defined _ALS_OnGameModeExit
		#undef OnGameModeExit
	#else
		#define _ALS_OnGameModeExit
	#endif
	forward Hooks_OnGameModeExit();
	#define OnGameModeExit Hooks_OnGameModeExit

	public OnPlayerConnect(playerid)
	{
		return Hooks_Run1("OnPlayerConnect", "i", true, "Hooks_OnPlayerConnect", playerid);
	}
	#if defined _ALS_OnPlayerConnect
		#undef OnPlayerConnect
	#else
		#define _ALS_OnPlayerConnect
	#endif
	forward Hooks_OnPlayerConnect(playerid);
	#define OnPlayerConnect Hooks_OnPlayerConnect

	public OnPlayerUpdate(playerid)
	{
		return Hooks_Run1("OnPlayerUpdate", "i", true, "Hooks_OnPlayerUpdate", playerid);
	}
	#if defined _ALS_OnPlayerUpdate
		#undef OnPlayerUpdate
	#else
		#define _ALS_OnPlayerUpdate
	#endif
	forward Hooks_OnPlayerUpdate(playerid);
	#define OnPlayerUpdate Hooks_OnPlayerUpdate

	forward Player_LoadOtherData(playerid);
	public Player_LoadOtherData(playerid)
	{
		return Hooks_Run1("Player_LoadOtherData", "i", true, "Hooks_Player_LoadOtherData", playerid);
	}
	#if defined _ALS_Player_LoadOtherData
		#undef Player_LoadOtherData
	#else
		#define _ALS_Player_LoadOtherData
	#endif
	forward Hooks_Player_LoadOtherData(playerid);
	#define Player_LoadOtherData Hooks_Player_LoadOtherData

	forward Player_OnSpawn(playerid);
	public Player_OnSpawn(playerid)
	{
		return Hooks_Run1("Player_OnSpawn", "i", true, "Hooks_Player_OnSpawn", playerid);
	}
	#if defined _ALS_Player_OnSpawn
		#undef Player_OnSpawn
	#else
		#define _ALS_Player_OnSpawn
	#endif
	forward Hooks_Player_OnSpawn(playerid);
	#define Player_OnSpawn Hooks_Player_OnSpawn

	public OnPlayerDisconnect(playerid, reason)
	{
		return Hooks_Run2("OnPlayerDisconnect", "ii", true, "Hooks_OnPlayerDisconnect", playerid, reason);
	}
	#if defined _ALS_OnPlayerDisconnect
		#undef OnPlayerDisconnect
	#else
		#define _ALS_OnPlayerDisconnect
	#endif
	forward Hooks_OnPlayerDisconnect(playerid, reason);
	#define OnPlayerDisconnect Hooks_OnPlayerDisconnect

	public OnPlayerSpawn(playerid)
	{
		return Hooks_Run1("OnPlayerSpawn", "i", true, "Hooks_OnPlayerSpawn", playerid);
	}
	#if defined _ALS_OnPlayerSpawn
		#undef OnPlayerSpawn
	#else
		#define _ALS_OnPlayerSpawn
	#endif
	forward Hooks_OnPlayerSpawn(playerid);
	#define OnPlayerSpawn Hooks_OnPlayerSpawn

	public OnPlayerDeath(playerid, killerid, WEAPON:reason)
	{
		return Hooks_Run3("OnPlayerDeath", "iii", true, "Hooks_OnPlayerDeath", playerid, killerid, reason);
	}
	#if defined _ALS_OnPlayerDeath
		#undef OnPlayerDeath
	#else
		#define _ALS_OnPlayerDeath
	#endif
	forward Hooks_OnPlayerDeath(playerid, killerid, reason);
	#define OnPlayerDeath Hooks_OnPlayerDeath

	public OnPlayerStateChange(playerid, PLAYER_STATE:newstate, PLAYER_STATE:oldstate)
	{
		return Hooks_Run3("OnPlayerStateChange", "iii", true, "Hooks_OnPlayerStateChange", playerid, newstate, oldstate);
	}
	#if defined _ALS_OnPlayerStateChange
		#undef OnPlayerStateChange
	#else
		#define _ALS_OnPlayerStateChange
	#endif
	forward Hooks_OnPlayerStateChange(playerid, newstate, oldstate);
	#define OnPlayerStateChange Hooks_OnPlayerStateChange

	public OnPlayerEnterVehicle(playerid, vehicleid, ispassenger)
	{
		return Hooks_Run3("OnPlayerEnterVehicle", "iii", true, "Hooks_OnPlayerEnterVehicle", playerid, vehicleid, ispassenger);
	}
	#if defined _ALS_OnPlayerEnterVehicle
		#undef OnPlayerEnterVehicle
	#else
		#define _ALS_OnPlayerEnterVehicle
	#endif
	forward Hooks_OnPlayerEnterVehicle(playerid, vehicleid, ispassenger);
	#define OnPlayerEnterVehicle Hooks_OnPlayerEnterVehicle

	public OnPlayerExitVehicle(playerid, vehicleid)
	{
		return Hooks_Run2("OnPlayerExitVehicle", "ii", true, "Hooks_OnPlayerExitVehicle", playerid, vehicleid);
	}
	#if defined _ALS_OnPlayerExitVehicle
		#undef OnPlayerExitVehicle
	#else
		#define _ALS_OnPlayerExitVehicle
	#endif
	forward Hooks_OnPlayerExitVehicle(playerid, vehicleid);
	#define OnPlayerExitVehicle Hooks_OnPlayerExitVehicle

	public OnVehicleSpawn(vehicleid)
	{
		return Hooks_Run1("OnVehicleSpawn", "i", true, "Hooks_OnVehicleSpawn", vehicleid);
	}
	#if defined _ALS_OnVehicleSpawn
		#undef OnVehicleSpawn
	#else
		#define _ALS_OnVehicleSpawn
	#endif
	forward Hooks_OnVehicleSpawn(vehicleid);
	#define OnVehicleSpawn Hooks_OnVehicleSpawn

	public OnVehicleDeath(vehicleid, killerid)
	{
		return Hooks_Run2("OnVehicleDeath", "ii", true, "Hooks_OnVehicleDeath", vehicleid, killerid);
	}
	#if defined _ALS_OnVehicleDeath
		#undef OnVehicleDeath
	#else
		#define _ALS_OnVehicleDeath
	#endif
	forward Hooks_OnVehicleDeath(vehicleid, killerid);
	#define OnVehicleDeath Hooks_OnVehicleDeath

	public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
	{
		new value = Hooks_CallDialog("OnDialogResponse", "iiiis", false, playerid, dialogid, response, listitem, inputtext);

		if(value == 1)
			return 1;

		if(funcidx("Hooks_OnDialogResponse") != -1)
			return CallLocalFunction("Hooks_OnDialogResponse", "iiiis", playerid, dialogid, response, listitem, inputtext);

		return value;
	}
	#if defined _ALS_OnDialogResponse
		#undef OnDialogResponse
	#else
		#define _ALS_OnDialogResponse
	#endif
	forward Hooks_OnDialogResponse(playerid, dialogid, response, listitem, inputtext[]);
	#define OnDialogResponse Hooks_OnDialogResponse

	public OnPlayerKeyStateChange(playerid, KEY:newkeys, KEY:oldkeys)
	{
		return Hooks_Run3("OnPlayerKeyStateChange", "iii", true, "Hooks_OnPlayerKeyStateChange", playerid, newkeys, oldkeys);
	}
	#if defined _ALS_OnPlayerKeyStateChange
		#undef OnPlayerKeyStateChange
	#else
		#define _ALS_OnPlayerKeyStateChange
	#endif
	forward Hooks_OnPlayerKeyStateChange(playerid, newkeys, oldkeys);
	#define OnPlayerKeyStateChange Hooks_OnPlayerKeyStateChange

	forward OnSecondTimer();
	public OnSecondTimer()
	{
		return Hooks_Run0("OnSecondTimer", "", true, "Hooks_OnSecondTimer");
	}
	#if defined _ALS_OnSecondTimer
		#undef OnSecondTimer
	#else
		#define _ALS_OnSecondTimer
	#endif
	forward Hooks_OnSecondTimer();
	#define OnSecondTimer Hooks_OnSecondTimer

	forward OnPlayerTimer(playerid);
	public OnPlayerTimer(playerid)
	{
		return Hooks_Run1("OnPlayerTimer", "i", true, "Hooks_OnPlayerTimer", playerid);
	}
	#if defined _ALS_OnPlayerTimer
		#undef OnPlayerTimer
	#else
		#define _ALS_OnPlayerTimer
	#endif
	forward Hooks_OnPlayerTimer(playerid);
	#define OnPlayerTimer Hooks_OnPlayerTimer

	forward OnNotyResponse(playerid, notyId, response);
	public OnNotyResponse(playerid, notyId, response)
	{
		return Hooks_Run3("OnNotyResponse", "iii", true, "Hooks_OnNotyResponse", playerid, notyId, response);
	}
	#if defined _ALS_OnNotyResponse
		#undef OnNotyResponse
	#else
		#define _ALS_OnNotyResponse
	#endif
	forward Hooks_OnNotyResponse(playerid, notyId, response);
	#define OnNotyResponse Hooks_OnNotyResponse

	forward OnPurchaseClearBasket(playerid);
	public OnPurchaseClearBasket(playerid)
	{
		return Hooks_Run1("OnPurchaseClearBasket", "i", true, "Hooks_OnPurchaseClearBasket", playerid);
	}
	#if defined _ALS_OnPurchaseClearBasket
		#undef OnPurchaseClearBasket
	#else
		#define _ALS_OnPurchaseClearBasket
	#endif
	forward Hooks_OnPurchaseClearBasket(playerid);
	#define OnPurchaseClearBasket Hooks_OnPurchaseClearBasket

	forward OnPurchaseBuyResponse(playerid);
	public OnPurchaseBuyResponse(playerid)
	{
		return Hooks_Run1("OnPurchaseBuyResponse", "i", true, "Hooks_OnPurchaseBuyResponse", playerid);
	}
	#if defined _ALS_OnPurchaseBuyResponse
		#undef OnPurchaseBuyResponse
	#else
		#define _ALS_OnPurchaseBuyResponse
	#endif
	forward Hooks_OnPurchaseBuyResponse(playerid);
	#define OnPurchaseBuyResponse Hooks_OnPurchaseBuyResponse

	forward OnPurchaseAddToBasket(playerid, itemId);
	public OnPurchaseAddToBasket(playerid, itemId)
	{
		return Hooks_Run2("OnPurchaseAddToBasket", "ii", true, "Hooks_OnPurchaseAddToBasket", playerid, itemId);
	}
	#if defined _ALS_OnPurchaseAddToBasket
		#undef OnPurchaseAddToBasket
	#else
		#define _ALS_OnPurchaseAddToBasket
	#endif
	forward Hooks_OnPurchaseAddToBasket(playerid, itemId);
	#define OnPurchaseAddToBasket Hooks_OnPurchaseAddToBasket

	forward OnPurchaseEditAmountInBasket(playerid, itemId, action[]);
	public OnPurchaseEditAmountInBasket(playerid, itemId, action[])
	{
		return Hooks_Run2S("OnPurchaseEditAmountBasket", "iis", true, "Hooks_OnPurEditBasket", playerid, itemId, action);
	}
	#if defined _ALS_OnPurEditBasket
		#undef OnPurchaseEditAmountInBasket
	#else
		#define _ALS_OnPurEditBasket
	#endif
	forward Hooks_OnPurEditBasket(playerid, itemId, action[]);
	#define OnPurchaseEditAmountInBasket Hooks_OnPurEditBasket

	forward OnPurchaseRemoveFromBasket(playerid, itemId);
	public OnPurchaseRemoveFromBasket(playerid, itemId)
	{
		return Hooks_Run2("OnPurchaseRemoveFromBasket", "ii", true, "Hooks_OnPurRemoveBasket", playerid, itemId);
	}
	#if defined _ALS_OnPurRemoveBasket
		#undef OnPurchaseRemoveFromBasket
	#else
		#define _ALS_OnPurRemoveBasket
	#endif
	forward Hooks_OnPurRemoveBasket(playerid, itemId);
	#define OnPurchaseRemoveFromBasket Hooks_OnPurRemoveBasket

	forward OnPurchaseEditAmountResponse(playerid, itemId, count);
	public OnPurchaseEditAmountResponse(playerid, itemId, count)
	{
		return Hooks_Run3("OnPurchaseSetAmountBasket", "iii", true, "Hooks_OnPurSetAmount", playerid, itemId, count);
	}
	#if defined _ALS_OnPurSetAmount
		#undef OnPurchaseEditAmountResponse
	#else
		#define _ALS_OnPurSetAmount
	#endif
	forward Hooks_OnPurSetAmount(playerid, itemId, count);
	#define OnPurchaseEditAmountResponse Hooks_OnPurSetAmount

	forward OnPurchaseClose(playerid);
	public OnPurchaseClose(playerid)
	{
		return Hooks_Run1("OnPurchaseClose", "i", true, "Hooks_OnPurchaseClose", playerid);
	}
	#if defined _ALS_OnPurchaseClose
		#undef OnPurchaseClose
	#else
		#define _ALS_OnPurchaseClose
	#endif
	forward Hooks_OnPurchaseClose(playerid);
	#define OnPurchaseClose Hooks_OnPurchaseClose

	forward OnPurchaseRemoveCategory(playerid, categoryId);
	public OnPurchaseRemoveCategory(playerid, categoryId)
	{
		return Hooks_Run2("OnPurchaseRemCategory", "ii", true, "Hooks_OnPurchaseRemoveCategory", playerid, categoryId);
	}
	#if defined _ALS_OnPurchaseRemoveCategory
		#undef OnPurchaseRemoveCategory
	#else
		#define _ALS_OnPurchaseRemoveCategory
	#endif
	forward Hooks_OnPurchaseRemoveCategory(playerid, categoryId);
	#define OnPurchaseRemoveCategory Hooks_OnPurchaseRemoveCategory

	forward OnNpcDialogResponse(playerid, dialogid, response);
	public OnNpcDialogResponse(playerid, dialogid, response)
	{
		return Hooks_Run3("OnNpcDialogResponse", "iii", true, "Hooks_OnNpcDialogResponse", playerid, dialogid, response);
	}
	#if defined _ALS_OnNpcDialogResponse
		#undef OnNpcDialogResponse
	#else
		#define _ALS_OnNpcDialogResponse
	#endif
	forward Hooks_OnNpcDialogResponse(playerid, dialogid, response);
	#define OnNpcDialogResponse Hooks_OnNpcDialogResponse

	forward OnTrainingNPCDialogBtn(playerid, actionid);
	public OnTrainingNPCDialogBtn(playerid, actionid)
	{
		return Hooks_Run2("OnTrainingNPCDialogBtn", "ii", true, "Hooks_OnTrainingNPCDialogBtn", playerid, actionid);
	}
	#if defined _ALS_OnTrainingNPCDialogBtn
		#undef OnTrainingNPCDialogBtn
	#else
		#define _ALS_OnTrainingNPCDialogBtn
	#endif
	forward Hooks_OnTrainingNPCDialogBtn(playerid, actionid);
	#define OnTrainingNPCDialogBtn Hooks_OnTrainingNPCDialogBtn

	forward OnClientRPCResponse(playerid, rpcid, BitStream:bs);
	public OnClientRPCResponse(playerid, rpcid, BitStream:bs)
	{
		return Hooks_Run3("OnClientRPCResponse", "iii", false, "Hooks_OnClientRPCResponse", playerid, rpcid, _:bs);
	}
	#if defined _ALS_OnClientRPCResponse
		#undef OnClientRPCResponse
	#else
		#define _ALS_OnClientRPCResponse
	#endif
	forward Hooks_OnClientRPCResponse(playerid, rpcid, BitStream:bs);
	#define OnClientRPCResponse Hooks_OnClientRPCResponse

	forward OnDynamicObjectMoved(objectid);
	public OnDynamicObjectMoved(objectid)
	{
		return Hooks_Run1("OnDynamicObjectMoved", "i", true, "Hooks_OnDynamicObjectMoved", objectid);
	}
	#if defined _ALS_OnDynamicObjectMoved
		#undef OnDynamicObjectMoved
	#else
		#define _ALS_OnDynamicObjectMoved
	#endif
	forward Hooks_OnDynamicObjectMoved(objectid);
	#define OnDynamicObjectMoved Hooks_OnDynamicObjectMoved
#endif


#if !defined _hooks_slot
	#define _hooks_slot 0
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@000(%1); public %0@000(%1)
#elseif _hooks_slot == 0
	#undef _hooks_slot
	#define _hooks_slot 1
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@001(%1); public %0@001(%1)
#elseif _hooks_slot == 1
	#undef _hooks_slot
	#define _hooks_slot 2
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@002(%1); public %0@002(%1)
#elseif _hooks_slot == 2
	#undef _hooks_slot
	#define _hooks_slot 3
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@003(%1); public %0@003(%1)
#elseif _hooks_slot == 3
	#undef _hooks_slot
	#define _hooks_slot 4
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@004(%1); public %0@004(%1)
#elseif _hooks_slot == 4
	#undef _hooks_slot
	#define _hooks_slot 5
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@005(%1); public %0@005(%1)
#elseif _hooks_slot == 5
	#undef _hooks_slot
	#define _hooks_slot 6
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@006(%1); public %0@006(%1)
#elseif _hooks_slot == 6
	#undef _hooks_slot
	#define _hooks_slot 7
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@007(%1); public %0@007(%1)
#elseif _hooks_slot == 7
	#undef _hooks_slot
	#define _hooks_slot 8
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@008(%1); public %0@008(%1)
#elseif _hooks_slot == 8
	#undef _hooks_slot
	#define _hooks_slot 9
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@009(%1); public %0@009(%1)
#elseif _hooks_slot == 9
	#undef _hooks_slot
	#define _hooks_slot 10
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@010(%1); public %0@010(%1)
#elseif _hooks_slot == 10
	#undef _hooks_slot
	#define _hooks_slot 11
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@011(%1); public %0@011(%1)
#elseif _hooks_slot == 11
	#undef _hooks_slot
	#define _hooks_slot 12
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@012(%1); public %0@012(%1)
#elseif _hooks_slot == 12
	#undef _hooks_slot
	#define _hooks_slot 13
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@013(%1); public %0@013(%1)
#elseif _hooks_slot == 13
	#undef _hooks_slot
	#define _hooks_slot 14
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@014(%1); public %0@014(%1)
#elseif _hooks_slot == 14
	#undef _hooks_slot
	#define _hooks_slot 15
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@015(%1); public %0@015(%1)
#elseif _hooks_slot == 15
	#undef _hooks_slot
	#define _hooks_slot 16
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@016(%1); public %0@016(%1)
#elseif _hooks_slot == 16
	#undef _hooks_slot
	#define _hooks_slot 17
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@017(%1); public %0@017(%1)
#elseif _hooks_slot == 17
	#undef _hooks_slot
	#define _hooks_slot 18
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@018(%1); public %0@018(%1)
#elseif _hooks_slot == 18
	#undef _hooks_slot
	#define _hooks_slot 19
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@019(%1); public %0@019(%1)
#elseif _hooks_slot == 19
	#undef _hooks_slot
	#define _hooks_slot 20
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@020(%1); public %0@020(%1)
#elseif _hooks_slot == 20
	#undef _hooks_slot
	#define _hooks_slot 21
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@021(%1); public %0@021(%1)
#elseif _hooks_slot == 21
	#undef _hooks_slot
	#define _hooks_slot 22
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@022(%1); public %0@022(%1)
#elseif _hooks_slot == 22
	#undef _hooks_slot
	#define _hooks_slot 23
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@023(%1); public %0@023(%1)
#elseif _hooks_slot == 23
	#undef _hooks_slot
	#define _hooks_slot 24
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@024(%1); public %0@024(%1)
#elseif _hooks_slot == 24
	#undef _hooks_slot
	#define _hooks_slot 25
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@025(%1); public %0@025(%1)
#elseif _hooks_slot == 25
	#undef _hooks_slot
	#define _hooks_slot 26
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@026(%1); public %0@026(%1)
#elseif _hooks_slot == 26
	#undef _hooks_slot
	#define _hooks_slot 27
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@027(%1); public %0@027(%1)
#elseif _hooks_slot == 27
	#undef _hooks_slot
	#define _hooks_slot 28
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@028(%1); public %0@028(%1)
#elseif _hooks_slot == 28
	#undef _hooks_slot
	#define _hooks_slot 29
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@029(%1); public %0@029(%1)
#elseif _hooks_slot == 29
	#undef _hooks_slot
	#define _hooks_slot 30
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@030(%1); public %0@030(%1)
#elseif _hooks_slot == 30
	#undef _hooks_slot
	#define _hooks_slot 31
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@031(%1); public %0@031(%1)
#elseif _hooks_slot == 31
	#undef _hooks_slot
	#define _hooks_slot 32
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@032(%1); public %0@032(%1)
#elseif _hooks_slot == 32
	#undef _hooks_slot
	#define _hooks_slot 33
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@033(%1); public %0@033(%1)
#elseif _hooks_slot == 33
	#undef _hooks_slot
	#define _hooks_slot 34
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@034(%1); public %0@034(%1)
#elseif _hooks_slot == 34
	#undef _hooks_slot
	#define _hooks_slot 35
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@035(%1); public %0@035(%1)
#elseif _hooks_slot == 35
	#undef _hooks_slot
	#define _hooks_slot 36
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@036(%1); public %0@036(%1)
#elseif _hooks_slot == 36
	#undef _hooks_slot
	#define _hooks_slot 37
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@037(%1); public %0@037(%1)
#elseif _hooks_slot == 37
	#undef _hooks_slot
	#define _hooks_slot 38
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@038(%1); public %0@038(%1)
#elseif _hooks_slot == 38
	#undef _hooks_slot
	#define _hooks_slot 39
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@039(%1); public %0@039(%1)
#elseif _hooks_slot == 39
	#undef _hooks_slot
	#define _hooks_slot 40
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@040(%1); public %0@040(%1)
#elseif _hooks_slot == 40
	#undef _hooks_slot
	#define _hooks_slot 41
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@041(%1); public %0@041(%1)
#elseif _hooks_slot == 41
	#undef _hooks_slot
	#define _hooks_slot 42
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@042(%1); public %0@042(%1)
#elseif _hooks_slot == 42
	#undef _hooks_slot
	#define _hooks_slot 43
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@043(%1); public %0@043(%1)
#elseif _hooks_slot == 43
	#undef _hooks_slot
	#define _hooks_slot 44
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@044(%1); public %0@044(%1)
#elseif _hooks_slot == 44
	#undef _hooks_slot
	#define _hooks_slot 45
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@045(%1); public %0@045(%1)
#elseif _hooks_slot == 45
	#undef _hooks_slot
	#define _hooks_slot 46
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@046(%1); public %0@046(%1)
#elseif _hooks_slot == 46
	#undef _hooks_slot
	#define _hooks_slot 47
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@047(%1); public %0@047(%1)
#elseif _hooks_slot == 47
	#undef _hooks_slot
	#define _hooks_slot 48
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@048(%1); public %0@048(%1)
#elseif _hooks_slot == 48
	#undef _hooks_slot
	#define _hooks_slot 49
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@049(%1); public %0@049(%1)
#elseif _hooks_slot == 49
	#undef _hooks_slot
	#define _hooks_slot 50
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@050(%1); public %0@050(%1)
#elseif _hooks_slot == 50
	#undef _hooks_slot
	#define _hooks_slot 51
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@051(%1); public %0@051(%1)
#elseif _hooks_slot == 51
	#undef _hooks_slot
	#define _hooks_slot 52
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@052(%1); public %0@052(%1)
#elseif _hooks_slot == 52
	#undef _hooks_slot
	#define _hooks_slot 53
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@053(%1); public %0@053(%1)
#elseif _hooks_slot == 53
	#undef _hooks_slot
	#define _hooks_slot 54
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@054(%1); public %0@054(%1)
#elseif _hooks_slot == 54
	#undef _hooks_slot
	#define _hooks_slot 55
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@055(%1); public %0@055(%1)
#elseif _hooks_slot == 55
	#undef _hooks_slot
	#define _hooks_slot 56
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@056(%1); public %0@056(%1)
#elseif _hooks_slot == 56
	#undef _hooks_slot
	#define _hooks_slot 57
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@057(%1); public %0@057(%1)
#elseif _hooks_slot == 57
	#undef _hooks_slot
	#define _hooks_slot 58
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@058(%1); public %0@058(%1)
#elseif _hooks_slot == 58
	#undef _hooks_slot
	#define _hooks_slot 59
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@059(%1); public %0@059(%1)
#elseif _hooks_slot == 59
	#undef _hooks_slot
	#define _hooks_slot 60
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@060(%1); public %0@060(%1)
#elseif _hooks_slot == 60
	#undef _hooks_slot
	#define _hooks_slot 61
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@061(%1); public %0@061(%1)
#elseif _hooks_slot == 61
	#undef _hooks_slot
	#define _hooks_slot 62
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@062(%1); public %0@062(%1)
#elseif _hooks_slot == 62
	#undef _hooks_slot
	#define _hooks_slot 63
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@063(%1); public %0@063(%1)
#elseif _hooks_slot == 63
	#undef _hooks_slot
	#define _hooks_slot 64
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@064(%1); public %0@064(%1)
#elseif _hooks_slot == 64
	#undef _hooks_slot
	#define _hooks_slot 65
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@065(%1); public %0@065(%1)
#elseif _hooks_slot == 65
	#undef _hooks_slot
	#define _hooks_slot 66
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@066(%1); public %0@066(%1)
#elseif _hooks_slot == 66
	#undef _hooks_slot
	#define _hooks_slot 67
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@067(%1); public %0@067(%1)
#elseif _hooks_slot == 67
	#undef _hooks_slot
	#define _hooks_slot 68
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@068(%1); public %0@068(%1)
#elseif _hooks_slot == 68
	#undef _hooks_slot
	#define _hooks_slot 69
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@069(%1); public %0@069(%1)
#elseif _hooks_slot == 69
	#undef _hooks_slot
	#define _hooks_slot 70
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@070(%1); public %0@070(%1)
#elseif _hooks_slot == 70
	#undef _hooks_slot
	#define _hooks_slot 71
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@071(%1); public %0@071(%1)
#elseif _hooks_slot == 71
	#undef _hooks_slot
	#define _hooks_slot 72
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@072(%1); public %0@072(%1)
#elseif _hooks_slot == 72
	#undef _hooks_slot
	#define _hooks_slot 73
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@073(%1); public %0@073(%1)
#elseif _hooks_slot == 73
	#undef _hooks_slot
	#define _hooks_slot 74
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@074(%1); public %0@074(%1)
#elseif _hooks_slot == 74
	#undef _hooks_slot
	#define _hooks_slot 75
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@075(%1); public %0@075(%1)
#elseif _hooks_slot == 75
	#undef _hooks_slot
	#define _hooks_slot 76
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@076(%1); public %0@076(%1)
#elseif _hooks_slot == 76
	#undef _hooks_slot
	#define _hooks_slot 77
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@077(%1); public %0@077(%1)
#elseif _hooks_slot == 77
	#undef _hooks_slot
	#define _hooks_slot 78
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@078(%1); public %0@078(%1)
#elseif _hooks_slot == 78
	#undef _hooks_slot
	#define _hooks_slot 79
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@079(%1); public %0@079(%1)
#elseif _hooks_slot == 79
	#undef _hooks_slot
	#define _hooks_slot 80
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@080(%1); public %0@080(%1)
#elseif _hooks_slot == 80
	#undef _hooks_slot
	#define _hooks_slot 81
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@081(%1); public %0@081(%1)
#elseif _hooks_slot == 81
	#undef _hooks_slot
	#define _hooks_slot 82
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@082(%1); public %0@082(%1)
#elseif _hooks_slot == 82
	#undef _hooks_slot
	#define _hooks_slot 83
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@083(%1); public %0@083(%1)
#elseif _hooks_slot == 83
	#undef _hooks_slot
	#define _hooks_slot 84
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@084(%1); public %0@084(%1)
#elseif _hooks_slot == 84
	#undef _hooks_slot
	#define _hooks_slot 85
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@085(%1); public %0@085(%1)
#elseif _hooks_slot == 85
	#undef _hooks_slot
	#define _hooks_slot 86
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@086(%1); public %0@086(%1)
#elseif _hooks_slot == 86
	#undef _hooks_slot
	#define _hooks_slot 87
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@087(%1); public %0@087(%1)
#elseif _hooks_slot == 87
	#undef _hooks_slot
	#define _hooks_slot 88
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@088(%1); public %0@088(%1)
#elseif _hooks_slot == 88
	#undef _hooks_slot
	#define _hooks_slot 89
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@089(%1); public %0@089(%1)
#elseif _hooks_slot == 89
	#undef _hooks_slot
	#define _hooks_slot 90
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@090(%1); public %0@090(%1)
#elseif _hooks_slot == 90
	#undef _hooks_slot
	#define _hooks_slot 91
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@091(%1); public %0@091(%1)
#elseif _hooks_slot == 91
	#undef _hooks_slot
	#define _hooks_slot 92
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@092(%1); public %0@092(%1)
#elseif _hooks_slot == 92
	#undef _hooks_slot
	#define _hooks_slot 93
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@093(%1); public %0@093(%1)
#elseif _hooks_slot == 93
	#undef _hooks_slot
	#define _hooks_slot 94
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@094(%1); public %0@094(%1)
#elseif _hooks_slot == 94
	#undef _hooks_slot
	#define _hooks_slot 95
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@095(%1); public %0@095(%1)
#elseif _hooks_slot == 95
	#undef _hooks_slot
	#define _hooks_slot 96
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@096(%1); public %0@096(%1)
#elseif _hooks_slot == 96
	#undef _hooks_slot
	#define _hooks_slot 97
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@097(%1); public %0@097(%1)
#elseif _hooks_slot == 97
	#undef _hooks_slot
	#define _hooks_slot 98
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@098(%1); public %0@098(%1)
#elseif _hooks_slot == 98
	#undef _hooks_slot
	#define _hooks_slot 99
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@099(%1); public %0@099(%1)
#elseif _hooks_slot == 99
	#undef _hooks_slot
	#define _hooks_slot 100
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@100(%1); public %0@100(%1)
#elseif _hooks_slot == 100
	#undef _hooks_slot
	#define _hooks_slot 101
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@101(%1); public %0@101(%1)
#elseif _hooks_slot == 101
	#undef _hooks_slot
	#define _hooks_slot 102
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@102(%1); public %0@102(%1)
#elseif _hooks_slot == 102
	#undef _hooks_slot
	#define _hooks_slot 103
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@103(%1); public %0@103(%1)
#elseif _hooks_slot == 103
	#undef _hooks_slot
	#define _hooks_slot 104
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@104(%1); public %0@104(%1)
#elseif _hooks_slot == 104
	#undef _hooks_slot
	#define _hooks_slot 105
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@105(%1); public %0@105(%1)
#elseif _hooks_slot == 105
	#undef _hooks_slot
	#define _hooks_slot 106
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@106(%1); public %0@106(%1)
#elseif _hooks_slot == 106
	#undef _hooks_slot
	#define _hooks_slot 107
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@107(%1); public %0@107(%1)
#elseif _hooks_slot == 107
	#undef _hooks_slot
	#define _hooks_slot 108
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@108(%1); public %0@108(%1)
#elseif _hooks_slot == 108
	#undef _hooks_slot
	#define _hooks_slot 109
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@109(%1); public %0@109(%1)
#elseif _hooks_slot == 109
	#undef _hooks_slot
	#define _hooks_slot 110
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@110(%1); public %0@110(%1)
#elseif _hooks_slot == 110
	#undef _hooks_slot
	#define _hooks_slot 111
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@111(%1); public %0@111(%1)
#elseif _hooks_slot == 111
	#undef _hooks_slot
	#define _hooks_slot 112
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@112(%1); public %0@112(%1)
#elseif _hooks_slot == 112
	#undef _hooks_slot
	#define _hooks_slot 113
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@113(%1); public %0@113(%1)
#elseif _hooks_slot == 113
	#undef _hooks_slot
	#define _hooks_slot 114
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@114(%1); public %0@114(%1)
#elseif _hooks_slot == 114
	#undef _hooks_slot
	#define _hooks_slot 115
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@115(%1); public %0@115(%1)
#elseif _hooks_slot == 115
	#undef _hooks_slot
	#define _hooks_slot 116
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@116(%1); public %0@116(%1)
#elseif _hooks_slot == 116
	#undef _hooks_slot
	#define _hooks_slot 117
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@117(%1); public %0@117(%1)
#elseif _hooks_slot == 117
	#undef _hooks_slot
	#define _hooks_slot 118
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@118(%1); public %0@118(%1)
#elseif _hooks_slot == 118
	#undef _hooks_slot
	#define _hooks_slot 119
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@119(%1); public %0@119(%1)
#elseif _hooks_slot == 119
	#undef _hooks_slot
	#define _hooks_slot 120
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@120(%1); public %0@120(%1)
#elseif _hooks_slot == 120
	#undef _hooks_slot
	#define _hooks_slot 121
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@121(%1); public %0@121(%1)
#elseif _hooks_slot == 121
	#undef _hooks_slot
	#define _hooks_slot 122
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@122(%1); public %0@122(%1)
#elseif _hooks_slot == 122
	#undef _hooks_slot
	#define _hooks_slot 123
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@123(%1); public %0@123(%1)
#elseif _hooks_slot == 123
	#undef _hooks_slot
	#define _hooks_slot 124
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@124(%1); public %0@124(%1)
#elseif _hooks_slot == 124
	#undef _hooks_slot
	#define _hooks_slot 125
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@125(%1); public %0@125(%1)
#elseif _hooks_slot == 125
	#undef _hooks_slot
	#define _hooks_slot 126
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@126(%1); public %0@126(%1)
#elseif _hooks_slot == 126
	#undef _hooks_slot
	#define _hooks_slot 127
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@127(%1); public %0@127(%1)
#elseif _hooks_slot == 127
	#undef _hooks_slot
	#define _hooks_slot 128
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@128(%1); public %0@128(%1)
#elseif _hooks_slot == 128
	#undef _hooks_slot
	#define _hooks_slot 129
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@129(%1); public %0@129(%1)
#elseif _hooks_slot == 129
	#undef _hooks_slot
	#define _hooks_slot 130
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@130(%1); public %0@130(%1)
#elseif _hooks_slot == 130
	#undef _hooks_slot
	#define _hooks_slot 131
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@131(%1); public %0@131(%1)
#elseif _hooks_slot == 131
	#undef _hooks_slot
	#define _hooks_slot 132
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@132(%1); public %0@132(%1)
#elseif _hooks_slot == 132
	#undef _hooks_slot
	#define _hooks_slot 133
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@133(%1); public %0@133(%1)
#elseif _hooks_slot == 133
	#undef _hooks_slot
	#define _hooks_slot 134
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@134(%1); public %0@134(%1)
#elseif _hooks_slot == 134
	#undef _hooks_slot
	#define _hooks_slot 135
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@135(%1); public %0@135(%1)
#elseif _hooks_slot == 135
	#undef _hooks_slot
	#define _hooks_slot 136
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@136(%1); public %0@136(%1)
#elseif _hooks_slot == 136
	#undef _hooks_slot
	#define _hooks_slot 137
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@137(%1); public %0@137(%1)
#elseif _hooks_slot == 137
	#undef _hooks_slot
	#define _hooks_slot 138
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@138(%1); public %0@138(%1)
#elseif _hooks_slot == 138
	#undef _hooks_slot
	#define _hooks_slot 139
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@139(%1); public %0@139(%1)
#elseif _hooks_slot == 139
	#undef _hooks_slot
	#define _hooks_slot 140
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@140(%1); public %0@140(%1)
#elseif _hooks_slot == 140
	#undef _hooks_slot
	#define _hooks_slot 141
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@141(%1); public %0@141(%1)
#elseif _hooks_slot == 141
	#undef _hooks_slot
	#define _hooks_slot 142
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@142(%1); public %0@142(%1)
#elseif _hooks_slot == 142
	#undef _hooks_slot
	#define _hooks_slot 143
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@143(%1); public %0@143(%1)
#elseif _hooks_slot == 143
	#undef _hooks_slot
	#define _hooks_slot 144
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@144(%1); public %0@144(%1)
#elseif _hooks_slot == 144
	#undef _hooks_slot
	#define _hooks_slot 145
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@145(%1); public %0@145(%1)
#elseif _hooks_slot == 145
	#undef _hooks_slot
	#define _hooks_slot 146
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@146(%1); public %0@146(%1)
#elseif _hooks_slot == 146
	#undef _hooks_slot
	#define _hooks_slot 147
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@147(%1); public %0@147(%1)
#elseif _hooks_slot == 147
	#undef _hooks_slot
	#define _hooks_slot 148
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@148(%1); public %0@148(%1)
#elseif _hooks_slot == 148
	#undef _hooks_slot
	#define _hooks_slot 149
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@149(%1); public %0@149(%1)
#elseif _hooks_slot == 149
	#undef _hooks_slot
	#define _hooks_slot 150
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@150(%1); public %0@150(%1)
#elseif _hooks_slot == 150
	#undef _hooks_slot
	#define _hooks_slot 151
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@151(%1); public %0@151(%1)
#elseif _hooks_slot == 151
	#undef _hooks_slot
	#define _hooks_slot 152
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@152(%1); public %0@152(%1)
#elseif _hooks_slot == 152
	#undef _hooks_slot
	#define _hooks_slot 153
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@153(%1); public %0@153(%1)
#elseif _hooks_slot == 153
	#undef _hooks_slot
	#define _hooks_slot 154
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@154(%1); public %0@154(%1)
#elseif _hooks_slot == 154
	#undef _hooks_slot
	#define _hooks_slot 155
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@155(%1); public %0@155(%1)
#elseif _hooks_slot == 155
	#undef _hooks_slot
	#define _hooks_slot 156
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@156(%1); public %0@156(%1)
#elseif _hooks_slot == 156
	#undef _hooks_slot
	#define _hooks_slot 157
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@157(%1); public %0@157(%1)
#elseif _hooks_slot == 157
	#undef _hooks_slot
	#define _hooks_slot 158
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@158(%1); public %0@158(%1)
#elseif _hooks_slot == 158
	#undef _hooks_slot
	#define _hooks_slot 159
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@159(%1); public %0@159(%1)
#elseif _hooks_slot == 159
	#undef _hooks_slot
	#define _hooks_slot 160
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@160(%1); public %0@160(%1)
#elseif _hooks_slot == 160
	#undef _hooks_slot
	#define _hooks_slot 161
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@161(%1); public %0@161(%1)
#elseif _hooks_slot == 161
	#undef _hooks_slot
	#define _hooks_slot 162
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@162(%1); public %0@162(%1)
#elseif _hooks_slot == 162
	#undef _hooks_slot
	#define _hooks_slot 163
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@163(%1); public %0@163(%1)
#elseif _hooks_slot == 163
	#undef _hooks_slot
	#define _hooks_slot 164
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@164(%1); public %0@164(%1)
#elseif _hooks_slot == 164
	#undef _hooks_slot
	#define _hooks_slot 165
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@165(%1); public %0@165(%1)
#elseif _hooks_slot == 165
	#undef _hooks_slot
	#define _hooks_slot 166
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@166(%1); public %0@166(%1)
#elseif _hooks_slot == 166
	#undef _hooks_slot
	#define _hooks_slot 167
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@167(%1); public %0@167(%1)
#elseif _hooks_slot == 167
	#undef _hooks_slot
	#define _hooks_slot 168
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@168(%1); public %0@168(%1)
#elseif _hooks_slot == 168
	#undef _hooks_slot
	#define _hooks_slot 169
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@169(%1); public %0@169(%1)
#elseif _hooks_slot == 169
	#undef _hooks_slot
	#define _hooks_slot 170
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@170(%1); public %0@170(%1)
#elseif _hooks_slot == 170
	#undef _hooks_slot
	#define _hooks_slot 171
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@171(%1); public %0@171(%1)
#elseif _hooks_slot == 171
	#undef _hooks_slot
	#define _hooks_slot 172
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@172(%1); public %0@172(%1)
#elseif _hooks_slot == 172
	#undef _hooks_slot
	#define _hooks_slot 173
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@173(%1); public %0@173(%1)
#elseif _hooks_slot == 173
	#undef _hooks_slot
	#define _hooks_slot 174
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@174(%1); public %0@174(%1)
#elseif _hooks_slot == 174
	#undef _hooks_slot
	#define _hooks_slot 175
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@175(%1); public %0@175(%1)
#elseif _hooks_slot == 175
	#undef _hooks_slot
	#define _hooks_slot 176
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@176(%1); public %0@176(%1)
#elseif _hooks_slot == 176
	#undef _hooks_slot
	#define _hooks_slot 177
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@177(%1); public %0@177(%1)
#elseif _hooks_slot == 177
	#undef _hooks_slot
	#define _hooks_slot 178
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@178(%1); public %0@178(%1)
#elseif _hooks_slot == 178
	#undef _hooks_slot
	#define _hooks_slot 179
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@179(%1); public %0@179(%1)
#elseif _hooks_slot == 179
	#undef _hooks_slot
	#define _hooks_slot 180
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@180(%1); public %0@180(%1)
#elseif _hooks_slot == 180
	#undef _hooks_slot
	#define _hooks_slot 181
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@181(%1); public %0@181(%1)
#elseif _hooks_slot == 181
	#undef _hooks_slot
	#define _hooks_slot 182
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@182(%1); public %0@182(%1)
#elseif _hooks_slot == 182
	#undef _hooks_slot
	#define _hooks_slot 183
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@183(%1); public %0@183(%1)
#elseif _hooks_slot == 183
	#undef _hooks_slot
	#define _hooks_slot 184
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@184(%1); public %0@184(%1)
#elseif _hooks_slot == 184
	#undef _hooks_slot
	#define _hooks_slot 185
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@185(%1); public %0@185(%1)
#elseif _hooks_slot == 185
	#undef _hooks_slot
	#define _hooks_slot 186
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@186(%1); public %0@186(%1)
#elseif _hooks_slot == 186
	#undef _hooks_slot
	#define _hooks_slot 187
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@187(%1); public %0@187(%1)
#elseif _hooks_slot == 187
	#undef _hooks_slot
	#define _hooks_slot 188
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@188(%1); public %0@188(%1)
#elseif _hooks_slot == 188
	#undef _hooks_slot
	#define _hooks_slot 189
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@189(%1); public %0@189(%1)
#elseif _hooks_slot == 189
	#undef _hooks_slot
	#define _hooks_slot 190
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@190(%1); public %0@190(%1)
#elseif _hooks_slot == 190
	#undef _hooks_slot
	#define _hooks_slot 191
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@191(%1); public %0@191(%1)
#elseif _hooks_slot == 191
	#undef _hooks_slot
	#define _hooks_slot 192
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@192(%1); public %0@192(%1)
#elseif _hooks_slot == 192
	#undef _hooks_slot
	#define _hooks_slot 193
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@193(%1); public %0@193(%1)
#elseif _hooks_slot == 193
	#undef _hooks_slot
	#define _hooks_slot 194
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@194(%1); public %0@194(%1)
#elseif _hooks_slot == 194
	#undef _hooks_slot
	#define _hooks_slot 195
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@195(%1); public %0@195(%1)
#elseif _hooks_slot == 195
	#undef _hooks_slot
	#define _hooks_slot 196
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@196(%1); public %0@196(%1)
#elseif _hooks_slot == 196
	#undef _hooks_slot
	#define _hooks_slot 197
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@197(%1); public %0@197(%1)
#elseif _hooks_slot == 197
	#undef _hooks_slot
	#define _hooks_slot 198
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@198(%1); public %0@198(%1)
#elseif _hooks_slot == 198
	#undef _hooks_slot
	#define _hooks_slot 199
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@199(%1); public %0@199(%1)
#elseif _hooks_slot == 199
	#undef _hooks_slot
	#define _hooks_slot 200
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@200(%1); public %0@200(%1)
#elseif _hooks_slot == 200
	#undef _hooks_slot
	#define _hooks_slot 201
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@201(%1); public %0@201(%1)
#elseif _hooks_slot == 201
	#undef _hooks_slot
	#define _hooks_slot 202
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@202(%1); public %0@202(%1)
#elseif _hooks_slot == 202
	#undef _hooks_slot
	#define _hooks_slot 203
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@203(%1); public %0@203(%1)
#elseif _hooks_slot == 203
	#undef _hooks_slot
	#define _hooks_slot 204
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@204(%1); public %0@204(%1)
#elseif _hooks_slot == 204
	#undef _hooks_slot
	#define _hooks_slot 205
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@205(%1); public %0@205(%1)
#elseif _hooks_slot == 205
	#undef _hooks_slot
	#define _hooks_slot 206
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@206(%1); public %0@206(%1)
#elseif _hooks_slot == 206
	#undef _hooks_slot
	#define _hooks_slot 207
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@207(%1); public %0@207(%1)
#elseif _hooks_slot == 207
	#undef _hooks_slot
	#define _hooks_slot 208
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@208(%1); public %0@208(%1)
#elseif _hooks_slot == 208
	#undef _hooks_slot
	#define _hooks_slot 209
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@209(%1); public %0@209(%1)
#elseif _hooks_slot == 209
	#undef _hooks_slot
	#define _hooks_slot 210
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@210(%1); public %0@210(%1)
#elseif _hooks_slot == 210
	#undef _hooks_slot
	#define _hooks_slot 211
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@211(%1); public %0@211(%1)
#elseif _hooks_slot == 211
	#undef _hooks_slot
	#define _hooks_slot 212
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@212(%1); public %0@212(%1)
#elseif _hooks_slot == 212
	#undef _hooks_slot
	#define _hooks_slot 213
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@213(%1); public %0@213(%1)
#elseif _hooks_slot == 213
	#undef _hooks_slot
	#define _hooks_slot 214
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@214(%1); public %0@214(%1)
#elseif _hooks_slot == 214
	#undef _hooks_slot
	#define _hooks_slot 215
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@215(%1); public %0@215(%1)
#elseif _hooks_slot == 215
	#undef _hooks_slot
	#define _hooks_slot 216
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@216(%1); public %0@216(%1)
#elseif _hooks_slot == 216
	#undef _hooks_slot
	#define _hooks_slot 217
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@217(%1); public %0@217(%1)
#elseif _hooks_slot == 217
	#undef _hooks_slot
	#define _hooks_slot 218
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@218(%1); public %0@218(%1)
#elseif _hooks_slot == 218
	#undef _hooks_slot
	#define _hooks_slot 219
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@219(%1); public %0@219(%1)
#elseif _hooks_slot == 219
	#undef _hooks_slot
	#define _hooks_slot 220
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@220(%1); public %0@220(%1)
#elseif _hooks_slot == 220
	#undef _hooks_slot
	#define _hooks_slot 221
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@221(%1); public %0@221(%1)
#elseif _hooks_slot == 221
	#undef _hooks_slot
	#define _hooks_slot 222
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@222(%1); public %0@222(%1)
#elseif _hooks_slot == 222
	#undef _hooks_slot
	#define _hooks_slot 223
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@223(%1); public %0@223(%1)
#elseif _hooks_slot == 223
	#undef _hooks_slot
	#define _hooks_slot 224
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@224(%1); public %0@224(%1)
#elseif _hooks_slot == 224
	#undef _hooks_slot
	#define _hooks_slot 225
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@225(%1); public %0@225(%1)
#elseif _hooks_slot == 225
	#undef _hooks_slot
	#define _hooks_slot 226
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@226(%1); public %0@226(%1)
#elseif _hooks_slot == 226
	#undef _hooks_slot
	#define _hooks_slot 227
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@227(%1); public %0@227(%1)
#elseif _hooks_slot == 227
	#undef _hooks_slot
	#define _hooks_slot 228
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@228(%1); public %0@228(%1)
#elseif _hooks_slot == 228
	#undef _hooks_slot
	#define _hooks_slot 229
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@229(%1); public %0@229(%1)
#elseif _hooks_slot == 229
	#undef _hooks_slot
	#define _hooks_slot 230
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@230(%1); public %0@230(%1)
#elseif _hooks_slot == 230
	#undef _hooks_slot
	#define _hooks_slot 231
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@231(%1); public %0@231(%1)
#elseif _hooks_slot == 231
	#undef _hooks_slot
	#define _hooks_slot 232
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@232(%1); public %0@232(%1)
#elseif _hooks_slot == 232
	#undef _hooks_slot
	#define _hooks_slot 233
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@233(%1); public %0@233(%1)
#elseif _hooks_slot == 233
	#undef _hooks_slot
	#define _hooks_slot 234
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@234(%1); public %0@234(%1)
#elseif _hooks_slot == 234
	#undef _hooks_slot
	#define _hooks_slot 235
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@235(%1); public %0@235(%1)
#elseif _hooks_slot == 235
	#undef _hooks_slot
	#define _hooks_slot 236
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@236(%1); public %0@236(%1)
#elseif _hooks_slot == 236
	#undef _hooks_slot
	#define _hooks_slot 237
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@237(%1); public %0@237(%1)
#elseif _hooks_slot == 237
	#undef _hooks_slot
	#define _hooks_slot 238
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@238(%1); public %0@238(%1)
#elseif _hooks_slot == 238
	#undef _hooks_slot
	#define _hooks_slot 239
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@239(%1); public %0@239(%1)
#elseif _hooks_slot == 239
	#undef _hooks_slot
	#define _hooks_slot 240
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@240(%1); public %0@240(%1)
#elseif _hooks_slot == 240
	#undef _hooks_slot
	#define _hooks_slot 241
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@241(%1); public %0@241(%1)
#elseif _hooks_slot == 241
	#undef _hooks_slot
	#define _hooks_slot 242
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@242(%1); public %0@242(%1)
#elseif _hooks_slot == 242
	#undef _hooks_slot
	#define _hooks_slot 243
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@243(%1); public %0@243(%1)
#elseif _hooks_slot == 243
	#undef _hooks_slot
	#define _hooks_slot 244
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@244(%1); public %0@244(%1)
#elseif _hooks_slot == 244
	#undef _hooks_slot
	#define _hooks_slot 245
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@245(%1); public %0@245(%1)
#elseif _hooks_slot == 245
	#undef _hooks_slot
	#define _hooks_slot 246
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@246(%1); public %0@246(%1)
#elseif _hooks_slot == 246
	#undef _hooks_slot
	#define _hooks_slot 247
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@247(%1); public %0@247(%1)
#elseif _hooks_slot == 247
	#undef _hooks_slot
	#define _hooks_slot 248
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@248(%1); public %0@248(%1)
#elseif _hooks_slot == 248
	#undef _hooks_slot
	#define _hooks_slot 249
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@249(%1); public %0@249(%1)
#elseif _hooks_slot == 249
	#undef _hooks_slot
	#define _hooks_slot 250
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@250(%1); public %0@250(%1)
#elseif _hooks_slot == 250
	#undef _hooks_slot
	#define _hooks_slot 251
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@251(%1); public %0@251(%1)
#elseif _hooks_slot == 251
	#undef _hooks_slot
	#define _hooks_slot 252
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@252(%1); public %0@252(%1)
#elseif _hooks_slot == 252
	#undef _hooks_slot
	#define _hooks_slot 253
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@253(%1); public %0@253(%1)
#elseif _hooks_slot == 253
	#undef _hooks_slot
	#define _hooks_slot 254
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@254(%1); public %0@254(%1)
#elseif _hooks_slot == 254
	#undef _hooks_slot
	#define _hooks_slot 255
	#if defined hook
		#undef hook
	#endif
	#define hook%0(%1) forward %0@255(%1); public %0@255(%1)
#else
	#error "HOOKS_MAX_SLOTS macro table is exhausted"
#endif
