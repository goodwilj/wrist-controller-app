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

double x_vals[5] = {0};
double y_vals[5] = {0};

double x, y;

int count = 0;

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

  int i;
  for (i = 0; i < 4; i++){
    x_vals[i+1] = x_vals[i];
    y_vals[i+1] = y_vals[i];
  }

  x_vals[0] = gyro.gyro.x;
  y_vals[0] = gyro.gyro.y;

  x = 0;
  y = 0;

  x += x_vals[0]*-0.0053;
  x += x_vals[1]*0.1901;
  x += x_vals[2]* 0.6304;
  x += x_vals[3]*0.1901;
  x += x_vals[4]*-0.0053;

  y += y_vals[0]*-0.0053;
  y += y_vals[1]*0.1901;
  y += y_vals[2]* 0.6304;
  y += y_vals[3]*0.1901;
  y += y_vals[4]*-0.0053;

  String data = "";

  data.concat(x);
  data.concat(",");
  data.concat(y);
  data.concat(",");
  data.concat(mag.magnetic.x);
  data.concat(",");
  data.concat(mag.magnetic.y);
  data.concat(",");
  data.concat(mag.magnetic.z);
  data.concat(",");
  data.concat(accel.acceleration.z);

  Serial.println(data);
}

void waitForResponse(){

  int response = 0;
  while(response != 1) {
    response = Serial.read();
  }

  delay(50);
}

