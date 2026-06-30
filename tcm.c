#include <stdio.h>

int main(void)
{

	/* telligent transmission state */
	int rpm	= 1780;
	int gear	= 12;
	int speed_kmh	=72;
	int coolant_temp = 82;
	int load_pct	=85;

	printf("=== MP2 TCM MONITOR ==\n");
	printf("Gear:	%d\n",	gear);
	printf("RPM:	%d\n",	rpm);
	printf("Speed:	%d km/h\n", speed_kmh);
	printf("Load:	%d%%\n", load_pct);
	
	/* --- UPSHIFT LOGIC --- */
if (rpm > 1900)
{
	printf("[TCM] SHIFT DENIED   E001 Overspeed inhibit\n");
	printf("[TCM] Upshift inhibited until RPM < 1700\n");
}

else if (coolant_temp < 60)
{
	printf("[TCM] SHIFT DENIED   E003 Cold inhibit\n");
	printf("[TCM] Coolant: %d C   minimum 60 C required\n", coolant_temp);
}
else if (gear >= 12)
{
	printf("[TCM]  No shift   already in top gear (12\n");
}
else if (load_pct > 80 && rpm >= 1750)
{
	printf("[TCM] Shift request UP   heavy load\n");
	printf("[TCM] Conditions OK   engaging gear %d\n", gear + 1);
}
else if (load_pct <= 80 && rpm >= 1700)
{
	printf("[TCM] Shift request UP    normal load\n");
	printf("[TCM] Conditions OK   engaging gear %d\n", gear + 1);
} 
else
{
	printf("[TCM] No shift   RPM %d below upshift window\n", rpm);
}

/*   DOWNSHIFT LOGIC   */
if (coolant_temp < 60)
{
	printf("[TCM] SHIFT DENIED   E003 Cold inhibit\n");
}
else if (rpm <= 1000 && gear > 1)
{
	printf("[TCM] Shift request Down\n");
	printf("[TCM] Conditions OK    engaging gear %d\n", gear - 1);
}
else
{
	printf("[TCM] No downshift   RPM %d above downshift window\n", rpm);
}

	return 0;
}
