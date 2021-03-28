#include "Arduino.h"
#define min(x, y) (((x) < (y)) ? (x) : (y))
#define max(a, b) ((a) > (b) ? (a) : (b))

const uint8_t OUT_PIN = GPIO_NUM_32;

const int INITIAL = 1520;
const int RANGE = 500; // 300 before, 700 no change
const int NUM_MEAN = 200;

int16_t arr[NUM_MEAN];
int idx = 0;

int waiting = INITIAL;

void setup()
{
  // analog setup
  analogReadResolution(10);       // Default of 12 is not very linear. Recommended to use 10 or 11 depending on needed resolution.
  analogSetAttenuation(ADC_11db); //

  Serial.begin(500000);
  delay(20);

  pinMode(OUT_PIN, OUTPUT);
  pinMode(GPIO_NUM_2, OUTPUT);

  digitalWrite(GPIO_NUM_2, HIGH);
  delay(200);
  digitalWrite(GPIO_NUM_2, LOW);
  Serial.println("Setup done");
}

void loop()
{
  int16_t value = analogRead(GPIO_NUM_39); // 0 - 1024
  arr[idx] = value;

  idx += 1;
  if (idx == NUM_MEAN)
  {
    idx = 0;
  }

  float mean = 0;

  for (int i = 0; i < NUM_MEAN; i++)
  {
    mean += arr[i];
  }
  mean /= float(NUM_MEAN);

  float fv = min(float(mean) / 900, 1.0) * -2;
  fv = 1.0 - exp(fv); // fv = [0, 1]

  waiting = INITIAL + fv * RANGE;
  //Serial.printf("value: %d, mean: %.2f\n", value, mean);

  //waiting = INITIAL + ((int)float(value) / 1024.0 * RANGE);
  digitalWrite(OUT_PIN, HIGH);
  delayMicroseconds(waiting);
  digitalWrite(OUT_PIN, LOW);
}