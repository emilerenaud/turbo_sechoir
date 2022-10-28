#include <Arduino.h>
#include <Wire.h>
#include "Adafruit_BMP280.h"
#include "Adafruit_AHTX0.h"
#include "DHT.h"

// I2C communication pins
#define BMP1_SCL  (22)
#define BMP1_SDA (21)

// Global
int readFreq = 10000;  // 10 sec
unsigned long previousTime = 0;

// BMP280 sensor 
TwoWire I2CBMP1 = TwoWire(0);
Adafruit_BMP280 bmp1;

float bmpTemp = 0;     // *C
float bmpPressure = 0; // Pa

// AHT20 sensor
Adafruit_AHTX0 aht1;
sensors_event_t ahtHumidity, ahtTemp;

// DHT22 sensor
#define DHTTYPE DHT22
#define DHTPIN 4
DHT dht1(DHTPIN, DHTTYPE);
float dhtTemp = 0;
float dhtHumidity = 0;

void readBMP()
{
  bmpTemp = bmp1.readTemperature();
  bmpPressure = bmp1.readPressure();
}

void readAHT()
{
  aht1.getEvent(&ahtHumidity, &ahtTemp);
}

void readDHT()
{
  dhtTemp = dht1.readTemperature();
  dhtHumidity = dht1.readHumidity();
}

void logHeader()
{
  Serial.println("\n");
  Serial.println("time,temperature BMP280,pressure BMP280,temperature AHT20,humidity AHT20,temperature DHT22,humidity DHT22");
  Serial.println("\n");
}

void logSerial()
{
  double elapsedTime = millis()/1000;
  Serial.print(elapsedTime);
  Serial.print(",");
  Serial.print(bmpTemp);
  Serial.print(",");
  Serial.print(bmpPressure);
  Serial.print(",");
  Serial.print(ahtTemp.temperature);
  Serial.print(",");
  Serial.print(ahtHumidity.relative_humidity);
  Serial.print(",");
  Serial.print(dhtTemp);
  Serial.print(",");
  Serial.print(dhtHumidity);
  Serial.println("\n");

}

void setup() {
  // Serial Comm
  Serial.begin(115200);
  while(!Serial)
  {
    delay(100);
  }
  logHeader();

  // // I2C Comm
  // I2CBMP1.begin(BMP1_SDA, BMP1_SCL, 100000);

  // BMP280
  if(!bmp1.begin())
  {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring or try a different address!"));
    while(1);
  }
  
  // AHT20
  if(!aht1.begin())
  {
    Serial.println("Could not find AHT20, check wiring");
    while(1);
  }

  // DHT22
  dht1.begin();
}

void loop() {
  if(millis()-previousTime >= readFreq)
  {
    readBMP();
    readAHT();
    readDHT();
    logSerial();
    previousTime = millis();
  }
}