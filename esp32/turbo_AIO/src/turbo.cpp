#include <turbo.h>

turbo::turbo(int pinServo, int pinLED, int pinStreet)
{
    ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);

    _servo = new Servo;
    _servo->setPeriodHertz(50);
    _servo->attach(pinServo,MIN_US,MAX_US);

    _neopixel = new Adafruit_NeoPixel[1];
    _neopixel[0].setPin(pinLED);
    _neopixel[0].updateLength(6);
    _neopixel[0].updateType(NEO_GRB + NEO_KHZ800);

    // _neopixel[1].setPin(pinStreet);
    // _neopixel[1].updateLength(9);
    // _neopixel[1].updateType(NEO_GRB + NEO_KHZ800);


    _neopixel[0].begin();
    // _neopixel[1].begin();

    // _neopixel = new Adafruit_NeoPixel(6, pinStreet, NEO_GRB + NEO_KHZ800);
    // _street = new Adafruit_NeoPixel(9, pinStreet, NEO_GRB + NEO_KHZ800);

    _flasher = 0;
    _enable = 0;

    _angle = ANGLE_OPEN_FLAP;
    _servo->write(ANGLE_OPEN_FLAP);

    // _flap->begin();
    // // _flap->clear();
    // // _flap->show();
    _neopixel[0].setBrightness(100);
    // _neopixel[1].setBrightness(100);
    // _street->begin();
    // // _street->clear();
    // // _street->show();
    // _street->setBrightness(100);
    this->open();
}

turbo::~turbo()
{

}

void turbo::open()
{   
    this->setStreet(RED);
    _servo->write(ANGLE_OPEN_FLAP);
    // Serial.println("test open");
    _angle = ANGLE_OPEN_FLAP;
    _enable = 1;
}

void turbo::close()
{
    this->setStreet(GREEN);
    _servo->write(ANGLE_CLOSE_FLAP);
    _angle = ANGLE_CLOSE_FLAP;
    _neopixel[0].clear();
    _neopixel[0].show();
    _enable = 0;
}

void turbo::setState(bool state)
{
    if(state)
        this->open();
    else
        this->close();
}

void turbo::flash()
{
    if(millis() - this->_lastMillisFlasher >= 500)
    {
        this->_lastMillisFlasher = millis();
        if(_enable)
        {
            if(_flasher & 0x01)
            {
                _neopixel[0].fill(_neopixel[0].Color(100, 0, 0));
                Serial.println("_flasher ON");
            }
            else
            {
                Serial.println("_flasher OFF");
                _neopixel[0].clear();
            }

            _neopixel[0].show();
            _flasher ++;
        }
    }
}

void turbo::updateServo()
{
    if(millis() - this->_lastMillisServo >= 100)
    {
        this->_lastMillisServo = millis();
        if(this->_lastAngle != this->_angle)
        {
            _servo->write(_lastAngle);
            if(this->_lastAngle - this->_angle > 0)
            {
                _lastAngle ++;
            }
            else
            {
                _lastAngle --;
            }
        }
    }
    
}

void turbo::setStreet(int light)
{
    // _street->setPixelColor(0,0,100,0);
    // if(_street->canShow())
    // {
    //     // _street->show();
    // }

    // switch (light)
    // {
    // case GREEN:
    //     _neopixel[1].clear();
    //     _neopixel[1].setPixelColor(0,0,100,0);
    //     _neopixel[1].setPixelColor(1,0,100,0);
    //     _neopixel[1].setPixelColor(2,0,100,0);
    //     break;
    // case YELLOW:
    //     _neopixel[1].clear();
    //     _neopixel[1].setPixelColor(3,100,100,0);
    //     _neopixel[1].setPixelColor(4,100,100,0);
    //     _neopixel[1].setPixelColor(5,100,100,0);
    //     break;
    // case RED:
    //     _neopixel[1].clear();
    //     _neopixel[1].setPixelColor(6,100,0,0);
    //     _neopixel[1].setPixelColor(7,100,0,0);
    //     _neopixel[1].setPixelColor(8,100,0,0);
    //     // _street->show();
    //     break;

    // default:
    //     break;
    // }
    // _neopixel[1].show();
}