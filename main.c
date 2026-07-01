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
	int compressor_active;
	float compressor_build_rate;
	int handbrake_engaged;
	float wheel_speed_front;
	float wheel_speed_rear;
	int throttle_pct;
	float fuel_level_liters;
	float intake_air_temp;
	float afr;
	float fuel_rate_lh;
	float economy_l100km;
}     ActrosState;
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
	/* --- SPRING BRAKE LOGIC --- */
if (truck->air_pressure_bar < 4.5f)
{
	truck->handbrake_engaged = 1;
	printf("[AIR] CRITICAL spring brakes engaged!\n");
	printf("[AIR] Pressure too low to release handbrake\n");
}
else if (truck->air_pressure_bar >= 5.5f && truck->handbrake_engaged == 1)
{
	truck->handbrake_engaged = 0;
	printf("[AIR] Pressure restored spring brakes released\n");
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

void check_fuel(ActrosState *truck)
{
	/* Fuel burn based on RPM */
if (truck->rpm > 1500)
	truck->fuel_level_liters -= 0.8f;
else if (truck->rpm > 1000)
	truck->fuel_level_liters -= 0.4f;
else
	truck->fuel_level_liters -= 0.1f;

	/* Fault checks */
if (truck->fuel_level_liters < 0.0f)
	truck->fuel_level_liters = 0.0f;

if (truck->fuel_level_liters == 0.0f)
	printf("[FUEL] CRITICAL  tank empty, engine shutdown!\n");
else if (truck->fuel_level_liters < 50.0f)
	printf("[FUEL] WARNING low fuel: %.1f L remaining\n", truck->fuel_level_liters);
else
	printf("[FUEL] Tank: %.1f L\n", truck->fuel_level_liters);
}

void calculate_afr(ActrosState *truck)
{
	/* Air mass estimate from boost and intake temp */
	/* Hottor air = less dense = less oxygen */
	float air_density = truck->boost / (1.0f + (truck->intake_air_temp - 20.0f) * 0.003f);

	/* Calculate AFR based on load and air density */
if (truck->rpm < 700)
{
	printf("[EDC] idle mode  AFR not monitored\n");
	printf("[EDC] Fuel rate: %.1f L/h\n", truck->fuel_rate_lh);
	printf("[EDC] Economy: N/A (stationary)\n");
	return;
}
if (truck->load_pct > 80)
	truck->afr = 14.0f * air_density;
else if (truck->load_pct > 50)
	truck->afr = 18.0f * air_density;
else if (truck->load_pct > 20)
	truck->afr = 28.0f * air_density;
else
	truck->afr = 50 * air_density;

	/* Fuel rate based on RPM and load */
	truck->fuel_rate_lh = (truck->rpm / 1000.0f) * (truck->load_pct / 100.0f) * 45.0f;

	/* Economy avoid divide by zero */
if (truck->speed_kmh > 0)
	truck->economy_l100km = (truck->fuel_rate_lh / truck->speed_kmh) * 100.0f;

	/* Display */
	printf("[EDC] AFR:  %.1f : 1\n", truck->afr);
	printf("[EDC] Fuel rate: %.1f L/h\n", truck->fuel_rate_lh);

if (truck->speed_kmh > 0)
	printf("[EDC] Economy: %.1f L/100km\n", truck->economy_l100km);
else
	printf("[EDC] Economy: N/A (stationary)\n");

	/* AFR fault checks */
if (truck->afr < 14.0f)
	printf("[EDU] WARNING  running rich, black smoke risk\n");
else if (truck->afr > 60.0f)
	printf("[EDC] WARNING  running too lean, power loss\n");
else
	printf("[ECU] AFR OK\n");
}

void update_compressor(ActrosState *truck)
{
	/* No engine no compressor */
if (truck->rpm < 500)
{
	truck->compressor_active = 0;
	printf("[COMP] Compressor offline engine off\n");
	return;
}

	/* Governor logic */
if (truck->air_pressure_bar >= 8.5f)
{
	truck->compressor_active = 0;
	printf("[COMP] Governor cutout pressure nominal\n");
}
else if (truck->air_pressure_bar <= 7.2f)
{
	truck->compressor_active = 1;
	/* Build rate based on RPM */
	truck->compressor_build_rate = (truck->rpm / 1000.0f) * 0.3f;
	truck->air_pressure_bar += truck->compressor_build_rate;
	printf("[COMP] Building pressure %.2f bar/cycle\n", truck->compressor_build_rate);
}
else
{
	printf("[COMP] Pressure in range standby\n");
}

	printf("[COMP] Tank: %.1f bar\n", truck->air_pressure_bar);
}



	int main(void)
	{

	ActrosState truck = {550, 1, 82, 95, 0.1f, 0, 5, 8.5f, 0.0f, 0.0f, 0, 1, 400.0f, 35.0f, 0.0f, 0.0f, 0.0f};

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
	check_fuel(&truck);
	calculate_afr(&truck);
	update_compressor(&truck);
	return 0;
}

