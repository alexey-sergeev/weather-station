//
#include <GyverSegment.h>
// Disp595_4 disp(PIN_DIO, PIN_CLK, PIN_LAT);

// #define DIO_PIN 12
// #define CLK_PIN 13

// Disp1637_4 disp(DIO_PIN, CLK_PIN);

#ifdef DISPLAY_COLON 

  Disp1637Colon disp(PIN_DIO, PIN_CLK);

#else

  Disp1637_4 disp(PIN_DIO, PIN_CLK);

#endif



class Display {
  
  public:

    void init() {

      // disp.brightness(4);

      disp.setCursor(0);
      // disp.print("hello");
      disp.print("Hi");
      disp.update();

    }  



    void run() {

      #ifdef CONF_2
        showFor2();
      #endif

      #ifdef CONF_7
        showFor7();
      #endif

      #ifdef CONF_10
        showFor10();
      #endif

      #ifdef CONF_13
        showFor13();
      #endif

      #ifdef CONF_14
        showFor13();
      #endif

      #ifdef CONF_15
        showFor13();
      #endif

    }  



    void print(String s, bool right = false) {

      static uint32_t timer;
      static String sOld;
      sOld.reserve(8);
      uint8_t o = 0;

      if ( sOld != s ) {

        disp.setCursor(0);
        
        if ( right ) disp.setCursorEnd();
        disp.printRight(right);
        
        disp.print(s);
        if ( !right ) disp.print("   ");
        disp.update();

        disp.printRight(false);

        sOld = s;

      }

      // if ( millis() - timer > 3000 ) {
    
      //   timer = millis();
      //   pln(s);

      // }



    }  




    void power(bool f) {

      disp.power(f);

    }



    void showFor2() {

      static String s;
      s.reserve(DISPLAY_LEN);
      static uint8_t n = 0;
      static uint32_t timer = 0;
      static uint32_t timer2 = 0;
      static bool f = true;

      if ( millis() - timer > SHOW_INTERVAL ) {
     
        timer = millis();
        if ( ++n >= SHOW_LEN ) n = 0;  
        f = true;
        disp.clear();

      }
       
      switch ( n ) {

        case 0:

          if ( millis() - timer2 > 60 ) {
          
            timer2 = millis();

            static uint8_t ns = 0;
            static uint8_t nc = 0;
            static uint8_t q[] = {0b00000111, 0b00001110, 0b00011100, 0b00111000, 0b00110001, 0b00100011, 0b00111111};

            disp.setCursor(ns);
            disp.writeByte(q[nc]);
            disp.update();

            if ( ++nc >= 7 ) {
             
              nc = 0;
              ++ns; 

            }

            if ( ns == 5 ) {

              ns = 0;
              n = 1;
              timer = millis();

            }

          }

          // s = (String) 1111;
          // print(s);

        break;

        case 1:

          if ( f ) {

            f = false;
            s = _parse("t2");
            if ( s.length() > 0 ) s[s.length()-1] = '*';
            else s = "----";
            disp.clear();
            print(s);
            
          }

        break;

        case 2:

          if ( f ) {

            f = false;
            s = _parse("bt4");
            if ( s.length() > 0 ) s[s.length()-1] = '*';
            else s = "----";
            disp.clear();
            print(s);

          }

        break;
      
      }


    }



    void showFor7() {

      // p("@");
      // static String s;
      // s.reserve(DISPLAY_LEN);

      // disp.clear();

      // s = _parse("t7");
      // if ( s.length() > 0 ) s[s.length()-1] = '*';
      // else s = "----";
      // disp.clear();
      // print(s);
            


      static String s;
      s.reserve(DISPLAY_LEN);
      static uint8_t n = 1;
      static uint32_t timer = 0;
      static uint32_t timer2 = 0;
      static bool f = true;

      if ( millis() - timer > SHOW_INTERVAL ) {
     
        timer = millis();
        if ( ++n >= SHOW_LEN ) n = 1;  
        f = true;
        disp.clear();

      }
       
      switch ( n ) {

        // case 0:

        //   if ( millis() - timer2 > 60 ) {
          
        //     timer2 = millis();

        //     static uint8_t ns = 0;
        //     static uint8_t nc = 0;
        //     static uint8_t q[] = {0b00000111, 0b00001110, 0b00011100, 0b00111000, 0b00110001, 0b00100011, 0b00111111};

        //     disp.setCursor(ns);
        //     disp.writeByte(q[nc]);
        //     disp.update();

        //     if ( ++nc >= 7 ) {
             
        //       nc = 0;
        //       ++ns; 

        //     }

        //     if ( ns == 5 ) {

        //       ns = 0;
        //       n = 1;
        //       timer = millis();

        //     }

        //   }

        //   // s = (String) 1111;
        //   // print(s);

        // break;

        case 1:

          if ( f ) {

            f = false;
            s = _parse("t7");
            if ( s.length() > 0 ) s[s.length()-1] = '*';
            else s = "----";
            disp.clear();
            print(s);
            
          }

        break;

        case 2:

          if ( f ) {

            f = false;
            s = _parse("h7");
            if ( s.length() > 0 ) s[s.length()-1] = '_';
            else s = "----";
            disp.clear();
            print(s);

          }

        break;
      
      }


    }
    
    
    
    
    void showFor10() {

      static String s;
      s.reserve(DISPLAY_LEN);
      static uint8_t n = 1;
      static uint32_t timer = 0;
      static uint32_t timer2 = 0;
      static bool f = true;

      if ( millis() - timer > SHOW_INTERVAL ) {
     
        timer = millis();
        if ( ++n >= SHOW_LEN ) n = 1;  
        f = true;
        disp.clear();

      }
       
      switch ( n ) {

        // case 0:

        //   if ( millis() - timer2 > 60 ) {
          
        //     timer2 = millis();

        //     static uint8_t ns = 0;
        //     static uint8_t nc = 0;
        //     static uint8_t q[] = {0b00000111, 0b00001110, 0b00011100, 0b00111000, 0b00110001, 0b00100011, 0b00111111};

        //     disp.setCursor(ns);
        //     disp.writeByte(q[nc]);
        //     disp.update();

        //     if ( ++nc >= 7 ) {
             
        //       nc = 0;
        //       ++ns; 

        //     }

        //     if ( ns == 5 ) {

        //       ns = 0;
        //       n = 1;
        //       timer = millis();

        //     }

        //   }

        //   // s = (String) 1111;
        //   // print(s);

        // break;

        case 1:

          if ( f ) {

            f = false;
            s = _parse(PREFIX_TEMPERATURE_B);
            if ( s.length() > 0 ) s[s.length()-1] = '*';
            else s = "----";
            disp.clear();
            print(s);
            
          }

        break;

        // case 2:

        //   if ( f ) {

        //     f = false;
        //     s = _parse("h7");
        //     if ( s.length() > 0 ) s[s.length()-1] = '_';
        //     else s = "----";
        //     disp.clear();
        //     print(s);

        //   }

        // break;
      
      }


    }


    
    
       
    void showFor13() {

      static String s;
      s.reserve(DISPLAY_LEN);
      static uint8_t n = 2;
      static uint32_t timer = 0;
      static uint32_t timer2 = 0;
      static bool f = true;

      if ( millis() - timer > SHOW_INTERVAL ) {
     
        timer = millis();
        if ( ++n >= SHOW_LEN ) n = 0;  
        f = true;
        // disp.clear();

      }
       
      switch ( n ) {

        case 0:

          if ( f ) {

            f = false;
            s = _parse(PREFIX_TEMPERATURE_B, 3);
            if ( s.length() == 0 ) s = "----";
            disp.colon(f);
            disp.clear();
            print(s+"*", true);
            
          }

        break;
        
        case 1:

          if ( f ) {

            f = false;
            s = _parse(PREFIX_TEMPERATURE_S, 3);
            if ( s.length() == 0 ) s = "----";
            disp.colon(f);
            disp.clear();
            print(s+"*", true);
            
          }

        break;

        default:
        
          if ( f ) {

            f = false;
            s = _getTimeNow();
            // p(s);
            // disp.clear();
            print(s);
          
          }
          
          disp.colon( ( (int) ( millis() / 500 ) % 2 ) ? true : false );

        break;

      }


    }



  private:

    String _parse(const char* k, uint8_t nn = 0 ) {

      String s = sensors.creatingDataRow(N_NOW);

      char c[s.length() + 1];

      for (int i = 0; i < s.length(); i++) c[i] = s.charAt(i);
      c[s.length()] = 0;

      char* sp = c;
      char* p = NULL;
      while ((p = strtok_r(sp, " ", &sp)) != NULL) {

        char * key = strtok(p, ":");
        char * value = strtok(NULL, ":");

        if ( strcmp(key, k) == 0 ) {

          // pln(value); 
          // pln(strlen(value)); 
          value[strlen(value) - nn] = 0; 
          // pln(value); 

          return (String) value;

        }
      }

      return "";

    }


    String _getTimeNow() {

      String s = "";
      time_t now = time(NULL);

      // uint32_t t = atol(c) + GMT;
      uint32_t t = now + GMT;

      uint8_t tt = 0; 
      
      tt = ( t % 86400 ) / 3600;
      if ( tt < 10 ) s += "0";
      s += (String) tt;
      // s += ":";

      tt = ( t % 3600 ) / 60;
      if ( tt < 10 ) s += "0";
      s += (String) tt;
      // s += ":";

      // tt = t % 60;
      // if ( tt < 10 ) s += "0";
      // s += (String) tt;

      return s;
      
    }


};

Display display;
