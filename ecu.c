#include <stdio.h>

int main(void) 
{
	/* OM541 Engine State */
	int rpm       =550;
	int coolent_temp = 82;
	int oil_temp = 95;
	float boost    = 0.1f;


	printf("=== MP2 ECU MONITOR ===\n");
	printf("RPM:   %d\n",  rpm);
	printf("Coolent: %d C\n", coolent_temp);
	printf("Boost: %.2f  bar\n", boost);
	printf("Oil:  %d C\n", oil_temp);	

	/* Coolent Temperature Checks */
if (coolent_temp < -10 || coolent_temp > 140)
{
	printf("[FAULT] Coolent sensor fault  impossible value\n");
}
else if (coolent_temp >= 110)	
{
	printf("[CRITICAL] Coolent temp shutdown!\n");
}
else if (coolent_temp >= 100)
{
	printf("[WARNING] Coolent temp high!\n");
}
else
{
	printf("[ECU] Coolent OK\n");
}

/* RPM Check */
if (rpm < 0 || rpm >3000)
{
	printf("[FAULT] RPM sensor fault  impossible value\n");
}
else if (rpm >= 1850)
{
	printf("[WARNING] Overspeed!\n");
}
else
{
	printf("[ECU] RPM OK\n");
}

/* Boost Check */
if (boost < -0.5f || boost > 4.0f)
{
	printf("[FAULT] Boost sensor fault impossible value\n");
}

else if (boost >= 2.2f)
{
	printf("[WARNING] Overboost!\n");
}
else
{
	printf("[ECU] Boost OK\n");
}

if (oil_temp < -20 || oil_temp > 180)
{
	printf("[FAULT]  Oil temp sensor fault  impossible value\n");
}
else if (oil_temp >= 155)
{
	printf("[SHUTDOWN] Oil temp critical  engine shutdown\n");
}
else if (oil_temp >= 145)
{
	printf("[CRITICAL] Oil temp  power derate active\n");
}
else if (oil_temp >= 130)
{
	printf("[WARNING] Oil  temp high\n");
}
else if (oil_temp < 60)
{
	printf("[WARNING] Oil cold  full load inhibited\n");
}
else
{
	printf("[ECU] Oil OK\n");

}

 	
	return 0;
}

