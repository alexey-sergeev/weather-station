//

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 0  // GPIO0
Adafruit_SSD1306 display(OLED_RESET);


class OLED_M_wrapper {
  
  public:

    void init() {

      pinMode(PIN_BUTTON_OLED, INPUT_PULLUP);

      display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
      display.display();
      display.clearDisplay();    

      delay(1000);

    }  



    void run() {

      print();

      static uint32_t timerButton;
      
      if ( !digitalRead(PIN_BUTTON_OLED) && millis() - timerButton > 500 ) { 
            
        timerButton = millis(); 
        flagOnOff = ! flagOnOff;

        display.ssd1306_command( ( flagOnOff ) ? SSD1306_DISPLAYON : SSD1306_DISPLAYOFF );

      } 


    }  




    void print() {

      display.clearDisplay();  

      display.setTextSize(2);
      display.setTextColor(WHITE);

      display.setCursor(0,0);
      display.println(_getTime());
      
      #ifdef SET_TEMPERATURE
        display.setCursor(0,16);

        display.print( String (sTemperature.get(N_NOW), 1) );  
        // display.print( 'C' );

        display.setCursor(display.getCursorX(), display.getCursorY() - 4);
        display.print( (char) 9 );
      #endif

      #ifdef SET_TEMPERATURE_B
        display.setCursor(0,32);
        display.print( String (sTemperatureB.get(N_NOW), 1) );

        // display.setCursor(48,28);
        display.setCursor(display.getCursorX(), display.getCursorY() - 4);
        display.print( (char) 9 );
        // display.print( 'C' );
      #endif

      display.display();

    }  



 protected:

    String _getTime() {

      String s = "";
      time_t t = time(NULL) + GMT;

      // uint8_t ss = t % 60;
      // uint8_t mm = ( t % 3600 ) / 60;
      // uint8_t hh = ( t % 86400 ) / 3600;
      
      uint8_t tt = 0; 
      
      tt = ( t % 86400 ) / 3600;
      if ( tt < 10 ) s += "0";
      s += (String) tt;
      // s += ":";
      s += ( t % 2 == 0 ) ? ":" : " ";

      tt = ( t % 3600 ) / 60;
      if ( tt < 10 ) s += "0";
      s += (String) tt;
      // s += ":";

      // tt = t % 60;
      // if ( tt < 10 ) s += "0";
      // s += (String) tt;

      return s;
      
    }

    bool flagOnOff = true;

};

OLED_M_wrapper oledM;
