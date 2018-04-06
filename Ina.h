#pragma once

#include "Subsystem.h"
#include "Flightdata.h" 
#include <Adafruit_INA219.h>

const unsigned long deltaTimeFlightIna = 1000;
const unsigned long deltaTimeTestIna = 10000;

class Ina:Subsystem {
public:
	void init(Flightdata &flightdata);
	void test(unsigned long currTime);
	void flightProcess(unsigned long currTime);
	void groundProcess();
	void teardown();
private:
	Flightdata *data;
	unsigned long lastActionTime = 0;
	unsigned long lastTestTime = 0;
};