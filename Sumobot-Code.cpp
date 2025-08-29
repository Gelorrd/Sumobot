
// INITIALIZATION:
#include "eGizmo_PBOT2018.h"
#include<Wire.h>

eGizmo_PBOT2018 PBOT;

//US-100 PIN ASSIGNMENT
#define TRIGPIN 15
#define ECHOPIN 16
float DISTANCE;
float DURATION;

int FULL_SPEED = 500; // FULL SPEED TO BUMP THE OPPONENT
int NORMAL_SPEED = 150;  // NORMAL SPEED 
int MAX_DISTANCE_DETECTION = 20; // DISTANCE TO DETECT THE OPPONENT
int MIN_DISTANCE_DETECTION = 0;

int FULL_STOP = 0; // HALT
int LINER = 0;
boolean GO = false;
/**********************************************************************************/


void setup()
{
  Serial.begin(9600);
  Wire.begin();
  PBOT.BEGIN();
delay(5000);
  pinMode(TRIGPIN,OUTPUT);
  pinMode(ECHOPIN,INPUT);

}

int ping()
{
  digitalWrite(TRIGPIN,LOW);
  delayMicroseconds(500);
  digitalWrite(TRIGPIN,HIGH);
  delayMicroseconds(500);
  digitalWrite(TRIGPIN,LOW);

  DURATION= pulseIn(ECHOPIN,HIGH);
  DISTANCE= DURATION*0.034/2;
  return(DISTANCE);
}
/**********************************************************************************/
void loop()
{ 
  float VALUE;
  VALUE=ping(); // THE VALUE OF ULTRASONIC
  delay(100);

  // IF LINE IS DETECTED
  if((PBOT.LS3_RIGHT() == HIGH|| PBOT.LS2_CENTER() == HIGH) || PBOT.LS1_LEFT() == HIGH)
  {
    LINER-=1;
  }


  // IF THERE IS NO LINE DETECTED AND THERE IS NO ENEMY
  while(LINER == 1 && VALUE > MAX_DISTANCE_DETECTION) //IF LESS THAN 20CM,SEARCH MODE.
  {
    STOP();                   
    delay(200);
    LOOKING_FOR_OPPONENT();
    delay(200);
    break;
  }
  // IF THERE IS AN ENEMY AND THERE IS NO LINE DETECTED
  while((VALUE < MAX_DISTANCE_DETECTION && VALUE > MIN_DISTANCE_DETECTION) && LINER == 1) // IF GREATER THAN 20cm,ATTACK MODE!
  {
    ATTACK();                  
    delay(200);
    break;
  }
 //WHILE THERE'S AN ENEMY, ATTACK MODE AND IF LINE IS DETECTED IT WILL GO BACK.
  while((VALUE < MAX_DISTANCE_DETECTION && VALUE > MIN_DISTANCE_DETECTION) && LINER == 0) 
  {
    STOP();
    delay(500);
    GO_BACKWARD ();
    delay(500);
    SPIN_LEFT();
    break;
  }


  // TURN AROUND IF LINE IS DETECTED
  if(LINER == 0)
  {
    SPIN_LEFT();
    delay(500);
    GO_FORWARD_SLOW();
  }
  LINER = 1;

 PBOT.SERVO(1, 90); //SERVO TURNS 90 DEGREES AFTER THE CONDITION WAS FINISHED

}

/*---------------------------*/
// SERVO AND ULTRSONIC FUNCTIONS
void LOOKING_FOR_OPPONENT ()
{
  int OPPONENT_ON_LEFT;
  int OPPONENT_ON_CENTER;
  int OPPONENT_ON_RIGHT;

  PBOT.SERVO(1, 20);
  delay(200);

  OPPONENT_ON_RIGHT = ping();
  WHILE_SEARCHING_LINE_DETECTED();

  PBOT.SERVO(1, 160);
  delay(200);

  OPPONENT_ON_LEFT = ping();
  WHILE_SEARCHING_LINE_DETECTED();

   PBOT.SERVO(1, 90);
  delay(200);

  OPPONENT_ON_CENTER= ping();
  WHILE_SEARCHING_LINE_DETECTED();
  /*---------------------------*/
  if(OPPONENT_ON_RIGHT <= MAX_DISTANCE_DETECTION && LINER == 1 )
  {
    STOP();  
    GO = true;
    if(OPPONENT_ON_RIGHT <= MAX_DISTANCE_DETECTION && GO == true){
      SPIN_RIGHT();
      delay(200); 
      ATTACK();  
      delay(400);
      GO = false;
    }
  }
  /*---------------------------*/
  else if(OPPONENT_ON_LEFT <= MAX_DISTANCE_DETECTION && LINER == 1)
  { 
    SPIN_LEFT();
    delay(200);
    ATTACK();                                       
    delay(400);
  }
  /*---------------------------*/
  else if(OPPONENT_ON_LEFT = OPPONENT_ON_RIGHT)
  {
    GO_FORWARD_SLOW();
    delay(200);
  }
}
/*---------------------------*/
// MOTOR CONTROL FUNCTION
void ATTACK()
{
PBOT.DIRECTION(MOTOR_BOTH,MOTOR_FWD);
PBOT.SPEED(MOTOR_BOTH, FULL_SPEED);
}
/*---------------------------*/
void GO_FORWARD_SLOW()
{
PBOT.DIRECTION(MOTOR_BOTH,MOTOR_FWD);
PBOT.SPEED(MOTOR_BOTH, NORMAL_SPEED);
}
/*---------------------------*/
void GO_BACKWARD()
{
PBOT.DIRECTION(MOTOR_BOTH,MOTOR_REV);
PBOT.SPEED(MOTOR_BOTH, NORMAL_SPEED);
}
/*---------------------------*/
void STOP()
{
PBOT.DIRECTION(MOTOR_BOTH,MOTOR_REV);
PBOT.SPEED(MOTOR_BOTH, FULL_STOP); 
}
/*---------------------------*/
void SPIN_LEFT()
{
PBOT.DIRECTION(MOTOR_A,MOTOR_REV);
PBOT.DIRECTION(MOTOR_B,MOTOR_FWD);
PBOT.SPEED(MOTOR_BOTH, FULL_SPEED);
}
/*---------------------------*/
void SPIN_RIGHT()
{
PBOT.DIRECTION(MOTOR_A,MOTOR_FWD);
PBOT.DIRECTION(MOTOR_B,MOTOR_REV);
PBOT.SPEED(MOTOR_BOTH, FULL_SPEED);
}
//**********************************************************************************
void WHILE_SEARCHING_LINE_DETECTED(){
  if(PBOT.LS3_RIGHT() == HIGH  || PBOT.LS1_LEFT() == HIGH)
  {
    LINER-=1;
  }
  if(LINER == 0){
    GO_BACKWARD();
    delay(200);
    SPIN_LEFT();
    delay(200);
    GO_FORWARD_SLOW();
    STOP();
  }
  LINER = 1;

}
// PROGRAM ENDS AND LOOP BACK FOREVER  
//**********************************************************************************//PARZ
