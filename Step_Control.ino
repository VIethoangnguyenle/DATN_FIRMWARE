//# include <StandardTypes.h>
# include <TimerTwo.h>
# include "TimerOne.h"
# include "head.h"
# include "uart.h"
bool dir;
void step1();
void step2();
void servo();
void setup() {
  delay(100);
  //  attachInterrupt(1,home11,RISING );/
  setup_Serial();
  setup_Step_Pin();
  setup_Servo();
  setup_Timer2();
  digitalWrite(Z_DIR, 1);
  digitalWrite(X_DIR, 0);

  home11();
}

void home11() {
  if (state == 0) {
    step1Count = 150000;
    numberDelay1 = 5;
    dir = 1;
    while (digitalRead(int_Pin) != 0) {
      step1();
    }
    step1Count = 0;
    dir = 0;
    digitalWrite(Z_DIR, LOW);
  }
}

void loop() {
  home11();
  uartLoop();
  if (state == 1) {
    while (step1Count != 0 ) {
      
      step1();//
      dirServo = 1;
      servo();
      step2Count = revdis2;
      if (step1Count == 0) dir = !dir;
    }
    while (step2Count != 0) {
      step2();
      dirServo = 0;
      servo();
      step1Count = revdis1;
    }

  }



}

void callback() {
  calculateDelay();
}
void step1() {
  if (timer1Flag) {
    if (step1Count != 0) {
      timer1Flag = false;
      if (--step1Count == 0) Serial.println("1");
      digitalWrite (Z_STP, !digitalRead(Z_STP));
      digitalWrite(Z_DIR, dir);
    }
  }
}

void step2() {
  if (timer2Flag) {
    if (step2Count != 0) {
      timer2Flag = false;
      if (--step2Count == 0) Serial.println("2");
      digitalWrite (X_STP, !digitalRead(X_STP));

    }
  }
}

void servo() {
  if (timer3Flag) {
    if (dirServo == 1) {
      timer3Flag = false;
      myservo.write(69);
    }
    else if (dirServo == 0) {
      timer3Flag = false;
      myservo.write(72);
    }
  }
}
