void SonneAuf()
{
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
		SonneIndex = 0;
		Balken = 0;
		break;
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
		SonneIndex = 0;
		Balken = 0;
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

/* The next function is calculateVal. When the loop value, i,
*  reaches the step size appropriate for one of the
*  colors, it increases or decreases the value of that color by 1.
*  (R, G, and B are each calculated separately.)
*/

int calculateVal(int step, int val, int i) {

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

/* crossFade() converts the percentage colors to a
*  0-255 range, then loops 1020 times, checking to see if
*  the value needs to be updated each time, then writing
*  the color values to the correct pins.
*/

void crossFade(int color[4]) {

	// Convert to 0-255
	int R = (color[0]);// *255) / 100;
	int G = (color[1]);// *255) / 100;
	int B = (color[2]);// *255) / 100;
	int W = (color[3]);// *255) / 100;

	int stepR = calculateStep(prevR, R);
	int stepG = calculateStep(prevG, G);
	int stepB = calculateStep(prevB, B);
	int stepW = calculateStep(prevW, W);





	//for (int LEDStep=0; LEDStep < 1020; LEDStep++){

	if (LEDStep <= 1020) {
		LEDStep++;
		if (LEDStep >= 1021) {
			LEDStep = 0;
			Durchlauf++;
			if (Durchlauf == 8) {
				SonneIndex == 0;
			}
		}

		redVal = calculateVal(stepR, redVal, LEDStep);
		grnVal = calculateVal(stepG, grnVal, LEDStep);
		bluVal = calculateVal(stepB, bluVal, LEDStep);
		whiteVal = calculateVal(stepW, whiteVal, LEDStep);

		for (int i = 0; i < NUMLEDS; i++) {
			strip.SetPixelColor(i, RgbwColor(redVal, grnVal, bluVal, whiteVal));

			delay(5);					// delay für besseres Timing
			strip.Show();
		}

		delay(wait);					// delay für Durchlaufzeit	

		Balken++;

		Serial.print("Loop/RGB: #");
		Serial.print(LEDStep);
		Serial.print(" | ");
		Serial.print(redVal);
		Serial.print(" / ");
		Serial.print(grnVal);
		Serial.print(" / ");
		Serial.print(bluVal);
		Serial.print(" / ");
		Serial.print(whiteVal);
		Serial.print(" / ");
		Serial.print(Durchlauf);
		Serial.print(" / ");
		Serial.println(Balken);

	}

	prevR = redVal;						// Update current values for next loop
	prevG = grnVal;
	prevB = bluVal;
	prevW = whiteVal;

}


