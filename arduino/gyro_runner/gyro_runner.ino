#include <SPI.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM9DS0.h>

Adafruit_LSM9DS0 lsm = Adafruit_LSM9DS0(1000);  // Use I2C, ID #1000
sensors_event_t accel, mag, gyro, temp;

#define LSM9DS0_XM_CS 10
#define LSM9DS0_GYRO_CS 9
#define LSM9DS0_SCLK 13
#define LSM9DS0_MISO 12
#define LSM9DS0_MOSI 11

void configureSensor(){

  lsm.setupAccel(lsm.LSM9DS0_ACCELRANGE_2G);
  lsm.setupMag(lsm.LSM9DS0_MAGGAIN_2GAUSS);
  lsm.setupGyro(lsm.LSM9DS0_GYROSCALE_245DPS);
}

void setup() {
  
#ifndef ESP8266
  while (!Serial); 
#endif

  Serial.begin(9600);
  
  if(!lsm.begin()){
    Serial.println("ERROR: no LSM9DS0 detected");
    while(1);
  }
  
  configureSensor();
}

void loop() {  

  waitForResponse();
  sendGyroscopeData();
}

void sendGyroscopeData() {

  lsm.read();
  lsm.getEvent(&accel, &mag, &gyro, &temp); 

  String data = "";

  data.concat(gyro.gyro.x);
  data.concat(",");
  data.concat(gyro.gyro.y);
  data.concat(",");
  data.concat(mag.magnetic.x);
  data.concat(",");
  data.concat(mag.magnetic.y);
  data.concat(",");
  data.concat(mag.magnetic.z);

  Serial.println(data);
}

void waitForResponse(){

  int response = 0;
  while(response != 1) {
    response = Serial.read();
  }

  delay(40);
}

