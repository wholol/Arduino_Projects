int pin[7] = {A0,A1,A2,A8,A9,A10,A11}; // pin that reads voltage
int x;int y;int z;
 #define speaker 3                     // speaker output pin
int volts[10] = {analogRead (pin[x])};  //stores 10 readings of voltage for ONE pin
                //number of samples for the average mean vaule is 10
int sum;int sum1;int average;       // aVERAGE VARIABLES
int store [7];                    //stores AVERAGE reading of voltages of ALL pins

void setup() {
  
for (x = 0;x<=6; x++){
pinMode (pin[x], INPUT);
pinMode(speaker,OUTPUT);

Serial.begin (115200);}}                     

void loop() {
const int samples = 10;  
                     
    for ( x = 0; x <= 6 ; x++){   
      for ( y = 0;y<= 9 ; y++){               
       analogRead (pin [x]);                    //read voltage at each pin
        volts [y] = analogRead (pin [x]);          // store voltage of each pin in the array volts(up to 10 values as the array can only store 10 values)
        sum = volts[y];                    // sum of all the votlage (10 values)
        sum1 = sum += analogRead (pin [x]);  //add the previous analog reading to the current
        average = sum1/samples;
         store [x] = average; 
          Serial.println ("");
          Serial.println (store [5]);}}


const int EV =97;                     //value obtained through testing and observing how the voltage changes during serial.
   
  if ( (store [0]) > EV){   // if average voltage for the first pin value stored in the array (pin A0) is larger than 97
tone (speaker,195.9977);}   // tone(pin,frequency) to play

  
   else if ((store [1]) > EV ){
    tone (speaker,220.0000);}
    

     else if(store [2] > EV){                                                               
     tone(speaker,246.9417);}                                     
                                    
        
  else if (store [3] > EV ){                                            
     tone ( speaker,261.6256);}
     
      
       else if (store [4] > EV){
        tone (speaker,293.6648);}
         
            
         else if (store [5] > EV){
           tone(speaker,329.6276);}
             
                
                else if (store [6] > EV){
                  tone(speaker,523.2511);}

                  else {noTone(speaker);}} // prevents speaker from playing continuously.
