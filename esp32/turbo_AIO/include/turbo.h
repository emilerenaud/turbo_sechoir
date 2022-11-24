#ifndef TURBO_H
#define TURBO_H

#include <Arduino.h>
#include <ESP32Servo.h>
#include <Adafruit_NeoPixel.h>


// Servo
#define ANGLE_CLOSE_FLAP 58
#define ANGLE_OPEN_FLAP 135

#define MIN_US 500
#define MAX_US 2400

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
        void updateServo();
        void setStreet(int light);
    private:
        Servo* _servo;
        Adafruit_NeoPixel* _neopixel = nullptr;
        // Adafruit_NeoPixel* _street = nullptr;

        // Servo Var
        int _angle;
        int _lastAngle;
        long _lastMillisServo;

        int _flasher;
        long _lastMillisFlasher;
        bool _enable;

};

#endif


