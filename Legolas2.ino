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
#define CAM_START 37

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

unsigned long dropStart;

bool ISBDCallback() {
	bool test = flightdata.chuteThresholdReachedBaro(); //for logging purposes
	test = flightdata.chuteThresholdReachedGPS();
	if (flightdata.getFlightState() == ASCENT) {
		gps.flightProcess(millis());
		dof.flightProcess(millis());
		ina.flightProcess(millis());
		bme280.flightProcess(millis());
		sdCard.flightProcess(millis());
		if (flightdata.imuDetectsFreeFall() || flightdata.gpsDetectsFreeFall() || flightdata.barometerDetectsFreeFall() || flightdata.busVoltageLow()) {
			cutDown();
			Serial.println("CUTDOWN.");
			debugFD();
			flightdata.setFlightState(FREEFALL);
		}
	}
	else if (flightdata.getFlightState() == FREEFALL) {
		gps.flightProcess(millis());
		dof.flightProcess(millis());
		ina.flightProcess(millis());
		bme280.flightProcess(millis());
		sdCard.flightProcess(millis());
		if (flightdata.chuteThresholdReachedBaro() || flightdata.chuteThresholdReachedGPS()) {
			deployChute();
			Serial.println("CHUTE DEPLOY.");
			debugFD();
			flightdata.setFlightState(UNDER_CHUTES);
		}
	}
	else if (flightdata.getFlightState() == UNDER_CHUTES) {
		gps.flightProcess(millis());
		dof.flightProcess(millis());
		ina.flightProcess(millis());
		bme280.flightProcess(millis());
		sdCard.flightProcess(millis());
	}
	else if (flightdata.getFlightState() == SAFING) {

	}

	gps.test(millis());
	dof.test(millis());
	ina.test(millis());
	bme280.test(millis());
	bool stupid = flightdata.barometerDetectsLanding();
	debugFD();
	
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
}


void setup() {
	Serial.begin(9600);

	pinMode(ARM, OUTPUT);
	pinMode(FIRE_CUTDOWN_A, OUTPUT);
	pinMode(FIRE_CUTDOWN_B, OUTPUT);
	pinMode(DEPLOY_CHUTE, OUTPUT);
	pinMode(CAM_START, OUTPUT);

	gps.init(kGpsRxPin, kGpsTxPin, flightdata);
	dof.init(flightdata);
	ina.init(flightdata);
	bme280.init(flightdata);
	com.init(flightdata);
	sdCard.init(flightdata);

	delay(5000);

	digitalWrite(CAM_START, HIGH);

	//run through all flight processes 1x to calibrate
	gps.flightProcess(millis());
	dof.flightProcess(millis());
	ina.flightProcess(millis());
	bme280.flightProcess(millis());
	sdCard.flightProcess(millis());

	Serial.println("Init complete.\n");
}

void loop() {
	//com.flightProcess(millis());
	ISBDCallback();
}

void debugFD() {
	Serial.println();
	Serial.print("imuDetectsFreeFall: ");
	Serial.println(flightdata.imuFreeFallWarning);
	Serial.print("gpsDetectsFreeFall: ");
	Serial.println(flightdata.gpsFreeFallWarning);
	Serial.print("barometerDetectsFreeFall: ");
	Serial.println(flightdata.baroFreeFallWarning);
	Serial.print("busVoltageLow: ");
	Serial.println(flightdata.lowVoltageWarning);
	Serial.print("chuteThresholdReachedBaro: ");
	Serial.println(flightdata.baroChuteDeployWarning);
	Serial.print("chuteThresholdReachedGPS: ");
	Serial.println(flightdata.GPSChuteDeployWarning);
	Serial.print("baroLandingWarning: ");
	Serial.println(flightdata.baroLandingWarning);
	Serial.println();
}



