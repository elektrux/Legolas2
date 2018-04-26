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
	float getVoltA() {
		return fVoltA;
	}
	float getVoltB() {
		return fVoltB;
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
		static char voltA[15];
		static char voltB[15];
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
		dtostrf(fVoltA, 9, 2, voltA);
		dtostrf(fVoltB, 9, 2, voltB);
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
        snprintf(telemString, sizeof(telemString), "AccelMag: %s\nVoltA: %s\nVoltB: %s\nLat:  %s\nLon:  %s\nGPS Alt: %s\nTime: %s:%s:%s\nTemp: %s\nPres: %s\nBaro Alt: %s\nHumidity: %s\n", 
        		accelMag, voltA, voltB, lat, lon, GPSAlt, hour, minute, second, temp, pres, alt, hum);

        return telemString;
	}
	int getFlightState() {
		return flightState;
	}
	bool getRemoteAbort() {
		return remoteAbort;
	}

	//situational assesment functions
	bool imuDetectsFreeFall() {
		float sum = 0;
		for (int i = 0; i < 6; i++) {
			sum += accelQueue[i];
		}
		float average = sum/6;
		if ((average > -2) && (average < 2)) {
			imuFreeFallWarning = true;
			return true;
		}
		else {
			return false;
		}
	}

	bool gpsDetectsFreeFall() {
		if (fGPSAlt < (highestGPSAlt - 200)) {
			gpsFreeFallWarning = true;
			return true;
		}
		else {
			return false;
		}
	}

	bool barometerDetectsFreeFall() {
		if (fAlt < (highestBaroAlt - 200)) {
			baroFreeFallWarning = true;
			return true;
		}
		else {
			return false;
		}
	}

	float busVoltageLow() {
		if (fVoltA < 3) { //!!!replace with actual values
			return fVoltA;
		}
	}

	//setters
	void setAccel(float x, float y, float z) {
		fAccelX = x;
		fAccelY = y;
		fAccelZ = z;
		for (int i = 0; i < 6; i++) { //add to beginning of queue and shift
			if (i ! = 5) {
				accelQueue[i] = accelQueue[i+1]; //shifts queue up
			}
			else {
				accelQueue[i] = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2)); //sets element 0 to most recent mag
			}
		}
	}
	void setVolt(float vA, float vB) {
		fVoltA = vA;
		fVoltB = vB;
	}
	void setPosition(float lat, float lon, float alt) {
		if (alt > highestGPSAlt) {
			highestGPSAlt = alt;
		}
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
		if (alt > highestBaroAlt) {
			highestBaroAlt = alt;
		}
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
	bool gpsFreeFallWarning = false;
	bool baroFreeFallWarning = false;
	bool imuFreeFallWarning = false;
private:
	int flightState = 1;


	float fAccelX = 0;
	float fAccelY = 0;
	float fAccelZ = 0;
	float fVoltA = 0;
	float fVoltB = 0;
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

	float accelQueue[6] = {10, 10, 10, 10, 10, 10};
	float highestGPSAlt = 0;
	float highestBaroAlt = 0;
};