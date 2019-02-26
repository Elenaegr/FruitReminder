#include "HX711.h"

const int DOUT = 3;
const int CLK = 2;

const float CALIBRATION = 2000.f;
const int MIN_DIFFERENCE = 3;
const int FREQ = 10;
const int ZERO = 2;

int oldWeight = 0;

HX711 scale(DOUT, CLK);

void initScale() {
  Serial.println("Initializing scale");
  
  scale.set_scale(CALIBRATION);
  scale.tare(); //Reset the scale to 0

  Serial.println("Scale has been init");

}

int getWeight() {
  float units = scale.get_units(FREQ);
  //Serial.println(units);
  
  return (int)(abs(units));
}

void updateOldWeight(int newWeight) {
  oldWeight = newWeight;
}

bool didUserEatFruit() {
  int newWeight = getWeight();
  if (oldWeight - newWeight >= MIN_DIFFERENCE) {
    updateOldWeight(newWeight);
    return true;
  }
  return false;
}

bool isEmpty() {
  int weight = getWeight();
  if (weight <= ZERO) {
    updateOldWeight(0);
    return true;
  }
  return false;
}

bool isRefill() {
  int newWeight = getWeight();
  if (/*oldWeight == 0 &&*/ newWeight - oldWeight >= MIN_DIFFERENCE) {
    updateOldWeight(newWeight);
    return true;
  }
  return false;
}

void ADCsleepMode() {
  scale.power_down();              // put the ADC in sleep mode
  delay(1000);
  scale.power_up();
}
