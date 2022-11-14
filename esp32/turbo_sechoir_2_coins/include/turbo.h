#ifndef TURBO_H
#define TURBO_H

#include <Arduino.h>
#include <Servo.h>
#include <Adafruit_NeoPixel.h>

// ANGLES
#define ANGLE_CLOSE_FLAP 90
#define ANGLE_OPEN_FLAP 10



class turbo
{
    public:
        turbo(int pinServo, int pinLED, int pinStreet);
        ~turbo();
        void setState(bool state);
        void open();
        void close();
        void flash();
    private:
        Servo* _servo;
        Adafruit_NeoPixel* _flap;
        Adafruit_NeoPixel* _street;
        bool _flasher;
        bool _enable;

};

#endif