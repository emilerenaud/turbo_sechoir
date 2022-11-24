#include <Arduino.h>

int LEDpin = 2;
unsigned long previous_time = 0;
int LEDdelay = 1000;

void blink_LED()
{
  if(millis() - previous_time >= LEDdelay )
  {
    bool value = digitalRead(LEDpin);
    if(value)
    {
      digitalWrite(LEDpin, LOW);
    }
    if(!value)
    {
      digitalWrite(LEDpin, HIGH);
    } 
    
    Serial.println(value);
    previous_time = millis();
  }
}

void setup() {
  // put your setup code here, to run once:
  pinMode(LEDpin, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:


  blink_LED();

  if(Serial.available())
  {
    int data = Serial.parseInt();
    if(data == 1)
    {
      LEDdelay = 500;
    }
    if(data == 2)
    {
      LEDdelay = 250;
    }
    if(data == 3)
    {
      LEDdelay = 100;
    }
    if(data == 4)
    {
      LEDdelay = 2000;
    }
    if(data == 5)
    {
      LEDdelay = 10000;
    }
  }



}