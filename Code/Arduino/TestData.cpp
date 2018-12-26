#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "Opcodes.h"
#include <Arduino.h>

#include <math.h>
#include <cstdlib>
#include <ctime>

int randNum(int min, int max)
{
	return rand() % max + min;
}

#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))

#define BUFFER_SIZE 16 * 1024

uint8_t buffer[BUFFER_SIZE];
uint32_t pointer = 0, lastPointer = 0;

bool canWrite(uint32_t bytes) {
	return pointer + bytes < BUFFER_SIZE;
}

void write(void* data, uint32_t bytes) {
	if (canWrite(bytes)) {
		memcpy((void*)(buffer + pointer), data, bytes);
		pointer += bytes;
	} else {
		Serial.print("Buffer out of space! Pointer: ");
		Serial.print(pointer);
		Serial.print(" Bytes: ");
		Serial.println(bytes);
	}
}


ArduinoDueData due;
CameraData camera;
AccelData accel;
PitotTubeData pitot;
AltimeterData altimeter;
SDCardData sd;
RadioData radio;

uint32_t launchTime;

uint32_t lastTime = 0;
float lastAltitude = 0.0f, lastVerticalSpeed = 0.0f;

float getAltitude() {
	float time = (millis() - launchTime) / 1000.0f;
	if (time < 0) {
		return 0;
	}
	else if (time < 14.8) {
		return -14.623 * time * (time - 22);
	} else {
		return max(2000 - 30 * time, 0);
	}
}


void setup() {
	lastTime = millis();
	srand(time(0));
}

void loop() {
	uint32_t now = millis();
	float delta = (now - lastTime) / 1000.0f;
	float altitude = getAltitude();
	float verticalSpeed = (altitude - lastAltitude) / delta;
	float verticalAccerlation = (verticalSpeed - lastVerticalSpeed) / delta;
	accel.ay = verticalAccerlation;
	
	due.batteryVoltage = 3.3 - (randNum(0, 10) * millis()) / 50000.0f;
	camera.bytesSaved = randNum(506, 1535);
	pitot.airSpeed = verticalSpeed + randNum(-100, 100) / 78.43f;
	altimeter.altitude = altitude + randNum(-100, 100) / 478.43f;
	sd.bytesWritten = camera.bytesSaved;
	radio.bytesSent = lastPointer;


	delay(10);
	lastVerticalSpeed = verticalSpeed;
	lastAltitude = altitude;
	lastTime = now;
}