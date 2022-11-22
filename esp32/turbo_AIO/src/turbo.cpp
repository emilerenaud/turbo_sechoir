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

    _flap = new Adafruit_NeoPixel(6, pinLED, NEO_GRB + NEO_KHZ800);
    _street = new Adafruit_NeoPixel(9, pinStreet, NEO_GRB + NEO_KHZ800);

    _flasher = 0;
    _enable = 0;

    _angle = ANGLE_OPEN_FLAP;
    _servo->write(ANGLE_OPEN_FLAP);

    _flap->begin();
    // _flap->clear();
    // _flap->show();
    _flap->setBrightness(100);
    _street->begin();
    // _street->clear();
    // _street->show();
    _street->setBrightness(100);
    this->open();
}

turbo::~turbo()
{

}

void turbo::open()
{   
    this->setStreet(RED);
    // _servo->write(ANGLE_OPEN_FLAP);
    _angle = ANGLE_OPEN_FLAP;
    _enable = 1;
}

void turbo::close()
{
    this->setStreet(GREEN);
    // _servo->write(ANGLE_CLOSE_FLAP);
    _angle = ANGLE_CLOSE_FLAP;
    _flap->clear();
    _flap->show();
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
    if(millis() - _lastMillisFlasher >= 1000)
    {
        this->_lastMillisFlasher = millis();
        if(_enable)
        {
            if(_flasher & 0x01)
            {
                _flap->fill(_flap->Color(100, 0, 0));
                Serial.println("_flasher ON");
            }
            else
            {
                Serial.println("_flasher OFF");
                _flap->clear();
            }

            _flap->show();
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
    switch (light)
    {
    case GREEN:
        _street->clear();
        _street->setPixelColor(0,0,100,0);
        _street->setPixelColor(1,0,100,0);
        _street->setPixelColor(2,0,100,0);
        break;
    case YELLOW:
        _street->clear();
        _street->setPixelColor(3,100,100,0);
        _street->setPixelColor(4,100,100,0);
        _street->setPixelColor(5,100,100,0);
        break;
    case RED:
        _street->clear();
        _street->setPixelColor(6,100,0,0);
        _street->setPixelColor(7,100,0,0);
        _street->setPixelColor(8,100,0,0);
        break;

    default:
        break;
    }
    _street->show();
}