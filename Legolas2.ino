#include "Flightdata.h"
#include "Gps.h"
#include "Dof.h"
#include "Ina.h"
#include "Bme280.h"
#include "Com.h"
#include "SdCard.h"

//actuators
#define ARM 47
#define FIRE_CUTDOWN_A 43
#define FIRE_CUTDOWN_B 45
#define DEPLOY_CHUTE 49

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

bool ISBDCallback() {
	if (flightdata.getFlightState() == ASCENT) {

		gps.flightProcess(millis());
		dof.flightProcess(millis());
		ina.flightProcess(millis());
		bme280.flightProcess(millis());
		sdCard.flightProcess(millis());
		if ((flightdata.getRemoteAbort()) || (flightdata.imuDetectsFreeFall()) || (flightdata.gpsDetectsFreeFall()) || (flightdata.barometerDetectsFreeFall()) || (flightdata.busVoltageLow())) {
			Serial.println("\nCutdown.\n");
			cutDown();
			flightdata.setFlightState(FREEFALL);
		}

	}
	else if (flightdata.getFlightState() == FREEFALL) {
		delay(10000); //min time to 20,000 ft 70.5 sec
		deployChute();
		flightdata.setFlightState(UNDER_CHUTES);
		dof.flightProcess(millis());
		ina.flightProcess(millis());
		bme280.flightProcess(millis());
		sdCard.flightProcess(millis());
	}
	else if (flightdata.getFlightState() == UNDER_CHUTES) {
		if (flightdata.barometerDetectsLanding()) {
			flightdata.setFlightState(SAFING);
		}	
	}
	else if (flightdata.getFlightState() == SAFING) {

	}

	gps.test(millis());
	dof.test(millis());
	ina.test(millis());
	bme280.test(millis());
	return true;
}

void cutDown() {
	digitalWrite(ARM, HIGH);
	digitalWrite(FIRE_CUTDOWN_A, HIGH);
	delay(1000);
	digitalWrite(FIRE_CUTDOWN_A, LOW);
	delay(20);
	digitalWrite(FIRE_CUTDOWN_B, HIGH);
	delay(1000);
	digitalWrite(FIRE_CUTDOWN_B, LOW);
	digitalWrite(ARM, LOW);
}

void deployChute() {
	digitalWrite(ARM, HIGH);
	digitalWrite(DEPLOY_CHUTE, HIGH);
	delay(1000);
	digitalWrite(DEPLOY_CHUTE, LOW);
	digitalWrite(ARM, LOW);

	flightdata.setFlightState(UNDER_CHUTES);
}


void setup() {
	Serial.begin(9600);
	gps.init(kGpsRxPin, kGpsTxPin, flightdata);
	dof.init(flightdata);
	ina.init(flightdata);
	bme280.init(flightdata);
	com.init(flightdata);
	sdCard.init(flightdata);

	delay(10000);

	//run through all flight processes 1x to calibrate
	gps.flightProcess(millis());
	dof.flightProcess(millis());
	ina.flightProcess(millis());
	bme280.flightProcess(millis());
	sdCard.flightProcess(millis());
}

void loop() {
	com.flightProcess(millis());
	ISBDCallback();
}