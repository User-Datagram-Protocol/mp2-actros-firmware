#include <stdio.h>

typedef struct {
	int rpm;
	int gear;
	int coolant_temp;
	int oil_temp;
	float boost;
	int speed_kmh;
	int load_pct;
	float air_pressure_bar;
	float wheel_speed_front;
	float wheel_speed_rear;
	int throttle_pct;
  } ActrosState;
void check_engine(ActrosState *truck)
{
if (truck->coolant_temp >= 110)
{
	printf("[ECU] CRITICAL   Coolant shutdown!\n");
}
else if (truck->coolant_temp >= 100)
{
	printf("[ECU] WARNING   Coolant high!\n");
}
else
{
	printf("[ECU] Coolant OK\n");
}

if (truck->rpm >= 1850)
{
	printf("[ECU] WARNING   Overspeed!\n");
}
else
{
	printf("[ECU] RPM OK\n");
  }
}

void check_transmission(ActrosState *truck)
{
if (truck->coolant_temp < 60)
{
	printf("[TCM] SHIFT DENIED   E003 Cold inhibit\n");
}
else if (truck->gear >= 12)
{
	printf("[TCM] No shift   already in top gear (12)\n");
}
else if (truck->rpm >  1900)
{
	printf("[TCM] SHIFT DENIED   E001 Overspeed inhibit\n");
}
else if (truck->load_pct > 80 && truck->rpm >= 1750)
{
	printf("[TCM] Shift UP   heavy load   engaging gear %d\n", truck->gear + 1);
}
else if (truck->load_pct <= 80 && truck->rpm >= 1700)
{
	printf("[TCM] Shift UP   normal load   engaging gear %d\n", truck->gear + 1);
}
else if (truck->rpm <= 1000 && truck->gear > 1)
{
	printf("[TCM] Shift DOWN   engaging gear %d\n", truck->gear - 1);
}
else
{
	printf("[TCM] No shift   RPM %d\n", truck->rpm);
}
}

void check_air(ActrosState *truck)
{
if (truck->air_pressure_bar < 5.5f)
{
	printf("[AIR] CRITICAL   pressure too low for safe braking!\n");
}
else if (truck->air_pressure_bar < 7.0f)
{
	printf("[AIR] WARNING low tank pressure\n");
}
else
{
	printf("[AIR] Tank pressure OK\n");
}

if (truck->air_pressure_bar < 8.0f)
{
	printf("[AIR] Dryer cycling ON\n");
}
else
{
	printf("[AIR] Dryer standby\n");
}
}

void check_abs(ActrosState *truck)
{
if (truck->air_pressure_bar < 5.5f)
{
	printf("[ABS] WARNING  low air pressure, reduced ABS effectiveness\n");
}
else if ((truck->speed_kmh - truck->wheel_speed_front) > 15.0f)
{
	printf("[ABS] ALERT front wheel lockup detected!\n");
	printf("[ABS] Pulsing brake pressure front axle\n");
}
else if ((truck->speed_kmh - truck->wheel_speed_rear) > 15.0f)
{
	printf("[ABS] ALERT rear wheel lockup detected!\n"); 
        printf("[ABS] Pulsing brake pressure rear axle\n");
}
else
{
	printf("[ABS] All wheels rolling normally\n");
} 
} 

void check_engine_brake(ActrosState *truck)
{
if (truck->throttle_pct > 0)
{
	printf("[EB] Engine brake inhibited throttle active\n");
}
else if (truck->gear < 8)
{
	printf("[EB] Engine brake inhibited gear too low\n");
}
else if (truck->rpm < 900)
{
	printf("[EB] Engine brake inhibited RPM too low\n");
}
else if (truck->speed_kmh > 80 &&truck->rpm >= 1400)
{
	printf("[EB] Engine brake LEVEL 2  full compression\n");
	printf("[EB] Brakeing force active on all cylinders\n");
}
else if (truck->speed_kmh > 50 && truck->rpm >= 1100)
{
	printf("[EB] Engine brake LEVEL 1  partial compression\n");
}
else
{
	printf("[EB] Engine brake standby\n");
}
}

void update_rpm(ActrosState *truck)
{
if (truck->throttle_pct == 0)
	truck->rpm = 550;
else if (truck->throttle_pct == 25)
	truck->rpm = 900;
else if (truck->throttle_pct == 50)
	truck->rpm = 1200;
else if (truck->rpm == 75)
	truck->rpm = 1500;
else
	truck->rpm = 1750;
}

void update_boost(ActrosState *truck)
{
if (truck->rpm < 700)
	truck->boost = 0.1f;
else if (truck->rpm < 1000)
	truck->boost = 0.4f;
else if (truck->rpm < 1200)
	truck->boost = 0.8f;
else if (truck->rpm < 1400)
	truck->boost = 1.2f;
else if (truck->rpm < 1600)
	truck->boost = 1.6f;
else
	truck->boost = 2.0f;
} 

	int main(void)
	{

	ActrosState truck = {550, 1, 82, 95, 0.1f, 0, 5, 8.5f, 0.0f, 0.0f, 0};
	char input;

	printf("Controls: w = throttle up | s = throttle down | q = quit\n");

while  (1)
	{
	  update_rpm(&truck);
	  update_boost(&truck);

	printf("=== MP2 ACTROS SYSTEM ===\n");
	printf("Throttle: %d%%\n", truck.throttle_pct);
	printf("RPM:   %d\n", truck.rpm);
	printf("Boost: %.2f bar\n", truck.boost);
	printf("Gear:  %d\n", truck.gear);

	
        check_engine(&truck);
	check_transmission(&truck);	
	check_air(&truck);
	check_abs(&truck);
	check_engine_brake(&truck);
	
	printf("Input: ");
	fflush(stdout);
	input = getchar();
while(getchar() != '\n');

	printf("DEBUG throttle before update: %d\n", truck.throttle_pct);

if (input == 'q') break;
else if (input == 'w' && truck.throttle_pct < 100)
		truck.throttle_pct += 25;
else if (input == 's' && truck.throttle_pct > 0)
		truck.throttle_pct -= 25;
}

	printf("\n[SYSTEM] Engine shutdown. \n");
	return 0;
}

