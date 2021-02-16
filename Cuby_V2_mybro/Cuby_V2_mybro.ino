//Libraries
#include <Wire.h>//https://www.arduino.cc/en/reference/wire
#include <Adafruit_PWMServoDriver.h>//https://github.com/adafruit/Adafruit-PWM-Servo-Driver-Library
#include <FastLED.h>
#include <CapacitiveSensor.h>

//Constants
#define nbPCAServo 7 //Number of servo = 7 

//LED def
#define NUM_STRIPS 6
#define NUM_LEDS_STRIP_1 41
#define NUM_LEDS_STRIP_2 29

#define UPDATERATE 25

CRGB Strip_1[NUM_LEDS_STRIP_1];
CRGB Strip_2[NUM_LEDS_STRIP_2];

const int Pin_Strip_1 = 7 ;
const int Pin_Strip_2 = 9 ;

int Next_LED_Brightness = 30 ;
int Next_Eye_Color = 200 ;
int Next_Body_Color = 0 ;

int Current_LED_Brightness = 30 ;
int Current_Eye_Color = 200 ;
int Current_Body_Color = 0 ;  //   90 - Orange

int Led_Change_Speed = 1 ;

//Def capacitiv sensor
CapacitiveSensor   cs_4_5 = CapacitiveSensor(4,5); 
CapacitiveSensor   cs_4_8 = CapacitiveSensor(4,8); 
long Capa_Sensor_1 ;
long Capa_Sensor_2 ;

//Parameters Servos
// 0-Left arm | 1-Right arm | 2-Mouth | 3-Eyes | 4-Left eyebrow | 5-Right eyebrow | 6-Flag

float MIN_IMP [nbPCAServo] = {500, 500, 500, 500, 500, 500, 500};
float MAX_IMP [nbPCAServo] = {2500, 2500, 2500, 2500, 2500, 2500, 2500};
float MIN_ANG [nbPCAServo] = {0, 0, 0, 0, 0, 0, 0};
float MAX_ANG [nbPCAServo] = {180, 180, 180, 180, 180, 180, 180};

float CURRENT_POS [nbPCAServo] = {70, 90, 115, 85, 95, 84, 63};      //Current angle of the servo
float NEXT_POS [nbPCAServo] = {CURRENT_POS[0], CURRENT_POS[1], CURRENT_POS[2], CURRENT_POS[3], CURRENT_POS[4], CURRENT_POS[5], CURRENT_POS[6]};  //Next position for the servos
float SERVOS_SPEED [nbPCAServo] = {0.4, 0.4, 1.5, 0.1, 2, 2, 0.1} ;

const float ARML_POS [3] = {70, 133, 144} ;
const float ARMR_POS [3] = {90, 28, 17} ;
const float MOUTH_POS [3] = {60, 95, 115} ;    //Stock predefined position ( for Fast and simple coding animation)
const float EYES_POS [3] = {60, 85, 102} ;
const float BROWL_POS [3] = {0, 95, 180} ;     
const float BROWR_POS [3] = {180, 84, 0} ;       
const float FLAG_POS [3] = {63, 120, 170} ;

//Other Variables
float Time ;
float Time_0 ;
float Waited ;

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
    Init_Servo() ;
    
    //Setup LED
    FastLED.addLeds<WS2812B, Pin_Strip_1>(Strip_1, NUM_LEDS_STRIP_1); //Strip 1
    FastLED.addLeds<WS2812B, Pin_Strip_2>(Strip_2, NUM_LEDS_STRIP_2); //Strip 2
    LED_POWER(Next_LED_Brightness) ;

    Time_0 = Min() ;
    Time = Min() ;

    pinMode(NozLeftPin, INPUT);     //Pin Noze button
    pinMode(NozRightPin, INPUT);

    NozLeftState = digitalRead(NozLeftPin);
    NozRightState = digitalRead(NozRightPin)  ;
    
    randomSeed(analogRead(0));  //Random seed generation

    Next_Body_Color = random(0, 255) ; 
    Current_Body_Color = Next_Body_Color ;

    //Initialise Color
    LED_all_Black() ;
    LED_Color(Next_Body_Color) ;
    LED_Color_Eyes(Next_Eye_Color) ;
}

//-----------------------------------------------------------Main Loop--------------------------------------------------------------
void loop(){
    Time_Update() ;
    
    Veille() ;
    CheckNoze() ;
    //CheckCapaSensor() ;

    //Display() ;
    Animation() ;
    
    Current_Body_Color = (Current_Body_Color%255) + 1 ;
    Apply_Color_Body(Current_Body_Color) ;
    delay(1000/UPDATERATE);
}

//---------------------------------------------------Display-----------------------------------------------------------------------
void Display(){
  Serial.print(RdNumber) ;
  Serial.print("  |  ") ;
  Serial.print(NozLeftState) ;
  Serial.print("  |  ") ;
  Serial.print(NozRightState) ;
  Serial.print("  |  ") ;
  Serial.print(Waited) ;
  Serial.println(" ") ;
}
//---------------------------------------------------Moove predefined--------------------------------------------------------------

//Function to moove the left arm to predefined position
void ArmL(int POS){
  NEXT_POS[0] = ARML_POS[POS] ;
  Moove_One(3,40,0);
}

//Function to moove the right arm to predefined position
void ArmR(int POS){
  NEXT_POS[1] = ARMR_POS[POS] ;
  Moove_One(3,40,1);
}

//Function to moove the Mouth to predefined position
void Mouth(int POS){
  NEXT_POS[2] = MOUTH_POS[POS] ;
  Moove_One(3,20,2);
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
//Update time
void Time_Update(){
    Time = Min() ;
    Waited = Time - Time_0 ;
}

//Function angle to impulson usualy (0-180° to 400-2400us)
int AngleToImp(double x,int i){
     float imp=(x - MIN_ANG[i]) * (MAX_IMP[i]-MIN_IMP[i]) / (MAX_ANG[i]-MIN_ANG[i]) + MIN_IMP[i];
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
    Serial.println(Brightness);
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
  for(int i = 0; i < NUM_LEDS_STRIP_2 - 2; i++) {
      //Serial.println(i) ;
      Strip_2[i] = CHSV(Color, 255, 255);
      delay(1000/UPDATERATE);
      FastLED.show();
  }
}

void LED_Color_Eyes(int Color_Eyes){
    Strip_2[27] = CHSV(Color_Eyes, 255, 255);
    Strip_2[28] = CHSV(Color_Eyes, 255, 255);
    FastLED.show();
}

void Apply_Color_Body(int Color){
  for(int i = 0; i < NUM_LEDS_STRIP_1; i++) {
      Strip_1[i] = CHSV(Color, 255, 255);
  }
  for(int i = 0; i < NUM_LEDS_STRIP_2 - 2; i++) {
      Strip_2[i] = CHSV(Color, 255, 255);
  }
  FastLED.show();
}
//---------------------------------------------------------Moove One------------------------------------------------------------

//Moove one servo with the specified speed and delay between each inc 
void Moove_One(float Speed,int wait, int Servos){
  int inc = (NEXT_POS[Servos] - CURRENT_POS[Servos])/abs(NEXT_POS[Servos] - CURRENT_POS[Servos]) ; //Sign of the inc
  while (abs(NEXT_POS[Servos] - CURRENT_POS[Servos]) > Speed) {
     CURRENT_POS[Servos] = CURRENT_POS[Servos] + inc * Speed ;
     pca.writeMicroseconds(Servos , AngleToImp(CURRENT_POS[Servos],Servos) ); 
     delay(wait);
  }
  pca.setPin(Servos,0,true); 
}


//-------------------------------------------------------UPDATE ALL-----------------------------------------------------------------

void UPDATE(){
    float dt = 0 ;
    while(CheckPosition_All() == false){
        for(int Servos = 0; Servos < nbPCAServo ; Servos++){
           if( CheckPosition(Servos) == false){
                float inc = (NEXT_POS[Servos] - CURRENT_POS[Servos])/abs(NEXT_POS[Servos] - CURRENT_POS[Servos]) * SERVOS_SPEED[Servos] ;
                CURRENT_POS[Servos] = CURRENT_POS[Servos] + inc ;
                pca.writeMicroseconds(Servos , AngleToImp(CURRENT_POS[Servos],Servos) );
           } 
           else {
                pca.writeMicroseconds(Servos , AngleToImp(CURRENT_POS[Servos],Servos) );
           }
        }
        if(abs(Next_LED_Brightness - Current_LED_Brightness) > Led_Change_Speed){
                float inc = (Next_LED_Brightness - Current_LED_Brightness)/abs(Next_LED_Brightness - Current_LED_Brightness) * Led_Change_Speed ;
                Current_LED_Brightness = Current_LED_Brightness + inc ;
                LED_POWER(Current_LED_Brightness) ;
        }
        if(abs(Next_Eye_Color - Current_Eye_Color) > Led_Change_Speed){
                float inc = (Next_Eye_Color - Current_Eye_Color)/abs(Next_Eye_Color - Current_Eye_Color) * Led_Change_Speed ;
                Current_Eye_Color = Current_Eye_Color + inc ;
                LED_Color_Eyes(Current_Eye_Color) ;
        }
        if(abs(Next_Body_Color - Current_Body_Color) > Led_Change_Speed){
                float inc = (Next_Body_Color - Current_Body_Color)/abs(Next_Body_Color - Current_Body_Color) * Led_Change_Speed ;
                Current_Body_Color = Current_Body_Color + inc ;
                Apply_Color_Body(Current_Body_Color) ;
        }
        delay(dt);
    }
    for(int Servos = 0; Servos < nbPCAServo ; Servos++){
        pca.setPin(Servos,0,true);
    }
}

bool CheckPosition_All(){
    bool All_finish = true ;
    for(int Servos = 0; Servos < nbPCAServo ; Servos++){
        if(abs(NEXT_POS[Servos] - CURRENT_POS[Servos]) > SERVOS_SPEED[Servos]){
            All_finish = false ;
        }
    }
    if(abs(Next_LED_Brightness - Current_LED_Brightness) > Led_Change_Speed){
        All_finish = false ;
        }
    if(abs(Next_Eye_Color - Current_Eye_Color) > Led_Change_Speed){
        All_finish = false ;
        }
    if(abs(Next_Body_Color - Current_Body_Color) > Led_Change_Speed){
        All_finish = false ;
        }
    return All_finish ;
}

bool CheckPosition(int Servos){
    bool finish = true ;
    if(abs(NEXT_POS[Servos] - CURRENT_POS[Servos]) > SERVOS_SPEED[Servos]){
         finish = false ;
    }
    return finish ;
}

//----------------------------------------------------------Wait--------------------------------------------------------------------
void Veille() {
  if ( 4 < Waited ){
    Serial.println("Veille 5");     
    RdNumber = 305 ;
    }
  else if ( 3 < Waited and Waited < 3.1 ){
    Serial.println("Veille 4");
    RdNumber = 304 ;
    }
  else if ( 2 < Waited and Waited < 2.1 ){
    Serial.println("Veille 3");
    RdNumber = 303 ;
    }
  else if ( 1.5 < Waited and Waited < 1.6 ){
    Serial.println("Veille 2");
    RdNumber = 302 ;
    }
  else if ( 1 < Waited and Waited < 1.1 ){
    Serial.println("Veille 1");     
    RdNumber = 301 ;
    }
  else if ( 0.5 < Waited and Waited < 0.6 ){
    Serial.println("Veille 0");
    RdNumber = 300 ;
    }
}

//-----------------------------------------------Animations----------------------------------------------------------
void Animation() {
    
    switch (RdNumber) {
      //----------------------------------------------------------------Left side----------------------------------------------
      case 100:
          Mouth(0);
          Eyes(0);
          delay(50);
          Eyes(2);
          delay(50);
          Eyes(1);
          ArmL(1);
          delay(100);
          ArmL(0);
          Mouth(2);
      break;
      case 101:
          ArmL(1);
          delay(100);
          ArmL(0);
      break;
      case 102:
          ArmL(1);
          delay(100);
          ArmL(0);
      break;
      case 103:
          ArmL(1);
          delay(100);
          ArmL(0);
      break;
      case 104:
          ArmL(1);
          delay(100);
          ArmL(0);
      break;
      case 105:
          ArmL(1);
          delay(100);
          ArmL(0);
      break;
      //--------------------------------------------------------Right Side--------------------------------------------------------
      case 200:
          ArmR(1);
          delay(100);
          ArmR(0);
      break;
      case 201:
//          NEXT_POS[0] = ARML_POS[1] ;
//          NEXT_POS[1] = ARMR_POS[1] ;
//          NEXT_POS[2] = MOUTH_POS[1] ;
//          NEXT_POS[3] = EYES_POS[1] ;
//          NEXT_POS[4] = BROWL_POS[1] ;
//          NEXT_POS[5] = BROWR_POS[0] ;
//          NEXT_POS[6] = FLAG_POS[0] ;
// 
//          Next_Eye_Color = 90 ;
//          Next_Body_Color = 20 ;
//          Next_LED_Brightness = 10 ;
//          
//          UPDATE() ;
//          delay(100);

          NEXT_POS[4] = 70 ;
          NEXT_POS[5] = 110 ;
          NEXT_POS[2] = MOUTH_POS[1] ;
          Next_Eye_Color = 95 ;
          Next_Body_Color = 95 ;
          Next_LED_Brightness = 100 ;
          UPDATE() ;
          delay(1000);
          NEXT_POS[1] = ARMR_POS[1] ;
          UPDATE() ;
          delay(100);
          NEXT_POS[1] = ARMR_POS[0] ;
          UPDATE() ;
          delay(500);
          NEXT_POS[4] = BROWL_POS[1] ;
          NEXT_POS[5] = BROWR_POS[1] ;
          NEXT_POS[2] = MOUTH_POS[2] ;
          Next_Eye_Color = 200 ;
          Next_LED_Brightness = 50 ;
          UPDATE() ;   
      break;
      case 202:
          ArmR(1);
          delay(100);
          ArmR(0);
      break;
      case 203:
          ArmR(1);
          delay(100);
          ArmR(0);
      break;
      case 204:
          ArmR(1);
          delay(100);
          ArmR(0);
      break;
      case 205:
          ArmR(1);
          delay(100);
          ArmR(0);
      break;
      //------------------------------Veille-------------------------
      case 300:
          Next_Body_Color = 10 ;
          NEXT_POS[4] = 70 ;
          NEXT_POS[5] = 110 ;
          UPDATE() ;
      break;
      case 301:
          Next_Body_Color = 10 ;
          UPDATE() ;
      break;
      case 302:
          Next_Body_Color = 10 ;
          UPDATE() ;
      break;
      case 303:
          Next_Body_Color = 10 ;
          UPDATE() ;
      break;
      case 304:
          Next_Body_Color = 10 ;
          UPDATE() ;
      break;
      case 305:
          Next_Body_Color = 10 ;
          UPDATE() ;
      break;
  } 
  RdNumber = 0 ;
}

//-------------------------------------------------Check Noze + Generate Rd Number------------------------------------------------------------------------------
void CheckNoze() {
  NozLeftState = digitalRead(NozLeftPin) ;
  NozRightState = digitalRead(NozRightPin)  ;
  
  if (NozLeftState == HIGH){
      RdNumber = 100 ; //random(100, 110) ;
      Time_0 = Min() ;
    }
  else if (NozRightState == HIGH){
      RdNumber = 200 ; //random(200, 210) ;
      Time_0 = Min() ;
    }
}

//---------------------------------------------------Check Capacitiv sensor--------------------------------------------------------------
void CheckCapaSensor(){
  Serial.println("Check capacitif : ") ;
  long Capa_Sensor_1 = cs_4_5.capacitiveSensor(30);
  long Capa_Sensor_2 = cs_4_8.capacitiveSensor(30);
  Serial.print(Capa_Sensor_1) ;
  Serial.print("   |   ") ;
  Serial.println(Capa_Sensor_2) ;
  delay(500);
}

//---------------------------------------------------Initialisation Servos--------------------------------------------------------------
//Function to initialise cuby's servos controlled by PCA9685 I2C Module
void Init_Servo(){
     Serial.println("Init servo");
     for (int i = 0; i < nbPCAServo; i++) {
        pca.writeMicroseconds(i , AngleToImp(CURRENT_POS[i],i) ); delay(300);     //set servos to its init position
        pca.setPin(i,0,true);                                                      // deactivate pin i ie motor can be mooved
     }
}
