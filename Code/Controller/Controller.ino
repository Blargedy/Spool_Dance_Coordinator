// Include the Arduino Stepper Library
#include <Stepper.h>

const int stepsPerRevolution = 200;   // stepper spec. either 400 or 200
const int homingSpeed = 20;           // In mm/s
const int guidlerTopSpeed = 350;      // In mm/s
const int spoolTopSpeed = 360;        // In mm/s
const int endstopPulloffSteps = 30;   // how many steps to move away from endstop trigger point
const boolean debug = true;

//internal widths between the walls of the spool, in mm.
const int spoolWidth1 = 25;  //small size
const int spoolWidth2 = 100; //medium size
const int spoolWidth3 = 150; //large size 

//physical restrictions
const int stepsToEndOfTravel = 2029;  //how many steps from the point of endstop trigger to hit the end wall

//==========PINS===========
const int endstopPin = 5;
const int homingButtonPin = 4;
const int toggleButton1Pin = 3;
const int toggleButton2Pin = 2;
const int potPin = A0;
const int rotaryEncoderButtonPin = A3;
const int rotaryEncoderSig1Pin = A1;
const int rotaryEncoderSig2Pin = A2;


// Create Instance of Stepper library
Stepper Guidler(stepsPerRevolution, 10, 11, 12, 13);
Stepper Spool(stepsPerRevolution, 6, 7, 8, 9);

//=====DO NOT EDIT BELOW THIS LINE=====


void setup()
{
  Serial.begin (9600);
  Guidler.setSpeed(guidlerTopSpeed);
  Spool.setSpeed(spoolTopSpeed);
  pinMode(endstopPin, INPUT_PULLUP);
  pinMode(homingButtonPin, INPUT_PULLUP);
  pinMode(toggleButton1Pin, INPUT_PULLUP);
  pinMode(toggleButton2Pin, INPUT_PULLUP);
  pinMode(potPin, INPUT);
  pinMode(rotaryEncoderButtonPin, INPUT_PULLUP);
  pinMode(rotaryEncoderSig1Pin, INPUT);
  pinMode(rotaryEncoderSig2Pin, INPUT);
  //Guidler.step(stepsPerRevolution * 3);
}

void loop()
{
  //Serial.print ("endstop status: ");
  //Serial.print (digitalRead(endstopPin));
  //Serial.print('\n');
  // step one revolution in one direction:
  //Serial.println("clockwise");
  //Guidler.step(stepsPerRevolution * 3);

  if(!digitalRead(homingButtonPin)){
    if(debug) Serial.print("homing\n");
    if(home()){
      if(debug) Serial.print("HOMING FAILED!\n");
    }
    else {
    if(debug) Serial.print("homing successful\n");
    }
  }

  if(!digitalRead(toggleButton1Pin)){
    top_speed_torture_test();
  }

  delay(500);
}

int home() {
  if(debug){
    Serial.print ("endstop status: ");
    Serial.print (digitalRead(endstopPin));
    Serial.print('\n');
  }

  if(digitalRead(endstopPin)){
    Guidler.step(-endstopPulloffSteps);
    if(digitalRead(endstopPin)){
      if(debug) Serial.print("ENDSTOP SWITCH STILL TRIGGERED AFTER PULLOFF\n");
      return 1;
    }
    else{
      if(debug) Serial.print("backoff successful\n");
    }
  }
  else{
    if(debug) Serial.print("already backed off\n");
  }

  if(debug) Serial.print("moving towards switch\n");

  Guidler.setSpeed(homingSpeed);
  while(!digitalRead(endstopPin)){
    Guidler.step(1);
  }
  Guidler.step(-endstopPulloffSteps*2);
  Guidler.setSpeed(homingSpeed/2);
  while(!digitalRead(endstopPin)){
    Guidler.step(1);
  }
  Guidler.setSpeed(homingSpeed);
  Guidler.step(-endstopPulloffSteps*2);

  return 0;
}

int countStepsToEnd(){
  int countedSteps = -1*endstopPulloffSteps*2;
  Guidler.setSpeed(homingSpeed/2);
  while(!digitalRead(toggleButton1Pin)){
    Guidler.step(-1);
    countedSteps--;
  }
  return countedSteps;
}

int test_speed(){
  int current_speed = homingSpeed;
  home();
  Guidler.setSpeed(current_speed);
  Guidler.step(-1*stepsToEndOfTravel/2);
  while(!digitalRead(toggleButton1Pin)){
    Serial.print("testing speed: ");
    Serial.println(current_speed);
    Guidler.setSpeed(current_speed);
    Guidler.step(150);
    Guidler.step(-150);
    current_speed++;
  }
  return current_speed;
}

void top_speed_torture_test(){
  home();
  Guidler.setSpeed(guidlerTopSpeed);
  Guidler.step(-1* stepsToEndOfTravel + endstopPulloffSteps*2);
  while(!digitalRead(toggleButton1Pin)){
    Guidler.step(stepsToEndOfTravel);
    Guidler.step(-1*stepsToEndOfTravel);
  }
}
