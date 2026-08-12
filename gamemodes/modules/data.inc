#define COLOR_OLDRED 		0xBE2D2DFF
#define COLOR_STATE 		0x73B461FF
#define COLOR_RED 			0xFF6347FF
#define COLOR_ORANGE 		0xFAAC58FF
#define COLOR_BROWN 		0xA52A2AFF
#define COLOR_GREEN 		0x42B02CFF
#define COLOR_GOLD          0xFFD700FF
#define COLOR_LIGHTBLUE 	0x6495EDFF
#define COLOR_LIGHTGREEN 	0x9ACD32FF
#define COLOR_YELLOW 		0xFFFF00FF
#define COLOR_WHITE 		0xFFFFFFFF
#define COLOR_BLUENEW 		0xB8CEF6FF
#define COLOR_BLUE 			0x94B0C1FF
#define COLOR_PURPLE 		0xC2A2DAFF
#define COLOR_LIME 			0x10F441FF
#define COLOR_GREY 			0xAFAFAFFF
#define COLOR_CHAT			0xCECECEFF
#define COLOR_SCHAT			0xF0E68CFF
#define COLOR_DOCHAT		0x4682B4FF
#define COLOR_RACIO			0x2DB043FF
#define COLOR_DEPAR			0x3399ffFF
#define COLOR_ACHAT   		0x99CC00FF
#define COLOR_ME            0xFF99FFFF
#define COLOR_GOV           0x045FB4FF

new 
	global_str[4097],
	String256[256],
	String512[512],
	GlobalWeather = 2,
	GlobalTime,
	gUnixTime;

#if !defined isnull
	#define isnull(%0)  ((!(%0[0])) || (((%0[0]) == '\1') && (!(%0[1]))))
#endif

#define func%0(%1) \
    forward %0(%1); \
    public %0(%1)

#define or ||
#define f format
#define SCM	SendClientMessage
#define SCMALL	SendClientMessageToAll
#define SCMF(%0,%1,%2,%3) format(global_str, 144, %2,%3), SCM(%0, %1, global_str)

func PlayKick(playerid)
{
    Kick(playerid);
}
#define Kick(%0) SetTimerEx("PlayKick",1000,false,"i",%0)