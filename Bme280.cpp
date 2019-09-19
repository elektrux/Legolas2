#include "Bme280.h"
#include <Adafruit_BME280.h> //BME280 library

Adafruit_BME280 bme;

void Bme280::init(Flightdata& flightdata) {
	data = &flightdata;
	bme.begin(0x76);
	Serial.println("BME280 --> init");
}

void Bme280::test(unsigned long currTime) {
	if ((lastTestTime + deltaTimeTestBme) <= currTime) { 
		lastTestTime = currTime;

		Serial.print("Temp: ");
		Serial.println(data->getTemp());
		Serial.print("Pres: ");
		Serial.println(data->getPres());
		Serial.print("Alt: ");
		Serial.println(data->getAlt());
		Serial.print("Humidity: ");
		Serial.println(data->getHum());
	}
}

void Bme280::flightProcess(unsigned long currTime) {
	if ((lastActionTime + deltaTimeFlightBme) <= currTime) { 
		lastActionTime = currTime;

		data->setBme(bme.readTemperature(), bme.readPressure(), bme.readAltitude(SEALEVELPRESSURE_HPA), bme.readHumidity());
		Serial.println("BME280 --> flightProcess");
	}
}

void Bme280::groundProcess () {
	return;
}

void Bme280::teardown() {
	return;
}