//-------------------------------------------------------------------Animations------------------------------------------------------------------------------

void Animation() {
    
    switch (RdNumber) {
      //----------------------------------------------------------------Left side----------------------------------------------
      case 100:
          delay(10);
      break;
      case 101:
          delay(10);
      break;
      case 102:
          delay(10);
      break;
      case 103:
          delay(10);
      break;
      case 104:
          delay(10);
      break;
      case 105:
          delay(10);
      break;
      //--------------------------------------------------------Right Side--------------------------------------------------------
      case 200:
          delay(10);
      break;
      case 201:
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
          Next_Eye_Color = 200 ;
          Next_Body_Color = 90 ;

          for(int i = 0; i < 3 ; i++){
              Next_LED_Brightness = 10 ;
              UPDATE_LED() ;         
              Next_LED_Brightness = 100 ;
              UPDATE_LED() ; 
              Next_LED_Brightness = 10 ;
              UPDATE_LED() ; 
              Next_LED_Brightness = 100 ;
              UPDATE_LED() ;
              Next_LED_Brightness = 10 ;
              UPDATE_LED() ;
              delay(1000);
          }
          Black_Body() ,
          dt = 30 ; 
          Next_Eye_Color = 100 ;
          Next_LED_Brightness = 100 ;
          UPDATE_LED() ;
          dt = 0 ; 
          while(CheckRight() == true and Waited < 0.25) {
                NEXT_POS[0] = ARML_POS[0] + 30 ;
                NEXT_POS[1] = ARMR_POS[0] - 30 ;
                UPDATE() ;
                NEXT_POS[0] = ARML_POS[0] ;
                NEXT_POS[1] = ARMR_POS[0] ;
                UPDATE() ;
                delay(500);
                Time_Update() ;
          }
          
          if(CheckRight() == false and CheckLeft() == false){
                Apply_Emotion(0);
                delay(1000) ;
          }

          else {
                Apply_Emotion(4);
                delay(1000) ;
                Reset_2_arm() ;
          }
          
      break;
      case 203:
          Apply_Emotion(8);
          Reset_2_arm() ;
          delay(500);
      break;
      case 204:
          Reset_Right() ;
          delay(100);
      break;
      case 205:
          Emo = random(0, 7) ;
          Apply_Emotion(Emo) ;
          Reset_Right() ;
          delay(1000);
      break;
 //-------------------------------------------------------Veille----------------------------------------------
      case 300:
          NEXT_POS[3] = EYES_POS[2] ;
          UPDATE() ;
          delay(300);
          NEXT_POS[3] = EYES_POS[0] ;
          UPDATE() ;
          delay(300) ;
          NEXT_POS[2] = MOUTH_POS[1] ;
          NEXT_POS[3] = EYES_POS[1] ;
          NEXT_POS[4] = 70 ;
          NEXT_POS[5] = 110 ;
          UPDATE() ;
          delay(1000);

          NEXT_POS[2] = MOUTH_POS[2] ;
          NEXT_POS[4] = BROWL_POS[1] ;
          NEXT_POS[5] = BROWR_POS[1] ;
          UPDATE() ;     
      break;
      case 301:
          NEXT_POS[0] = ARML_POS[2] ;
          UPDATE() ;
          delay(100);
          NEXT_POS[0] = ARML_POS[0] ;
          UPDATE() ;
          delay(1000);
          NEXT_POS[0] = ARML_POS[1] ;
          NEXT_POS[1] = ARMR_POS[1] ;
          UPDATE() ;
          delay(100);
          NEXT_POS[0] = ARML_POS[0] ;
          NEXT_POS[1] = ARMR_POS[0] ;
          UPDATE() ;
      break;
      case 302:
          NEXT_POS[3] = EYES_POS[0] ;
          UPDATE() ;
          NEXT_POS[3] = EYES_POS[2] ;
          UPDATE() ;
          delay(500);
          NEXT_POS[3] = EYES_POS[1] ;
          UPDATE() ;
          NEXT_POS[3] = EYES_POS[2] ;
          UPDATE() ;
          NEXT_POS[3] = EYES_POS[1] ;
          UPDATE() ;
          delay(500);
      break;
      case 303:
          Emo = random(0, 8) ;
          Apply_Emotion(Emo) ;
          delay(1000);
      break;
      case 304:
          Moove_Flag(8) ;
          delay(1000);
      break;
      case 305:
          Apply_Emotion(0) ;
          delay(1000);
      break;
  } 
  RdNumber = 0 ;
}

//------------------------------------------------------------------Veille-----------------------------------------------------------------------------------
void Veille() {
  if ( 3 < Waited and Waited < 3.05){
    Serial.println("Veille 5");     
    RdNumber = 305 ;
    }
  else if ( 2.5 < Waited and Waited < 2.55 ){
    Serial.println("Veille 4");
    RdNumber = 304 ;
    }
  else if ( 2 < Waited and Waited < 2.05 ){
    Serial.println("Veille 3");
    RdNumber = 303 ;
    }
  else if ( 1.5 < Waited and Waited < 1.55 ){
    Serial.println("Veille 2");
    RdNumber = 302 ;
    }
  else if ( 1 < Waited and Waited < 1.05 ){
    Serial.println("Veille 1");     
    RdNumber = 301 ;
    }
  else if ( 0.5 < Waited and Waited < 0.55 ){
    Serial.println("Veille 0");
    RdNumber = 300 ;
    }
}

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


//----------------------------------------------Humeur--------------------------------------------------------

void Emotion(int n){
  //Basic
  if( n == 0 ){
        NEXT_POS[0] = ARML_POS[0] ;
        NEXT_POS[1] = ARMR_POS[0] ;
        NEXT_POS[2] = MOUTH_POS[2] ;
        NEXT_POS[3] = EYES_POS[1] ;
        NEXT_POS[4] = BROWL_POS[1] ;
        NEXT_POS[5] = BROWR_POS[1] ;
        NEXT_POS[6] = FLAG_POS[0] ;
 
        Next_Eye_Color = 200 ;
        Next_Body_Color = 190 ;
          
        UPDATE() ;
        delay(100);
  }
  //Sick
  if( n == 1 ){
        NEXT_POS[0] = ARML_POS[0] ;
        NEXT_POS[1] = ARMR_POS[0] ;
        NEXT_POS[2] = MOUTH_POS[1] ;
        NEXT_POS[3] = EYES_POS[1] ;
        NEXT_POS[4] = BROWL_POS[1] ;
        NEXT_POS[5] = BROWR_POS[1] ;
        NEXT_POS[6] = FLAG_POS[0] ;
 
        Next_Eye_Color = 200 ;
        Next_Body_Color = 5 ;
          
        UPDATE() ;
        delay(100);
  }
  //Sad
  if( n == 2 ){
        NEXT_POS[0] = ARML_POS[0] ;
        NEXT_POS[1] = ARMR_POS[0] ;
        NEXT_POS[2] = MOUTH_POS[0] ;
        NEXT_POS[3] = EYES_POS[1] ;
        NEXT_POS[4] = BROWL_POS[2] ;
        NEXT_POS[5] = BROWR_POS[2] ;
        NEXT_POS[6] = FLAG_POS[0] ;
 
        Next_Eye_Color = 200 ;
        Next_Body_Color = 170 ;
          
        UPDATE() ;
        delay(100);
  }
  //Angry
  if( n == 3 ){
        NEXT_POS[0] = ARML_POS[0] ;
        NEXT_POS[1] = ARMR_POS[0] ;
        NEXT_POS[2] = MOUTH_POS[0] ;
        NEXT_POS[3] = EYES_POS[1] ;
        NEXT_POS[4] = BROWL_POS[0] ;
        NEXT_POS[5] = BROWR_POS[0] ;
        NEXT_POS[6] = FLAG_POS[0] ;
 
        Next_Eye_Color = 100 ;
        Next_Body_Color = 100 ;
          
        UPDATE() ;
        delay(100);
  }
  //Diabolic
  if( n == 4 ){
        NEXT_POS[0] = ARML_POS[0] + 20 ;
        NEXT_POS[1] = ARMR_POS[0] - 20 ;
        NEXT_POS[2] = MOUTH_POS[2] ;
        NEXT_POS[3] = EYES_POS[1] ;
        NEXT_POS[4] = BROWL_POS[0] ;
        NEXT_POS[5] = BROWR_POS[0] ;
        NEXT_POS[6] = FLAG_POS[0] ;
 
        Next_Eye_Color = 90 ;
        Next_Body_Color = 145 ;
          
        UPDATE() ;
        delay(100);
  }
  //love 
  if( n == 5 ){
        NEXT_POS[0] = ARML_POS[0] + 20 ;
        NEXT_POS[1] = ARMR_POS[0] - 20 ;
        NEXT_POS[2] = MOUTH_POS[2] ;
        NEXT_POS[3] = EYES_POS[1] ;
        NEXT_POS[4] = BROWL_POS[2] ;
        NEXT_POS[5] = BROWR_POS[2] ;
        NEXT_POS[6] = FLAG_POS[0] ;
 
        Next_Eye_Color = 110 ;
        Next_Body_Color = 110 ;
        UPDATE() ;
        
        delay(100);
  }
  //Pumpkin
  if( n == 6 ){
        NEXT_POS[0] = ARML_POS[0] ;
        NEXT_POS[1] = ARMR_POS[0] ;
        NEXT_POS[2] = MOUTH_POS[1] ;
        NEXT_POS[3] = EYES_POS[2] ;
        NEXT_POS[4] = BROWL_POS[1] ;
        NEXT_POS[5] = BROWR_POS[1] ;
        NEXT_POS[6] = FLAG_POS[2] ;
 
        Next_Eye_Color = 50 ;
        Next_Body_Color = 87 ;
          
        UPDATE() ;
        delay(100);
  }
  //cold
  if( n == 7 ){
        NEXT_POS[0] = ARML_POS[0] ;
        NEXT_POS[1] = ARMR_POS[0] ;
        NEXT_POS[2] = MOUTH_POS[1] ;
        NEXT_POS[3] = EYES_POS[1] ;
        NEXT_POS[4] = BROWL_POS[0] ;
        NEXT_POS[5] = BROWR_POS[0] ;
        NEXT_POS[6] = FLAG_POS[0] ;
 
        Next_Eye_Color = 180 ;
        Next_Body_Color = 165 ;
        UPDATE() ;
        
        delay(100);
  }
  //colorfull
  if( n == 8 ){
        Current_Body_Color = (Current_Body_Color%255) + 1 ;
        Apply_Color_Body(Current_Body_Color) ;
  }
}
