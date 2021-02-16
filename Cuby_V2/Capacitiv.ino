//-------------------------------------------------------------Check Capacitiv sensor------------------------------------------------------------------------

void CheckCapaSensor(){
  Serial.println("Check capacitif : ") ;
  long Capa_Sensor_1 = cs_4_5.capacitiveSensor(30);
  long Capa_Sensor_2 = cs_4_8.capacitiveSensor(30);
  Serial.print(Capa_Sensor_1) ;
  Serial.print("   |   ") ;
  Serial.println(Capa_Sensor_2) ;
  delay(500);
}
