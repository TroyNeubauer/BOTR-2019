#include <stdint.h>
#include <Arduino.h>

#define TEST_COUNT 5000
#define TYPE int
#define OPERATION f + f

//int	+: 0.69 micros
//		-: 0.5 micros
//		*:
//		/:


void setup() {
	Serial.begin(115200);

	TYPE items[TEST_COUNT];

	TYPE f;
	uint32_t start = micros();
	for (int i = 0; i < TEST_COUNT; i++) {
		f = (TYPE)i;
		TYPE result = OPERATION;
		items[i] = result;
	}
	uint32_t time = micros() - start;
	Serial.println();
	Serial.print(F("Total time in micros: "));
	Serial.println(time);
	Serial.print(F("Average time per iteration: "));
	Serial.println((float)time / (float)TEST_COUNT);
	Serial.println();
	for (int i = 0; i < TEST_COUNT; i++) {
		Serial1.print(items[i]);
		Serial1.print(',');
		Serial1.print(' ');
	}
}


void loop() {
	
}
