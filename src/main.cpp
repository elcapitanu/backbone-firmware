#include <Arduino.h>

#include "RP2040_PWM.h"

RP2040_PWM *motor, *rudder, *piston;

float dcMotor, dcRudder, dcPiston;

float mapInt2Float(int value, int min, int max, float minF, float maxF)
{
  return static_cast<float>(value - min) / (max - min) * (maxF - minF) + minF;
}

void setup()
{
  Serial.begin(19200);

  motor = new RP2040_PWM(19, 50, 0);
  rudder = new RP2040_PWM(22, 50, 0);
  piston = new RP2040_PWM(21, 50, 0);

  dcMotor = dcRudder = dcPiston = 7.5;
}

void loop()
{
  String rv = Serial.readStringUntil('\n');

  if (rv.length() != 0)
  {
    char *header = strtok((char *)rv.c_str(), ",");

    if (!strcmp(header, "RUDDER"))
      dcRudder = mapInt2Float(atoi(strtok(NULL, ",")), -127, 127, 2.1f, 12.8f);
    else if (!strcmp(header, "PISTON"))
      dcPiston = mapInt2Float(atoi(strtok(NULL, ",")), -127, 127, 2.1f, 12.8f);
    else if (!strcmp(header, "MOTOR"))
      dcMotor = mapInt2Float(atoi(strtok(NULL, ",")), -127, 127, 5.5f, 9.5f);
  }

  motor->setPWM(19, 50, dcMotor);
  rudder->setPWM(22, 50, dcRudder);
  piston->setPWM(21, 50, dcPiston);
}