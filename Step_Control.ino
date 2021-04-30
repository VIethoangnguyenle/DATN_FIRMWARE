#include <Arduino_FreeRTOS.h>
# include <TimerTwo.h>
# include "TimerOne.h"
# include "head.h"
# include "uart.h"

boolean X_ACCEPT;
boolean Y_ACCEPT;
boolean X_ALLOW_RUN = false;
boolean Y_ALLOW_RUN = false;
int XDis, YDis, XVel, YVel;

TaskHandle_t Task_HandleStepX;
TaskHandle_t Task_HandleStepY;
TaskHandle_t Task_HandleReadUart;

void TaskStepX(void *param);
void TaskStepY(void *param);
void TaskReadUart(void *param);
void StepXRun(int dis, int vel);
void StepXGoHome();
void StepXGoToReadyPoint();
void StepYRun(int dis, int vel);
void StepYGoHome();
void StepYGoToReadyPoint();
void calculate_StepX_Velocity();
void calculate_StepY_Velocity();
bool ALLOW_TO_RUN();

void setup() {
  Serial.begin(9600);
  //Set up OUTPUT stepMotor X
  pinMode(X_PULSE, OUTPUT);
  pinMode(X_DIR, OUTPUT);
  pinMode(X_ENABLE, OUTPUT);

  //Set up OUTPUT stepMotor X
  pinMode(Y_PULSE, OUTPUT);
  pinMode(Y_DIR, OUTPUT);
  pinMode(Y_ENABLE, OUTPUT);

  //Set up INPUT
  pinMode(X_LIMIT, INPUT);
  pinMode(Y_LIMIT, INPUT);

  //Set up 3 task run simultaneously
  xTaskCreate(TestStepX,"Control Step X", 100, NULL, 1, &Task_HandleStepX);
  xTaskCreate(TestStepY,"Control Step Y", 100, NULL, 1, &Task_HandleStepY);
  // xTaskCreate(TaskReadUart,"Read Uart"  , 100, NULL, 1, &Task_HandleReadUart);

}

void loop() {

}

void TaskStepX(void *param){
  (void) param;
  if (HOME_FLAG) StepXGoHome();
  else
  {
    if (ALLOW_TO_RUN())
    {
      StepXGoToReadyPoint();
      while (1)
        StepXRun(XDis,XVel);
    }
  }
}

void TaskStepY(void *param){
  (void) param;
  if(HOME_FLAG) StepYGoHome();
  else
  {
    StepYGoToReadyPoint();
    while(ALLOW_TO_RUN())
      StepYRun(YDis,YVel);
  }
}

void TaskReadUart(void *param){
  (void) param;
  while (1)
    uartLoop();
}

void StepXGoHome(){
   while(X_LIMIT == false)
   {
       digitalWrite(X_DIR, HIGH);
       digitalWrite(X_ENABLE, HIGH);
       digitalWrite(X_PULSE, HIGH);
       delayMicroseconds(50);
       digitalWrite(X_PULSE, LOW);
       delayMicroseconds(50);
   } 
 X_ALLOW_RUN = true;
}

void StepYGoHome(){
 while(Y_LIMIT == false)
 {
     digitalWrite(X_DIR, HIGH);
     digitalWrite(X_ENABLE, HIGH);
     digitalWrite(X_PULSE, HIGH);
     delayMicroseconds(50);
     digitalWrite(X_PULSE, LOW);
     delayMicroseconds(50);
 } 
 Y_ALLOW_RUN = true;
}

void StepXRun(int dis, int vel){
 for (size_t i = 0; i < dis; i++)
   {
     digitalWrite(X_DIR, HIGH);
     digitalWrite(X_ENABLE, HIGH);
     digitalWrite(X_PULSE, HIGH);
     delayMicroseconds(vel);
     digitalWrite(X_PULSE, LOW);
     delayMicroseconds(vel);
   }
}

void StepYRun(int dis, int vel){
 for (size_t i = 0; i < dis; i++)
   {
     digitalWrite(X_DIR, HIGH);
     digitalWrite(X_ENABLE, HIGH);
     digitalWrite(X_PULSE, HIGH);
     delayMicroseconds(vel);
     digitalWrite(X_PULSE, LOW);
     delayMicroseconds(vel);
   }
}

void StepXGoToReadyPoint(){
 for(int i=0;i<200;i++)
 {
   digitalWrite(X_DIR,HIGH);
   digitalWrite(X_ENABLE,HIGH);
   digitalWrite(X_PULSE,HIGH);
   delayMicroseconds(50);
   digitalWrite(X_PULSE,LOW);
   delayMicroseconds(50);
   Serial.println(i);
 }
}

void StepYGoToReadyPoint(){
 for(int i=0;i<200;i++)
 {
   digitalWrite(Y_DIR,HIGH);
   digitalWrite(Y_ENABLE,HIGH);
   digitalWrite(Y_PULSE,HIGH);
   delayMicroseconds(50);
   digitalWrite(Y_PULSE,LOW);
   delayMicroseconds(50);
   Serial.println(i);
 }
}

bool ALLOW_TO_RUN(){
 if (X_ALLOW_RUN == true && Y_ALLOW_RUN == true)
   return true;
 else 
   return false;
}

void calculate_StepX_Velocity(){

}

void calculate_StepY_Velocity(){

}

void TestStepY(void *param){
  (void) param;
  for(int i=0;i<64000;i++)
  {
    digitalWrite(Y_DIR,HIGH);
    digitalWrite(Y_ENABLE,HIGH);
    digitalWrite(Y_PULSE,HIGH);
    delayMicroseconds(100);
    digitalWrite(Y_PULSE,LOW);
    delayMicroseconds(100);
    Serial.println(i);
  }
}
void TestStepX(void *param){
  (void) param;
  while (1)
  {
    for(int i=0;i<32000;i++)
    {
      digitalWrite(X_DIR,HIGH);
      digitalWrite(X_ENABLE,HIGH);
      digitalWrite(X_PULSE,HIGH);
      delayMicroseconds(50);
      digitalWrite(X_PULSE,LOW);
      delayMicroseconds(50);
      Serial.println(i);
    }
  }
}