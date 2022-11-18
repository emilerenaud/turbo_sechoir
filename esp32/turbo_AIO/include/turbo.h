#ifndef TURBO_H
#define TURBO_H

#include <Arduino.h>
#include <ESP32Servo.h>
#include <Adafruit_NeoPixel.h>


// Servo
#define ANGLE_CLOSE_FLAP 0
#define ANGLE_OPEN_FLAP 150

#define MIN_US 1000
#define MAX_US 2000

// street light
#define GREEN 0
#define YELLOW 1
#define RED 2


class turbo
{
    public:
        turbo(int pinServo, int pinLED, int pinStreet);
        ~turbo();
        void setState(bool state);
        void open();
        void close();
        void flash();
        void setStreet(int light);
    private:
        Servo* _servo;
        Adafruit_NeoPixel* _flap;
        Adafruit_NeoPixel* _street;
        
        int _flasher;
        bool _enable;

};

#endif