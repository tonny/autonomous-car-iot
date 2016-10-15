/********************************************************************************************
 * Project name : Teslita Autonomous
 * Description : Autonoums car, that could be controlled by a bluetooth
 * Authors: Antonio Mamani - antonio.mq@gmail.com
 ********************************************************************************************/
#include <Servo.h>

//---------- servo motores dc ---------------------------------------
//int llantasAdelante= 2,3,
//int llantaAtras = 4,5
//Pines de conexión del driver
int pinMotorFrontA = 5;
int pinMotorFrontB = 4;

int Pin_Motor_Izq_A = 10;
int Pin_Motor_Izq_B = 11;



//bluethood  0 , 1, TX, RX

//------------------------- sensor ultrasonic with servo -------------------------
Servo servoAhead;
int pinServoAhead = 6;

int sensorUltrasonicAheadEcho = 9;
int sensorUltrasonicAheadTrig = 8;

int sensorUltrasonicBackEcho = 11;
int sensorUltrasonicBackTrig = 12;

//------------------------- global variables -------------------------------------
long distanceFront;
long timeToWait;
int distancePermited = 30;
int delayTurnServo = 25;
bool areThereSpaceLeft = false;
bool areThereSpaceRight = false;

//****************************************************************************************************/
//******************************** All about scan free space *****************************************/
//****************************************************************************************************/
void turnRightServoAhead(){
  for(int angle = 0; angle < 90; angle += 1) { // Bucle to move servo motor between 0 to 90 degrees
      servoAhead.write(angle);
      calculateDistance();
      if(areThereSpace()){
        //@TODO think how to return to center 
        areThereSpaceRight = true;
        servoAhead.write(45); 
        break; 
      }
      delay(delayTurnServo); // waiting some miliseconds
  }
}

void turnLeftSevoAhead(){
  for(int angle = 90; angle >= 1; angle -= 1) { // Bucle to move sero motor between 90 to 0 degrees
     servoAhead.write(angle); 
     calculateDistance();
     if(areThereSpace()){
        //@TODO think how to return to center 
        areThereSpaceLeft = true;
        servoAhead.write(45);
        break; 
      }
     delay(delayTurnServo); // waiting some miliseconds
  }
}

void scanFreeSpace(){
  turnLeftSevoAhead();
  if(!areThereSpaceLeft){
   turnRightServoAhead(); 
  }
}

void calculateDistance(){
  digitalWrite(sensorUltrasonicAheadTrig,LOW); /* stabilize sensor*/
  delayMicroseconds(5);
  digitalWrite(sensorUltrasonicAheadTrig, HIGH); /* send pulse ultrasonic*/
  delayMicroseconds(10);
  timeToWait=pulseIn(sensorUltrasonicAheadEcho, HIGH); /* Function to measure the longitude from pulse  acquired.
  Measures the time elapsed between sending the ultrasonic pulse and when the sensor receives the bounce.*/
  distanceFront= int(0.017*timeToWait); /*formula to calculate the distance getting to an integer value*/
  /*Monitorización en centímetros por el monitor serial*/
  Serial.println("Distancia ");
  Serial.print(distanceFront);
  Serial.println(" cm");
  delay(500);  
}

bool areThereSpace(){
  return distanceFront > distancePermited;
}


void controlFrontObstacles(){
  calculateDistance();
  if(distanceFront > distancePermited){
    calculateDistance();
  }else{
    //Stop car and scan free space
    scanFreeSpace();  
  }  
}

void setupUltrasonics(){

  servoAhead.attach(pinServoAhead);
  
  pinMode(sensorUltrasonicAheadTrig,OUTPUT); // pulso ultrasonic
  pinMode(sensorUltrasonicAheadEcho,INPUT); // time to bounce from ultrasonic
}

//****************************************************************************************************/
//********************************** Run Teslita *******************************/
void setup() {
  // setup code
  //Serial.begin(9600);
  
  setupUltrasonics();
 
  //pinMode(pinMotorFrontA,OUTPUT);
  //pinMode(pinMotorFrontB,OUTPUT);
  
}

void loop() {
  
  controlFrontObstacles();
  
}

