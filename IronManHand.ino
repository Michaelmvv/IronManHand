#include "Arduino.h"
#include "Adafruit_CircuitPlayground.h"

//Mask Global Vars
int faceButtonPin = 1;
int faceLedPin = 2;
int buttonLastState = 0;
int buttonState = 0;
int faceLedState = LOW;
unsigned long lastClickTime;
unsigned long delayBetweenClicks = 1000;

//Hand Global vars
#define ACCEL_LIMIT 8
bool LampStateOn = false;
#define POWER_UP_TIME 1000
#define MAX_BRIGHTNESS 50
int triggerButton = 3;
int triggerButtonLastState = 0;

//Function prototypes
void faceLightToggle();
void hand();
void powerUp();
void powerDown();
void setBrightness(int i);
void pewPewButton();

void setup() {
	CircuitPlayground.begin();
	CircuitPlayground.playTone(500, 100);
	pinMode(faceButtonPin, INPUT);
	pinMode(faceLedPin, OUTPUT);
	CircuitPlayground.setBrightness(0);
	for (int i = 0; i < 9; ++i) {
		CircuitPlayground.setPixelColor(i, 255, 255, 255);
	}

	Serial.begin(9600);
}

void loop() {
	faceLightToggle();
	hand();
}

void hand() {
	float x = CircuitPlayground.motionX();
//	float y = CircuitPlayground.motionY();
//	Serial.print("X: ");
//	Serial.print(x);
//	Serial.print(" Y: ");
//	Serial.print(y);
//	Serial.print("\n");

	if (x >= ACCEL_LIMIT) {
		if (!LampStateOn)
			powerUp();
	} else if (x <= 0) {
		if (LampStateOn)
			powerDown();
	} else if (x >= ACCEL_LIMIT) {
		//left up
	} else if (x <= -ACCEL_LIMIT) {
		//right up
	}

	if (LampStateOn)
		pewPewButton();
}

void powerUp() {
	for (int i = 0; i <= MAX_BRIGHTNESS; ++i) {
		setBrightness(i);

		CircuitPlayground.playTone(600 + (i * 1.5),
		POWER_UP_TIME / MAX_BRIGHTNESS, true); //TODO MAKE THE TONE BETTER
	}
	LampStateOn = true;
}

void powerDown() {
	int og_b = CircuitPlayground.strip.getBrightness();
	for (int i = og_b; i >= 0; i--) {
		setBrightness(i);
		CircuitPlayground.playTone(600 + (i * 1.5), POWER_UP_TIME / og_b, true); //TODO MAKE THE TONE BETTER
	}
	LampStateOn = false;
}

void setBrightness(int i) {
	CircuitPlayground.setBrightness(i);
	for (int i = 0; i < 10; ++i) {
		CircuitPlayground.setPixelColor(i, 255, 255, 255);
	}

}
void pewPewButton() {
	int input = digitalRead(triggerButton);
	if (input != triggerButtonLastState) {
		triggerButtonLastState = input;
		setBrightness(255);
		CircuitPlayground.playTone(800, 500, true);
		setBrightness(MAX_BRIGHTNESS);
	}
}

//A toggle for the led's in the mask.
void faceLightToggle() {
	int input = digitalRead(faceButtonPin);
	Serial.println(input);
	if (input != buttonLastState) {
		lastClickTime = millis();
		buttonLastState = input;
	}

	if ((millis() - lastClickTime) > delayBetweenClicks) {
		if (buttonState != buttonLastState) {
			buttonState = buttonLastState;
			if (buttonState == 1) {
				faceLedState = !faceLedState;
				digitalWrite(faceLedPin, faceLedState);
			}
		}
	}

}
