# include <Wire.h>
// # include <Arduino.h>
# include <stdlib.h>

# include "LedControl.h"

#define DISPLAYS 2
#define DISPLAYDIGITS 8

#define MINRANDOM 100000
#define MAXRANDOM 99999999

// EasyESP or NodeMCU Pin D8 to DIN, D7 to Clk, D6 to LOAD, no.of devices is 1
LedControl lc=LedControl(D8,D7,D6,1);

float *randNumber;

/*****
  https://forum.arduino.cc/index.php?topic=225326.0
  This function converts a number to a string representation and then displays
  it on the LED array. THe output is right-justified on the display.

  Parameter List:
    float val                  the number to be converted
    unsigned char precision    the number of digits after the decimal point
    char *buffer               a character array. It is assumed to be large
                               enough to hold the number represented as an
                               ASCII number plus the null.
  Return value:
    void
*****/
void ConvertNumberToString(long addr, float val, unsigned char precision)
{
  int i;
  int start;

  char *buffer;

  dtostrf(val, DISPLAYDIGITS, precision, buffer);
  start = DISPLAYDIGITS - precision - 1;

  for (i = 0; i < DISPLAYDIGITS; i++) {
    if (buffer[i] != ' ') {              // If we have a digit character
      if (buffer[i + 1] == '.') {        // Need a decimal point?
        lc.setDigit(addr, start--, buffer[i] - '0', true);   // Yes
        i++;
      } else {
        lc.setDigit(addr, start--, buffer[i] - '0', false);  // No
      }
    } else {
      start--;
    }
  }
}

void setup() {
    // put your setup code here, to run once:

    Serial.begin(9600);

    // Initialize the MAX7219 device
    for (int i = 0; i < DISPLAYS; i++) {
      lc.shutdown(i,false);   // Enable display
      lc.setIntensity(i,15);  // Set brightness level (0 is min, 15 is max)
      lc.clearDisplay(i);     // Clear display register

      randNumber[i] = random(MINRANDOM, MAXRANDOM);

      Serial.print("Numero gerado display ");
      Serial.print(i);
      Serial.print(": ");
      Serial.println(randNumber[i]);
    }



}

void loop() {
    // put your main code here, to run repeatedly:

    int i;

    for (i = 0; i < DISPLAYS; i++) {
      ConvertNumberToString(i, randNumber[i], 0);
    }

    delay(5000);
}
