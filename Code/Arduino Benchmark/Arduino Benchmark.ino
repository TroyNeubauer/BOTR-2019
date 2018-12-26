#include <stdint.h>
#include <Arduino.h>
#include <math.h>

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
#define OPERATION atanf(f) / sqrtf(f2)
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
