#include <VL53L1X.h>
#include "moving_average.h"
class Sensor
{

  public:
  int avg;
    void Initialize(int SENSOR_PIN, VL53L1X ID, uint8_t ADDRESS, String SensorName){
        Sensor = ID;
        pin = SENSOR_PIN;
        SensorAddress = ADDRESS;
        Name = SensorName;
        Data.StartAvg();
        pinMode(pin, OUTPUT);
        digitalWrite(pin,LOW);
        delay(50);
      }
      void Startup() {
        digitalWrite(pin, HIGH);
        delay(150);
        Sensor.init();
        delay(100);
        Sensor.setAddress(SensorAddress);
      }
      void DataStart() {
        Sensor.setDistanceMode(VL53L1X::Long);
        Sensor.setMeasurementTimingBudget(50000);
        Sensor.startContinuous(50);
        Sensor.setTimeout(100);
      }
      void Update() {
        Serial.print(Name + ": ");
        avg = Data.add(Sensor.read());
        Serial.print(avg);
        if (Sensor.timeoutOccurred()) { Serial.print(" TIMEOUT"); }
        Serial.println();
        delay(50);
      }
     private:
      int pin;
      MovingAverage<5> Data;
      VL53L1X Sensor;
      uint8_t SensorAddress;
      String Name;
};
