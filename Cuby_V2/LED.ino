//----------------------------------------------------------------LED COLOR----------------------------------------------------------------------------------
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

//------------------------------------------------Turn off----------------------------------------------------
void Black_Body(){
  for(int i = 0; i < NUM_LEDS_STRIP_1; i++) {
      Strip_1[i] = CRGB::Black;
  }
  for(int i = 0; i < NUM_LEDS_STRIP_2 - 2; i++) {
      Strip_2[i] = CRGB::Black;
  }
  FastLED.show();
}

void LED_all_Black(){
  Serial.println("Black") ;
  for(int i = 0; i < NUM_LEDS_STRIP_1; i++) {
      Strip_1[i] = CRGB::Black;
  }
  for(int i = 0; i < NUM_LEDS_STRIP_2; i++) {
      Strip_2[i] = CRGB::Black;
  }
  delay(1000/UPDATERATE);
  FastLED.show();
}


//-------------------------------------------------Brightness-------------------------------------------------
void LED_POWER(int Power){
    int Brightness = (constrain(Power,0,100)*255)/100 ;
    FastLED.setBrightness(Brightness);
    FastLED.show();
}
