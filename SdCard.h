#pragma once
#include "Subsystem.h"
#include <SD.h>
#include "Flightdata.h"

const unsigned long deltaTimeFlightSdCard = 200;


class SdCard:Subsystem {
public:
	void init(Flightdata& flightdata);
	void test();
	void flightProcess(unsigned long currTime);
	void groundProcess();
	void teardown();
private:
	
	Flightdata *data;
	unsigned long lastActionTime = 0;

};
