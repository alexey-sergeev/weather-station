//

#include <GyverOLED.h>
GyverOLED<SSD1306_128x64, OLED_NO_BUFFER> display;
// GyverOLED<SSD1306_128x64, OLED_BUFFER> display;

#include <pcf8574.h>
PCF8574 ex1(ADRESS_BUTTON_OLED);


#define ITEMS_OF_PAGE 4
#define OLED_COL 11


const static uint8_t bitmapLogo[][32] PROGMEM = {

{0b00000000, 
0b00000000, 
0b00000000, 
0b00000000, 
0b00000000, 
0b00000000, 
0b00000000, 
0b00000000, 
0b00000000, 
0b00000000, 
0b00000000, 
0b00000000, 
0b00000000, 
0b00000000, 
0b00000000, 
0b00000000, 
0b00000000, 
0b00000000, 
0b00000000, 
0b00111000, 
0b01111000, 
0b11111000, 
0b11110000, 
0b11110000, 
0b11100000, 
0b11100000, 
0b11000000, 
0b00000000, 
0b00000000, 
0b00000000, 
0b00000000, 
0b00000000}, 

{0b00000000, 
0b00000000, 
0b00000000, 
0b00000000, 
0b00000000, 
0b00000000, 
0b11111110, 
0b11111111, 
0b11111111, 
0b11111110, 
0b00000000, 
0b00000000, 
0b00000000, 
0b00000000, 
0b00000000, 
0b00000000, 
0b00000000, 
0b10000000, 
0b11000000, 
0b11000000, 
0b11100000, 
0b11100000, 
0b11110000, 
0b11110001, 
0b11110001, 
0b01111000, 
0b01111000, 
0b01111000, 
0b01110000, 
0b00000000, 
0b00000000, 
0b00000000}, 

{0b00000000, 
0b00000000, 
0b00000000, 
0b00000000, 
0b00000000, 
0b00000000, 
0b00000000, 
0b00000000, 
0b00000000, 
0b00000000, 
0b00000000, 
0b00000000, 
0b00000000, 
0b00000000, 
0b00000000, 
0b00000000, 
0b00000000, 
0b00000000, 
0b00000000, 
0b00000000, 
0b00000000, 
0b11000000, 
0b11100000, 
0b11100000, 
0b11110000, 
0b11110000, 
0b11111000, 
0b01111000, 
0b00111000, 
0b00000000, 
0b00000000, 
0b00000000}, 


{0b00000000, 
0b00000000, 
0b00000000, 
0b00000000, 
0b00000000, 
0b00000000, 
0b00000000, 
0b00000001, 
0b00000001, 
0b00000001, 
0b00000001, 
0b00000000, 
0b00000000, 
0b10000000, 
0b11110000, 
0b11111100, 
0b00000000, 
0b00000000, 
0b00000000, 
0b00000000, 
0b00000000, 
0b00000000, 
0b00000000, 
0b10000000, 
0b10000000, 
0b10000000, 
0b10000000, 
0b00000000, 
0b00000000, 
0b00000111, 
0b00111111, 
0b11111111}, 

{0b11111111, 
0b01111111, 
0b00011111, 
0b00000111, 
0b11000011, 
0b11100001, 
0b11110000, 
0b11110000, 
0b11111000, 
0b01111000, 
0b01111000, 
0b01111000, 
0b01111000, 
0b11110000, 
0b11110000, 
0b11100000, 
0b11111111, 
0b11111000, 
0b11100000, 
0b10000000, 
0b10001111, 
0b00001111, 
0b00001111, 
0b00000011, 
0b00000000, 
0b00000000, 
0b00000000, 
0b00000000, 
0b00000000, 
0b10000000, 
0b11100011, 
0b11111111}, 

{0b11000000, 
0b10000000, 
0b00000000, 
0b00000000, 
0b00000000, 
0b00000001, 
0b00000001, 
0b00000001, 
0b00000001, 
0b00000000, 
0b00000000, 
0b00000000, 
0b00000000, 
0b00000000, 
0b00000000, 
0b00000000, 
0b11111111, 
0b11111111, 
0b00011100, 
0b00000000, 
0b00000000, 
0b10000000, 
0b10000000, 
0b10000000, 
0b10000000, 
0b00000000, 
0b00000000, 
0b00000000, 
0b00000000, 
0b00000000, 
0b00000000, 
0b00000000}, 

{0b00000000, 
0b00000000, 
0b00000000, 
0b00011100, 
0b00011110, 
0b00011111, 
0b00001111, 
0b00001111, 
0b00000111, 
0b00000111, 
0b00000011, 
0b00000000, 
0b00000000, 
0b00000000, 
0b00000000, 
0b00000000, 
0b00000000, 
0b00000000, 
0b00000000, 
0b00000000, 
0b00000000, 
0b00000000, 
0b00000000, 
0b00000000, 
0b00000000, 
0b00000000, 
0b00000000, 
0b00000000, 
0b00000000, 
0b00000000, 
0b00000000, 
0b00000000}, 

{0b00000001, 
0b00000011, 
0b00000111, 
0b00000111, 
0b00001111, 
0b00001111, 
0b00011111, 
0b10011110, 
0b10011110, 
0b00011110, 
0b00011110, 
0b00001111, 
0b00001111, 
0b00001111, 
0b00000111, 
0b00000011, 
0b00000000, 
0b00000000, 
0b00000000, 
0b00000000, 
0b00000000, 
0b00000000, 
0b01111111, 
0b11111111, 
0b11111111, 
0b01111111, 
0b00000000, 
0b00000000, 
0b00000000, 
0b00000000, 
0b00000000, 
0b00000000},

{0b00000001, 
0b00000000, 
0b00000000, 
0b00000000, 
0b00000000, 
0b00000011, 
0b00000111, 
0b00000111, 
0b00001111, 
0b00001111, 
0b00011111, 
0b00011110, 
0b00011100, 
0b00000000, 
0b00000000, 
0b00000000, 
0b00000000, 
0b00000000, 
0b00000000, 
0b00000000, 
0b00000000, 
0b00000000, 
0b00000000, 
0b00000000, 
0b00000000, 
0b00000000, 
0b00000000, 
0b00000000, 
0b00000000, 
0b00000000, 
0b00000000, 
0b00000000}

};


class OLED_wrapper {
  
  public:

    void init() {

      // pinMode(PIN_BUTTON_OLED, INPUT_PULLUP);
      pinMode(ex1, 7, INPUT_PULLUP);

      display.init(); 
      display.clear(); 

      display.drawBitmap(40, 8, bitmapLogo[0], 16, 16);
      display.drawBitmap(40, 24, bitmapLogo[3], 16, 16);
      display.drawBitmap(40, 40, bitmapLogo[6], 16, 16);
      display.drawBitmap(56, 8, bitmapLogo[1], 16, 16);
      display.drawBitmap(56, 24, bitmapLogo[4], 16, 16);
      display.drawBitmap(56, 40, bitmapLogo[7], 16, 16);
      display.drawBitmap(72, 8, bitmapLogo[2], 16, 16);
      display.drawBitmap(72, 24, bitmapLogo[5], 16, 16);
      display.drawBitmap(72, 40, bitmapLogo[8], 16, 16);

      delay(500);

    }  



    void run() {

      static uint32_t timerButton;
      bool flag = false;

      // if ( !digitalRead(PIN_BUTTON_OLED) && millis() - timerButton > 500 ) { 
      if ( !digitalRead(ex1, 7) && millis() - timerButton > 500 ) { 
            
        timerButton = millis(); 
        flag = true;

        _nPage = ( _nPage + 1 ) % _nMaxPage;

        display.setPower( (_nPage == 0) ? false : true );

      }

      static uint32_t timerOled;
      
      if ( millis() - timerOled > 1000 || flag ) { 
            
        timerOled = millis();

        _print();

      } 

    }  



  private:


    void _print() {

      // String s = sensors.creatingDataRow(N_NOW);
      // String s = "";
      // String s = sensors.creatingDataRow(N_DAY);

      static String s;
      s.reserve(DATA_BUF_COUNT);
      s = sensors.creatingDataRow(N_NOW);

      static char data[DATA_BUF_COUNT];
      // static char data[DATA_OLED_COUNT];

      for ( int8_t i = 0; i < s.length(); i++ ) data[i] = s.charAt(i);
      data[s.length()] = 0;


      display.home();
      display.setScale(2);  
        
      for ( int8_t i = 0; i < ITEMS_OF_PAGE; i++ ) {

        uint8_t n = ( _nPage - 1 ) * ITEMS_OF_PAGE + i; 

        display.setCursor(0, i * 2);  

        if ( n < _lenItems ) {

          char si[strlen(items[n]) + 3];
          si[0] = ' ';
          strcpy(si + 1, items[n]);
          si[strlen(items[n]) + 1] = ':';
          si[strlen(items[n]) + 2] = 0;

          char *sc;
          sc = strstr(data, si);

          if ( sc != NULL) {

            char sw[strlen(sc)];

            for ( int8_t w = 1; w < strlen(sc); w++ ) {
              if ( sc[w] == ' ' ) break;
              sw[w - 1] = sc[w];
              sw[w] = 0;
            }

            if ( strchr(sw, ':') != 0 ) {

              char * key = strtok(sw, ":");
              char * value = strtok(NULL, ":");
 
              _printItem(key, value);

            }

          } else {

            char st[OLED_COL * 2];
            strcpy(st, items[n]);

            if ( st[0] == '@' ) {

              strcpy(st - 1, st);
              display.print(st);

            } else if ( strcmp(items[n], "time") == 0 ) {

              display.print(_getTime());

            } else {

              display.print(items[n]);
              display.print(" -");

            }


          }



        } 

        display.print("         ");
        
      }

    }  




    void _printItem(char * key, char * value) {

      const char* keyP0[] = { "s", "u", "r", "g", "l", "ai", "at", "ac" };
      const char* keyP1[] = { "t", "h", "p", "bt" };
      const char* keyP2[] = { "va", "vb" };

      for ( int8_t i = 0; i < sizeof(keyP0)/sizeof(keyP0[0]); i++ ) 
        if ( strncmp(keyP0[i], key, strlen(keyP0[i])) == 0 ) value[strlen(value) - 3] = 0;

      for ( int8_t i = 0; i < sizeof(keyP1)/sizeof(keyP1[0]); i++ ) 
        if ( strncmp(keyP1[i], key, strlen(keyP1[i])) == 0 ) value[strlen(value) - 1] = 0;

      display.print(value);

      String m = "";
      
      if ( strncmp("t", key, 1) == 0 ) m = "C";
      if ( strncmp("p", key, 1) == 0 ) m = "atm";
      if ( strncmp("h", key, 1) == 0 ) m = "%";

      // if ( strncmp("ai", key, 1) == 0 ) m = "";
      if ( strncmp("at", key, 1) == 0 ) m = "ppb";
      if ( strncmp("ac", key, 1) == 0 ) m = "ppm";

      if ( strncmp("l", key, 1) == 0 ) m = "lux";

      display.print(" ");
      display.print(m);
      

    }

    String _getTime() {

      String s = "00:00:00";

        #ifdef SET_WIFI

        time_t t = time(NULL) + GMT;

        uint8_t tt = 0; 
        s = "";

        tt = ( t % 86400 ) / 3600;
        if ( tt < 10 ) s += "0";
        s += (String) tt;
        s += ":";

        tt = ( t % 3600 ) / 60;
        if ( tt < 10 ) s += "0";
        s += (String) tt;
        s += ":";

        tt = t % 60;
        if ( tt < 10 ) s += "0";
        s += (String) tt;

      #endif 

      return s;
      
    }


    uint8_t _lenItems = sizeof(items) / sizeof(items[0]);

    uint8_t _nPage = 1;
    uint8_t _nMaxPage = 1 + (int) ceil( (float) _lenItems / ITEMS_OF_PAGE ); 

};

OLED_wrapper oled;
