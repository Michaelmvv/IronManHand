#include "Arduino.h"
#include <Adafruit_CircuitPlayground.h>

//Mask Global Vars
int faceButtonPin = 0;
int faceLedPin = 0;
int buttonLastState = LOW;
int buttonState = LOW;
int faceLedState = LOW;
long lastDebounceTime;
long debounceDelay = 1000;

//Hand Global vars

void setup() {
	CircuitPlayground.begin();
	CircuitPlayground.playTone(500, 100);
	pinMode(faceButtonPin, INPUT);
	pinMode(faceLedPin, OUTPUT);
}

void loop() {
	faceLightToggle();
}

void faceLightToggle() {
	int input = digitalRead(faceButtonPin);
	if (input != buttonLastState) {
		lastDebounceTime = millis();
		buttonLastState = input;
	}

	if ((millis() - lastDebounceTime) > debounceDelay) {
		if (buttonState != buttonLastState) {
			buttonState = buttonLastState;
			if (buttonState == HIGH) {
				faceLedState = !faceLedState;
				digitalWrite(faceLedPin, faceLedState);
			}
		}
	}

}
