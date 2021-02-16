void InitLed(){

    //Setup LED
    FastLED.addLeds<WS2812B, Pin_Strip_1>(Strip_1, NUM_LEDS_STRIP_1); //Strip 1
    FastLED.addLeds<WS2812B, Pin_Strip_2>(Strip_2, NUM_LEDS_STRIP_2); //Strip 2
    LED_POWER(Next_LED_Brightness) ;

  
}
