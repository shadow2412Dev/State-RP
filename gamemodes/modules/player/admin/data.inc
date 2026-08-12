enum
{
	ADM_NONE = 0,
	ADM_MODER = 1,
	ADM_SUPER_MODER = 2,
	ADM_ADMIN = 3,
	ADM_SUPER_ADMIN = 4,
	ADM_VICE_GRAND_ADMIN = 5,
	ADM_GRAND_ADMIN = 6,
	ADM_GRAND = 7
};

#define NO_DUTY_TEXT    "[A] Вы не авторизованы. Используйте {33CCFF}/apanel"
new bool:ADuty[MAX_PLAYERS] = false;

new AdminPasswords[8][64] = {"", "9222", "5232", "4542", "2513", "2142", "2454", "700909"};