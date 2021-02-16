//---------------------------------------------------------------UPDATE ALL----------------------------------------------------------------------------------

void UPDATE(){
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
        if(abs(Next_LED_Brightness - Current_LED_Brightness) > Led_Brightness_Speed){
                float inc = (Next_LED_Brightness - Current_LED_Brightness)/abs(Next_LED_Brightness - Current_LED_Brightness) * Led_Brightness_Speed ;
                Current_LED_Brightness = Current_LED_Brightness + inc ;
                LED_POWER(Current_LED_Brightness) ;
        }
        if(abs(Next_Eye_Color - Current_Eye_Color) > Led_Eye_Speed){
                float inc = (Next_Eye_Color - Current_Eye_Color)/abs(Next_Eye_Color - Current_Eye_Color) * Led_Eye_Speed ;
                Current_Eye_Color = Current_Eye_Color + inc ;
                LED_Color_Eyes(Current_Eye_Color) ;
        }
        if(abs(Next_Body_Color - Current_Body_Color) > Led_Body_Speed){
                float inc = (Next_Body_Color - Current_Body_Color)/abs(Next_Body_Color - Current_Body_Color) * Led_Body_Speed ;
                Current_Body_Color = Current_Body_Color + inc ;
                Apply_Color_Body(Current_Body_Color) ;
        }
    }
    for(int Servos = 0; Servos < nbPCAServo ; Servos++){
        pca.setPin(Servos,0,true);
    }
}

//---------------------------------------------------------------UPDATE ALL----------------------------------------------------------------------------------

void UPDATE_LED(){
    while(CheckPosition_LED() == false){
        if(abs(Next_LED_Brightness - Current_LED_Brightness) > Led_Brightness_Speed){
                float inc = (Next_LED_Brightness - Current_LED_Brightness)/abs(Next_LED_Brightness - Current_LED_Brightness) * Led_Brightness_Speed ;
                Current_LED_Brightness = Current_LED_Brightness + inc ;
                LED_POWER(Current_LED_Brightness) ;
        }
        if(abs(Next_Eye_Color - Current_Eye_Color) > Led_Eye_Speed){
                float inc = (Next_Eye_Color - Current_Eye_Color)/abs(Next_Eye_Color - Current_Eye_Color) * Led_Eye_Speed ;
                Current_Eye_Color = Current_Eye_Color + inc ;
                LED_Color_Eyes(Current_Eye_Color) ;
        }
        if(abs(Next_Body_Color - Current_Body_Color) > Led_Body_Speed){
                float inc = (Next_Body_Color - Current_Body_Color)/abs(Next_Body_Color - Current_Body_Color) * Led_Body_Speed ;
                Current_Body_Color = Current_Body_Color + inc ;
                Apply_Color_Body(Current_Body_Color) ;
        }
        delay(dt);
    }
}

//-------------------------------------------------------Check final state-----------------------------------------------------------------------------------

bool CheckPosition_All(){
    bool All_finish = true ;
    for(int Servos = 0; Servos < nbPCAServo ; Servos++){
        if(abs(NEXT_POS[Servos] - CURRENT_POS[Servos]) > SERVOS_SPEED[Servos]){
            All_finish = false ;
        }
    }
    if(abs(Next_LED_Brightness - Current_LED_Brightness) > Led_Brightness_Speed){
        All_finish = false ;
        }
    if(abs(Next_Eye_Color - Current_Eye_Color) > Led_Eye_Speed){
        All_finish = false ;
        }
    if(abs(Next_Body_Color - Current_Body_Color) > Led_Body_Speed){
        All_finish = false ;
        }
    return All_finish ;
}

bool CheckPosition_LED(){
    bool All_finish = true ;
    if(abs(Next_LED_Brightness - Current_LED_Brightness) > Led_Brightness_Speed){
        All_finish = false ;
        }
    if(abs(Next_Eye_Color - Current_Eye_Color) > Led_Eye_Speed){
        All_finish = false ;
        }
    if(abs(Next_Body_Color - Current_Body_Color) > Led_Body_Speed){
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

void Apply_Emotion(int Emotion_Number){
    Emo = Emotion_Number ;
    Emotion(Emo) ;
}
