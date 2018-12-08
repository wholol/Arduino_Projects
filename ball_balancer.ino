#include <Servo.h>

double TotalError = 0;  // initialize intergral error to be zero.
double LastError = 0;   //initialize error to be zero.
double Outmax = 60;   //clamping angle output
double Outmin = -60;  //clamping angle output
#define servopin 4;   // servopin on arduino.

Servo servo1;  //instantiate class object

void setup() {
 Servo.attach(servopin);  //define servopin
 Serial.begin(115200);    //baud rate. CRUCIAL FOR SERIAL COMMUNICATION TO BE IN SYNC.
}

class PID_controller {

private:
  const double *const setpoint = 319;
  
 const double Kp = 0; const double Ki = 2; ; const double Kd = 0;

  public:
  double Error(double  CurrentVal) {
    return = CurrentVal - setpoint;}

  double Proportional(double error) { 
    return Kp *error;}

  double intergral(double error) {
    return Ki*(TotalError += error);}

  double Derivative(double error) {
    return Kd * (LastError - error) ;}

  double PIDcompute(double P, double I, double D){
    return ( P + I + D );
    return Output;} 
};
void loop() {
 PID_controller* pid;     //instantiate class 
 double E = pid->Error(//read computer vision data through serial//);
 double P = pid->Proportional(E);
 double I = pid->Intergral(E);
 double D = pid->Derivative(E);
 double Outptut = pid->PIDcompute(P,I,D);
 if (Output > 60){      //clamp controller output
  Output = OutMax;
  };
  if (Output < -60){    //clamp controller output
  Output = OutMin;
  };
 Servo.write(90 + Output);
 LastError = E;
  
  }

 
  
  
