#include "Flightdata.h"
#include "Gps.h"
#include "Dof.h"
#include "Ina.h"
#include "Bme280.h"
#include "Com.h"
#include "SdCard.h"

//actuators
#define ARM 12
#define FIRE_CUTDOWN_A 11
#define FIRE_CUTDOWN_B 10
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

void cutDown() {
	digitalWrite(ARM, LOW);
	digitalWrite(FIRE_CUTDOWN_A, HIGH);
	delay(100);
	digitalWrite(FIRE_CUTDOWN_A, LOW);
	delay(20);
	digitalWrite(FIRE_CUTDOWN_B, HIGH);
	delay(100);
	digitalWrite(FIRE_CUTDOWN_B, LOW);
	digitalWrite(ARM, HIGH);
}

void deployChute() {
	digitalWrite(ARM, LOW);
	digitalWrite(DEPLOY_CHUTE, HIGH);
	delay(100);
	digitalWrite(DEPLOY_CHUTE, LOW);
	digitalWrite(ARM, HIGH);

	flightdata.setFlightState(UNDER_CHUTES);
}

void setup() {
	pinMode(ARM, OUTPUT);
	pinMode(FIRE_CUTDOWN_A, OUTPUT);
	pinMode(FIRE_CUTDOWN_B, OUTPUT);
	pinMode(DEPLOY_CHUTE, OUTPUT);

	Serial.begin(9600);
	gps.init(kGpsRxPin, kGpsTxPin, flightdata);
	dof.init(flightdata);
	ina.init(flightdata);
	bme280.init(flightdata);
	//com.init(flightdata);
	sdCard.init(flightdata);
}

void loop() {

	if (flightdata.getFlightState() == ASCENT) {
		//gps.flightProcess(millis());
		dof.flightProcess(millis());
		ina.flightProcess(millis());
		bme280.flightProcess(millis());
		sdCard.flightProcess(millis());
		/*if (!comTransmitting) {
			com.flightProcess(millis());
		}*/
		Serial.println(flightdata.imuDetectsFreeFall());
		if ((flightdata.getRemoteAbort()) || /*(flightdata.imuDetectsFreeFall()) || */(flightdata.gpsDetectsFreeFall()) || (flightdata.barometerDetectsFreeFall()) || (flightdata.busVoltageLow())) {
			Serial.println("Cutdown.");
			//Serial.println(flightdata.busVoltageLow());
			cutDown();
			flightdata.setFlightState(FREEFALL);
		}

	}
	else if (flightdata.getFlightState() == FREEFALL) {
		delay(70500); //min time to 20,000 ft 70.5 sec
		deployChute();
		flightdata.setFlightState(UNDER_CHUTES);
	}
	gps.test(millis());
	dof.test(millis());
	ina.test(millis());
	bme280.test(millis());	

}