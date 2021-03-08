# include <Servo.h>

Servo myservo;
void callback();


# define EN 8     // stepper motor enable , active low
# define X_DIR 5  // X - axis stepper motor direction control
# define Y_DIR 6  // y - axis stepper motor direction control
# define Z_DIR 7  // z - axis stepper motor direction control
# define X_STP 2  // x - axis stepper control
# define int_Pin 3// y - axis stepper control
# define Z_STP 4  // z - axis stepper control
# define inter 50 // Cứ 50 us timer ngắt một lần
# define servoPin 12
float angleServo = 90.0;
int numberDelay1  = 0;  // Số lần cần ngắt cho một delay
int numberDelay2  = 0;  // Số lần cần ngắt cho một delay
int numberDelay3  = 0;  // Số lần cần ngắt cho một delay
int timesDelay1   = 0;  // Biến đếm số lần ngắt
int timesDelay2   = 0;  // Biến đếm số lần ngắt
int timesDelay3   = 0;  // Biến đếm số lần ngắt
boolean timer1Flag  = false;  // Khi đủ thời gian delay thì cờ bằng true
boolean timer2Flag  = false;  // Khi đủ thời gian delay thì cờ bằng true
boolean timer3Flag  = false;  // Khi đủ thời gian delay thì cờ bằng true

long int step1Count = 0;
long int step2Count = 0;
long int step3Count = 0;
 


void setup_Timer2(){
  Timer2.init(inter, callback);
  Timer2.start();
}

void setup_Serial(){
  Serial.begin(2000000);
}

void setup_Step_Pin(){
  pinMode (X_DIR, OUTPUT); pinMode (X_STP, OUTPUT);
  pinMode (Y_DIR, OUTPUT); pinMode (int_Pin, INPUT);
  pinMode (Z_DIR, OUTPUT); pinMode (Z_STP, OUTPUT);
  pinMode (EN, OUTPUT);
//  pinMode (cbht,INPUT);
  digitalWrite (EN, LOW);
}

void home11();

void setup_Servo(){
  myservo.attach(servoPin);
}



// Tính toán thời gian delay cho mỗi động cơ
void calculateDelay(){
  // Số lần ngắt bằng số lần cần ngắt của delay1
  if(++timesDelay1 == numberDelay1){
    timesDelay1 = 0;
    timer1Flag = true;
  }
  if(++timesDelay2 == numberDelay2){
    timesDelay2 = 0;
    timer2Flag = true;
  }
  if(++timesDelay3 == numberDelay3){
    timesDelay3 = 0;
    timer3Flag = true;
  }
}
