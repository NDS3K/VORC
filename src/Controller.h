#include <DistanceSensor.h>

PS2X ps2x ;

int y = 128 ;
int x = 127 ;
double min_speed = 0.0;
double speed = 0.0;
double max_speed = 0.8;
double output = 0;

//Connect to Gamepad
void setupPS2controller()
{
    int error = -1 ;
    while(error != 0)
    {
        error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, true, true);
        Serial.println(error);
    }
}
//Funtion to set power for DC Motor
void setPower(int pin1, int pin2, int x)
{
    x = constrain(x, -4095, 4095); 
    if (x > 0) {
        pwm.setPWM(pin1, 0, x);
        pwm.setPWM(pin2, 0, 0);
    } else {
        pwm.setPWM(pin1, 0, 0);
        pwm.setPWM(pin2, 0,-x);
    }
}
//Base
void move_forward(int x)
{
    setPower(pin1_dc1,pin2_dc1,x);
    setPower(pin2_dc4,pin1_dc4,x);
}
void turn_left(int x)
{
    setPower(pin1_dc1,pin2_dc1,x);
    setPower(pin1_dc4,pin2_dc4,x);
}
void turn_right(int x)
{
    setPower(pin2_dc4,pin1_dc4,x);
    setPower(pin2_dc1,pin1_dc1,x);
}
void move_backward(int x)
{
    setPower(pin2_dc1,pin1_dc1,x);
    setPower(pin1_dc4,pin2_dc4,x);
}
void stop()
{
    setPower(pin1_dc1,pin2_dc1,0);
    setPower(pin2_dc4,pin1_dc4,0);
}
//Intake
void intake_run(int x)
{ 
    setPower(pin1_dc3,pin2_dc3,x);
}
void intake_stop()
{
    setPower(pin1_dc3,pin2_dc3,0);
}
void intake_reverse(int x)
{
    setPower(pin2_dc3,pin1_dc3,x);
}
//Climber
void climber_up(int x)
{
    setPower(pin1_dc2,pin2_dc2,x);
}
void climber_down(int x) 
{
    setPower(pin2_dc2,pin1_dc2,x);
}
void climber_stop()
{
    setPower(pin1_dc2,pin2_dc2,0);
}

//Classify ball
void classify(int val)
{   
    if(val == 1)
    {
        pwm.writeMicroseconds(servo_m,1600);//Servo rotates slowly to control the ball flow
        pwm.setPWM(servo_cd,0,0); // Turn Servo to 0 deg pos
    }
    else if(val == 0)
    {
        pwm.writeMicroseconds(servo_m,1600);//Servo rotates slowly to control the ball flow
        pwm.setPWM(servo_cd,0,410);// Turn Servo to 90 deg pos
    }
    
    else if(val == -1)
    {
        pwm.writeMicroseconds(servo_m,1600);//Servo rotates slowly to control the ball flow
    }
}

//Control
void controller()
{   
    int x ;
    double output;

//Joystick Setting Up
    int ly = ps2x.Analog(PSS_LY);
    int ry = ps2x.Analog(PSS_RY);
    int rx = ps2x.Analog(PSS_RX);
    int ly_raw = y - ly;
    int ry_raw = y - ry;
    int rx_raw = x - rx; 
    int ly_val = abs(ly_raw);
    int ry_val = abs(ry_raw);
    int rx_val = abs(rx_raw);
    int ly_po = map(ly_val,0,128,0,4095);
    int ry_po = map(ry_val,0,128,0,4095);
    int rx_po = map(rx_val,0,128,0,4095);
  

//SoftStart     
    if((ly_raw != 0 || rx_raw != 0) && speed < max_speed)
        {
            speed += 0.05;
        }
    else
        {
            stop();
            speed = min_speed;
        }
//Base Control
    if(ly_raw > 0)
    {
        move_forward(ly_po*speed);
    }
    else if(ly_raw < 0)
    {
        move_backward(ly_po*speed);
    }
    else if(rx_raw > 0)
    {
        turn_left(rx_po*speed);
    }
    else if(rx_raw < 0)
    {
        turn_right(rx_po*speed);
    }
    else
    {
        stop();
        speed = min_speed; 
    }

//Intake Control    
    if (ps2x.ButtonPressed(PSB_PAD_LEFT))
    {
        intake_reverse(4095*0.8);
    }
    else if(ps2x.ButtonPressed(PSB_PAD_RIGHT))
    {
        intake_run(4095*0.8); 
    }
    else if(ps2x.ButtonPressed(PSB_CROSS))
    {
        intake_stop();
    }

//ClimberControl
    //Setpoint_1
    if(ps2x.Button(PSB_SQUARE))
    {   
        output = constrain(PIDControl(31.6,meansured_distance()),-1,1);
        x = map(output,-1,1,-4095,4095);
        climber_up(x);
    }
    //Setpoint_2
    else if(ps2x.Button(PSB_TRIANGLE))
    {
        output = constrain(PIDControl(1800,meansured_distance()),-1,1);
        x = map(output,-1,1,-4095,4095);
        climber_up(x);
    }
    //Setpoint_3
    else if(ps2x.Button(PSB_CIRCLE))
    {
        output = constrain(PIDControl(0,meansured_distance()),-1,1);
        x = map(output,-1,1,-4095,4095);
        climber_up(x);
    }
    // Up
    else if(ry_raw > 0)
    {
        climber_up(ry_po);
    }
    // Down
    else if(ry_raw < 0)
    {
        climber_down(ry_po);
    }
    // Stop
    else
    {
        climber_stop;
    }

// Scoring
    if(ps2x.NewButtonState(PSB_L1))
    {
        pwm.setPWM(servo_wg,0,205); // Turn Servo to 90 deg pos
    }
    else if(ps2x.NewButtonState(PSB_L1))
    {
        pwm.setPWM(servo_wg,0,0); // Turn Servo to 0 deg pos
    }
    if(ps2x.ButtonPressed(PSB_R1))
    {
        pwm.writeMicroseconds(servo_bg,2000);// Servo CW direction 
    }
    else if(ps2x.ButtonPressed(PSB_R2))
    {
        pwm.writeMicroseconds(servo_bg,1000);//  Servo CCW direction 
    }   
    else
    {
        pwm.writeMicroseconds(servo_bg,1500);// Stop Servo
    }
}