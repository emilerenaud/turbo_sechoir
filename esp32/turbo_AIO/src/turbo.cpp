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
    _servo->write(ANGLE_OPEN_FLAP);
    _enable = 1;
}

void turbo::close()
{
    _servo->write(ANGLE_CLOSE_FLAP);
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

void turbo::setStreet(int light)
{
    switch (light)
    {
    case GREEN:
        _street->clear();
        _street->setPixelColor(1,0,100,0);
        _street->setPixelColor(1,0,100,0);
        _street->setPixelColor(1,0,100,0);
        break;
    case YELLOW:
        _street->clear();
        _street->setPixelColor(2,100,100,0);
        _street->setPixelColor(2,100,100,0);
        _street->setPixelColor(2,100,100,0);
        break;
    case RED:
        _street->clear();
        _street->setPixelColor(3,100,0,0);
        _street->setPixelColor(3,100,0,0);
        _street->setPixelColor(3,100,0,0);
        break;

    default:
        break;
    }
    _street->show();
}