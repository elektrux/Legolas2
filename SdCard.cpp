#include "SdCard.h"

File dataLog;

void SdCard::init(Flightdata& flightdata) {
	data = &flightdata;
	pinMode(53, OUTPUT);
	if (!SD.begin(53)) {
		Serial.println("SD --> init FAIL");
		return;
	}
	dataLog = SD.open("log.txt", FILE_WRITE);
	dataLog.println("SysTime,AccelMag,AccelX,AccelY,AccelZ,VoltA,VoltB,CurA,CurB,Latitude,Longitude,GPSAlt,Hour,Minute,Second,Sats,Temp,Pres,BaroAlt,Humidity,HighestGPSAlt,HighestBaroAlt,FlightState,GpsFreeFallWarning,ImuFreeFallWarning,BaroFreeFallWarning,LowVoltageWarning,BaroLandingWarning,ChuteThresholdReachedBaro,ChuteThreshholdReachedGPS,RemoteAbort");
	dataLog.close();
	Serial.println("SD --> init");
}

void SdCard::test() {
	return;
}

void SdCard::flightProcess(unsigned long currTime) {
	if ((lastActionTime + deltaTimeFlightSdCard) <= currTime) {
		lastActionTime = currTime;

		static char sysTime[15];
		static char accelMag[15];
		static char accelX[15];
		static char accelY[15];
		static char accelZ[15];
		static char voltA[15];
		static char voltB[15];
		static char curA[15];
		static char curB[15];
		static char lat[15];
		static char lon[15];
		static char GPSAlt[15];
		static char hour[15];
		static char minute[15];
		static char second[15];
		static char sats[15];
		static char temp[15];
		static char pres[15];
		static char alt[15];
		static char hum[15];
		static char highestGPSAlt[15];
		static char highestBaroAlt[15];
		static char flightState[15];
		static char gpsFreeFallWarning[15];
		static char imuFreeFallWarning[15];
		static char baroFreeFallWarning[15];
		static char lowVoltageWarning[15];
		static char baroLandingWarning[15];
		static char baroChuteDeployWarning[15];
		static char GPSChuteDeployWarning[15];
		static char remoteAbort[15];

		dtostrf(currTime, 9, 2, sysTime);
		dtostrf(data->getAccelMag(), 9, 2, accelMag);
		dtostrf(data->getAccelX(), 9, 2, accelX);
		dtostrf(data->getAccelY(), 9, 2, accelY);
		dtostrf(data->getAccelZ(), 9, 2, accelZ);
		dtostrf(data->getVoltA(), 9, 2, voltA);
		dtostrf(data->getVoltB(), 9, 2, voltB);
		dtostrf(data->getCurA(), 9, 2, curA);
		dtostrf(data->getCurB(), 9, 2, curB);
		dtostrf(data->getLat(), 9, 2, lat);
		dtostrf(data->getLon(), 9, 2, lon);
		dtostrf(data->getGPSAlt(), 9, 2, GPSAlt);
		dtostrf(data->getHour(), 9, 2, hour);
		dtostrf(data->getMinute(), 9, 2, minute);
		dtostrf(data->getSecond(), 9, 2, second);
		dtostrf(data->getSats(), 9, 2, sats);
		dtostrf(data->getTemp(), 9, 2, temp);
		dtostrf(data->getPres(), 9, 2, pres);
		dtostrf(data->getAlt(), 9, 2, alt);
		dtostrf(data->getHum(), 9, 2, hum);
		dtostrf(data->getHighestGPSAlt(), 9, 2, highestGPSAlt);
		dtostrf(data->getHighestBaroAlt(), 9, 2, highestBaroAlt);
		dtostrf(data->getFlightState(), 9, 2, flightState);
		dtostrf(data->gpsFreeFallWarning, 9, 2, gpsFreeFallWarning);
		dtostrf(data->imuFreeFallWarning, 9, 2, imuFreeFallWarning);
		dtostrf(data->baroFreeFallWarning, 9, 2, baroFreeFallWarning);
		dtostrf(data->lowVoltageWarning, 9, 2, lowVoltageWarning);
		dtostrf(data->baroLandingWarning, 9, 2, baroLandingWarning);
		dtostrf(data->baroChuteDeployWarning, 9, 2, baroChuteDeployWarning);
		dtostrf(data->GPSChuteDeployWarning, 9, 2, GPSChuteDeployWarning);
		dtostrf(data->getRemoteAbort(), 9, 2, remoteAbort);

		dataLog = SD.open("log.txt", FILE_WRITE);
		if (dataLog) {
			char telemString[340]; //max Iridium send size
        	snprintf(telemString, sizeof(telemString), "%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s", 
        		sysTime, accelMag, accelX, accelY, accelZ, voltA, voltB, curA, curB, lat, lon, GPSAlt, hour, minute, second, sats,temp, pres, alt, hum, highestGPSAlt, highestBaroAlt, flightState, gpsFreeFallWarning, imuFreeFallWarning, baroFreeFallWarning, lowVoltageWarning, baroLandingWarning, baroChuteDeployWarning,GPSChuteDeployWarning);
			dataLog.println(telemString);
			dataLog.close();
		}
		else {
			Serial.println("SD Error.");
		}
		Serial.println("SD --> flightProcess");
	}
}

void SdCard::groundProcess() {
	return;
}

void SdCard::teardown() {
	return;
}