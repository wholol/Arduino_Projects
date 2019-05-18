int RowPins[3] = {11,12,13};      //rows are also the tower base.
int columnPins[4] = {6,3,4,5};        //CATHODE PINS (the pin numbers depends on the conenction of the wire).
int timer = 100;
int row = 0;        //initialzie row
int RoundCount = 1;       //tracks the round


void setup() {
  for (int rowPin = 0; rowPin < 3; rowPin ++){
    pinMode(RowPins[rowPin],OUTPUT);
    //digitalWrite(RowPins[rowPin],HIGH);             //since the rws are anodes, it has to be set to HIGH (pushes voltage)
  }

  for (int colPin = 0; colPin < 4 ; colPin ++){
    pinMode(columnPins[colPin],OUTPUT);
   // digitalWrite(columnPins[colPin],LOW);
  }

}

void turnLEDon(){                         //this function grounda all columns and makes the row anodes.
   for (int row = 0; row < 3; row++){
    digitalWrite(RowPins[row],HIGH);
  }

  for (int column = 0; column < 4; column++){
    digitalWrite(columnPins[column],LOW);
  }
}

void DownToUp(){
   for (int col = 0 ; col <= 3 ; col ++){       
    digitalWrite(columnPins[col], LOW);                               //set cathode to LOW (receive voltage)
    digitalWrite(RowPins[row], HIGH);                                 //set row andoe to HIGH ( give voltage)
    delay(timer);
    digitalWrite(columnPins[col], HIGH);
    digitalWrite(RowPins[row], LOW);}
    row = row + 1;
}

void UpToDown(){   
    for (int col = 3 ; col >= 0 ; col --){           
    digitalWrite(columnPins[col], LOW);                                                //set cathode to LOW (receive voltage)
    digitalWrite(RowPins[row], HIGH);                                                 //set row andoe to HIGH ( give votlage)
    delay(timer);
    digitalWrite(columnPins[col], HIGH);                                              //set cathod to HIGH (give voltage, however pin polarity blocks it )
     digitalWrite(RowPins[row], LOW);}
     row = row + 1;
}

  

void loop(){
 
if (row > 2){           // 1 round is completed
    row = 0;        //reset row
    RoundCount += 1;
}


DownToUp();
UpToDown();






}

