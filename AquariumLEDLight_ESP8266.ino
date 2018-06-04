
/*
    Name:       AquariumLEDLight_ESP8266.ino
    Created:	03.06.2018 22:22:48
    Author:     MEINER\André Reichl
*/

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "4463a0cab2b8474ab4083dc952c4c3c2";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Andre+Janina";
char pass[] = "winter12";

void setup()
{
	// Debug console
	Serial.begin(115200);
	Blynk.begin(auth, ssid, pass);

}

void loop()
{
	Blynk.run();

}
