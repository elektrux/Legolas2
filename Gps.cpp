#include "Gps.h"

void Gps::init(int rx, int tx, Flightdata &flightdata) {
	data = &flightdata;
	//gpsSerial = SoftwareSerial(62, 63);
	gpsSerial.begin(9600);
	gpsCom.begin(gpsSerial);
	gpsCom.setUART1Output(COM_TYPE_UBX);
	setNavModeFlight();
	Serial.print("GPS nav mode set to: ");
	Serial.println(getNavMode());
	gpsCom.setNavigationFrequency(4);
	gpsCom.saveConfiguration();
	while (gpsCom.getSIV() < 3) {
		Serial.println("Acquiring GPS lock...");
	}
	Serial.println("GPS --> init");
}

void Gps::test(unsigned long currTime) {
    if ((lastTestTime + deltaTimeTestGps) <= currTime) {
		lastTestTime = currTime;

		Serial.print("Lat: ");
		Serial.println(data->getLat());
		Serial.print("Lon: ");
		Serial.println(data->getLon());
		Serial.print("Alt: ");
		Serial.println(data->getGPSAlt());
		Serial.print("Time: ");
		Serial.print(data->getHour());
		Serial.print(":");
		Serial.print(data->getMinute());
		Serial.print(":");
		Serial.println(data->getSecond());

	}
}

void Gps::flightProcess(unsigned long currTime) {

    if ((lastActionTime + deltaTimeFlightGps) <= currTime) {
		lastActionTime = currTime;

		data->setPosition(gpsCom.getLatitude(), gpsCom.getLongitude(), (gpsCom.getAltitude() / 1000));
		data->setTime(gpsCom.getHour(), gpsCom.getMinute(), gpsCom.getSecond());
		data->setSats(gpsCom.getSIV());

		Serial.println("GPS --> flightProcess");

	}
}

void Gps::groundProcess() {
    return;
}

void Gps::teardown() {
    return;
}

// Set the navigation mode (Airborne, 1G)
void Gps::setNavModeFlight() {
	uint8_t command[] = {0xB5, 0x62, 0x06, 0x24, 0x24, 0x00, 0xFF, 0xFF, 0x06, 0x03, 0x00, 0x00, 0x00, 0x00, 0x10, 0x27, 0x00, 0x00, 0x05, 0x00, 0xFA, 0x00, 0xFA, 0x00, 0x64, 0x00, 0x2C, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x16, 0xDC};
	for (int i = 0; i < sizeof(command)/sizeof(uint8_t); i++) {
		gpsSerial.write(command[i]);
	}
}

int Gps::getNavMode() {
	// Query
	uint8_t request[8] = {0xB5, 0x62, 0x06, 0x24, 0x00, 0x00, 0x2A, 0x84};
	for (int i = 0; i < 8; i++) {
		gpsSerial.write(request[i]);
	}

	// Receive response
	uint8_t buf[90];
	int i = 0;
	gpsSerial.listen();
	while (gpsSerial.available()) {
		if (i > 90) {
			return -1;
		}
		buf[i] = gpsSerial.read();
		i++;
	}

	// Verify sync and header bytes
	/*if( buf[0] != 0xB5 || buf[1] != 0x62 ){
		return -2;
	}
	if( buf[2] != 0x06 || buf[3] != 0x24 ){
		return -3;
	}*/

	return buf[8];
}