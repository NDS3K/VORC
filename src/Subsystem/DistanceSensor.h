#include<Subsystem/ColorSensor.h>
   
double Kp = 0.0;
double Kd = 0.0;
double Ki = 0.0;

double integralSum = 0;
double lastError = 0;
double lastTime = 0;

double distance;

// Calculate the distance
double meansured_distance()
{
    unsigned long duration; 
    
    digitalWrite(trig,0);   
    delayMicroseconds(2);
    digitalWrite(trig,1);   
    delayMicroseconds(5);   
    digitalWrite(trig,0);   
        
    duration = pulseIn(echo,HIGH);  
    
    distance = int(duration/2/29.412);
  
    Serial.print(distance);
    Serial.println("cm");
    delay(50);
    return distance;
}
// Function to calculate the PID output
double PIDControl(double reference, double state)
{
    double runtime =(millis()-lastTime)/1000;
    double error = reference - state;
    integralSum = error * (runtime);
    double derivative = (error - lastError)/runtime;
    lastError = error;
    lastTime =  millis();
    double output = (error*Kp)+(integralSum*Ki)+(derivative*Kd);
    return  output;
}