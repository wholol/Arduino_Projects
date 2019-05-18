int RowPins[3] = {11,12,13};      //  rows are also the tower base.
int columnPins[4] = {3,4,5,6};    //  initialize columnPns
int timer = 500;                  //timer


void setup() {
  for (int rowPin = 0; rowPin < 3; rowPin ++){            //initialize LED pins as output
    pinMode(RowPins[rowPin],OUTPUT);
  }

  for (int colPin = 0; colPin < 4 ; colPin ++){     //initialize LED pins as output
    pinMode(columnPins[colPin],OUTPUT);
    digitalWrite(columnPins[colPin],LOW);
  }
}

void turnLEDon(){                         //this function grounda all columns and makes the row anodes (testing purposes).
   for (int row = 0; row < 3; row++){
    digitalWrite(RowPins[row],HIGH);
  }

  for (int column = 0; column < 4; column++){   
    digitalWrite(columnPins[column],LOW);
  }
}

void loop() {
for (int row = 0; row < 3 ;row++){    //loops through eahc row.
    digitalWrite(RowPins[row], HIGH);
    delay(timer);
    digitalWrite(RowPins[row],LOW);
    if (timer == 50){
      timer = 500;
    }   
    }
 timer = timer - 50;
 
}
