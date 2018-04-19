#include "Ina.h"

Adafruit_INA219 ina219A;
Adafruit_INA219 ina219B(0x41);

void Ina::init(Flightdata &flightdata) {
	data = &flightdata;
	ina219A.begin();
	ina219B.begin();
	Serial.println("INA A, INA B init.");
}

void Ina::test(unsigned long currTime) {
	if ((lastTestTime + deltaTimeTestIna) <= currTime) {
		lastTestTime = currTime;

		Serial.print("VoltageA: ");
		Serial.println(data->getVoltA());
		Serial.print("VoltageB: ");
		Serial.println(data->getVoltB());
	}
}

void Ina::groundProcess() {
	return;
}

void Ina::flightProcess(unsigned long currTime) {
	if ((lastActionTime + deltaTimeFlightIna) <= currTime) {
		lastActionTime = currTime;

		float voltageA = ina219A.getBusVoltage_V();
		float voltageB = ina219B.getBusVoltage_V();
		data->setVolt(voltageA, voltageB);

		Serial.println("Ina ---> flightProcess");
	}
}

void Ina::teardown() {
	return;
}