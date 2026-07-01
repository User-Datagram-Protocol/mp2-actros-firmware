#include <stdio.h>

int main(void)
{

	/* Engine Brake State */
	int rpm	  	=1400;
	int speed_kmh 	=85;
	int throttle_pct =0;
	int brake_level =0;
	int gear 	=8;

	printf("=== MP2 ENGINE BRAKE ===\n");
	printf("RPM: %d\n", rpm);
	printf("Speed: %d km/h\n", speed_kmh);
	printf("Throttle %d%%\n", throttle_pct);
	printf("Gear: %d\n", gear);
	printf("Brake lvl: %d\n", brake_level);

	/* --- ENGINE BRAKE LOGIC --- */
if (throttle_pct > 0)
{
	printf("[EB] Engine brake inhibited throttle active\n");
}
else if (gear < 8)
{
	printf("[EB] Engine brake inhibited  gear tool low\n");
}
else if (rpm < 900)
{
	printf("[EB] Engine brake inhibited RPM too low\n");
}
else if (speed_kmh > 80 && rpm >=1400)
{
	printf("[EB] Engine brake LEVEL 2 full compression\n");
	printf("[EB] Braking force active on all cylinders\n");
}
else if  (speed_kmh > 50 && rpm >= 1100)
{
	printf("[EB] Engine brake LEVEL 1  partial compression/n");
}
else
{
	printf("[EB] Engine brake standby/n");
}


	return 0;

}
