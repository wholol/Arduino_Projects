#include <Servo.h>
#include <NewPing.h>

double TotalError;  // initialize intergral error to be zero.
double LastError;   //initialize error to be zero.
double OutMax = 40;   //clamping angle output
double OutMin = -40;  //clamping angle output
#define servopin    4 // servopin on arduino.
#define TRIGGER_PIN  9  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     10  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 20 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

Servo servo1;  //instantiate class object

void setup() {
servo1.attach(servopin);  //define servopin
Serial.begin(115200);    //baud rate. CRUCIAL FOR SERIAL COMMUNICATION TO BE IN SYNC.
}

class PID_controller {

private:
double Kp;double Ki;double Kd; double setpoint;

public:
  PID_controller(double Kp,double Ki, double Kd,double setpoint){    //constructor to initialzie private variables
    this->Kp = Kp;
    this->Ki = Ki;
    this->Kd = Kd;
    this->setpoint = setpoint;
  }

  double Error(int Currentvalue) {
    return ( Currentvalue - setpoint);}

  double Proportional(double error) { 
    return Kp *(error);}

  double Intergral(double error) {
    return Ki*(TotalError += error);}

  double Derivative(double error) {
    return Kd * (error - LastError) ;}

  double PIDcompute(double P, double I, double D){
    return ( P + I + D );}
    
};

void loop() {
  while(Serial.available() > 0){
    int sensor = Serial.parseInt();   //store coordinates
    if (sensor < -20000){
      servo1.write(60);
    }
 PID_controller pid(0.134,0.005,1.9,307);  //0.134,0.0055,1.9 (Kp,Ki,Kd,setpoint)
 double E = pid.Error((sensor));            //pass in coordiantes.
 double P = pid.Proportional(E);
 double I = pid.Intergral(E);
 double D = pid.Derivative(E);
 double Output = pid.PIDcompute(P,I,D);


 if (Output > OutMax){      //clamp controller output
  Output = OutMax;
  }
   if (Output < OutMin){    //clamp controller output
    Output = OutMin;
   }

    if ((sensor >= 270) && (sensor <=340)){   //deadband region
      Output = 0;
  }
  
  servo1.write(60 + Output);    //write angle
  LastError = E;            //refresh error
 
}

if (Serial.available() <=0){      //set at default position
  servo1.write(60);
}

}

 
  
  
