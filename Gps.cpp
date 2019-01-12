#include "Gps.h"



void Gps::init(int rx, int tx, Flightdata &flightdata) {
	data = &flightdata;
	gpsSerial = new SoftwareSerial(rx, tx);
	gpsSerial->begin(9600);
	setupGPS();
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
		gps_check_nav();

		if(navmode != 6) {
			// Check and set the navigation mode (Airborne, 1G)
			uint8_t setNav[] = {0xB5, 0x62, 0x06, 0x24, 0x24, 0x00, 0xFF, 0xFF, 0x06, 0x03, 0x00, 0x00, 0x00, 0x00, 0x10, 0x27, 0x00, 0x00, 0x05, 0x00, 0xFA, 0x00, 0xFA, 0x00, 0x64, 0x00, 0x2C, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x16, 0xDC};
			sendUBX(setNav, sizeof(setNav)/sizeof(uint8_t));
		}

		gps_check_lock();
		gps_get_position();
		gps_get_time();

		Serial.println("GPS --> flightProcess");

		//battV = analogRead(2);
		// n=sprintf (superbuffer, "$$EURUS,%d,%02d:%02d:%02d,%ld,%ld,%ld,%d,%d,%d,%d", count, hour, minute, second, lat, lon, alt, sats, lock, battV, navmode);
		// n = sprintf (superbuffer, "%s*%04X\n", superbuffer, gps_CRC16_checksum(superbuffer));

		// Serial.println(superbuffer);
	}
}

void Gps::groundProcess() {
    return;
}

void Gps::teardown() {
    return;
}

// Send a byte array of UBX protocol to the GPS
void Gps::sendUBX(uint8_t *MSG, uint8_t len) {
	for(int i=0; i<len; i++) {
		gpsSerial->write(MSG[i]);
	}
	//gpsSerial.println();
}

uint16_t Gps::gps_CRC16_checksum (char *string)
{
	size_t i;
	uint16_t crc;
	uint8_t c;

	crc = 0xFFFF;

	// Calculate checksum ignoring the first two $s
	for (i = 2; i < strlen(string); i++)
	{
		c = string[i];
		crc = _crc_xmodem_update (crc, c);
	}

	return crc;
}

void Gps::setupGPS() {
	//Turning off all GPS NMEA strings apart on the uBlox module
	gpsSerial->println("$PUBX,40,GLL,0,0,0,0*5C");
	delay(100);
	gpsSerial->println("$PUBX,40,GGA,0,0,0,0*5A");
	delay(100);
	gpsSerial->println("$PUBX,40,GSA,0,0,0,0*4E");
	delay(100);
	gpsSerial->println("$PUBX,40,RMC,0,0,0,0*47");
	delay(100);
	gpsSerial->println("$PUBX,40,GSV,0,0,0,0*59");
	delay(100);
	gpsSerial->println("$PUBX,40,VTG,0,0,0,0*5E");
	delay(3000); // Wait for the GPS to process all the previous commands

	// Check and set the navigation mode (Airborne, 1G)
	uint8_t setNav[] = {0xB5, 0x62, 0x06, 0x24, 0x24, 0x00, 0xFF, 0xFF, 0x06, 0x03, 0x00, 0x00, 0x00, 0x00, 0x10, 0x27, 0x00, 0x00, 0x05, 0x00, 0xFA, 0x00, 0xFA, 0x00, 0x64, 0x00, 0x2C, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x16, 0xDC};
	sendUBX(setNav, sizeof(setNav)/sizeof(uint8_t));

}

void Gps::PSMgps(){
	 setupGPS();
	 //set GPS to Eco mode (reduces current by 4mA)
	 uint8_t setEco[] = {0xB5, 0x62, 0x06, 0x11, 0x02, 0x00, 0x00, 0x04, 0x1D, 0x85};
	 sendUBX(setEco, sizeof(setEco)/sizeof(uint8_t));
}

void Gps::gpsPower(int i){
	if(i == 0){
		//turn off GPS
		//  uint8_t GPSoff[] = {0xB5, 0x62, 0x02, 0x41, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x4D, 0x3B};
		uint8_t GPSoff[] = {0xB5, 0x62, 0x06, 0x04, 0x04, 0x00, 0x00, 0x00, 0x08, 0x00, 0x16, 0x74};
		sendUBX(GPSoff, sizeof(GPSoff)/sizeof(uint8_t));
		gpsstatus = 0;
	}
	else if (i == 1){
		//turn on GPS
		// uint8_t GPSon[] = {0xB5, 0x62, 0x02, 0x41, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x4C, 0x37};
		uint8_t GPSon[] = {0xB5, 0x62, 0x06, 0x04, 0x04, 0x00, 0x00, 0x00, 0x09, 0x00, 0x17, 0x76};
		sendUBX(GPSon, sizeof(GPSon)/sizeof(uint8_t));
		gpsstatus = 1;
		delay(1000);
		setupGPS();
	}
}

void Gps::gps_ubx_checksum(uint8_t* data, uint8_t len, uint8_t* cka,
				uint8_t* ckb)
{
		*cka = 0;
		*ckb = 0;
		for( uint8_t i = 0; i < len; i++ )
		{
				*cka += *data;
				*ckb += *cka;
				data++;
		}
}

bool Gps::_gps_verify_checksum(uint8_t* data, uint8_t len)
{
		uint8_t a, b;
		gps_ubx_checksum(data, len, &a, &b);
		if( a != *(data + len) || b != *(data + len + 1))
				return false;
		else
				return true;
}

/**
 * Get data from GPS, times out after 1 second.
 */
void Gps::gps_get_data()
{
		int i = 0;
		unsigned long startTime = millis();
		gpsSerial->listen();
		while (1) {
		// Make sure data is available to read
		if (gpsSerial->available()) {
			buf[i] = gpsSerial->read();
			i++;
		}
		// Timeout if no valid response in 1 seconds
		if (millis() - startTime > 1000) {
			break;
		}
		}
}

/**
 * Check the navigation status to determine the quality of the
 * fix currently held by the receiver with a NAV-STATUS message.
 */
void Gps::gps_check_lock()
{
		GPSerror = 0;
		gpsSerial->flush();
		// Construct the request to the GPS
		uint8_t request[8] = {0xB5, 0x62, 0x01, 0x06, 0x00, 0x00,
				0x07, 0x16};
		sendUBX(request, 8);

		// Get the message back from the GPS
		gps_get_data();
		// Verify the sync and header bits
		if( buf[0] != 0xB5 || buf[1] != 0x62 ) {
			GPSerror = 11;
		}
		if( buf[2] != 0x01 || buf[3] != 0x06 ) {
			GPSerror = 12;
		}

		// Check 60 bytes minus SYNC and CHECKSUM (4 bytes)
		if( !_gps_verify_checksum(&buf[2], 56) ) {
			GPSerror = 13;
		}

		if(GPSerror == 0){
		// Return the value if GPSfixOK is set in 'flags'
		if( buf[17] & 0x01 )
				lock = buf[16];
		else
				lock = 0;

		sats = buf[53];
		}
		else {
			lock = 0;
		}
}

/**
 * Poll the GPS for a position message then extract the useful
 * information from it - POSLLH.
 */
void Gps::gps_get_position()
{
		GPSerror = 0;
		gpsSerial->flush();
		// Request a NAV-POSLLH message from the GPS
		uint8_t request[8] = {0xB5, 0x62, 0x01, 0x02, 0x00, 0x00, 0x03,
				0x0A};
		sendUBX(request, 8);

		// Get the message back from the GPS
		gps_get_data();

		// Verify the sync and header bits
		if( buf[0] != 0xB5 || buf[1] != 0x62 )
				GPSerror = 21;
		if( buf[2] != 0x01 || buf[3] != 0x02 )
				GPSerror = 22;

		if( !_gps_verify_checksum(&buf[2], 32) ) {
			GPSerror = 23;
		}

		if(GPSerror == 0) {
			// 4 bytes of longitude (1e-7)
			lon = (int32_t)buf[10] | (int32_t)buf[11] << 8 |
					(int32_t)buf[12] << 16 | (int32_t)buf[13] << 24;
			//lon /= 1000;

			// 4 bytes of latitude (1e-7)
			lat = (int32_t)buf[14] | (int32_t)buf[15] << 8 |
					(int32_t)buf[16] << 16 | (int32_t)buf[17] << 24;
			//lat /= 1000;

			// 4 bytes of altitude above MSL (mm)
			alt = (int32_t)buf[22] | (int32_t)buf[23] << 8 |
					(int32_t)buf[24] << 16 | (int32_t)buf[25] << 24;
			alt /= 1000;
		}
		if (hasCycled) {
			data->setPosition(lat, lon, alt);
		}
		hasCycled = true;

}

/**
 * Get the hour, minute and second from the GPS using the NAV-TIMEUTC
 * message.
 */
void Gps::gps_get_time()
{
		GPSerror = 0;
		gpsSerial->flush();
		// Send a NAV-TIMEUTC message to the receiver
		uint8_t request[8] = {0xB5, 0x62, 0x01, 0x21, 0x00, 0x00,
				0x22, 0x67};
		 sendUBX(request, 8);

		// Get the message back from the GPS
		gps_get_data();

		// Verify the sync and header bits
		if( buf[0] != 0xB5 || buf[1] != 0x62 )
				GPSerror = 31;
		if( buf[2] != 0x01 || buf[3] != 0x21 )
				GPSerror = 32;

		if( !_gps_verify_checksum(&buf[2], 24) ) {
			GPSerror = 33;
		}

		if(GPSerror == 0) {
			if(hour > 23 || minute > 59 || second > 59)
			{
				GPSerror = 34;
			}
			else {
				hour = buf[22];
				minute = buf[23];
				second = buf[24];
			}
		}

		data->setTime(hour, minute, second);
}

/**
 * Verify that the uBlox 6 GPS receiver is set to the <1g airborne
 * navigaion mode.
 */
uint8_t Gps::gps_check_nav(void)
{
		uint8_t request[8] = {0xB5, 0x62, 0x06, 0x24, 0x00, 0x00,
				0x2A, 0x84};
		sendUBX(request, 8);

		// Get the message back from the GPS
		gps_get_data();

		// Verify sync and header bytes
		if( buf[0] != 0xB5 || buf[1] != 0x62 ){
			GPSerror = 41;
		}
		if( buf[2] != 0x06 || buf[3] != 0x24 ){
			GPSerror = 42;
		}
		// Check 40 bytes of message checksum
		if( !_gps_verify_checksum(&buf[2], 40) ) {
			GPSerror = 43;
		}

		// Return the navigation mode and let the caller analyse it
		navmode = buf[8];
}
