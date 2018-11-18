#pragma once

#define NORMAL_STATUS					0
#define ERROR_STATUS_BAD_DATA			1
#define ERROR_STATUS_WRONG_DEVICE		2
#define ERROR_STATUS_FAILED_TO_CONNECT	3

struct SystemStatus {
	uint8_t generalStatus;
};


#define ARDUINO_DUE_DATA	1
struct ArduinoDueData {
	float batteryVoltage;
};
struct ArduinoDueStatus : SystemStatus {
	uint32_t clockSpeed;
	bool lowPowerMode;
};

#define CAMERA_DATA			2
struct CameraData {
	uint32_t bytesSaved;//The number of bytes recorded since the last packet
};
#define ACCEL_DATA			3
struct AccelData {
	int16_t ax, ay, az;
	int16_t gx, gy, gz;
	int16_t temp;
};

#define GPS_DATA			4
#define PITOT_TUBE_DATA		5
#define ALTIMETER_DATA		6
#define SD_CARD_DATA		7
#define RADIO_DATA			8
#define 
#define 
#define 
#define 
#define 
#define 
#define 
#define 
#define 
#define 
