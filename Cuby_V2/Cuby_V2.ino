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

float Next_LED_Brightness = 100 ;
float Next_Eye_Color = 200 ;
float Next_Body_Color = 0 ;

int Current_LED_Brightness = 0 ;
int Current_Eye_Color = 0 ;
int Current_Body_Color = 0 ;  //   90 - Orange

int Led_Brightness_Speed = 1 ;
int Led_Eye_Speed = 1 ;
int Led_Body_Speed = 1 ;

int dt = 0 ;

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

float CURRENT_POS [nbPCAServo] = {70, 90, 116, 85, 95, 84, 63};      //Current angle of the servo
float NEXT_POS [nbPCAServo] = {CURRENT_POS[0], CURRENT_POS[1], CURRENT_POS[2], CURRENT_POS[3], CURRENT_POS[4], CURRENT_POS[5], CURRENT_POS[6]}; 
float SERVOS_SPEED [nbPCAServo] = {0.6, 0.6, 2, 0.5, 2, 2, 3} ;

const float ARML_POS [3] = {70, 133, 144} ;
const float ARMR_POS [3] = {90, 28, 17} ;
const float MOUTH_POS [3] = {67, 92, 116} ;    //Stock predefined position ( for Fast and simple coding animation)
const float EYES_POS [3] = {60, 85, 102} ;
const float BROWL_POS [3] = {65, 95, 125} ;     
const float BROWR_POS [3] = {115, 85, 55} ;       
const float FLAG_POS [3] = {66, 120, 170} ;

//Other Variables
float Time ;
float Time_0 ;
float Waited ;

int RdNumber ;
int Emo = 0 ;

const int NozLeftPin = 3 ; 
const int NozRightPin = 2 ; 

int NozLeftState ;
int NozRightState ;

//Objects
Adafruit_PWMServoDriver pca= Adafruit_PWMServoDriver(0x40); //First card (No solder)

//-----------------------------------------------------------------Setup------------------------------------------------------------------------------------

void setup(){
    //Init Serial USB
    Serial.begin(9600);
    Serial.println("Initialize System");

    pca.begin();
    pca.setPWMFreq(50);  // Analog servos run at ~50 Hz updates

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

    Current_Body_Color = random(0, 255) ; 

    //Initialise Color
    LED_all_Black() ;
    UPDATE() ;
    Apply_Emotion(Emo) ;
    delay(500);
}

//-------------------------------------------------------------------Main Loop-------------------------------------------------------------------------------
void loop(){
    Time_Update() ;
    
    Veille() ;
    CheckNoze() ;
    //CheckCapaSensor() ;

    //Display() ;
    Animation() ;
    Apply_Emotion(Emo) ;
    
    delay(1000/UPDATERATE);
}

//----------------------------------------------------------------Display------------------------------------------------------------------------------------
//Print for debug

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
