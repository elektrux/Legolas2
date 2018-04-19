#include "Flightdata.h"
#include "Gps.h"
#include "Dof.h"
#include "Ina.h"
#include "Bme280.h"
#include "Com.h"
#include "SdCard.h"

//pins
const int kGpsRxPin = 62; //switched wiring A8
const int kGpsTxPin = 63; //A9

Flightdata flightdata;
Gps gps;
Dof dof;
Ina ina;
Bme280 bme280;
Com com;
SdCard sdCard;

void cutDown() {
	//do cutDown
	flightdata.setFlightState(FREEFALL);
	return;
}

void setup() {
	Serial.begin(9600);
	gps.init(kGpsRxPin, kGpsTxPin, flightdata);
	dof.init(flightdata);
	ina.init(flightdata);
	bme280.init(flightdata);
	//com.init(flightdata);
	sdCard.init(flightdata);
}

void loop() {
	gps.flightProcess(millis());
	dof.flightProcess(millis());
	ina.flightProcess(millis());
	bme280.flightProcess(millis());
	//com.flightProcess(millis());
	sdCard.flightProcess(millis());

	gps.test(millis());
	dof.test(millis());
	ina.test(millis());
	bme280.test(millis());



	//actuation logic
	if (flightdata.getFlightState() == ASCENT) {
		if (flightdata.getRemoteAbort()) {
			cutDown();
		}
	}
	

}