#include <Servo.h>
#include <NewPing.h>

double TotalError;  // initialize intergral error to be zero.
double LastError;   //initialize error to be zero.
double OutMax = 60;   //clamping angle output
double OutMin = -60;  //clamping angle output
int sensorvalue = 0;  // sensorvalue counter for median array
#define servopin    4 // servopin on arduino.
#define TRIGGER_PIN  9  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     10  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 21 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

Servo servo1;  //instantiate class object
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); //construct NewPing object

void setup() {
servo1.attach(servopin);  //define servopin
 Serial.begin(115200);    //baud rate. CRUCIAL FOR SERIAL COMMUNICATION TO BE IN SYNC.
}

class PID_controller {

private:
double Kp;double Ki;double Kd; double setpoint; //private variables

public:
  PID_controller(double Kp,double Ki, double Kd,double setpoint){    //constructor to initialzie private variables
    this->Kp = Kp;          
    this->Ki = Ki;
    this->Kd = Kd;
    this->setpoint = setpoint;
  }

  double Error(double &Currentvalue) {    //error 
    return (Currentvalue - setpoint);}

  double Proportional(double &error) {    //proprotional controller
    return Kp *(error);}

  double Intergral(double &error) {       // integral member function
    return Ki*(TotalError += error);}

  double Derivative(double &error) {      //derivative member function
    return Kd * (error - LastError) ;}

  double PIDcompute(double P, double I, double D){    //output member function
    return ( P + I + D );}
    
};

int sort_desc(const void *cmp1, const void *cmp2)   // comparator function for median filter
{
  // Need to cast the void * to int *
  int a = *((int *)cmp1);
  int b = *((int *)cmp2);
  // The comparison
 return b - a;
}

void loop() {
 double distancearray[5];       //media filter array
 double distance = sonar.ping_cm(); //ultrasonic sensor reading
  while (sensorvalue < 5) {         

    distancearray[sensorvalue] = distance;    //store sensor values in array
    sensorvalue = sensorvalue + 1;            //add counter to sensorvalue

    if (sensorvalue == 4) {         // if all array elements are filled
      int n = sizeof(distancearray) / sizeof(distancearray[0]);   //array size
      qsort(distancearray, n, sizeof(distancearray[0]), sort_desc); //sort the array from the smallest value to the largest value
      break;
    }
  }
Serial.println(distancearray[2]);             
 PID_controller pid(3.2,0,0.3125,10);  //Construct and instantiate PID controller here. Kp = 3.2 ,Ki = 0, Kd = 0.3125, setpoint = 10
 double E = pid.Error(distancearray[2]); //provide median value to the error term
 double P = pid.Proportional(E);          //compute Proportional Controller
 double I = pid.Intergral(E);             // Compute intergral controller
 double D = pid.Derivative(E);            // compute derivative controller
 double Output = pid.PIDcompute(P,I,D);   // Compute Output
//Serial.println(distance);
//Serial.println(Output);



 if (Output > OutMax){      //clamp controller output
  Output = OutMax;          
  }
   if (Output < OutMin){    //clamp controller output
  Output = OutMin;
   }
   
Serial.println(Output);
servo1.write(80 + Output);
LastError = E;              // refresh the lasterror function after loop ends
delay(100);
sensorvalue = 0;            //reset senrovalue to zero.

}

 
  
  
