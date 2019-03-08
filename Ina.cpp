#include "Ina.h"

Adafruit_INA219 ina219A;
Adafruit_INA219 ina219B(0x41);

void Ina::init(Flightdata &flightdata) {
	data = &flightdata;
	ina219A.begin();
	ina219B.begin();
	Serial.println("INA --> init");
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
		float currentA = ina219A.getCurrent_mA();
		float voltageB = ina219B.getBusVoltage_V();
		float currentB = ina219B.getCurrent_mA();
		data->setVolt(voltageA, currentA, voltageB, currentB);
		Serial.println("INA --> flightProcess");

	}
}

void Ina::teardown() {
	return;
}