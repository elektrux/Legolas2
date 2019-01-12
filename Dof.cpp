#include "Dof.h"

void Dof::init(Flightdata &flightdata) {
	data = &flightdata;
	Serial.println("IMU --> init");

}
void Dof::test(unsigned long currTime) {
	if ((lastTestTime + deltaTimeTestDof) <= currTime) {
		lastTestTime = currTime;

		Serial.print("X: ");
		Serial.println(data->getAccelX());
		Serial.print("Y: ");
		Serial.println(data->getAccelY());
		Serial.print("Z: ");
		Serial.println(data->getAccelZ());
		Serial.print("Mag: ");
		Serial.println(data->getAccelMag());
	}
}
void Dof::flightProcess(unsigned long currTime) {
	if ((lastActionTime + deltaTimeFlightDof) <= currTime) {
		lastActionTime = currTime;

		Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(54321); //why does this have to be in a function?
		accel.begin();
		sensors_event_t event;
		accel.getEvent(&event);
		data->setAccel(event.acceleration.x, event.acceleration.y, event.acceleration.z);
		Serial.println("IMU --> flightProcess");
	}
}
void Dof::groundProcess() {
	return;
}
void Dof::teardown() {
	return;
}