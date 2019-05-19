#ifndef LEGOLAS_GPS_H
#define LEGOLAS_GPS_H

#include "Subsystem.h"
#include "Flightdata.h"
#include <SoftwareSerial.h>
#include <SparkFun_Ublox_Arduino_Library.h>

const unsigned long deltaTimeFlightGps = 1000;
const unsigned long deltaTimeTestGps = 1000;

class Gps:Subsystem {
    public:
    void init(int rx, int tx, Flightdata &flightdata);
    void test(unsigned long currTime);
    void flightProcess(unsigned long currTime);
    void groundProcess();
    void teardown();

	private:
	Flightdata *data;
	SoftwareSerial gpsSerial = SoftwareSerial(62, 63);
	SFE_UBLOX_GPS gpsCom;
	unsigned long lastActionTime = 0;
	unsigned long lastTestTime = 0;

	void setNavModeFlight();
	int getNavMode();

};

#endif //LEGOLAS_GPS_H