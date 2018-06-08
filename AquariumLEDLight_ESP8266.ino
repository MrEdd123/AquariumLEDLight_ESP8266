
/*
    Name:       AquariumLEDLight_ESP8266.ino
    Created:	03.06.2018 22:22:48
    Author:     MEINER\André Reichl
*/

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Adafruit_NeoPixel.h>
#include <EEPROM.h>
/********************  NeoPixel Config *************/

#define PIN 2
#define NUMLEDS 90

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMLEDS, PIN, NEO_GRBW + NEO_KHZ800);

/*************** Blynk und WIFI Config **************/
char auth[] = "4463a0cab2b8474ab4083dc952c4c3c2";

char ssid[] = "Andre+Janina";
char pass[] = "winter12";


WidgetLCD lcd(V10);
BlynkTimer timer;			//Blynk Timer aktivieren


/******* Variablen ********************************/

uint8_t SoAuDa = 5;
uint8_t SoUnDa = 5;
uint8_t maxHell = 50;
uint8_t minHell ;

uint16_t Zeit;


/**************** NeoPixel Init ******************/

//// Sonnenaufgang Color Array
////				{ R, G , B, W }
//int SonAu1[4] = { 7,7,12,0 };
//int SonAu2[4] = { 39,21,12,0 };
//int SonAu3[4] = { 157,13,1,0 };
//int SonAu4[4] = { 163,21,1,0 };
//int SonAu5[4] = { 186,67,1,0 };
//int SonAu6[4] = { 227,237,56,0 };
//int SonAu7[4] = { 227,237,56,50 };
//
//// Sonnenuntergang Color Array
////				{ R, G , B, W }
//int SonUn1[4] = { 220,230,50,40 };
//int SonUn2[4] = { 220,230,50,0 };
//int SonUn3[4] = { 186,67,1,0 };
//int SonUn4[4] = { 163,21,1,0 };
//int SonUn5[4] = { 157, 13, 1, 0 };
//int SonUn6[4] = { 39, 21, 12, 0 };
//int SonUn7[4] = { 0, 0, 12, 0 };


// Sonnenaufgang Color Array
//				{ R, G , B, W }
int SonAu1[4] = { 50,0,0,0 };
int SonAu2[4] = { 150,5,0,0 };
int SonAu3[4] = { 157,13,0,0 };
int SonAu4[4] = { 163,21,1,0 };
int SonAu5[4] = { 186,67,1,0 };
int SonAu6[4] = { 240,256,30,50 };
int SonAu7[4] = { 250,200,30,150 };

// Sonnenuntergang Color Array
//				{ R, G, B, W }
int SonUn1[4] = { 220,230,50,40 };
int SonUn2[4] = { 220,230,50,0 };
int SonUn3[4] = { 186,68,2,0 };
int SonUn4[4] = { 150,20,2,0 };
int SonUn5[4] = { 100, 14, 2, 0 };
int SonUn6[4] = { 2, 0, 6, 0 };
int SonUn7[4] = { 0, 0, 12, 2 };


// Set initial color
uint8_t redVal = 0;
uint8_t grnVal = 0;
uint8_t bluVal = 0;
uint8_t whiteVal = 0;

uint16_t wait = 250;      // 10ms internal crossFade delay; increase for slower fades

uint8_t prevR = redVal;
uint8_t prevG = grnVal;
uint8_t prevB = bluVal;
uint8_t prevW = whiteVal;
uint16_t LEDStep = 0;
uint8_t Durchlauf = 1;
uint8_t SonneAUswitch = 0;
uint8_t Programmswitch = 0;

uint16_t ablaufwert = 0;



/******** BLYNK FUNKTIONEN  ********************/

/**** Sonnenaufgang Starten **********/

BLYNK_WRITE(V1) {

	int i = param.asInt();
	if (i == 1) {

		Programmswitch = 1;
		delay(250);
	}
}
	

/**** Sonne Mittag Starten **********/

BLYNK_WRITE(V2) {

	int i = param.asInt();
	if (i == 1) {
		Programmswitch = 2;
		delay(250);

	}
}


/**** Sonnenuntergang Starten **********/

BLYNK_WRITE(V3) {

	int i = param.asInt();
	if (i == 1) {
		Programmswitch = 3;
		delay(250);
		
	}
}

/**** Durchlaufzeit einstellen *****/


BLYNK_WRITE(V4) {

	Blynk.virtualWrite(V4, param.asInt());
	wait = param.asInt();	
	EEPROM.begin(256);
	EEPROM.put(0, wait);
	EEPROM.commit();
	EEPROM.end();

}

/**** Gesamthelligkeit *************/

BLYNK_WRITE(V5) {

	Blynk.virtualWrite(V5, param.asInt());
	maxHell = param.asInt();
	EEPROM.begin(256);
	EEPROM.put(5, maxHell);
	EEPROM.commit();
	EEPROM.end();

}


void setup()
{
	Serial.begin(115200);
	Blynk.begin(auth, ssid, pass);

	EEPROM.begin(256);
	EEPROM.get(0, wait);
	EEPROM.get(5, maxHell);
	EEPROM.end();

	lcd.clear();
	lcd.print(4, 0, "START");
	
	/*********** Adafruit Neopixel Starten *******/
	strip.begin();
	strip.show();

}

void loop()
{
	
	Blynk.run();
	strip.setBrightness(maxHell);
	strip.show();

	switch (Programmswitch) {

	case 1:
		SonneAuf();
		lcd.print(0, 1,"Aufgang: ");
		ablaufwert++;
		lcd.print(12, 1, ablaufwert);
		break;
	case 2:
		
		break;
	case 3:
		SonneUn();
		lcd.print(0, 1,"Untergang: ");
		ablaufwert--;
		lcd.print(12, 1,ablaufwert);
		break;
	case 4:
		break;
	}
	
}


void SonneAuf()
{
	if (SonneAUswitch = 1) {

		switch (Durchlauf)
		{
		case 1:
			crossFade(SonAu1);
			break;
		case 2:
			crossFade(SonAu2);
			break;
		case 3:
			crossFade(SonAu3);
			break;
		case 4:
			crossFade(SonAu4);
			break;
		case 5:
			crossFade(SonAu5);
			break;
		case 6:
			crossFade(SonAu6);
			break;
		case 7:
			crossFade(SonAu7);
			break;
		case 8:
			Durchlauf = 1;
			Programmswitch = 4;
			break;
		}
	}
}
	

void SonneUn()
{
	switch (Durchlauf)
	{
	case 1:
		crossFade(SonUn1);
		break;
	case 2:
		crossFade(SonUn2);
		break;
	case 3:
		crossFade(SonUn3);
		break;
	case 4:
		crossFade(SonUn4);
		break;
	case 5:
		crossFade(SonUn5);
		break;
	case 6:
		crossFade(SonUn6);
		break;
	case 7:
		crossFade(SonUn7);
		break;
	case 8:
		Durchlauf = 1;
		Programmswitch = 4;
		break;
	}
}

int calculateStep(int prevValue, int endValue) {
	int step = endValue - prevValue; // What's the overall gap?
	if (step) {                      // If its non-zero, 
		step = 1020 / step;              //   divide by 1020
	}
	return step;
}

int calculateVal(int step, int val, int i) {
	/* The next function is calculateVal. When the loop value, i,
	*  reaches the step size appropriate for one of the
	*  colors, it increases or decreases the value of that color by 1.
	*  (R, G, and B are each calculated separately.)
	*/
	if ((step) && i % step == 0) { // If step is non-zero and its time to change a value,
		if (step > 0) {              //   increment the value if step is positive...
			val += 1;
		}
		else if (step < 0) {         //   ...or decrement it if step is negative
			val -= 1;
		}
	}
	// Defensive driving: make sure val stays in the range 0-255
	if (val > 255) {
		val = 255;
	}
	else if (val < 0) {
		val = 0;
	}
	return val;
}

void crossFade(int color[4]) {
	/* crossFade() converts the percentage colors to a
	*  0-255 range, then loops 1020 times, checking to see if
	*  the value needs to be updated each time, then writing
	*  the color values to the correct pins.
	*/
	// Convert to 0-255
	int R = (color[0]);// *255) / 100;
	int G = (color[1]);// *255) / 100;
	int B = (color[2]);// *255) / 100;
	int W = (color[3]);// *255) / 100;

	int stepR = calculateStep(prevR, R);
	int stepG = calculateStep(prevG, G);
	int stepB = calculateStep(prevB, B);
	int stepW = calculateStep(prevW, W);

	if (LEDStep <= 1020) {
		LEDStep++;
		if (LEDStep >= 1021) {
			LEDStep = 0;
			Durchlauf++;
		}

		redVal = calculateVal(stepR, redVal, LEDStep);
		grnVal = calculateVal(stepG, grnVal, LEDStep);
		bluVal = calculateVal(stepB, bluVal, LEDStep);
		whiteVal = calculateVal(stepW, whiteVal, LEDStep);

		for (int i = 0; i < NUMLEDS; i++) {
			strip.setPixelColor(i,strip.Color(redVal, grnVal, bluVal, whiteVal));
		}

		delay(wait);					// delay für Durchlaufzeit	

	}

	prevR = redVal;						// Update current values for next loop
	prevG = grnVal;
	prevB = bluVal;
	prevW = whiteVal;

	strip.show();
}


