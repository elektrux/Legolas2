#include "SdCard.h"

File dataLog;

void SdCard::init(Flightdata& flightdata) {
	data = &flightdata;
	pinMode(53, OUTPUT);
	if (!SD.begin(53)) {
		Serial.println("SD init failed.");
		return;
	}
	Serial.println("SD init.");
}

void SdCard::test() {
	return;
}

void SdCard::flightProcess(unsigned long currTime) {
	if ((lastActionTime + deltaTimeFlightSdCard) <= currTime) {
		lastActionTime = currTime;

		static char accelMag[15];
		static char accelX[15];
		static char accelY[15];
		static char accelZ[15];
		static char volt[15];
		static char lat[15];
		static char lon[15];
		static char GPSAlt[15];
		static char hour[15];
		static char minute[15];
		static char second[15];
		static char temp[15];
		static char pres[15];
		static char alt[15];
		static char hum[15];

		dtostrf(data->getAccelMag(), 9, 2, accelMag);
		dtostrf(data->getAccelX(), 9, 2, accelX);
		dtostrf(data->getAccelY(), 9, 2, accelY);
		dtostrf(data->getAccelZ(), 9, 2, accelZ);
		dtostrf(data->getVolt(), 9, 2, volt);
		dtostrf(data->getLat(), 9, 2, lat);
		dtostrf(data->getLon(), 9, 2, lon);
		dtostrf(data->getGPSAlt(), 9, 2, GPSAlt);
		dtostrf(data->getHour(), 9, 2, hour);
		dtostrf(data->getMinute(), 9, 2, minute);
		dtostrf(data->getSecond(), 9, 2, second);
		dtostrf(data->getTemp(), 9, 2, temp);
		dtostrf(data->getPres(), 9, 2, pres);
		dtostrf(data->getAlt(), 9, 2, alt);
		dtostrf(data->getHum(), 9, 2, hum);

		dataLog = SD.open("D://FlightLog.csv", FILE_WRITE);
		if (dataLog) {
			dataLog.println("testA,testB,testC");
			dataLog.close();
		}
		else {
			Serial.println("Logging failed.");
		}

	}
}

void SdCard::groundProcess() {
	return;
}

void SdCard::teardown() {
	return;
}