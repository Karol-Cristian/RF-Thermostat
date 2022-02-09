
void updateSetpoint()
{
  u8g2.firstPage();
  do
  {
    u8g2.setFont(u8g2_font_logisoso50_tf);
    u8g2.setCursor(7, 57);
    u8g2.print(setTemp, 1); 
  } while ( u8g2.nextPage() );

}

void readTemp()
{
 double Vout, Rth, adc_value; 

  //adc_value = analogRead(A0);
  Vout = (adc_value * VCC) / adc_resolution;
  Rth = (VCC * R2 / Vout) - R2;

/*  Steinhart-Hart Thermistor Equation:
 *  Temperature in Kelvin = 1 / (A + B[ln(R)] + C[ln(R)]^3)
 *  where A = 0.001129148, B = 0.000234125 and C = 8.76741*10^-8  */
  temperature = (1 / (A + (B * log(Rth)) + (C * pow((log(Rth)),3))));   // Temperature in kelvin

  temperature = temperature ;//- 275.40;  // Temperature in degree celsius
  Serial.print("\n ");
  Serial.print(temperature);
 // Serial.println(" degree celsius");
}

  void updateRelayState()
{
    if(temperature>setTemp)
    {
      relayState = 0;
    } else {    
      relayState = 1;  
    }
}
