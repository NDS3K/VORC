#include <Subsystem/Controller.h>

int  ball_val;

void setup() 
{
  initialize();
  Sensor_ini();
  distance_ini();
  setupPS2controller();
}
void loop() 
{
  ps2x.read_gamepad(0,0);
  tcs.getRawData(&r,&g,&b,&c);
  ball_val = rgb_to_hsv(r,g,b);
  classify(ball_val);
  controller();
  delay(50); //Avoid cache overflow when loading a large amount of information
}

