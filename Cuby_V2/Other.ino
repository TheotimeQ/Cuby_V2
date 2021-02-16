//----------------------------------------------------------------Annexes------------------------------------------------------------------------------------
//Update time
void Time_Update(){
    Time = Min() ;
    Waited = Time - Time_0 ;
}

//----------------------------------------------------------Convert Angle->Impul-----------------------------------------------------------------------------
//Function angle to impulson usualy (0-180° to 400-2400us)

int AngleToImp(double x,int i){
     float imp=(x - MIN_ANG[i]) * (MAX_IMP[i]-MIN_IMP[i]) / (MAX_ANG[i]-MIN_ANG[i]) + MIN_IMP[i];
     imp=max(imp,MIN_IMP[i]);
     imp=min(imp,MAX_IMP[i]);
     return imp;
}

//-----------------------------------------------------------------Calcul time-------------------------------------------------------------------------------
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

//-------------------------------------------------------Check Noze + Generate Rd Number---------------------------------------------------------------------

void CheckNoze() {
  NozLeftState = digitalRead(NozLeftPin) ;
  NozRightState = digitalRead(NozRightPin)  ;
  
  if (NozLeftState == HIGH){
      RdNumber = 100 ; //random(100, 110) ;
      Time_0 = Min() ;
    }
  else if (NozRightState == HIGH){
      RdNumber = random(200, 205) ;
      Time_0 = Min() ;
    }
}

bool  CheckLeft() {
  NozLeftState = digitalRead(NozLeftPin) ;
  if (NozLeftState == HIGH){
        return true ;
    }
  return false ;
}

bool CheckRight() {
  NozRightState = digitalRead(NozRightPin)  ;
  if (NozRightState == HIGH){
        return true ;
    }
  return false ;
}
