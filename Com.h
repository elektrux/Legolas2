#pragma once
#include "Subsystem.h"
#include <IridiumSBD.h>
#include "Flightdata.h"

//const unsigned long deltaTimeFlightCom = 120000;
const unsigned long deltaTimeFlightCom = 20000;


class Com:Subsystem {
public:
	void init(Flightdata& flightdata);
	void test();
	void flightProcess(unsigned long currTime);
	void groundProcess();
	void teardown();
private:
	
	Flightdata *data;
	unsigned long lastActionTime = 0;
	int signalQuality = -1;
  	int err;

};
