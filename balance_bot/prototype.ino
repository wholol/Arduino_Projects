#include <Wire.h>
#include <SoftwareSerial.h>


// MPU 6050 variable setup//
double accelX, accelY, accelZ,accAngleX, accAngleY;
double now, lastTime, SampleTime;
double roll,pitch,yaw;
double gyroX, gyroY, gyroZ,gyroAngleX,gyroAngleY;
double accErrorX, accErrorY, GyroErrorX, GyroErrorY, GyroErrorZ;
int counter = 0;

//DC motor variable setup
const int IN1 = 7;
const int IN2 = 6;
const int IN3 = 5;
const int IN4 = 4;
const int ENB = 3;
const int ENA = 9;

double LastError = 0, TotalError = 0;   //PID variables

void setup() {
  Serial.begin(9600);
  pinMode(ENB,OUTPUT);
  pinMode(ENA,OUTPUT);
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
  pinMode(IN3,OUTPUT);
  pinMode(IN4,OUTPUT);
  Wire.begin();
  Wire.beginTransmission(0x68); //initialize MPU6050 I2C
  Wire.write(0x6B);             //register 6B
  Wire.write(0x00);             //reset register
  Wire.endTransmission();       //end the transmission
  compute_errors();             //computes gyro errors to compensate for angles (zero error).
  delay(20);
  
  SoftwareSerial Overflow_Check(A4,A5);  //checks for overflows for MPU 6050.
  Overflow_Check.begin(9600);
   if (Overflow_Check.overflow()) {
     Serial.println("SoftwareSerial overflow!"); }
    else{
      Serial.println("no ovf"); 
      }
}

void recordAccelData() {
  Wire.beginTransmission(0x68); //I2C address of the MPU
  Wire.write(0x3B); //Starting register for Accel Readings
  Wire.endTransmission();
  Wire.requestFrom(0x68,6,true); //Request Accel Registers (3B - 40) for all x,y, and  axis.
  while(Wire.available() < 6);
  //dividing 16384 for a sensitivity of 2g.//
  accelX = (Wire.read()<<8|Wire.read()) /16384.0 ; //accel in X value.
  accelY = (Wire.read()<<8|Wire.read()) / 16384.0; //accel in Y value.
  accelZ = (Wire.read()<<8|Wire.read()) / 16384.0 ; //accel in Z value.
  accAngleX = (atan(accelY / sqrt(pow(accelX,2) + pow( accelZ,2)))*180/PI) - 0.45;    //compute accelerometer in roll
  accAngleY = (atan(-1* accelX / sqrt(pow(accelY,2) + pow( accelZ,2)))*180/PI) - 2.17;  //compute accelerometer in yaw
}

void recordGyroData() {
  
  Wire.beginTransmission(0b1101000); //I2C address of the MPU
  Wire.write(0x43); //Starting register for Gyro Readings
  Wire.endTransmission();
  Wire.requestFrom(0b1101000,6); //Request Gyro Registers (43 - 48)
  while(Wire.available() < 6);
  //divide by 131 with a sensitity of 250 deg/s
  gyroX = (Wire.read()<<8|Wire.read()) / 131.0; //Store first two bytes into accelX
  gyroY = (Wire.read()<<8|Wire.read()) / 131.0; //Store middle two bytes into accelY
  gyroZ = (Wire.read()<<8|Wire.read()) / 131.0; //Store last two bytes into accelZ
  gyroX = gyroX - 2.37;   //correct the deg/s values by performing error calcs
  gyroY = gyroY + 0.70;      //correct the deg/s values by performing error calcs
  gyroZ = gyroZ + 0.32;   //correct the deg/s values by performing error calcs

}

void compute_errors(){
  while (counter < 200){
     Wire.beginTransmission(0x68);
    Wire.write(0x3B);
    Wire.endTransmission(false);
    Wire.requestFrom(0x68, 6, true);
    accelX = (Wire.read() << 8 | Wire.read()) / 16384.0 ;
    accelY = (Wire.read() << 8 | Wire.read()) / 16384.0 ;
    accelZ = (Wire.read() << 8 | Wire.read()) / 16384.0 ;
    // Sum all readings
    accErrorX = accErrorX + ((atan((accelY) / sqrt(pow((accelX), 2) + pow((accelZ), 2))) * 180 / PI));
    accErrorY = accErrorY + ((atan(-1 * (accelX) / sqrt(pow((accelY), 2) + pow((accelZ), 2))) * 180 / PI));
    counter++;
  }
  //Divide the sum by 200 to get the error value
  accErrorX = accErrorX / 200;
  accErrorY = accErrorY / 200;
  counter = 0;
  // Read gyro values 400 times
  while (counter < 200) {
    Wire.beginTransmission(0x68);
    Wire.write(0x43);
    Wire.endTransmission(false);
    Wire.requestFrom(0x68, 6, true);
    gyroX = Wire.read() << 8 | Wire.read();
    gyroY = Wire.read() << 8 | Wire.read();
    gyroZ = Wire.read() << 8 | Wire.read();
    // Sum all readings
    GyroErrorX = GyroErrorX + (gyroX / 131.0);
    GyroErrorY = GyroErrorY + (gyroY / 131.0);
    GyroErrorZ = GyroErrorZ + (gyroZ / 131.0);
    counter++;
  }
  //Divide the sum by 200 to get the error value
  GyroErrorX = GyroErrorX / 200;
  GyroErrorY = GyroErrorY / 200;
  GyroErrorZ = GyroErrorZ / 200;
  // Print the error values on the Serial Monitor
  Serial.print("AccErrorX: ");
  Serial.println(accErrorX);
  Serial.print("AccErrorY: ");
  Serial.println(accErrorY);
  Serial.print("GyroErrorX: ");
  Serial.println(GyroErrorX);
  Serial.print("GyroErrorY: ");
  Serial.println(GyroErrorY);
  Serial.print("GyroErrorZ: ");
  Serial.println(GyroErrorZ);
}

class PID_controller{
  private:
  double Kp; double Ki; double Kd; double setpoint; double PID_SampleTime;
  public:

  PID_controller(double Kp, double Ki,double Kd,double setpoint){
    this->Kp = Kp;
    this->Ki = Ki;
    this->Kd = Kd;
    this->setpoint;
  }

  double Error(double &currentVal){
    return (currentVal - setpoint);
  }

  double Proportional(double &error){
    return (Kp*error);
  }

  double Intergral(double &error){
    return Ki*(TotalError += error);
  }

  double Derivative(double &error){
    return Kd*(error - LastError);
  }

  double Output(double P, double I, double D){
    return (P + I + D);
  }
  
};
void loop() {
  recordAccelData();
  lastTime = now;
  now = millis();
  SampleTime = (now - lastTime)/1000;
  recordGyroData();
  gyroAngleX = gyroAngleX + gyroX*SampleTime;
  gyroAngleY = gyroAngleY + gyroY*SampleTime;
  roll = 0.96 * gyroAngleX + 0.04 * accAngleX;
  pitch = 0.96 * gyroAngleY + 0.04 * accAngleY;
  /*Serial.print(roll);
  Serial.print("///");
  Serial.print(pitch);
  Serial.println(" ");*/
  PID_controller pid(50,0,0,0.00);
  double error = pid.Error(pitch);
  double P = pid.Proportional(error);
  double I = pid.Intergral(error);
  double D = pid.Derivative(error);
  double Output = pid.Output(P,I,D);
  Serial.println(Output);
}
