#include <turbo.h>

turbo::turbo(int pinServo, int pinLED, int pinStreet)
{
    _servo = new Servo;
    _servo->attach(pinServo);
    _servo->write(ANGLE_CLOSE_FLAP);

    _flap = new Adafruit_NeoPixel(6, pinLED, NEO_GRB + NEO_KHZ800);
    _street = new Adafruit_NeoPixel(3, pinStreet, NEO_GRB + NEO_KHZ800);

    _flasher = 0;
    _enable = 0;

    _flap->begin();
    _flap->setBrightness(100);
    _street->begin();
    _street->setBrightness(100);
}

turbo::~turbo()
{

}

void turbo::open()
{
    _servo->write(ANGLE_OPEN_FLAP);
    _enable = true;
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
        }
        else
        {
            _flap->clear();
        }

        _flap->show();
        _flasher ++;
    }
}