#include <Arduino.h>
#include <turbo.h>


#define SERVO_PIN_1 5
#define SERVO_PIN_2 6
#define SERVO_PIN_3 9
#define SERVO_PIN_4 10
#define LED_1 2
#define LED_2 3
#define LED_3 7
#define LED_4 8
#define LED_COIN_1 4
#define LED_COIN_2 12
#define LED_COIN_3 A0
#define LED_COIN_4 A1


// Objets

turbo turbo_1(SERVO_PIN_1, LED_1, LED_COIN_1);
turbo turbo_2(SERVO_PIN_2, LED_2, LED_COIN_2);
turbo turbo_3(SERVO_PIN_3, LED_3, LED_COIN_3);
turbo turbo_4(SERVO_PIN_4, LED_4, LED_COIN_4);


void setup() {

}

void loop() {
  static long lastMillis = 0;
  static int streetState = 0;
  static long counterTime = 0;
  static bool stateTurbo[4] = {0,0,0,0};
  static turbo turboTab[4] = {turbo_1,turbo_2,turbo_3,turbo_4};


  if(millis()- lastMillis >250)
  {
    counterTime ++;
    lastMillis = millis();
   
    for(int i =0; i<4; i++)
    {
      turboTab[i].flash();
      turboTab[i].setState(stateTurbo[i]);
    }
  }

  if(counterTime > 4)
  {
    counterTime = 0;
    switch (streetState)
    {
    case 0:
        stateTurbo[0] = 1;
        stateTurbo[1] = 0;
        stateTurbo[3] = 0;
        stateTurbo[4] = 0;
        streetState ++;
      break;
    case 1:
        stateTurbo[0] = 0;
        stateTurbo[1] = 1;
        stateTurbo[2] = 0;
        stateTurbo[3] = 0;
        streetState ++;
      break;
    case 2:
        stateTurbo[0] = 0;
        stateTurbo[1] = 0;
        stateTurbo[2] = 1;
        stateTurbo[3] = 0;
        streetState ++;
      break;
    case 3:
        stateTurbo[0] = 0;
        stateTurbo[1] = 0;
        stateTurbo[2] = 0;
        stateTurbo[3] = 1;
        streetState ++;
      break;

    default:
      break;
    }
  }
  // put your main code here, to run repeatedly:
}