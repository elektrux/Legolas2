#include "Flightdata.h"
#include "Gps.h"
#include "Dof.h"
#include "Ina.h"
#include "Bme280.h"
#include "Com.h"
#include "SdCard.h"

//actuators
#define ARM -1
#define FIRE_CUTDOWN_A -1
#define FIRE_CUTDOWN_B -1
#define DEPLOY_CHUTE -1

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
	pinMode(ARM, LOW);
	pinMode(FIRE_CUTDOWN_A, HIGH);
	delay(500);
	pinMode(FIRE_CUTDOWN_A, LOW);
	delay(100);
	pinMode(FIRE_CUTDOWN_B, HIGH);
	delay(500);
	pinMode(FIRE_CUTDOWN_B, LOW);
	pinMode(ARM, HIGH);
}

void deployChute() {
	pinMode(ARM, LOW);
	pinMode(DEPLOY_CHUTE, HIGH);
	delay(500);
	pinMode(DEPLOY_CHUTE, LOW);
	pinMode(ARM, HIGH);

	flightdata.setFlightState(UNDER_CHUTES);
}

void setup() {
	pinMode(ARM, OUTPUT);
	digitalWrite(ARM, HIGH);

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
		if ((flightdata.getRemoteAbort()) || (flightdata.imuDetectsFreeFall()) || (flightdata.gpsDetectsFreeFall()) || (flightdata.barometerDetectsFreeFall())) {
			cutDown();
			flightdata.setFlightState(FREEFALL);
		}

	}
	

}