//Libraries
#include <Wire.h>//https://www.arduino.cc/en/reference/wire
#include <Adafruit_PWMServoDriver.h>//https://github.com/adafruit/Adafruit-PWM-Servo-Driver-Library
#include <FastLED.h>

//Constants
#define nbPCAServo 7 //Number of servo = 7 

//LED def
#define NUM_STRIPS 6
#define NUM_LEDS_STRIP_1 21
#define NUM_LEDS_STRIP_2 27
#define NUM_LEDS_STRIP_3 0
#define NUM_LEDS_STRIP_4 0
#define NUM_LEDS_STRIP_5 0
#define NUM_LEDS_STRIP_6 0

#define UPDATERATE 200

CRGB Strip_1[NUM_LEDS_STRIP_1];
CRGB Strip_2[NUM_LEDS_STRIP_2];
//CRGB Strip_3[NUM_LEDS_STRIP_3];
//CRGB Strip_4[NUM_LEDS_STRIP_4];
//CRGB Strip_5[NUM_LEDS_STRIP_5];
//CRGB Strip_6[NUM_LEDS_STRIP_6];

const int Pin_Strip_1 = 7 ;
const int Pin_Strip_2 = 9 ;
const int Pin_Strip_3 = 6 ;
const int Pin_Strip_4 = 10 ;
const int Pin_Strip_5 = 11 ;
const int Pin_Strip_6 = 12 ;

int LED_Brightness = 70 ;

//Parameters Servos
// 0-Left arm | 1-Right arm | 2-Mouth | 3-Eyes | 4-Left eyebrow | 5-Right eyebrow | 6-Flag

int MIN_IMP [nbPCAServo] = {500, 500, 500, 500, 500, 500, 500};
int MAX_IMP [nbPCAServo] = {2500, 2500, 2500, 2500, 2500, 2500, 2500};
int MIN_ANG [nbPCAServo] = {0, 0, 0, 0, 0, 0, 0};
int MAX_ANG [nbPCAServo] = {180, 180, 180, 180, 180, 180, 180};

int CURRENT_POS [nbPCAServo] = {10, 125, 139, 113, 120, 70, 0};      //Current angle of the servo
int NEXT_POS [nbPCAServo] = {CURRENT_POS[0], CURRENT_POS[1], CURRENT_POS[2], CURRENT_POS[3], CURRENT_POS[4], CURRENT_POS[5], CURRENT_POS[6]};  //Next position for the servos

int ARML_POS [3] = {10, 70, 80} ;
int ARMR_POS [3] = {125, 60, 50} ;
int MOUTH_POS [3] = {80, 120, 139} ;    //Stock predefined position ( for Fast and simple coding animation)
int EYES_POS [3] = {92, 113, 128} ;
int BROWL_POS [3] = {70, 120, 170} ;      //{0, 90, 180}
int BROWR_POS [3] = {120, 70, 20} ;      //{0, 90, 180} 
int FLAG_POS [3] = {0, 90, 120} ;

//Other Variables
float Time ;
float Time_0 ;

int RdNumber ;

const int NozLeftPin = 3 ; 
const int NozRightPin = 2 ; 

int NozLeftState ;
int NozRightState ;

//Objects
Adafruit_PWMServoDriver pca= Adafruit_PWMServoDriver(0x40); //First card (No solder)

//--------------------------------------------------Setup-------------------------------------------------------------------

void setup(){
    //Init Serial USB
    Serial.begin(9600);
    Serial.println("Initialize System");

    pca.begin();
    pca.setPWMFreq(50);  // Analog servos run at ~50 Hz updates

    //Setup LED
    FastLED.addLeds<WS2812B, Pin_Strip_1>(Strip_1, NUM_LEDS_STRIP_1); //Strip 1
    FastLED.addLeds<WS2812B, Pin_Strip_2>(Strip_2, NUM_LEDS_STRIP_2); //Strip 2
    //FastLED.addLeds<WS2812B, Pin_Strip_3>(Strip_3, NUM_LEDS_STRIP_3); //Strip 1
    //FastLED.addLeds<WS2812B, Pin_Strip_4>(Strip_4, NUM_LEDS_STRIP_4); //Strip 2
    //FastLED.addLeds<WS2812B, Pin_Strip_5>(Strip_5, NUM_LEDS_STRIP_5); //Strip 1
    //FastLED.addLeds<WS2812B, Pin_Strip_6>(Strip_6, NUM_LEDS_STRIP_6); //Strip 2
    LED_POWER(LED_Brightness) ;
    
    //Initialise Color
    LED_all_Black() ;
    delay(1000);
    LED_Color(120) ;

    Init();  // Initialise servos
    Time_0 = Min() ;
    Time = Min() ;

    pinMode(NozLeftPin, INPUT);     //Pin Noze button
    pinMode(NozRightPin, INPUT);

    NozLeftState = digitalRead(NozLeftPin);
    NozRightState = digitalRead(NozRightPin)  ;
    
    randomSeed(analogRead(0));  //Random seed generation
    
    delay(1000);
}

//-----------------------------------------------------------Main Loop--------------------------------------------------------------
void loop(){
    Time = Min() ;
    //Veille() ;
    //Animation() ;
    //CheckNoze() ;
}

//---------------------------------------------------Moove predefined--------------------------------------------------------------

//Function to moove the left arm to predefined position
void ArmL(int POS){
  NEXT_POS[0] = ARML_POS[POS] ;
  Moove_One(2,10,0);
}

//Function to moove the right arm to predefined position
void ArmR(int POS){
  NEXT_POS[1] = ARMR_POS[POS] ;
  Moove_One(2,10,1);
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
void BrowL(int POS){
  NEXT_POS[4] = BROWL_POS[POS] ;
  Moove_One(2,3,4);
}

//Function to moove the Flag to predefined position
void BrowR(int POS){
  NEXT_POS[5] = BROWR_POS[POS] ;
  Moove_One(2,3,5);
}

//Function to moove the Flag to predefined position
void Flag(int POS){
  NEXT_POS[6] = FLAG_POS[POS] ;
  Moove_One(1,5,6);
}

//----------------------------------------------------------Annexes----------------------------------------------------------------
//Function angle to impulson usualy (0-180° to 400-2400us)
int AngleToImp(double x,int i){
     int imp=(x - MIN_ANG[i]) * (MAX_IMP[i]-MIN_IMP[i]) / (MAX_ANG[i]-MIN_ANG[i]) + MIN_IMP[i];
     imp=max(imp,MIN_IMP[i]);
     imp=min(imp,MAX_IMP[i]);
     return imp;
}

//Update le temps
float Min() {
  float Temps ;
  float NbRollOver = 0;
  float MinDebordement = 71582;
  float LastMin = 0 ;
  float CurrentMin = ((millis())*0.001)/60;
  
  if (CurrentMin < LastMin) {
     NbRollOver += 1;
  }
  LastMin = CurrentMin ;
  Temps = NbRollOver*MinDebordement+CurrentMin ;

  return Temps ;
}

void LED_POWER(int Power){
    int Brightness = (constrain(Power,0,100)*255)/100 ;
    FastLED.setBrightness(Brightness);
    Serial.print("LED Power set to : ") ;
    Serial.println(Brightness) ;
}

//--------------------------------------------------------LED COLOR-----------------------------------------------------------
void LED_all_Black(){
  Serial.println("Black") ;
  for(int i = 0; i < NUM_LEDS_STRIP_1; i++) {
      //Serial.println(i) ;
      Strip_1[i] = CRGB::Black;
  }
  for(int i = 0; i < NUM_LEDS_STRIP_2; i++) {
      //Serial.println(i) ;
      Strip_2[i] = CRGB::Black;
  }
  delay(1000/UPDATERATE);
  FastLED.show();
}

void LED_Color(int Color){
  Serial.println("Light") ;
  for(int i = 0; i < NUM_LEDS_STRIP_1; i++) {
      //Serial.println(i) ;
      Strip_1[i] = CHSV(Color, 255, 255);
      delay(1000/UPDATERATE);
      FastLED.show();
  }
  for(int i = 0; i < NUM_LEDS_STRIP_2; i++) {
      //Serial.println(i) ;
      Strip_2[i] = CHSV(Color, 255, 255);
      delay(1000/UPDATERATE);
      FastLED.show();
  }
 
}

//---------------------------------------------------------Moove One------------------------------------------------------------

//Moove one servo with the specified speed and delay between each inc 
void Moove_One(int Speed,int wait, int Servos){
  int inc = (NEXT_POS[Servos] - CURRENT_POS[Servos])/abs(NEXT_POS[Servos] - CURRENT_POS[Servos]) ; //Sign of the inc
  while (abs(NEXT_POS[Servos] - CURRENT_POS[Servos]) > Speed) {
     CURRENT_POS[Servos] = CURRENT_POS[Servos] + inc * Speed ;
     pca.writeMicroseconds(Servos , AngleToImp(CURRENT_POS[Servos],Servos) ); delay(wait);
  }
  pca.setPin(Servos,0,true); 
}

//----------------------------------------------------------Wait--------------------------------------------------------------------
void Veille() {
  float Waited = Time - Time_0 ;

  Serial.println(Waited) ;
  
  if ( 4 < Waited ){
    Serial.println("Veille 5");     
    BrowL(1);
    BrowR(1);
    Flag(1);
    }
  else if ( 3 < Waited and Waited < 3.1 ){
    Serial.println("Veille 4");
    ArmL(0);
    ArmR(2);
    }
  else if ( 2 < Waited and Waited < 2.1 ){
    Serial.println("Veille 3");
    Eyes(2);     
    BrowL(2);
    BrowR(2);
    }
  else if ( 1.5 < Waited and Waited < 1.6 ){
    Serial.println("Veille 2");
    Eyes(0);     
    BrowL(0);
    BrowR(0);
    }
  else if ( 1 < Waited and Waited < 1.1 ){
    Serial.println("Veille 1");     
    BrowL(2);
    BrowR(2);
    }
  else if ( 0.5 < Waited and Waited < 0.6 ){
    Serial.println("Veille 0");
    BrowL(0);
    BrowR(0);
    }
}

//-----------------------------------------------Animations----------------------------------------------------------
void Animation() {
  //Serial.println("Animation") ;
//     ArmL(0);
//     ArmR(0);
//     Mouth(0);
//     Eyes(0);
//     BrowL(0);
//     BrowR(0);
//     Flag(0);

//       Eyes(0);
//       Mouth(0);
//       BrowL(0);
//       BrowR(0);
         //ArmL(0);
         //ArmR(0);
       delay(1000);
//       Eyes(1);
//       Mouth(1);
//       BrowL(1);
//       BrowR(1);
         //ArmL(1);
         //ArmR(1);
       delay(1000);
//       Eyes(2);
//       Mouth(2);
//       BrowL(2);
//       BrowR(2);
         //ArmL(2);
         //ArmR(0);
       delay(1000);
         //Eyes(2);
//       Mouth(2);
//       BrowL(2);
//       BrowR(2);
         //ArmL(2);
         //ArmR(2);
       delay(1000);
}

//-------------------------------------------------Check Noze + Generate Rd Number------------------------------------------------------------------------------
void CheckNoze() {
  //Serial.println("CheckNoze") ;
  NozLeftState = digitalRead(NozLeftPin) ;
  NozRightState = digitalRead(NozRightPin)  ;

  if (NozLeftState == HIGH){
      RdNumber = random(100, 110) ;
      Time_0 = Min() ;
    }

  else if (NozRightState == HIGH){
      RdNumber = random(200, 210) ;
      Time_0 = Min() ;
    }

  else {
      RdNumber = 0 ;
    }

  //Serial.print(NozLeftState) ;
  //Serial.print("  |  ") ;
  //Serial.println(NozRightState) ;
  Serial.println(RdNumber) ;
}

//---------------------------------------------------Initialisation Servos--------------------------------------------------------------
//Function to initialise cuby's servos controlled by PCA9685 I2C Module
void Init(){
     for (int i = 0; i < nbPCAServo; i++) {
      
//        Serial.print("Servo : ");
//        Serial.print(i);
//        Serial.print("  |  Init pos : ");
//        Serial.print(CURRENT_POS[i]);
//        Serial.print("  |  Init Angle : ");
//        Serial.println(AngleToImp(CURRENT_POS[i],i));
        
        pca.writeMicroseconds(i , AngleToImp(CURRENT_POS[i],i) ); delay(500);     //set servos to its init position
        pca.setPin(i,0,true);                                                      // deactivate pin i ie motor can be mooved
        delay(50);
 }
}
