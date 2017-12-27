//
// Test Arduion Duemilanove
// Reads Temperatures from DS18B20s and displays it on a LCD
//

#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal.h>

#define ONE_WIRE_PIN 10
#define TEMPERATURE_PRECISION 12
#define MAX_SENSORS 8

/*
   Variables Start
*/

OneWire onew(ONE_WIRE_PIN); // Create OneWire Object
DallasTemperature sensors(&onew);
uint8_t ds18_count = 0; // number of DS18B20 sensors

// arrays to hold 8 device addresses and temperatures
DeviceAddress ds18_sensors[MAX_SENSORS];
float temperatures[MAX_SENSORS];

//LCD Pins
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


/*
   Variables End
*/

void setup() {
  // put your setup code here, to run once:
  Serial.begin(19200);
  delay(200);

  sensors.begin(); // Start up the Dallas library

  // locate devices on the bus
  ds18_count = sensors.getDeviceCount();
  Serial.print("Found ");
  Serial.print(ds18_count);
  Serial.println(" devices.");

  // Search for sensors:
  onew.reset_search();
  for (int i = 0; i < ds18_count; ++i) {
    if (!onew.search(ds18_sensors[i])) Serial.println("Unable to find address for sensor.");
    printAddress(ds18_sensors[i]);
    Serial.println();
    sensors.setResolution(ds18_sensors[i], TEMPERATURE_PRECISION);
  }

  // LCD
  lcd.begin(16, 2);
  lcd.cursor();

  // Flash LED threetimes:
  LedFlash(LED_BUILTIN, 100, 3);
}

void loop() {
  // request to all devices on the bus
  Serial.print("Requesting temperatures...");
  sensors.requestTemperatures();
  Serial.println("DONE");

  // get temperatures
  Serial.print("Temperatures: ");
  lcd.setCursor(0,0);
  for (int i = 0; i < ds18_count; ++i) {

    temperatures[i] = sensors.getTempC(ds18_sensors[i]);
    Serial.print(temperatures[i]);
    Serial.print(" ");
    lcd.print(temperatures[i]);
    lcd.print(" ");
    Serial.println("");
  }

  // Flash LED once:
  LedFlash(LED_BUILTIN, 500, 1);

}

/*
   Flash the LED at LedPin with a LedDealy LedCount times.
*/
void LedFlash(int LedPin, int LedDelay, int LedCount)
{
  int i;

  for (i = 0; i < LedCount; ++i)
  {
    digitalWrite(LedPin, HIGH);
    delay(LedDelay);
    digitalWrite(LedPin, LOW);
    delay(LedDelay);
  }
}

// function to print a device address
void printAddress(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    // zero pad the address if necessary
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
  Serial.print(" - ");
  if (deviceAddress[7] < 16) Serial.print("0");
  Serial.print(deviceAddress[7], HEX);
}


// function to print the temperature for a device
void printTemperature(DeviceAddress deviceAddress)
{
  float tempC = sensors.getTempC(deviceAddress);
  Serial.print("Temp C: ");
  Serial.print(tempC);
}

// function to print a device's resolution
void printResolution(DeviceAddress deviceAddress)
{
  Serial.print("Resolution: ");
  Serial.print(sensors.getResolution(deviceAddress));
  Serial.println();
}

// main function to print information about a device
void printData(DeviceAddress deviceAddress)
{
  Serial.print("Device Address: ");
  printAddress(deviceAddress);
  Serial.print(" ");
  printTemperature(deviceAddress);
  Serial.println();
}



