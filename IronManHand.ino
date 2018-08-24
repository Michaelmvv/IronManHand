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
_Bool LampState = false;
#define POWER_UP_TIME 2000
#define MAX_BRIGHTNESS 255

//Function prototypes
void faceLightToggle();
void hand();
void powerUp();
void powerDown();

void setup() {
	CircuitPlayground.begin();
	CircuitPlayground.playTone(500, 100);
	pinMode(faceButtonPin, INPUT);
	pinMode(faceLedPin, OUTPUT);
	CircuitPlayground.setBrightness(0);
	for (int i = 0; i < 9; ++i) {
		CircuitPlayground.setPixelColor(i, 255, 255, 255);
	}
}

void loop() {
	faceLightToggle();
	hand();
}
void hand() {
	float x = CircuitPlayground.motionX();
	float y = CircuitPlayground.motionY();

	if (y >= ACCEL_LIMIT) {
		powerUp();
	} else if (x <= -ACCEL_LIMIT) {
		powerDown();
	} else if (x >= ACCEL_LIMIT) {
		//left up
	} else if (x <= -ACCEL_LIMIT) {
		//right up
	}

}
void powerUp() {
	for (int i = 0; i <= MAX_BRIGHTNESS; ++i) {
		CircuitPlayground.setBrightness(i);
		delay(POWER_UP_TIME / MAX_BRIGHTNESS);
	}
}
void powerDown() {
	int og_b = CircuitPlayground.strip.getBrightness();
	for (int i = og_b; i >= 0; i--) {
		CircuitPlayground.setBrightness(i);
		delay(POWER_UP_TIME / og_b);
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
