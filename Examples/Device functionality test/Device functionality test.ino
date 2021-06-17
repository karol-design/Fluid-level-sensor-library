#include <Fluid_level_sensor.h>

Fluid_level_sensor  testDevice;

void setup() {
  Serial.begin(115200);
}

void loop() {
  float distance = testDevice.measureDistance();
  String msg = testDevice.createMessage(-50, 0x0, 95, distance); 

  Serial.print("Measured distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  
  Serial.print("Message to the user: ");
  Serial.println(msg);
}
