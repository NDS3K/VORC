#include<Configs.h>

uint16_t r, g, b, c;
//Declare variables to store HSV color values
double H = 0;
double S = 0;
double V = 0; 
//Convert RGB to HSV
int rgb_to_hsv(double red, double green, double blue)
{
  double R = red/255;
  double G = green/255;
  double B = blue/255;
  double x = max(R,G);
  double y = min(R,G);
  double Cmax = max(x,B);
  double Cmin = min(y,B);
  double delta = Cmax - Cmin;
  V = Cmax;
  if(Cmax == 0)
  {
      S = 0;
  }
  else
  {
      S = (Cmax - Cmin)/Cmax ;
  }
  if(Cmax == Cmin)
  {
      H = 0;
  }
  if(Cmax == R)
  {
    H = 60*((G-B)/delta);
    if(H < 0)
    {
      H += 360;
    }
  }
  else if(Cmax == G)
  {
    H = 60*((B-R)/delta+2);
  }
  else if(Cmax == B)
  {
    H = 60*((R-G)/delta+4);
  }
//Check the condition 
  // Return 0 If black ball
  if(S <= 0.2 && V <= 0.2)
  {
    Serial.println("black");
    return 0;
  }
  //Return 1 If white ball
  else if(S <= 0.2 && V >= 0.8)
  {
    Serial.println("white");
    return 1;
  }
  //If null return -1
  else 
  {
    return -1; 
  }
}
