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
		

        char telemString[175]; //max Iridium send size
        snprintf(telemString, sizeof(telemString), "%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s", 
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
		if ((GPSAltQueue[0] < (highestGPSAlt - 200)) && (GPSAltQueue[1] < (highestGPSAlt - 200)) && (GPSAltQueue[2] < (highestGPSAlt - 200))) { //only trigger if last 3 readings all are below max alt
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

	bool busVoltageLow() {
		if (fVoltA < 3) { //!!!replace with actual values
			lowVoltageWarning = true;
			return true;
		}
		else {
			return false;
		}
	}

	bool barometerDetectsLanding() {
		int currDist;
		for (int i = 0; i < 3; i++) {
			for (int j  = 0; j < 3; j++) {
				currDist = sqrt(pow(baroQueue[i] - baroQueue[j], 2));
				if (currDist > .1) return false;
			}
		}
		baroLandingWarning = true;
		return true;
	}

	bool chuteThresholdReachedBaro() {
		if (fAlt < 610) {
			baroChuteDeployWarning = true;
			return true;
		}
		else {
			return false;
		}
	}

	bool chuteThresholdReachedGPS() {
		if (fGPSAlt < 610) {
			GPSChuteDeployWarning = true;
			return true;
		}
		else {
			return false;
		}
	}

	//setters
	void setAccel(float x, float y, float z) {
		fAccelX = x;
		fAccelY = y;
		fAccelZ = z;
		for (int i = 0; i < 6; i++) { //add to beginning of queue and shift
			if (i != 5) {
				accelQueue[i] = accelQueue[i+1]; //shifts queue up
			}
			else {
				accelQueue[i] = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2)); 
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

		for (int i = 0; i < 3; i++) { //add to beginning of queue and shift
			if (i != 2) {
				GPSAltQueue[i] = GPSAltQueue[i+1];
			}
			else {
				GPSAltQueue[i] = alt;
			}
		}
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

		for (int i = 0; i < 3; i++) { //add to beginning of queue and shift
			if (i != 2) {
				baroQueue[i] = baroQueue[i+1];
			}
			else {
				baroQueue[i] = alt;
			}
		}
	}
	void setRemoteAbort() { //lets com call an abort from received message
		remoteAbort = true;
	}
	void setFlightState(int state) {
		flightState = state;
	}

	//publicly accessible trigger indicators for SD logging
	bool gpsFreeFallWarning = false;
	bool baroFreeFallWarning = false;
	bool imuFreeFallWarning = false;
	bool lowVoltageWarning = false;
	bool baroLandingWarning = false;

	//drop test indicators
	bool baroChuteDeployWarning = false;
	bool GPSChuteDeployWarning = false;
	
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
	float GPSAltQueue[3] = {0, 0, 0};
	float baroQueue[3] = {0, 0, 0};
	float highestGPSAlt = 0;
	float highestBaroAlt = 0;
};