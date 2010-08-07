 #include "ds182x.h"
 #include "thermo.h"

float nativeGetTemperature() {
	u_int ign 		= init();
	u_int temp	= readTemperature(0);
	u_int frac;
	convertTemperature(0, &temp, &frac);	
	float ret = (float)temp + (frac / 10000.f);	
	return ret;
}

