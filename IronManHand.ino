#include "Arduino.h"
#include "Adafruit_CircuitPlayground.h"

//Mask Global Vars
int faceButtonPin = 1;
int faceLedPin = 2;
int buttonLastState = 0;
int buttonState = 0;
int faceLedState = LOW;
unsigned long lastClickTime;
unsigned long pressTime = 500;

//Hand Global vars
#define ACCEL_LIMIT 8
bool LampStateOn = false;
#define POWER_UP_TIME 1000
#define MAX_BRIGHTNESS 40
int triggerButton = 3;
int triggerButtonLastState = 0;
bool randomColorOnShoot = false;

//Function prototypes (You might not need them for Arduino)
void faceLightToggle();
void hand();
void powerUp();
void powerDown();
void setBrightness(int i);
void pewPewButton();
void setBrightness(int i, int r, int g, int b);

void setup() {
	CircuitPlayground.begin();
	CircuitPlayground.playTone(500, 100);
	pinMode(faceButtonPin, INPUT);
	pinMode(faceLedPin, OUTPUT);
	setBrightness(0);

	Serial.begin(9600);
}

void loop() {
	faceLightToggle();
	hand();
}

void hand() {
	float x = CircuitPlayground.motionX();
	float y = CircuitPlayground.motionY();
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
	} else if (y >= ACCEL_LIMIT) {
		randomColorOnShoot = true;
	} else if (x <= -ACCEL_LIMIT) {
		randomColorOnShoot = false;
	}

	if (LampStateOn)
		pewPewButton();
}

void powerUp() {
	for (int i = 0; i <= MAX_BRIGHTNESS; ++i) {
		setBrightness(i);

		CircuitPlayground.playTone(600 + (i * 1.5),
		POWER_UP_TIME / MAX_BRIGHTNESS, true);
	}
	LampStateOn = true;
}

void powerDown() {
	int og_b = CircuitPlayground.strip.getBrightness();
	for (int i = og_b; i >= 0; i--) {
		setBrightness(i);
		CircuitPlayground.playTone(600 + (i * 1.5), POWER_UP_TIME / og_b, true);
	}
	LampStateOn = false;
}

void setBrightness(int i) {
	setBrightness(i, 255, 255, 255);
}
void setBrightness(int i, int r, int g, int b) {
	CircuitPlayground.setBrightness(i);
	for (int i = 0; i < 10; ++i) {
		CircuitPlayground.setPixelColor(i, r, g, b);
	}
}
void pewPewButton() {
	int input = digitalRead(triggerButton);
	if (input) {
		if (!randomColorOnShoot) {
			setBrightness(255, 100, 100, 255);
			CircuitPlayground.playTone(900, 500, true);
		} else {
			CircuitPlayground.playTone(900, 500, false);
			long sm = millis();
			while (sm - millis() < 500) {
				setBrightness(random(255), random(255), random(255), 255);
				delay(50);
			}
		}
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

	if ((millis() - lastClickTime) > pressTime) {
		if (buttonState != buttonLastState) {
			buttonState = buttonLastState;
			if (buttonState == 1) {
				faceLedState = !faceLedState;
				digitalWrite(faceLedPin, faceLedState);
			}
		}
	}

}
