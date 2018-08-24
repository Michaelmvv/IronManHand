#include "Arduino.h"
#include <Adafruit_CircuitPlayground.h>

//Mask Global Vars
int faceButtonPin = 0;
int faceLedPin = 0;
int buttonLastState = LOW;
int buttonState = LOW;
int faceLedState = LOW;
unsigned long lastClickTime;
unsigned long debounceDelay = 1000;

//Hand Global vars
#define ACCEL_LIMIT 500

//Function prototypes
void faceLightToggle();
void hand();

void setup() {
	CircuitPlayground.begin();
	CircuitPlayground.playTone(500, 100);
	pinMode(faceButtonPin, INPUT);
	pinMode(faceLedPin, OUTPUT);
}

void loop() {
	faceLightToggle();
	hand();
}
void hand() {
	float x = CircuitPlayground.motionX();
	float y = CircuitPlayground.motionY();

	if (y >= ACCEL_LIMIT) {
		//usb up
	} else if (x <= -ACCEL_LIMIT) {
		//batt up
	} else if (x >= ACCEL_LIMIT) {
		//left up
	} else if (x <= -ACCEL_LIMIT) {
		//right up
	}

}

void faceLightToggle() {
	int input = digitalRead(faceButtonPin);
	if (input != buttonLastState) {
		lastClickTime = millis();
		buttonLastState = input;
	}

	if ((millis() - lastClickTime) > debounceDelay) {
		if (buttonState != buttonLastState) {
			buttonState = buttonLastState;
			if (buttonState == HIGH) {
				faceLedState = !faceLedState;
				digitalWrite(faceLedPin, faceLedState);
			}
		}
	}

}
