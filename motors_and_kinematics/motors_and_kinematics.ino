#include <AccelStepper.h>
#include <Servo.h>
#include <math.h>

// switches used for homing robot 
#define limitSwitch1  9 // x limit connector
#define limitSwitch2 10 // y limit connector
#define limitSwitch3 11 // z limit connector 
#define limitSwitch4 A3 // coolEn connector
#define ElectroMagnet A0 // Electromagnet connector (Abort connector)


// Define the stepper motors and the pins the will use
AccelStepper stepper1(1, 2, 5); // (Type:driver, STEP, DIR) Conectado al eje X
AccelStepper stepper2(1, 3, 6); // Conectado al eje Y
AccelStepper stepper3(1, 4, 7); // Conectado al eje Z

//AccelStepper stepper4(AccelStepper::DRIVER_A4988, 12, 13); // conectado al spindle 

//Distances for picking up and down the pieces
double zup = 0;  //Z for up position in mm
double zdn = 0;  //Z for down position in mm  

//L1 y L2 son las medidas de los brazos 
double L1 = 228; // L1 = 228mm
double L2 = 136.5; // L2 = 136.5mm
double theta1, theta2, z;  

int stepper1Position, stepper2Position,stepper3Position, stepper3PositionUp,stepper3PositionDn;

bool STOP = false;
int pick,finish; //variable for pick and place 

//Caracterización de los motores 

const float theta1AngleToSteps = 11.1111;
const float theta2AngleToSteps = 8.8888;
const float zDistanceToSteps = 15.714; //steps per mm

// byte inputValue[5];
// int k = 0;

String content = "";

int data[3]; //bus en donde se encuentran todos los datos necesarios que la rasp le manda al arduino



void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  // We set the limit switches connections to Input pullup signals 
  pinMode(limitSwitch1, INPUT_PULLUP);
  pinMode(limitSwitch2, INPUT_PULLUP);
  pinMode(limitSwitch3, INPUT_PULLUP);
  pinMode(ElectroMagnet, OUTPUT);


  // Set the Velocity of the joints for Homming
  stepper1.setSpeed(600);
  stepper2.setSpeed(600);
  stepper3.setSpeed(600);

  //Set the acceleration of the joints
  stepper1.setAcceleration(400);
  stepper2.setAcceleration(400);
  stepper3.setAcceleration(400);

  // Stepper motors max speed for safety
  stepper1.setMaxSpeed(800);
  stepper1.setAcceleration(600);
  stepper2.setMaxSpeed(800);
  stepper2.setAcceleration(600);
  stepper3.setMaxSpeed(800);
  stepper3.setAcceleration(600);


  delay(1000);
  homing(); 

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
    data[2]= Piece tipe (P=1,R=2,N=3,B=4,Q=5,K=6)
    

    */
  }
  
  //Inverse Kinematics

  inverseKinematics(data[0],data[1]);

  //Check the piece to pick up

  pieceTipe(data[2]);
    

  //Set the steps to move for each motor 
  stepper1Position =theta1*theta1AngleToSteps; //Position of theta1
  stepper2Position =theta2*theta2AngleToSteps; //Position of theta2 
  stepper3PositionUp = zup*zDistanceToSteps; //Position of the stepper 3 up
  stepper3PositionDn = zdn*zDistanceToSteps; //Position of the stepper 3 Dn 

  //////////////////////////////////Move the robot to the desired positions//////////////////////////// 
  /////////////////////////////////////////////////////////////////////////////////////////////////////

  //Move the tobot to the up position
  stepper3.moveTo(stepper3PositionUp);
  while (stepper3.currentPosition() != stepper3PositionUp){
    stepper3.run();
  }

  //Move the robot to the x and y coordinates
  stepper1.moveTo(stepper1Position);
  stepper2.moveTo(stepper2Position);
 while (stepper1.currentPosition() != stepper1Position ||stepper2.currentPosition() != stepper2Position) {
    stepper1.run();   
    stepper2.run(); 
  }

  //Move the robot to the down position
  if (data[2]!=0){
    stepper3.moveTo(stepper3PositionDn);
    }
  while (stepper3.currentPosition() !=stepper3PositionDn) {
    stepper3.run();
    finish =0;
  }

  if (stepper3.currentPosition() ==stepper3PositionDn){
  pickPlace();
  finish =1;
  data[2] =0;
  }
  // Pick and place the pieces depending on the previous state
   

  // stepper3.moveTo(stepper3PositionUp);
  // while (stepper3.currentPosition() !=stepper3PositionUp) {
  //   //Serial.println(stepper3.currentPosition());
  //   //Serial.println(stepper3PositionUp);
  //   stepper3.run();
  // }

}


void pickPlace(){
  if (finish ==0){
    if (pick==1){
      digitalWrite(ElectroMagnet,HIGH);      
      pick=0;
      Serial.println("Iman prendido");
    }

    else{
      digitalWrite(ElectroMagnet,LOW);
      pick=1;
      Serial.println("Iman apagado");
    }
  }
}

void verifEnd(){
  bool button1St = digitalRead(limitSwitch1);
  bool button2St = digitalRead(limitSwitch2);
  bool button3St = digitalRead(limitSwitch3);

  if (button1St ==true || button2St ==true || button3St ==true){
    STOP=true;
  }
  else {
    STOP = false;
  }
}

void serialFlush() {
  while (Serial.available() > 0) {  //while there are characters in the serial buffer, because Serial.available is >0
    Serial.read();         // get one character
  }
}


void pieceTipe(int piece){
  if (piece ==1){   //Pawn
    zdn=152;
  }
  else if (piece ==2){   //Rook
    zdn=146;
  }
  else if (piece ==3){   //Night
    zdn=139;
  }
  else if (piece ==4){   //Bishop
    zdn=136;
  }
  else if (piece ==5){   //Queen
    zdn=126;
  }
  else if (piece ==6){   //King
    zdn=113;
  }
  else {
    zdn =0; 
  }

  
  zdn =zdn;
  //data[2]=0;
}


void inverseKinematics(float x, float y) {
  theta2 = acos((sq(x) + sq(y) - sq(L1) - sq(L2)) / (2 * L1 * L2));
  if (x < 0 & y < 0) {
    theta2 = (-1) * theta2;
  }
  
  theta1 = atan(x / y) - atan((L2 * sin(theta2)) / (L1 + L2 * cos(theta2)));
  
  theta2 = (-1) * theta2 * 180 / PI;
  theta1 = theta1 * 180 / PI;

 // Angles adjustment depending in which quadrant the final tool coordinate x,y is
  if (x >= 0 & y >= 0) {       // 1st quadrant
    theta1 = 90 - theta1;
  }
  if (x < 0 & y > 0) {       // 2nd quadrant
    theta1 = 90 - theta1;
  }
  if (x < 0 & y < 0) {       // 3d quadrant
    theta1 = 270 - theta1;
  }
  if (x > 0 & y < 0) {       // 4th quadrant
    theta1 = -90 - theta1;
  }
  if (x < 0 & y == 0) {
    theta1 = 270 + theta1;
  }
  

  theta1=round(theta1);
  theta2=round(theta2);
}

void homing() {
  // Homing Stepper3 (Z position)
  while (digitalRead(limitSwitch3) != 1) {
    stepper3.setSpeed(-1500);
    stepper3.runSpeed();
    stepper3.setCurrentPosition(-1662); // When limit switch pressed set position to -1662 steps
  }
  delay(20);

  stepper3.moveTo(0);
  while (stepper3.currentPosition() != 0) {
    stepper3.run();
  }

  // Homing Stepper2 (Theta1 position)
  while (digitalRead(limitSwitch2) != 1) {
    stepper2.setSpeed(-1200);
    stepper2.runSpeed();
    stepper2.setCurrentPosition(-1400); // When limit switch pressed set position to -5440 steps
  }
  delay(20);


  stepper2.moveTo(0);
  while (stepper2.currentPosition() != 0) {
    stepper2.run();
    
  }

  // Homing Stepper1
  while (digitalRead(limitSwitch1) != 1) {
    stepper1.setSpeed(-1200);
    stepper1.runSpeed();
    stepper1.setCurrentPosition(0); // When limit switch pressed set position to 0 steps
    //stepper1.setCurrentPosition(-5420); -900
  }
  delay(20);
  stepper1.moveTo(0);
  while (stepper1.currentPosition() != 0) {
    stepper1.run();
  }
}
