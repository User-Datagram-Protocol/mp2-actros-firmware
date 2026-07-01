#include <stdio.h>

int main(void)
{
	
	/* ABS System State */
	float wheel_speed_front = 72.0f;
	float wheel_speed_rear = 50.0f;
	float truck_speed  = 72.0f;
	float brake_pressure  = 6.5f;
	int abs_active  = 0;


	printf("=== MP2 ABS SYSTEM ===\n");
	printf("Front wheel: %.1f km/h\n", wheel_speed_front);
	printf("Rear wheel: %.1f km/h\n", wheel_speed_rear);
	printf("Truck speed: %.1f km/h\n", truck_speed);
	printf("Brake pres: %.1f bar\n", brake_pressure);
	printf("ABS active: %s\n", abs_active ? "YES" : "NO");

	/* --- ABS LOCKUP DETECTION --- */
if (brake_pressure < 5.5f)
{
	printf("[ABS] WARNING  low air pressure, reduced ABS effectiveness\n");
}
else if ((truck_speed - wheel_speed_front) > 15.0f)
{
	printf("[ABS] ALERT  front wheel lockup detected!\n");
	printf("[ABS] Pulsing brake pressure front axle\n");
}
else if ((truck_speed - wheel_speed_rear) > 15.0f)
{
	printf("[ABS] ALERT  rear wheel lockup detected!\n");
	printf("[ABS] Pulsing brake pressure rear axle\n");
}
else
{
	printf("[ABS] All wheels rolling normally\n");
}


	return 0;

}
