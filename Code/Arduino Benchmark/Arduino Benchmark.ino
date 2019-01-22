#include <stdint.h>
#include <Arduino.h>
#include <math.h>

#include <Wire.h>
#include <SparkFunMPL3115A2.h>


//Create an instance of the object
MPL3115A2 myPressure;

void setup()
{
	Wire.begin();        // Join i2c bus
	Serial.begin(115200);  // Start serial for output

	myPressure.begin(); // Get sensor online

	// Configure the sensor
	//myPressure.setModeAltimeter(); // Measure altitude above sea level in meters
	myPressure.setModeBarometer(); // Measure pressure in Pascals from 20 to 110 kPa

	myPressure.setOversampleRate(0); // Set Oversample to the recommended 128
	myPressure.enableEventFlags(); // Enable all three pressure and temp event flags 
}

void loop()
{
	/*float altitude = myPressure.readAltitude();
	Serial.print("Altitude(m):");
	Serial.print(altitude, 2);

	altitude = myPressure.readAltitudeFt();
	Serial.print(" Altitude(ft):");
	Serial.print(altitude, 2);*/

	float pressure = myPressure.readPressure();
	Serial.print("Pressure(Pa):");
	Serial.print(pressure, 2);

	//float temperature = myPressure.readTemp();
	//Serial.print(" Temp(c):");
	//Serial.print(temperature, 2);

	float temperature = myPressure.readTempF();
	Serial.print(" Temp(f):");
	Serial.print(temperature, 2);

	Serial.println();
}



#if 0

#include <Adafruit_GPS.h>

Adafruit_GPS GPS(&Serial1);

void setup()
{
	Serial.begin(115200);
	while (!Serial)
		;
	Serial.println("Starting Init");
	GPS.begin(9600);
	GPS.sendCommand("$PMTK251,38400*27");  //set baud rate to 38400
	Serial1.end();
	GPS.begin(38400);
	Serial.println("Initalized...");
}

void loop() {
	char c;
	if ((c = GPS.read()) != 0) {
		Serial.print(c);
	}
}

#elif 0
#include <MPU6050.h>


// I2Cdev and MPU6050 must be installed as libraries, or else the .cpp/.h files
// for both classes must be in the include path of your project
#include "I2Cdev.h"
#include "MPU6050.h"

// Arduino Wire library is required if I2Cdev I2CDEV_ARDUINO_WIRE implementation
// is used in I2Cdev.h
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
#include "Wire.h"
#endif

// class default I2C address is 0x68
// specific I2C addresses may be passed as a parameter here
// AD0 low = 0x68 (default for InvenSense evaluation board)
// AD0 high = 0x69
MPU6050 accelgyro;
//MPU6050 accelgyro(0x69); // <-- use for AD0 high

int16_t ax, ay, az;
int16_t gx, gy, gz;



// uncomment "OUTPUT_READABLE_ACCELGYRO" if you want to see a tab-separated
// list of the accel X/Y/Z and then gyro X/Y/Z values in decimal. Easy to read,
// not so easy to parse, and slow(er) over UART.
#define OUTPUT_READABLE_ACCELGYRO

// uncomment "OUTPUT_BINARY_ACCELGYRO" to send all 6 axes of data as 16-bit
// binary, one right after the other. This is very fast (as fast as possible
// without compression or data loss), and easy to parse, but impossible to read
// for a human.
//#define OUTPUT_BINARY_ACCELGYRO


#define LED_PIN 13
bool blinkState = false, connected = false;

void setup() {
	// join I2C bus (I2Cdev library doesn't do this automatically)
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
	Wire.begin();
#elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
	Fastwire::setup(400, true);
#endif

	// initialize serial communication
	// (38400 chosen because it works as well at 8MHz as it does at 16MHz, but
	// it's really up to you depending on your project)
	Serial.begin(115200);

	// initialize device
	Serial.println("Initializing I2C devices...");
	accelgyro.initialize();

	// verify connection
	Serial.println("Testing device connections...");
	Serial.println((connected = accelgyro.testConnection()) ? "MPU6050 connection successful" : "MPU6050 connection failed");
	Serial.print("Device id: ");
	Serial.println(accelgyro.getDeviceID());
	// use the code below to change accel/gyro offset values
	/*
	Serial.println("Updating internal sensor offsets...");
	// -76	-2359	1688	0	0	0
	Serial.print(accelgyro.getXAccelOffset()); Serial.print("\t"); // -76
	Serial.print(accelgyro.getYAccelOffset()); Serial.print("\t"); // -2359
	Serial.print(accelgyro.getZAccelOffset()); Serial.print("\t"); // 1688
	Serial.print(accelgyro.getXGyroOffset()); Serial.print("\t"); // 0
	Serial.print(accelgyro.getYGyroOffset()); Serial.print("\t"); // 0
	Serial.print(accelgyro.getZGyroOffset()); Serial.print("\t"); // 0
	Serial.print("\n");
	accelgyro.setXGyroOffset(220);
	accelgyro.setYGyroOffset(76);
	accelgyro.setZGyroOffset(-85);
	Serial.print(accelgyro.getXAccelOffset()); Serial.print("\t"); // -76
	Serial.print(accelgyro.getYAccelOffset()); Serial.print("\t"); // -2359
	Serial.print(accelgyro.getZAccelOffset()); Serial.print("\t"); // 1688
	Serial.print(accelgyro.getXGyroOffset()); Serial.print("\t"); // 0
	Serial.print(accelgyro.getYGyroOffset()); Serial.print("\t"); // 0
	Serial.print(accelgyro.getZGyroOffset()); Serial.print("\t"); // 0
	Serial.print("\n");\

	*/
	// configure Arduino LED for
	pinMode(LED_PIN, OUTPUT);
}

void loop() {
	// read raw accel/gyro measurements from device
	accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

	// these methods (and a few others) are also available
	//accelgyro.getAcceleration(&ax, &ay, &az);
	//accelgyro.getRotation(&gx, &gy, &gz);
	if (connected) {
#ifdef OUTPUT_READABLE_ACCELGYRO
		// display tab-separated accel/gyro x/y/z values
		Serial.print("a/g:\t");
		Serial.print(ax); Serial.print("\t");
		Serial.print(ay); Serial.print("\t");
		Serial.print(az); Serial.print("\t");
		Serial.print(gx); Serial.print("\t");
		Serial.print(gy); Serial.print("\t");
		Serial.println(gz);
#endif

#ifdef OUTPUT_BINARY_ACCELGYRO
		Serial.write((uint8_t)(ax >> 8)); Serial.write((uint8_t)(ax & 0xFF));
		Serial.write((uint8_t)(ay >> 8)); Serial.write((uint8_t)(ay & 0xFF));
		Serial.write((uint8_t)(az >> 8)); Serial.write((uint8_t)(az & 0xFF));
		Serial.write((uint8_t)(gx >> 8)); Serial.write((uint8_t)(gx & 0xFF));
		Serial.write((uint8_t)(gy >> 8)); Serial.write((uint8_t)(gy & 0xFF));
		Serial.write((uint8_t)(gz >> 8)); Serial.write((uint8_t)(gz & 0xFF));
#endif
	}
	// blink LED to indicate activity
	blinkState = !blinkState;
	digitalWrite(LED_PIN, blinkState);
}

#elif 0

//On arduino mega 2560
//int	+: 1.45 micros
//		-: 1.51 micros
//		*: 1.96 micros
//		/: 15.63 micros

//float	+: 20.99 micros
//		-: 21.87 micros
//		*: 22.42 micros
//		/: 44.29 micros

//double+: 20.81 micros
//		-: 21.69 micros
//		*: 22.12 micros
//		/: 43.97 micros
//	atan(f) / sqrt(f2): 267.46 micros

#define TEST_COUNT 5000
#define TYPE float
#define OPERATION atanf(f) / sqrtf(f2) * (tanf(f) / cbrtf(f2))
//#define OPERATION f / f2

//On arduino due
//int	+: 0.27 micros
//		-: 0.27 micros
//		*: 0.25 micros
//		/: 0.25 micros

//float	+: 0.33 micros
//		-: 0.33 micros
//		*: 2.76 micros - 10X slower than int *
//		/: 4.33 micros - 20X slower than int /
//	atanf(f) / sqrtf(f2): 36.28 micros

//double+: 4.35 micros
//		-: 4.47 micros
//		*: 4.00 micros
//		/: 13.31 micros
//	atan(f) / sqrt(f2): 85.38 micros
#define STRING2(X) #X
#define STRING(X) STRING2(X)


void setup() {
	Serial.begin(115200);
	while (!Serial)//Wait for the serialport to connect
		;
	TYPE items[TEST_COUNT];//The array that will store the results
	//We have to use an array so that the compiler doesnt optomize away the operation we are testing
	TYPE f, f2;

	Serial.println();
	Serial.println();
	Serial.println();
	Serial.println();
	Serial.println();
	Serial.println();
	Serial.println();
	Serial.println();
	Serial.println("Starting test...");
	Serial.print("Type ");
	Serial.print(STRING(TYPE));
	Serial.print("Size: ");
	Serial.println(sizeof(TYPE));

	Serial.print("Array size ");
	Serial.println(sizeof(items));

	uint32_t start = micros();
	for (int i = 0; i < TEST_COUNT; i++) {
		f = (TYPE)i;
		f2 = (TYPE)i * f;
		TYPE result = OPERATION;
		items[i] = result;
	}
	uint32_t time = micros() - start;
	Serial.println();
	Serial.print(F("Total time in micros: "));
	Serial.println(time);
	Serial.print(F("Average time per iteration: "));
	float averageTime = (float)time / (float)TEST_COUNT;
	Serial.println(averageTime);

	Serial.print(F("Number of operations per microsecond "));
	Serial.println(1.0f / averageTime);

	for (int i = 0; i < TEST_COUNT; i++) {
		Serial1.print(items[i]);
		Serial1.print(',');
		Serial1.print(' ');
	}
}


void loop() {
	
}


#endif