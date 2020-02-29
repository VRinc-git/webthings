#include <Arduino.h>
#include"RGB.h"



void RGB::Red()
{
  digitalWrite(_red, HIGH);
  digitalWrite(_green, LOW);
  digitalWrite(_blue, LOW);
}
void RGB::Green()
{
  digitalWrite(_red, LOW);
  digitalWrite(_green, HIGH);
  digitalWrite(_blue, LOW);
}
void RGB::Blue()
{
  digitalWrite(_red, LOW);
  digitalWrite(_green, LOW);
  digitalWrite(_blue, HIGH);
}
void RGB::Cyan()
{
  digitalWrite(_red, LOW);
  digitalWrite(_green, HIGH);
  digitalWrite(_blue, HIGH);
}
void RGB::Purple()
{
  digitalWrite(_red, HIGH);
  digitalWrite(_green, LOW);
  digitalWrite(_blue, HIGH);
}
void RGB::Yellow()
{
  digitalWrite(_red, HIGH);
  digitalWrite(_green, HIGH);
  digitalWrite(_blue, LOW);
}
void RGB::White()
{
  digitalWrite(_red, HIGH);
  digitalWrite(_green, HIGH);
  digitalWrite(_blue, HIGH);
}
