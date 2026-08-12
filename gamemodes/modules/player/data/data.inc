enum pInfo
{
	pID,
	pNickName[MAX_PLAYER_NAME],
	pPassword[64],
	pRegIP[16],
	pIP[16],
	pLastIP[16],
	pReferal[24],
	pMail[50],
	pLevel,
	pExp,
	pAdmin,
	pMoney,
	pBank
};
new PI[MAX_PLAYERS][pInfo];

new PI_EOS[pInfo] =
{
	0, // pID	
	"The State", // pNickName
	"", // pPassword
	"No IP Adress", // pRegIP
	"No IP Adress", // pIP
	"No IP Adress", // pLastIP
	"No Referal", // pReferal
	"No Mail Adress", // pMail
	1, // pLevel
	0, // pExp
	0, // pAdmin
	500, // pMoney
	0 // pBank
};

#define PN(%0) PI[%0][pNickName]