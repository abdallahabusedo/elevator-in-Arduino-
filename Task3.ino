#include<Keypad.h>
#include<Stepper.h>
// vars 
#define  MotorA 11
#define  MotorB 12
#define VoltageSource  A1 
const int stepsPerRevolution = 40;
int CurrentFloor = 0 ; 
int inputs[3] = {0,1,2}; // A,B,C 
int UPorDown = 1  ; 
int MovingWay=100 ; 
byte BCD[16][4] ={{0,0,0,0},{1,0,0,0},{0,1,0,0},{1,1,0,0},{0,0,1,0},{1,0,1,0},{0,1,1,0},{1,1,1,0},{0,0,0,1}}; //BCD code
byte keypad[8]={5,161,143,126,84,69,54,18};
byte val=0;
bool FloorNumBtn[8]={0,0,0,0,0,0,0,0};
float Voltage = 0 ;
const float reference = 5.0 ; 
Stepper myStepper(stepsPerRevolution, MotorA,MotorB);

//setup function 
void setup(){
  myStepper.setSpeed(50);
  for(int a = 0; a < 3; a++){
  pinMode(inputs[a], OUTPUT);}
  for(int i = 3 ; i<11;i++)
  {
    pinMode(i,INPUT);
  };
  pinMode(13,OUTPUT);
}
/////////////////////////////////
void MoveMotor(int UPorDown){
  if(UPorDown==-1)
  {
  myStepper.step(-stepsPerRevolution);   
  }
else {
  myStepper.step(stepsPerRevolution);   
     }
}

void Reading (){
        for (int i = 0 ; i < 8 ; i++){
          int reading = digitalRead(i+3);
          if(reading==1)
            FloorNumBtn[i]=1;
          }
           val = analogRead(A0);
           for(byte i = 0 ; i < 8; i++)
          {
            if(val == keypad[i])
            {
             FloorNumBtn[i]=1;  
            }
          }
}

//////////////////////////////////
void loop(){
  MovingWay=100;
  for (byte i = 0  ; i< 8 ; i++)
  {
    if(FloorNumBtn[i]==true)
    {
      if(abs(CurrentFloor-i)<MovingWay&&CurrentFloor-i!=0)
         MovingWay=abs(CurrentFloor-i);
         if(CurrentFloor-i>0)
           UPorDown=-1 ; // down
         else 
           UPorDown=1 ; // up 
     }
   }
    if(MovingWay==0)
      MovingWay=100; 
    if(MovingWay<8){
      for(byte i = 0 ; i<MovingWay;i++){
        MoveMotor(UPorDown);
        Reading();
        if(UPorDown==-1)
          CurrentFloor-=1;
        else 
          CurrentFloor+=1; 
        for (int i = 0 ; i<3 ;i++){
       digitalWrite(inputs[i], BCD[CurrentFloor][i]);
        }
        if(FloorNumBtn[CurrentFloor]==1)
          {
            delay(1000);
           FloorNumBtn[CurrentFloor]=0;
          }
      }
       FloorNumBtn[CurrentFloor]=0; 
       delay(1000);
    }
  Reading();
  for (int i = 0 ; i<3 ;i++){
       digitalWrite(inputs[i], BCD[CurrentFloor][i]);
    }
  
  int value = analogRead(VoltageSource);
  Voltage= (value/1023.0)*reference ; 
  if(Voltage>=3){
    digitalWrite(13,HIGH);
    while(Voltage>=3){
      value = analogRead(VoltageSource); 
      Voltage= (value/1023.0)*reference ;  
      Reading();
     }
     digitalWrite(13,LOW);
    }  
}
