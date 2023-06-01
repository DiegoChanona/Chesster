#include <AccelStepper.h>
#include <Servo.h>
#include <math.h>

// switches used for homing robot 
#define limitSwitch1 12 // z limit connector Spindle enable
#define limitSwitch2 10 // y limit connector 
#define limitSwitch3 9 // x limit connector
#define limitSwitch4 A3 // coolEn connector
#define ElectroMagnet A0 // Electromagnet connector (Abort connector)


// Define the stepper motors and the pins the will use
AccelStepper stepper1(1, 2, 5); // (Type:driver, STEP, DIR) Conectado al eje X
AccelStepper stepper2(1, 3, 6); // Conectado al eje Y
AccelStepper stepper3(1, 4, 7); // Conectado al eje Z

//AccelStepper stepper4(AccelStepper::DRIVER_A4988, 12, 13); // conectado al spindle 

double xd = 250.0; //X desired in mm
double yd = 250.0; //Y desired in mm 
double zup = 200;  //Z for up position in mm
double zdn = 60;  //Z for down position in mm  

//L1 y L2 son las medidas de los brazos 
double L1 = 228; // L1 = 228mm
double L2 = 136.5; // L2 = 136.5mm
double theta1, theta2, z;  

int stepper1Position, stepper2Position, stepper3PositionUp,stepper3PositionDn;

bool STOP = false;
bool pick = false;

//Caracterización de los motores 

// const float theta1AngleToSteps = 44.444444;
const float theta1AngleToSteps = .01;
// const float theta2AngleToSteps = 35.555555;
const float theta2AngleToSteps = .5;
//const float phiAngleToSteps = 10;
const float zDistanceToSteps = 100;

// byte inputValue[5];
// int k = 0;

String content = "";

int data[3]; //bus en donde se encuentran todos los datos necesarios que la rasp le manda al arduino



void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  // We set the limit switches connections to Input pullup signals 
  pinMode(limitSwitch1, INPUT);
  pinMode(limitSwitch2, INPUT);
  pinMode(limitSwitch3, INPUT);
  pinMode(limitSwitch4, INPUT);
  pinMode(ElectroMagnet, OUTPUT);


  // Set the Velocity of the joints
  stepper1.setSpeed(100);
  stepper2.setSpeed(100);
  stepper3.setSpeed(100);

  //Set the acceleration of the joints
  stepper1.setAcceleration(100);
  stepper2.setAcceleration(100);
  stepper3.setAcceleration(100);

  // Stepper motors max speed for safety
  stepper1.setMaxSpeed(400);
  stepper1.setAcceleration(200);
  stepper2.setMaxSpeed(400);
  stepper2.setAcceleration(200);
  stepper3.setMaxSpeed(200);
  stepper3.setAcceleration(100);


  delay(1000);
  //data[2] = 100; // We set the Z position to 100 in the begining 
  homing(); 

}

double Theta1Ang(double x, double y){ 

  double t1 = atan(x/y)-atan(L2*sin(theta2))/(L1+L2*cos(theta2));

   //We adjust the angles depending on the cuadrant we want the robot to move to 
  if (x >= 0 & y >= 0) {       // 1st quadrant
    t1 = 90 - t1;
  }
  if (x < 0 & y > 0) {       // 2nd quadrant
    t1 = 90 - t1;
  }
  if (x < 0 & y < 0) {       // 3d quadrant
    t1 = 270 - t1;
  }
  if (x > 0 & y < 0) {       // 4th quadrant
    t1 = -90 - t1;
  }
  if (x < 0 & y == 0) {
    t1 = 270 + t1;
  }

  double deg = t1*180/3.1416; //convert from radians to degrees
  return deg;
}

double Theta2Ang(double x, double y){

  double t2 = acos(((x*x)+(y*y)-(L1*L1)-(L2*L2))/(2*L1*L2));
  if (x < 0 & y < 0) {
    t2 = (-1) * t2;
  }
  double deg = (t2)*180/3.1416; //convert from radians to degrees
  return deg;
}

void loop() {
  // put your main code here, to run repeatedly:

  if (Serial.available()){
    content = Serial.readString();  //Read the data that is sent from Raspberry 
    for (int i = 0;i<3; i++) {
      int index = content.indexOf(","); //Locate the first "," 
      data[i] = atol(content.substring(0,index).c_str()); //Extract the number of the first value of the string
      content = content.substring(index + 1); //Remove the number of the string 
    }
    /*

    data[0]=x position
    data[1]=y position
    data[2]=z position

    */
  }
  
  //Inverse Kinematics
  
  theta1 = Theta1Ang(100,50); //Joint 1 angle
  theta2 = Theta2Ang(100,50); //Joint 2 angle

  stepper1Position = theta1*theta1AngleToSteps; //Position of the stepper 1
  stepper2Position = theta2*theta2AngleToSteps; //Position of the stepper 2 
  stepper3PositionUp = zup*zDistanceToSteps; //Position of the stepper 3 up
  stepper3PositionDn = zdn*zDistanceToSteps; //Position of the stepper 3 Dn 

  Serial.println(stepper1Position);
  Serial.println("/////////");
  Serial.println(stepper2Position);

  //we Set the velocity for the robot 

  stepper1.moveTo(stepper1Position);
  stepper2.moveTo(stepper2Position);
  //stepper3.moveTo(stepper3PositionUp);

  // while (stepper3.currentPosition() !=stepper3PositionUp) {
  //   stepper3.run();
  // }
  while (stepper1.currentPosition() != stepper1Position) {
    // verifEnd();
    Serial.println("mov theta1");
    Serial.println(stepper1.currentPosition());
    Serial.println(stepper1Position);
    stepper1.run();    
  }
  while (stepper2.currentPosition() != stepper2Position){
    Serial.println("mov theta2");
    Serial.println(stepper2.currentPosition());
    Serial.println(stepper2Position);
    stepper2.run();
  }
  stepper3.moveTo(stepper3PositionDn);
  while (stepper3.currentPosition() !=stepper3PositionDn) {
    // verifEnd();
    //Serial.println("movZ Down");
    //stepper3.run();
  }

  // Pick and place the pieces depending on the previous state
  if (pick=true){
    digitalWrite(ElectroMagnet,LOW);
    pick=false;
  }

  else{
    digitalWrite(ElectroMagnet,HIGH);
    pick=true;
  }

  while (stepper3.currentPosition() !=stepper3PositionUp) {
    // verifEnd();

    Serial.println("Movz Up");
    stepper3.run();
  }

}


void verifEnd(){
  bool button1St = digitalRead(limitSwitch4);
  bool button2St = digitalRead(limitSwitch2);
  bool button3St = digitalRead(limitSwitch3);

  if (button1St ==true || button2St ==true || button3St ==true){
    STOP=true;
  }
  else {
    STOP = false;
  }
}


void homing() {
  // Homing Stepper3
  while (digitalRead(limitSwitch3) != 1) {
    stepper3.setSpeed(-1100);
    stepper3.runSpeed();
    stepper3.setCurrentPosition(-1662); // When limit switch pressed set position to -1662 steps
  }
  Serial.println(stepper3.currentPosition());
  delay(20);

  stepper3.moveTo(0);
  while (stepper3.currentPosition() != 0) {
    stepper3.run();
  }
  Serial.println(stepper3.currentPosition());

  // Homing Stepper2
  while (digitalRead(limitSwitch2) != 1) {
    stepper2.setSpeed(-1300);
    stepper2.runSpeed();
    stepper2.setCurrentPosition(-5420); // When limit switch pressed set position to -5440 steps
  }
  delay(20);

  stepper2.moveTo(0);
  while (stepper2.currentPosition() != 0) {
    stepper2.run();
  }

  // Homing Stepper1
  while (digitalRead(limitSwitch3) != 1) {
    stepper1.setSpeed(-1200);
    stepper1.runSpeed();
    stepper1.setCurrentPosition(-3955); // When limit switch pressed set position to 0 steps
  }
  delay(20);
  stepper1.moveTo(0);
  while (stepper1.currentPosition() != 0) {
    stepper1.run();
  }
}
