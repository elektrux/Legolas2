#include "Com.h"

void Com::init(Flightdata& flightdata) {
	data = &flightdata;
	Serial1.begin(19200);
  Serial.println("Com --> init");
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
    		Serial.println(err); 
    		if (err == ISBD_NO_MODEM_DETECTED) {
    		}
    		return;
		}
        isbd.adjustSendReceiveTimeout(120); //300s by default
		  err = isbd.getSignalQuality(signalQuality);
  		if (err != ISBD_SUCCESS) {
    		Serial.println(err);
    		return;
    	}

    	//Serial.println(data->getStrFlightdata());

      char* telemetry;
      telemetry = data->getStrFlightdata();
      Serial.println(telemetry);
      isbd.sendSBDBinary(telemetry, 175);


      /*char rxBuffer[200];
      size_t rxBufferSize = sizeof(rxBuffer);

      err = isbd.sendReceiveSBDBinary(data->getStrFlightdata(), 215, rxBuffer, rxBufferSize);*/

    	if (err != ISBD_SUCCESS) {
    		Serial.println(err);
    		if (err == ISBD_SENDRECEIVE_TIMEOUT) {
    		}
  		}

  		else {
        //Serial.print("Received: ");
        //Serial.println(receive);
  		}

  		isbd.sleep();
      Serial.println("Com --> flightProcess");
	}
}

void Com::groundProcess() {
	return;
}

void Com::teardown() {
	return;
}  