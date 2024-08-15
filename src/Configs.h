#include <Adafruit_PWMServoDriver.h>
#include <Adafruit_TCS34725.h>
#include <Wire.h>
#include <PS2X_lib.h>
#include <stdio.h>
#include <stdlib.h>
#include <Arduino.h>
#include <algorithm>
#include <SPI.h>

//DC Motor
#define pin1_dc1 8 
#define pin2_dc1 9 
#define pin1_dc2 10
#define pin2_dc2 11
#define pin1_dc3 12
#define pin2_dc3 13
#define pin1_dc4 14
#define pin2_dc4 15

//Servo
#define servo_bg 7
#define servo_wg 6
#define servo_m 3
#define servo_cd 2

//Gamepad
#define PS2_DAT 12 // MISO
#define PS2_CMD 13 // MOSI
#define PS2_SEL 15 // SS
#define PS2_CLK 14 // SLK

//Distance Sensor
const int trig = 2;    
const int echo = 39;    


// Initialize objects for using PWM and TCS34725
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_401MS, TCS34725_GAIN_1X);

//Initialize
void initialize()
{
    Wire.begin(); // Initialize I2C communication
    Serial.begin(115200); // Initialize Serial communication
    pwm.begin(); // Initialize PWM usage
    pwm.setOscillatorFrequency(27000000); // Set the oscillator frequency
    pwm.setPWMFreq(50); // Set the PWM frequency
    Wire.setClock(400000); // Set the I2C clock speed
}
// Connect the color sensor 
void Sensor_ini()
{
    if (tcs.begin()) {
    Serial.println("Found sensor");
  } else {
    Serial.println("No TCS34725 found ... check your connections");
    while (1);
  }
} 
// Initialize pins for the distance sensor
void distance_ini()
{  
    pinMode(trig,OUTPUT);  
    pinMode(echo,INPUT);    
}
 

