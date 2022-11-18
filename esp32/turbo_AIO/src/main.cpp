#include <Arduino.h>
#include <turbo.h>

#define SERVO_PIN 13
#define LED_PIN 14
#define STREET_PIN 12

turbo turbo_1(SERVO_PIN, LED_PIN, STREET_PIN);



void setup() {
	Serial.begin(115200);
  Serial.println("Turbo start here");
}

void loop() {
  static long lastMillis = 0;
  static long counterTime = 0;
  static long counter2 = 0;

  if(millis()- lastMillis > 250)
  {
    counterTime ++;
    counter2 ++;
    lastMillis = millis();
   
    if(counterTime == 4)
    {
      counterTime = 0;
      turbo_1.flash();
    }

    if(counter2 == 20)
    {
      turbo_1.setStreet(GREEN);
    }
    if(counter2 == 40)
    {
      turbo_1.setStreet(YELLOW);
    }
    if(counter2 == 60)
    {
      turbo_1.setStreet(RED);
      counter2 = 0;
    }
  }

}