#pragma once

#include "Subsystem.h"
#include "Flightdata.h"
#include <Adafruit_BME280.h> //BME280 library

#define SEALEVELPRESSURE_HPA (1013.25)

const unsigned long deltaTimeFlightBme = 1000; 
const unsigned long deltaTimeTestBme = 10000;

class Bme280:Subsystem {
public:
	void init(Flightdata& flightdata);
	void test(unsigned long currTime);
	void flightProcess(unsigned long currTime); 
	void groundProcess();
	void teardown();
private:
	unsigned long lastActionTime = 0;
	unsigned long lastTestTime = 0;
	Flightdata *data; 
};
