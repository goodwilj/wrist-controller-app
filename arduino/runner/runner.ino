#include <Wire.h>
#include <Adafruit_MMA8451.h>
#include <Adafruit_Sensor.h>

Adafruit_MMA8451 mma = Adafruit_MMA8451();
double coords[3]; // holds x, y, z values
String orientation;
sensors_event_t event; 

void setup() {
  Serial.begin(9600);

// initialize the device
if (! mma.begin()) {
    Serial.println("ERROR");
    while (1);
  }
  Serial.println("TRANSMITTING.");

  mma.setRange(MMA8451_RANGE_8_G);
}

void loop() {

  mma.read();
  readAcceleration();
  transmitPacket();

  // TODO: get rid of acceleration due to gravity (very hard to do)

  delay(10); // 10ms poll time
}

void readAcceleration(){

  mma.getEvent(&event);
  
  coords[0] = event.acceleration.x;
  coords[1] = event.acceleration.y;
  coords[2] = event.acceleration.z;

  uint8_t o = mma.getOrientation();

  switch (o) {
    case MMA8451_PL_PUF: 
      orientation = "PUF";
      break;
    case MMA8451_PL_LRF: 
      orientation = "LRF";
      break;
    case MMA8451_PL_LLF:  
      orientation = "LLF";
      break;
    default:
      orientation = "NA";
      break;
  }
}

void transmitPacket(){

  String data = "";
  data.concat(String(coords[0]));
  data.concat(",");
  data.concat(String(coords[1]));
  data.concat(",");
  data.concat(String(coords[2]));
  data.concat(",");
  data.concat(orientation);
  
  Serial.println(data);
}

