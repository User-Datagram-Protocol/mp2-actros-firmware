#include <stdio.h>

int main(void)
{

	/* Air System State */
	float air_pressure_bar = 6.5f;
	float brake_damand_bar = 0.0f;
	int air_dryer_active = 0;

	printf("=== MP2 AIR SYSTEM ===\n");
	printf("Tank pressure: %.1f bar\n", air_pressure_bar);
	printf("Brake demand: %.1f bar\n", brake_damand_bar);
	printf("Air dryer: %s\n", air_dryer_active ? "ON" : "OFF");

	/*    AIR PRESSURE CHECKS   */
if (air_pressure_bar < 5.5f)
{
	printf("[AIR] CRITICAL pressure too low for safe braking!\n");
}
else if (air_pressure_bar <7.0f)
{
	printf("[AIR] WARNING  low tank pressure\n");
}
else
{
	printf("[AIR] Tank pressure OK\n");
}

/*   AIR DRYER CYCLE   */
if (air_pressure_bar <8.0f)
{
	printf("[AIR] Dryer cycling ON  building pressure\n");
}
else
{
	printf("[AIR] Dryer standby\n");
}

	return 0;
}

