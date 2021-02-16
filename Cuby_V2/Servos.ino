//----------------------------------------------------------Initialisation Servos----------------------------------------------------------------------------
//Function to initialise cuby's servos controlled by PCA9685 I2C Module

void Init_Servo(){
     Serial.println("Init servo");
     for (int i = 0; i < nbPCAServo; i++) {
        pca.writeMicroseconds(i , AngleToImp(CURRENT_POS[i],i) ); delay(300);     //set servos to its init position
        pca.setPin(i,0,true);                                                      // deactivate pin i ie motor can be mooved
     }
}

//------------------------------------------------------------------------------------------------------------

void Moove_Flag(int n){
    UPDATE() ;
    for(int i = 0; i < n ; i++){
           NEXT_POS[6] = FLAG_POS[2] ;
           UPDATE() ;
           delay(100);
           NEXT_POS[6] = FLAG_POS[1] ;
           UPDATE() ;
           delay(100);
    }
    NEXT_POS[6] = FLAG_POS[0] ;
    UPDATE() ;
}

//Both arm reset button

void Reset_2_arm() {
    NEXT_POS[0] = ARML_POS[1] ;
    NEXT_POS[1] = ARMR_POS[1] ;
    UPDATE() ;
    delay(100);
    NEXT_POS[0] = ARML_POS[0] ;
    NEXT_POS[1] = ARMR_POS[0] ;
    UPDATE() ;
}

void Reset_Right(){
    NEXT_POS[1] = ARMR_POS[1] ;
    UPDATE() ;
    delay(100);
    NEXT_POS[1] = ARMR_POS[0] ;
    UPDATE() ;
}

void Reset_Left() {
    NEXT_POS[0] = ARML_POS[1] ;
    UPDATE() ;
    delay(100);
    NEXT_POS[0] = ARML_POS[0] ;
    UPDATE() ;
}
