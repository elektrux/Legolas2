#ifndef LEGOLAS_GPS_H
#define LEGOLAS_GPS_H

#include "Subsystem.h"
#include "Flightdata.h"
#include <SPI.h>
#include <util/crc16.h>
#include <SoftwareSerial.h>

const unsigned long deltaTimeFlightGps = 10000;
const unsigned long deltaTimeTestGps = 10000;

class Gps:Subsystem {
    public:
    void init(int rx, int tx, Flightdata &flightdata);
    void test(unsigned long currTime);
    void flightProcess(unsigned long currTime);
    void groundProcess();
    void teardown();

	private:
	Flightdata *data; 

	bool hasCycled = false; // Prevent erronius first reading from triggering premature cutdown
	unsigned long lastActionTime = 0;
	unsigned long lastTestTime = 0;
	SoftwareSerial* gpsSerial;

	int32_t lat = 0, lon = 0, alt = 0;
	uint8_t hour = 0, minute = 0, second = 0, lock = 0, sats = 0;
	int GPSerror = 0, count = 1, n, gpsstatus, lockcount = 0, battV = 0;
	int navmode = 0, radio_power = 0;
	uint8_t buf[90]; //GPS receive buffer
	char superbuffer [80]; //Telem string buffer

	void sendUBX(uint8_t *MSG, uint8_t len);
	uint16_t gps_CRC16_checksum (char *string);
	void setupGPS();
	void PSMgps();
	void gpsPower(int i);
	void gps_ubx_checksum(uint8_t* data, uint8_t len, uint8_t* cka,
				uint8_t* ckb);
	bool _gps_verify_checksum(uint8_t* data, uint8_t len);
	void gps_get_data();
	void gps_check_lock();
	void gps_get_position();
	void gps_get_time();
	uint8_t gps_check_nav(void);

};


#endif //LEGOLAS_GPS_H
