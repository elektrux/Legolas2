#include "Com.h"

void Com::init(Flightdata& flightdata) {
	data = &flightdata;
	Serial1.begin(19200);
    Serial.println("Com init.");
}

void Com::test() {
	return;
}

void Com::flightProcess(unsigned long currTime) {
	if ((lastActionTime + deltaTimeFlightCom) <= currTime) {
		lastActionTime = currTime;

		IridiumSBD isbd(Serial1);

  		err = isbd.begin();
  		if (err != ISBD_SUCCESS) {
    		Serial.print("Begin failed: error ");
    		Serial.println(err);
    		if (err == ISBD_NO_MODEM_DETECTED) {
      			Serial.println("No modem detected: check wiring.");
    		}
    		return;
		}
        isbd.adjustSendReceiveTimeout(120); //300s by default
		err = isbd.getSignalQuality(signalQuality);
  		if (err != ISBD_SUCCESS) {
    		Serial.print("SignalQuality failed: error ");
    		Serial.println(err);
    		return;
    	}
    	Serial.print("Signal quality: ");
    	Serial.println(signalQuality);

    	Serial.println("Sending telemetry:\n\n");
    	Serial.println(data->getStrFlightdata());

        size_t bufferSize;
        uint8_t receiveBuffer[270];
        uint8_t *buffer = receiveBuffer;
        err = isbd.sendReceiveSBDBinary(data->getStrFlightdata(), 340, buffer, bufferSize);
      //Serial.print("Received: ");
      //Serial.println(receive);

      //err = isbd.sendSBDBinary(data->getStrFlightdata(), 340);
      //err = isbd.sendSBDText("Test");

    	if (err != ISBD_SUCCESS) {
    		Serial.print("sendSBDText failed: error ");
    		Serial.println(err);
    		if (err == ISBD_SENDRECEIVE_TIMEOUT) {
      			Serial.println("Try again with a better view of the sky.");
    		}
  		}

  		else {
    		Serial.println("Iridium ---> flightProcess");
        //Serial.print("Received: ");
        //Serial.println(receive);
  		}

  		isbd.sleep();
	}
}

void Com::groundProcess() {
	return;
}

void Com::teardown() {
	return;
}  