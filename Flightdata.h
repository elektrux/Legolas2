#pragma once
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#define SAFING 0
#define ASCENT 1
#define FREEFALL 2
#define UNDER_DROGUES 3
#define UNDER_CHUTES 4

class Flightdata {
public:
	//getters
	float getAccelX() {
		return fAccelX;
	}
	float getAccelY() {
		return fAccelY;
	}
	float getAccelZ() {
		return fAccelZ;
	}
	float getAccelMag() {
		return sqrt(pow(fAccelX, 2) + pow(fAccelY, 2) + pow(fAccelZ, 2));
	}
	float getVolt() {
		return fVolt;
	}
	float getLat() {
		return fLat;
	}
	float getLon() {
		return fLon;
	}
	float getGPSAlt() {
		return fGPSAlt;
	}
	float getHour() {
		return iHour;
	}
	float getMinute() {
		return iMinute;
	}
	float getSecond() {
		return iSecond;
	}
	float getTemp() {
		return fTemp;
	}
	float getPres() {
		return fPres;
	}
	float getAlt() {
		return fAlt;
	}
	float getHum() {
		return fHum;
	}
	char* getStrFlightdata() { 
		static char accelMag[15];
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

		dtostrf(getAccelMag(), 9, 2, accelMag);
		dtostrf(fVolt, 9, 2, volt);
		dtostrf(fLat, 9, 2, lat);
		dtostrf(fLon, 9, 2, lon);
		dtostrf(fGPSAlt, 9, 2, GPSAlt);
		dtostrf(iHour, 9, 2, hour);
		dtostrf(iMinute, 9, 2, minute);
		dtostrf(iSecond, 9, 2, second);
		dtostrf(fTemp, 9, 2, temp);
		dtostrf(fPres, 9, 2, pres);
		dtostrf(fAlt, 9, 2, alt);
		dtostrf(fHum, 9, 2, hum);
		

        char telemString[340]; //max Iridium send size
        snprintf(telemString, sizeof(telemString), "AccelMag: %s\nVolt: %s\nLat:  %s\nLon:  %s\nGPS Alt: %s\nTime: %s:%s:%s\nTemp: %s\nPres: %s\nBaro Alt: %s\nHumidity: %s\n", 
        		accelMag, volt, lat, lon, GPSAlt, hour, minute, second, temp, pres, alt, hum);

        return telemString;
	}
	int getFlightState() {
		return flightState;
	}
	bool getRemoteAbort() {
		return remoteAbort;
	}

	//setters
	void setAccel(float x, float y, float z) {
		fAccelX = x;
		fAccelY = y;
		fAccelZ = z;
	}
	void setVolt(float v) {
		fVolt = v;
	}
	void setPosition(float lat, float lon, float alt) {
		fLat = lat;
		fLon = lon;
		fGPSAlt = alt;
	}
	void setTime(int h, int m, int s) {
		iHour = h;
		iMinute = m;
		iSecond = s;
	}
	void setBme(float temp, float pres, float alt, float hum) {
		fTemp = temp;
		fPres = pres;
		fAlt = alt;
		fHum = hum;
	}
	void setRemoteAbort() { //lets com call an abort from received message
		remoteAbort = true;
	}
	void setFlightState(int state) {
		flightState = state;
	}

private:
	int flightState = 0;

	float fAccelX = 0;
	float fAccelY = 0;
	float fAccelZ = 0;
	float fVolt = 0;
	float fLat = 0;
	float fLon = 0;
	float fGPSAlt = 0;
	int iHour = 0;
	int iMinute = 0;
	int iSecond = 0;
	float fTemp = 0;
	float fPres = 0;
	float fAlt = 0;
	float fHum = 0;

	bool remoteAbort = false;
};