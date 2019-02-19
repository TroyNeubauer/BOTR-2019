
#include <stdint.h>
#include <Wire.h>
#include <ArduCAM.h>
#include <SPI.h>
#include <SD.h>
#include "memorysaver.h"


#define FRAME_SIZE OV5642_640x480

#define TOTAL_FRAMES 50	
#define CAMERA_CS 7
#define SD_CARD_CS 9

#define SPI_SPEED 20000000 //20mhz

#define FRAME_BUFFER_SIZE 8000000 //8MB
#define BUFFER_SIZE 2048

ArduCAM myCAM(OV5642, CAMERA_CS);


static void Video2SD() {
	File file;
	byte buffer[BUFFER_SIZE];
	uint32_t uVideoLen = 0;
	
	//Open the new file
	digitalWrite(SD_CARD_CS, LOW);
	file = SD.open("video.dat", O_WRITE | O_CREAT | O_TRUNC);
	if (!file) {
		Serial.println(F("File open failed"));
		while (1);
		return;
	}
	digitalWrite(SD_CARD_CS, HIGH);

	Serial.print(F("\nRecording "));
	Serial.print(TOTAL_FRAMES);
	Serial.println(F(" video frames: please wait...\n"));

	uint32_t startms = millis();

	//Write video data, frame by frame
	for (int frame = 0; frame < TOTAL_FRAMES; frame++) {
		myCAM.CS_LOW();
		myCAM.flush_fifo();//Flush the buffer
		myCAM.clear_fifo_flag();//Clear the capture done flag
		myCAM.start_capture();//Start capture
		
		Serial.print("Length ");
		while (!myCAM.get_bit(ARDUCHIP_TRIG, CAP_DONE_MASK));
		uint32_t length = myCAM.read_fifo_length();
		uVideoLen += length;
		Serial.println(length);
		SPI.beginTransaction(SPISettings(SPI_SPEED, MSBFIRST, SPI_MODE0));

		myCAM.set_fifo_burst();
		uint32_t transfered = 0;
		while (transfered < length) {
			uint32_t bytes = min(length - transfered, BUFFER_SIZE);
			SPI.transfer(buffer, bytes);
			myCAM.CS_HIGH();
			transfered += bytes;
			digitalWrite(SD_CARD_CS, LOW);
			file.write(buffer, bytes);
			digitalWrite(SD_CARD_CS, HIGH);
		}
		digitalWrite(SD_CARD_CS, LOW);
		file.write("DONE", 4);
		digitalWrite(SD_CARD_CS, HIGH);
	}	// End cycle for all frames
	file.close();

	// Compute statistics
	uint32_t elapsedms = millis() - startms;
	float fRealFPS = (1000.0f * (float) TOTAL_FRAMES) / ((float) elapsedms);
	float fmicroseconds_per_frame = 1000000.0f / fRealFPS;
	uint8_t iAttainedFPS = round(fRealFPS); // Will overwrite AVI header placeholder
	uint32_t us_per_frame = round(fmicroseconds_per_frame); // Will overwrite AVI header placeholder

	Serial.println(F("\n*** Video recorded and saved ***\n"));
	Serial.print(F("Recorded "));
	Serial.print(TOTAL_FRAMES);
	Serial.print(F("s in "));
	Serial.print(elapsedms / 1000.0);
	Serial.print(F("ms\nActual FPS is "));
	Serial.print(fRealFPS, 2);
	Serial.print(F("\nFrame duration is "));
	Serial.print(us_per_frame);
	Serial.println(F(" us"));
	Serial.print(F("Average frame length is "));
	Serial.print(uVideoLen / TOTAL_FRAMES);
	Serial.println(F(" bytes"));
}


void setup() {
	Wire.begin();
	Serial.begin(115200);
	while (!Serial);

	delay(200);
	Serial.println(F("ArduCAM Start!\n"));

	pinMode(SD_CARD_CS, OUTPUT);
	digitalWrite(SD_CARD_CS, LOW);

	SPI.begin();

	//Initialize SD Card
	while (!SD.begin(SD_CARD_CS)) {
		Serial.println(F("SD Card Error!")); delay(1000);
	}
	Serial.println(F("SD Card detected."));
	digitalWrite(SD_CARD_CS, HIGH);


	myCAM.CS_LOW();
	delay(10);
	myCAM.write_reg(0x07, 0x80);
	myCAM.write_reg(0x07, 0x00);
	delay(10);

	while (1) {
		//Check if the ArduCAM SPI bus is OK
		myCAM.write_reg(ARDUCHIP_TEST1, 0x55);
		uint8_t temp;
		temp = myCAM.read_reg(ARDUCHIP_TEST1);
		if (temp != 0x55) {
			Serial.println(F("SPI interface Error!"));
			delay(1000); continue;
		} else {
			Serial.println(F("SPI interface OK.")); break;
		}
	}
	delay(100);

	while (1) {
		uint8_t vid, pid;
		//Check if the camera module type is OV5642
		myCAM.wrSensorReg16_8(0xff, 0x01);
		myCAM.rdSensorReg16_8(OV5642_CHIPID_HIGH, &vid);
		myCAM.rdSensorReg16_8(OV5642_CHIPID_LOW, &pid);
		if ((vid != 0x56) || (pid != 0x42)) {
			Serial.print(F("Can't find OV5642 module! vid: "));
			Serial.print(vid);
			Serial.print(", pid: ");
			Serial.println(pid);
			delay(1000); continue;
		} else {
			Serial.println(F("OV5642 detected.")); break;
		}
	}
	delay(1000);
	myCAM.set_format(JPEG);
	myCAM.InitCAM();

	myCAM.write_reg(ARDUCHIP_TIM, VSYNC_LEVEL_MASK);   //VSYNC is active HIGH
	myCAM.OV5642_set_JPEG_size(FRAME_SIZE);

	delay(100);

	Video2SD();
}

void loop() {
	delay(500000);
}
