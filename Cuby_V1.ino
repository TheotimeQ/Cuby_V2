//Libraries
#include <Wire.h>//https://www.arduino.cc/en/reference/wire
#include <Adafruit_PWMServoDriver.h>//https://github.com/adafruit/Adafruit-PWM-Servo-Driver-Library

//Constants
#define nbPCAServo 7 //Number of servo max = 7 

//Parameters
// 0-Left arm | 1-Right arm | 2-Mouth | 3-Eyes | 4-Left eyebrow | 5-Right eyebrow | 6-Flag

int MIN_IMP [nbPCAServo] = {500, 500, 500, 500, 500, 500, 500};
int MAX_IMP [nbPCAServo] = {2500, 2500, 2500, 2500, 2500, 2500, 2500};
int MIN_ANG [nbPCAServo] = {0, 0, 0, 0, 0, 0, 0};
int MAX_ANG [nbPCAServo] = {180, 180, 180, 180, 180, 180, 180};

int CURRENT_POS [nbPCAServo] = {84, 99, 100, 113, 90, 90, 10};
int NEXT_POS [nbPCAServo] = {CURRENT_POS[0], CURRENT_POS[1], CURRENT_POS[2], CURRENT_POS[3], CURRENT_POS[4], CURRENT_POS[5], CURRENT_POS[6]};

int ARML_POS [3] = {84, 110, 140} ;
int ARMR_POS [3] = {99, 70, 40} ;
int MOUTH_POS [3] = {70, 100, 120} ;
int EYES_POS [3] = {92, 113, 128} ;


int FLAG_POS [2] = {40, 120} ;

//Objects
Adafruit_PWMServoDriver pca= Adafruit_PWMServoDriver(0x40); //First card (No solder)

//----------------------------------------------------------------------------------------------------------------------------------

void setup(){
    //Init Serial USB
    Serial.begin(9600);
    Serial.println(F("Initialize System"));

    pca.begin();
    pca.setPWMFreq(50);  // Analog servos run at ~50 Hz updates

    Init();  // Initialise servos
    delay(1000);
}

void loop(){
     Flag(0);
     Mouth(0);
     ArmL(0);
     ArmR(0);
     Eyes(0);
     
     Flag(1);
     Mouth(1);
     ArmL(1);
     ArmR(1);
     Eyes(1);
     
     Mouth(2);
     ArmL(2);
     ArmR(2);
     Eyes(2);
}

//----------------------------------------------------------------------------------------------------------------------------------

//Functionn to initialise the cuby's servos controlled by PCA9685 I2C Module
void Init(){
     for (int i = 0; i < nbPCAServo; i++) {
        Serial.print("Servo : ");
        Serial.print(i);
        Serial.print("  |  Init pos : ");
        Serial.print(CURRENT_POS[i]);
        Serial.print("  |  Init Angle : ");
        Serial.println(AngleToImp(CURRENT_POS[i],i));
        pca.writeMicroseconds(i , AngleToImp(CURRENT_POS[i],i) ); delay(500);     //set servos to its init position
        pca.setPin(i,0,true);                                                      // deactivate pin i ie motor can be mooved
        delay(50);
     }
}

//----------------------------------------------------------------------------------------------------------------------------------

//Function to moove the left arm to predefined position
void ArmL(int POS){
  NEXT_POS[0] = ARML_POS[POS] ;
  Moove_One(1,9,0);
}

//Function to moove the right arm to predefined position
void ArmR(int POS){
  NEXT_POS[1] = ARMR_POS[POS] ;
  Moove_One(1,9,1);
}

//Function to moove the Mouth to predefined position
void Mouth(int POS){
  NEXT_POS[2] = MOUTH_POS[POS] ;
  Moove_One(1,15,2);
}

//Function to moove the Flag to predefined position
void Eyes(int POS){
  NEXT_POS[3] = EYES_POS[POS] ;
  Moove_One(1,15,3);
}

//Function to moove the Flag to predefined position
void Flag(int POS){
  NEXT_POS[6] = FLAG_POS[POS] ;
  Moove_One(1,1,6);
}



//----------------------------------------------------------Annexes----------------------------------------------------------------

//Function angle to impulson usualy (0,180° to 400,2400us)
int AngleToImp(double x,int i){
     int imp=(x - MIN_ANG[i]) * (MAX_IMP[i]-MIN_IMP[i]) / (MAX_ANG[i]-MIN_ANG[i]) + MIN_IMP[i];
     imp=max(imp,MIN_IMP[i]);
     imp=min(imp,MAX_IMP[i]);
     return imp;
}

void Moove_One(int Speed,int wait, int Servos){
  int inc = (NEXT_POS[Servos] - CURRENT_POS[Servos])/abs(NEXT_POS[Servos] - CURRENT_POS[Servos]) ; //Sign of the inc
  while (abs(NEXT_POS[Servos] - CURRENT_POS[Servos]) > Speed) {
     CURRENT_POS[Servos] = CURRENT_POS[Servos] + inc * Speed ;
     pca.writeMicroseconds(Servos , AngleToImp(CURRENT_POS[Servos],Servos) ); delay(wait);
  }
  pca.setPin(Servos,0,true); 
}

//----------------------------------------------------------------------------------------------------------------------------------

/*
void Init(){
for (int i=0; i<nbPCAServo; i++) {
 Serial.print("Servo");
 Serial.println(i);
     for(int pos=(MAX_IMP[i]+MIN_IMP[i])/2;pos<MAX_IMP[i];pos+=10){  // 90 to 180
       pca.writeMicroseconds(i,pos);delay(10);
     }
     for(int pos=MAX_IMP[i];pos>MIN_IMP[i];pos-=10){  // 180 to 0
       pca.writeMicroseconds(i,pos);delay(10);
     }
     for(int pos=MIN_IMP[i];pos<(MAX_IMP[i]+MIN_IMP[i])/2;pos+=10){ // 0 to 90
       pca.writeMicroseconds(i,pos);delay(10);
     }
     pca.setPin(i,0,true); // deactivate pin i
   }
}

*/
