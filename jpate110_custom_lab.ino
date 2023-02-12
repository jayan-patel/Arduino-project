#include <Wire.h>
#include <SPI.h>

#define chipAddress 80
//Marios WAS able to confirm my NOKIA 5110 LCD screen was broken in OHs and my replacement did not come in time which hindered my ability to fully complete that complexity.

//Ultrasonic sensor
long duration = 0;
long cm = 0;

//Button inputs
bool button1 = false;
bool button2 = false;
bool tiltButton = false;

//EEPROM
unsigned int cellAddress = 101;

//Game logic
int highestScore;
int currentScore = 0;
int led;
bool button1Lock = false;
bool button2Lock = false;
bool tiltLock = false;
bool sonicLock = false;
bool gameOver = false;

/*
//LCD
int nums[10][5] {
  { 0x3E, 0x51, 0x49, 0x45, 0x3E }, // 0
  { 0x00, 0x42, 0x7F, 0x40, 0x00 }, // 1
  { 0x42, 0x61, 0x51, 0x49, 0x46 }, // 2
  { 0x21, 0x41, 0x45, 0x4B, 0x31 }, // 3
  { 0x18, 0x14, 0x12, 0x7F, 0x10 }, // 4
  { 0x27, 0x45, 0x45, 0x45, 0x39 }, // 5
  { 0x3C, 0x4A, 0x49, 0x49, 0x30 }, // 6
  { 0x01, 0x71, 0x09, 0x05, 0x03 }, // 7
  { 0x36, 0x49, 0x49, 0x49, 0x36 }, // 8
  { 0x06, 0x49, 0x49, 0x29, 0x1E }  // 9
};
*/

typedef struct task {
  int state;
  unsigned long period;
  unsigned long elapsedTime;
  int (*TickFct)(int);

} task;

int delay_gcd;
const unsigned short tasksNum = 5;
task tasks[tasksNum];


enum Button1_Inputs { INIT_1, input_1};

int Button1_Tick(int state1) {
  switch(state1) {
    case INIT_1:
      state1 = input_1;
      break;

    case input_1:
      state1 = input_1;
      break;
   }

  switch(state1) {
    case INIT_1:
      break;

    case input_1:
       if(button1Lock == false) {
        if (digitalRead(2) == 1) {
          button1 = false;
          //Serial.println("1");
        }
        else if (digitalRead(2) == 0){
          button1 = true;
          button1Lock = true;
          //Serial.println("0");
        }
       }
      break;
  }
  return state1;
}


enum Button2_Inputs { INIT_2, input_2};

int Button2_Tick(int state2) {
  switch(state2) {
    case INIT_2:
      state2 = input_2;
      break;

    case input_2:
      state2 = input_2;
      break;
   }


  switch(state2) {
    case INIT_2:
      break;

    case input_2:
      if (button2Lock == false) {
        if (digitalRead(3) == 1) {
          button2 = false;
          //Serial.println("1");
        }
        else if (digitalRead(3) == 0){
          button2 = true;
          button2Lock = true;
          //Serial.println("0");
        }
      }
      break;
  }
  return state2;
}


enum Tilt_Input { INIT_3, input_3};

int Tilt_Tick(int state3) {
  switch(state3) {
    case INIT_3:
      state3 = input_3;
      break;

    case input_3:
      state3 = input_3;
      break;
   }

  switch(state3) {
    case INIT_3:
      break;

    case input_3:
      if (tiltLock == false) {
        if (digitalRead(4) == 0) {
          tiltButton = false;
          //Serial.println("1");
        }
        else if (digitalRead(4) == 1){
          tiltButton = true;
          tiltLock = true;
          //Serial.println("0");
        }
      }
      break;
  }
  return state3;
}

enum ultrasonic_Input {INIT, input};

int ultrasonic_Tick(int state4) {
  switch(state4) {
    case INIT:
      state4 = input;
      break;

    case input:
      state4 = input;
      break;
   }

  switch(state4) {
    case INIT:
      break;

    case input:
      if (sonicLock == false) {
        //Trigers the sensor
        //digitalWrite(A0, LOW);
        //delayMicroseconds(5);
        digitalWrite(A0, HIGH);
        delayMicroseconds(10);
        digitalWrite(A0, LOW);
        
        //pinMode(A1, INPUT);
        duration = pulseIn(A1, HIGH); //Duration in microseconds from trigger to echo

        /*
        while (digitalRead(A1) == LOW) {}
        start = micros();
        while (digitalRead(A1) == HIGH) {}
        finish = micros();
        duration = finish - start;
        */

        cm = (duration/2) / 29.1;

        if(cm <= 5) {
          sonicLock = true;
        }
        
        //Serial.print(cm);
        //Serial.println("cm");
        
      }
      break;
  }
  return state4;
}

enum game_logic {game_INIT, gameSetup, logic, done};

int logic_Tick(int state5) {
  switch(state5) {
    case game_INIT:
      state5 = gameSetup;
      break;

    case gameSetup:
      state5 = logic;
      break;
    
    case logic:
    Serial.print("gameOver:");
    Serial.println(gameOver);
      if (gameOver == false) {
        state5 = logic;
      } 
      else if (gameOver == true) {
        state5 = done;
      }
      break;

    case done:
      state5 = gameSetup;
      break;
    
   }


  switch(state5) {
    case game_INIT:
      break;

    case gameSetup:
      led = random(0,4);
      
      //Serial.println("In Game Setup");
      //Serial.print("led: ");
      //Serial.println(led);
      //Serial.println();
      
      digitalWrite(led + 5, HIGH);
      /*
      for (int i = 5; i <= 8; i++) {
        if((led + 5) == i) {
          digitalWrite(led + 5, HIGH);
        } else {
          digitalWrite (i, LOW);
        }
      }
      */
      
      sonicLock = false;
      button1Lock = false;
      button2Lock = false;
      tiltLock = false;
      break;
    
    case logic:
      digitalWrite(5, LOW);
      digitalWrite(6, LOW);
      digitalWrite(7, LOW);
      digitalWrite(8, LOW);

      sonicLock = false;
      button1Lock = false;
      button2Lock = false;
      tiltLock = false;

      //Serial.print("In logic");
      //Serial.println();

      Serial.print("Led: ");
      Serial.println(led);
      Serial.println();

      Serial.print("cm: ");
      Serial.println(cm);

      Serial.print("button 1: ");
      Serial.println(button1);

      Serial.print("button 2: ");
      Serial.println(button2);

      Serial.print("tilt: ");
      Serial.println(tiltButton);

      if (led == 0 && cm <= 5 && button1 == false && button2 == false && tiltButton == false) {
        currentScore += 3;
      }
      else if (led == 1 && cm >= 5 && button1 == true && button2 == false && tiltButton == false) {
        currentScore += 1;
      }
      else if (led == 2 && cm >= 5 && button1 == false && button2 == true && tiltButton == false) {
        currentScore += 1;
      }
      else if (led == 3 && cm >= 5 && button1 == false && button2 == false && tiltButton == true) {
        currentScore += 2;
      }
      else {
        currentScore = 0;
        Serial.print("HIGHEST SCORE:");
        Serial.println(highestScore);
        Serial.println();

        gameOver = true;

        //TO DO: update screen to game over

        for (int i = 0; i < 504; i++){
          digitalWrite(A3, 1);
          SPI.transfer(0x00);
        }

        digitalWrite(A3, 0);
        SPI.transfer(0x40);
        SPI.transfer(0x80);
        
        digitalWrite(A3, 1);
        //g
        SPI.transfer(0x3E);
        SPI.transfer(0x41);
        SPI.transfer(0x49);
        SPI.transfer(0x49);
        SPI.transfer(0x7A);
        SPI.transfer(0x00);
        SPI.transfer(0x00);
        //a
        SPI.transfer(0x20);
        SPI.transfer(0x54);
        SPI.transfer(0x54);
        SPI.transfer(0x54);
        SPI.transfer(0x78);
        SPI.transfer(0x00);
        SPI.transfer(0x00);
        //m
        SPI.transfer(0x7C);
        SPI.transfer(0x04);
        SPI.transfer(0x18);
        SPI.transfer(0x04);
        SPI.transfer(0x78);
        SPI.transfer(0x00);
        SPI.transfer(0x00);
        //e
        SPI.transfer(0x38);
        SPI.transfer(0x54);
        SPI.transfer(0x54);
        SPI.transfer(0x54);
        SPI.transfer(0x18);
        SPI.transfer(0x00);
        SPI.transfer(0x00);
        //SPACE
        SPI.transfer(0x00);
        SPI.transfer(0x00);
        SPI.transfer(0x00);
        SPI.transfer(0x00);
        //o
        SPI.transfer(0x38);
        SPI.transfer(0x44);
        SPI.transfer(0x44);
        SPI.transfer(0x44);
        SPI.transfer(0x38);
        SPI.transfer(0x00);
        SPI.transfer(0x00);
        //v
        SPI.transfer(0x1C);
        SPI.transfer(0x20);
        SPI.transfer(0x40);
        SPI.transfer(0x20);
        SPI.transfer(0x1C);
        SPI.transfer(0x00);
        SPI.transfer(0x00);
        //e
        SPI.transfer(0x38);
        SPI.transfer(0x54);
        SPI.transfer(0x54);
        SPI.transfer(0x54);
        SPI.transfer(0x18);
        SPI.transfer(0x00);
        SPI.transfer(0x00);
        //r
        SPI.transfer(0x7C);
        SPI.transfer(0x08);
        SPI.transfer(0x04);
        SPI.transfer(0x04);
        SPI.transfer(0x08);
        SPI.transfer(0x00);
        SPI.transfer(0x00);
        //!
        SPI.transfer(0x00);
        SPI.transfer(0x00);
        SPI.transfer(0x5f);
        SPI.transfer(0x00);
        //SPI.transfer(0x00);
        //!
        SPI.transfer(0x00);
        SPI.transfer(0x00);
        SPI.transfer(0x5f);
        SPI.transfer(0x00);
        //SPI.transfer(0x00);
        //!
        SPI.transfer(0x00);
        SPI.transfer(0x00);
        SPI.transfer(0x5f);
        SPI.transfer(0x00);
        //SPI.transfer(0x00);


        break;
      }
      
      Serial.print("Current score:");
      Serial.println(currentScore);
      Serial.println();

      //TO DO: Write to screen here
      //H score: ##
      //C score: ##

      for (int i = 0; i < 504; i++){
        digitalWrite(A3, 1);
        SPI.transfer(0x00);
      }
    
      digitalWrite(A3, 0);
      SPI.transfer(0x40);
      SPI.transfer(0x80);
      
      digitalWrite(A3, 1);
      //H
      SPI.transfer(0x7F);
      SPI.transfer(0x08);
      SPI.transfer(0x08);
      SPI.transfer(0x08);
      SPI.transfer(0x7F);
      SPI.transfer(0x00);
      SPI.transfer(0x00);
      //i
      SPI.transfer(0x00);
      SPI.transfer(0x44);
      SPI.transfer(0x7D);
      SPI.transfer(0x40);
      SPI.transfer(0x00);
      SPI.transfer(0x00);
      SPI.transfer(0x00);
      //g
      SPI.transfer(0x0C);
      SPI.transfer(0x52);
      SPI.transfer(0x52);
      SPI.transfer(0x52);
      SPI.transfer(0x3E);
      SPI.transfer(0x00);
      SPI.transfer(0x00);
      //h
      SPI.transfer(0x7F);
      SPI.transfer(0x08);
      SPI.transfer(0x04);
      SPI.transfer(0x04);
      SPI.transfer(0x78);
      SPI.transfer(0x00);
      SPI.transfer(0x00);
      //SPACE
      SPI.transfer(0x00);
      SPI.transfer(0x00);
      SPI.transfer(0x00);
      SPI.transfer(0x00);
      //s
      SPI.transfer(0x48);
      SPI.transfer(0x54);
      SPI.transfer(0x54);
      SPI.transfer(0x54);
      SPI.transfer(0x20);
      SPI.transfer(0x00);
      SPI.transfer(0x00);
      //c
      SPI.transfer(0x38);
      SPI.transfer(0x44);
      SPI.transfer(0x44);
      SPI.transfer(0x44);
      SPI.transfer(0x20);
      SPI.transfer(0x00);
      SPI.transfer(0x00);
      //o
      SPI.transfer(0x38);
      SPI.transfer(0x44);
      SPI.transfer(0x44);
      SPI.transfer(0x44);
      SPI.transfer(0x38);
      SPI.transfer(0x00);
      SPI.transfer(0x00);
      //r
      SPI.transfer(0x7C);
      SPI.transfer(0x08);
      SPI.transfer(0x04);
      SPI.transfer(0x04);
      SPI.transfer(0x08);
      SPI.transfer(0x00);
      SPI.transfer(0x00);
      //e
      SPI.transfer(0x38);
      SPI.transfer(0x54);
      SPI.transfer(0x54);
      SPI.transfer(0x54);
      SPI.transfer(0x18);
      SPI.transfer(0x00);
      SPI.transfer(0x00);
      //:
      SPI.transfer(0x00);
      SPI.transfer(0x36);
      SPI.transfer(0x36);
      SPI.transfer(0x00);
      SPI.transfer(0x00);
      
      /*
      //number output
      for(int i = 100; i >= 1; i = i / 10) {
        for (int j = 1; j = 5; j++) {
          SPI.transfer( (nums [(137 / i) % 10]) [5] ) ;
        }
      }
      */

      
      /*
      //TO DO: CHANGE "C SCORE" TO SECOND ROW
      digitalWrite(A3, 0);
      SPI.transfer(0x54);
      SPI.transfer(0x80);
      
      digitalWrite(A3, 1);
      //C
      SPI.transfer(0x3E);
      SPI.transfer(0x41);
      SPI.transfer(0x41);
      SPI.transfer(0x41);
      SPI.transfer(0x22);
      SPI.transfer(0x00);
      SPI.transfer(0x00);

      //SPACE
      SPI.transfer(0x00);
      SPI.transfer(0x00);
      SPI.transfer(0x00);
      SPI.transfer(0x00);
      //s
      SPI.transfer(0x48);
      SPI.transfer(0x54);
      SPI.transfer(0x54);
      SPI.transfer(0x54);
      SPI.transfer(0x20);
      SPI.transfer(0x00);
      SPI.transfer(0x00);
      //c
      SPI.transfer(0x38);
      SPI.transfer(0x44);
      SPI.transfer(0x44);
      SPI.transfer(0x44);
      SPI.transfer(0x20);
      SPI.transfer(0x00);
      SPI.transfer(0x00);
      //o
      SPI.transfer(0x38);
      SPI.transfer(0x44);
      SPI.transfer(0x44);
      SPI.transfer(0x44);
      SPI.transfer(0x38);
      SPI.transfer(0x00);
      SPI.transfer(0x00);
      //r
      SPI.transfer(0x7C);
      SPI.transfer(0x08);
      SPI.transfer(0x04);
      SPI.transfer(0x04);
      SPI.transfer(0x08);
      SPI.transfer(0x00);
      SPI.transfer(0x00);
      //e
      SPI.transfer(0x38);
      SPI.transfer(0x54);
      SPI.transfer(0x54);
      SPI.transfer(0x54);
      SPI.transfer(0x18);
      SPI.transfer(0x00);
      SPI.transfer(0x00);
      //:
      SPI.transfer(0x00);
      SPI.transfer(0x36);
      SPI.transfer(0x36);
      SPI.transfer(0x00);
      SPI.transfer(0x00);
      */


      if (currentScore > highestScore) {
        highestScore = currentScore;
        writeScore(chipAddress, cellAddress, currentScore);
      }

      led = random(0,4);

      int cnt = 1000;
      while(cnt--) {
        digitalWrite(5, LOW);
        digitalWrite(6, LOW);
        digitalWrite(7, LOW);
        digitalWrite(8, LOW);
      }

      digitalWrite(led + 5, HIGH);      
      break;

    case done:
      gameOver = false;
      //Serial.println("In done");
      //Serial.println();
      break;
      
  }
  return state5;
}





void setup() {
  Serial.begin(9600);
  Wire.begin();
  SPI.begin();

      //DIGITAL PINS
  //Buttons and tilt ball
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  //LEDs
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  //NOKIA 5110
  pinMode(9, OUTPUT);
  //pin 11 not used
  pinMode(11, OUTPUT);
  //pin 12 not used
  pinMode(13, OUTPUT);

      //ANALOG PINS
  //Ultrasonic sensor
  pinMode(A0, OUTPUT);
  pinMode(A1, INPUT);
  //NOKIA 5110
  pinMode(A2, OUTPUT); //CE
  pinMode(A3, OUTPUT); //DC


  //SPI for LCD
  digitalWrite(A2, HIGH);
  delay(5);
  digitalWrite(9,0);
  delay(100);
  digitalWrite(9,1);

  digitalWrite(A2, LOW);
  digitalWrite(A3,0);
  SPI.transfer(0x21);
  delay(5);
  SPI.transfer(0x90);
  delay(5);
  SPI.transfer(0x20);
  delay(5);
  SPI.transfer(0x0C);
  delay(5);

  for (int i = 0; i < 504; i++){
    digitalWrite(A3, 1);
    SPI.transfer(0x00);
  }


  //Use to reinitialize space in memory to 0
  //writeScore(chipAddress, cellAddress, 0);
  highestScore = readScore(chipAddress, cellAddress);

  unsigned char i = 0;
  tasks[i].state = INIT_1;
  tasks[i].period = 100;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &Button1_Tick;
  i++;
  tasks[i].state = INIT_2;
  tasks[i].period = 100;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &Button2_Tick;
  i++;
  tasks[i].state = INIT_3;
  tasks[i].period = 100;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &Tilt_Tick;
  i++;
  tasks[i].state = INIT;
  tasks[i].period = 10;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &ultrasonic_Tick;
  i++;
  tasks[i].state = game_INIT;
  tasks[i].period = 3000;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &logic_Tick;
}




//Function to write highest score
void writeScore(int address, unsigned int cellAddress, byte writeData) {
  
  Wire.beginTransmission(address);
  Wire.write((int) (cellAddress >> 8)); //To get to actual data portion
  Wire.write((int) (cellAddress & 0xFF)); //To store only the data portion of transmitted information
  Wire.write(writeData);
  Wire.endTransmission();

  delay(5);
}

//Function to read highest score
byte readScore(int address, unsigned int cellAddress) {
  byte readData = 0;
  
  Wire.beginTransmission(address);
  Wire.write((int) (cellAddress >> 8)); //To get to actual data portion
  Wire.write((int) (cellAddress & 0xFF)); //To store only the data portion of transmitted information
  Wire.endTransmission();

  Wire.requestFrom(address, 1);

  if(Wire.available()) {
    readData = Wire.read();
  }

  return readData;
}


void loop() {
  unsigned char i;
  
  for (i = 0; i < tasksNum; ++i) {
    if ( (millis() - tasks[i].elapsedTime) >= tasks[i].period) {
      tasks[i].state = tasks[i].TickFct(tasks[i].state);
      tasks[i].elapsedTime = millis(); // Last time this task was ran
    }
  }

}
