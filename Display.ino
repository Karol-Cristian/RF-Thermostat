

void updateDisplay()
{
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_t0_11_tf);
   // u8g2.drawStr(2,8,"Humidity");
 
    u8g2.setCursor(41, 8);
//    u8g2.print();
   // u8g2.drawStr(114,8,"%");


    u8g2.setCursor(80, 63);
   // u8g2.print("1014hPa");  
    
    u8g2.setCursor(2, 20);
    u8g2.print("T");
    u8g2.setFont(u8g2_font_u8glib_4_tf);
    u8g2.print("SET ");
    
    u8g2.setFont(u8g2_font_logisoso16_tf);
    u8g2.setCursor(2, 44);
    u8g2.print(setTemp, 1);
    

  /*  u8g2.setFont(u8g2_font_t0_11_tf);
    u8g2.setCursor(44, 8);
    u8g2.print(lastMillis);
    u8g2.print(" ms");    // requires enableUTF8Print()*/
            
    u8g2.drawHLine(2, 10, 130);
    u8g2.drawHLine(2, 52, 130);
  //  u8g2.drawVLine(42, 11, 41);
  //  u8g2.drawVLine(2, 10, 130);
    u8g2.setFont(u8g2_font_logisoso24_tf);
    u8g2.setCursor(50, 44);
    u8g2.printf("%.1f",temperature);
    u8g2.print("°C");    // requires enableUTF8Print()
    
    u8g2.setFont(u8g2_font_t0_11_tf);
    u8g2.setCursor(2, 62);

//    u8g2.setFontMode(1);    // 0=solid, 1=transparent
    if(temperature>setTemp)
    {
      u8g2.drawBox(98,53,27,11);
      u8g2.setFontMode(1);
      u8g2.setDrawColor(2);
      u8g2.print("Heat  DORMITOR  Cool");
    } else {
      u8g2.drawBox(2,53,27,11);
      u8g2.setFontMode(1);
      u8g2.setDrawColor(2);
      u8g2.print("Heat  DORMITOR  Cool");    
    }
    
  } while ( u8g2.nextPage() );

}
