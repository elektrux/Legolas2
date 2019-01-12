#pragma once

#include "Flightdata.h"
#include "Subsystem.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>

const unsigned long deltaTimeFlightDof = 1000;
const unsigned long deltaTimeTestDof = 10000;


class Dof:Subsystem {
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