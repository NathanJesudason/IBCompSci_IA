#include <Wire.h>
#include "Sensor.h"
#include <VL53L1X.h>


VL53L1X lox1 = VL53L1X();
VL53L1X lox2 = VL53L1X();
VL53L1X lox3 = VL53L1X();

Sensor Sensor1;
Sensor Sensor2;
Sensor Sensor3;

//Extension section

int Threshold = 0;


struct spo2_cmd_ref_t {
  signed char msgType;
  signed char msgVer;
  signed char msgLen;
  signed char speed;
  signed char steer;
}
ugv_msg;



void setup() {
  /*Initialize Motor Controller
  SabertoothTXPinSerial.begin(9600);
  ST.autobaud();
              
  ST.drive(0);
  ST.turn(0); */
 // Initialize I2C
  delay(600);
  Wire.begin();
  Wire.beginTransmission(0x29);
  Serial.begin (9600);
  Serial2.begin(115200);

  Sensor1.Initialize(3, lox2, 0x33, "Sensor1"); 
  Sensor2.Initialize(2, lox1, 0x31, "Sensor2");
  Sensor3.Initialize(4, lox3, 0x35, "Sensor3");

  
  Sensor1.Startup();
  Sensor2.Startup();
  Sensor3.Startup();
  
  Sensor1.DataStart();
  Sensor2.DataStart();
  Sensor3.DataStart();

  delay(150);
  Serial.println("addresses set");

  delay (120);  
for(int i = 0; i < 6; i ++){
  Sensor1.Update();
  Sensor2.Update();
  Sensor3.Update();
  delay(250);
}
  setspeedsteer(0, 0);
  delay(10000);
}

void loop()
{

 // ST.drive(20);
 // ST.turn(0);
  setspeedsteer(20, 0);
  delay(20);
  Sensor1.Update();
  Sensor2.Update();
  Sensor3.Update();
  delay(125);
  if ((Sensor1.avg < Threshold) || (Sensor2.avg < Threshold) || (Sensor3.avg < Threshold)){
  interrupt();
  }
  

}



void setspeedsteer(signed char speed, signed char steer)
{
  ugv_msg.msgType = 1;
  ugv_msg.msgVer = 1;
  ugv_msg.msgLen = 5;
  ugv_msg.speed = speed;
  ugv_msg.steer = steer;
  Serial2.write((byte*) &ugv_msg, sizeof(ugv_msg) ); 
}
  void interrupt(){
    // Incrementer for loop
    for (int c = 0; c < 10; c++){   
      if((Sensor1.avg < Threshold)&&(Sensor2.avg < Threshold)&&(Sensor3.avg < Threshold)){
        Serial.print("all");
        Serial.println();
        setspeedsteer(-20, 30);
        c = 0;
      }
      else if(( (Sensor1.avg < Threshold) && (Sensor2.avg < Threshold))||(Sensor1.avg <Threshold)){
        Serial.print("Front and Right, or just Front");
        Serial.println();
        setspeedsteer(15,20);
        c = 0;
      }
      else if((Sensor1.avg < Threshold)&&(Sensor3.avg < Threshold)){
        Serial.print("Front and Left");
        Serial.println();
        setspeedsteer(15, -20);
        c = 0;
      }
      else if((Sensor2.avg < Threshold)&&(Sensor3.avg < Threshold)){
        Serial.print("Sides");
        Serial.println();
        setspeedsteer(-15, 20);
        c = 0;
      }
      else if(Sensor2.avg <Threshold){
        Serial.print("Right");
        Serial.println();
        setspeedsteer(15, 10);
        c = 0;
      }
      else if (Sensor3.avg < Threshold){
        Serial.print("left");
        Serial.println();
        setspeedsteer(15, -20);
        c = 0;
      }
      else {
        setspeedsteer(20, 0);
      }
        Serial.print("in Interrupt Mode");
        Serial.println();
        Sensor1.Update();
        Sensor2.Update();
        Sensor3.Update();
        delay(125);
    }
  }
