 #include "ds182x.h"
 #include "thermo.h"

float NativeGetTemperature() {
	u_int ign 		= TM_Init();
	u_int temp	= TM_Read_temperature(0);
	u_int frac;
	TM_Convert_temperature(0, &temp, &frac);	
	float ret = temp + (frac / 10000);	
	return ret;
}

