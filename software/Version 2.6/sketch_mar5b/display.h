//
#include <GyverSegment.h>
// Disp595_4 disp(PIN_DIO, PIN_CLK, PIN_LAT);

// #define DIO_PIN 12
// #define CLK_PIN 13

// Disp1637_4 disp(DIO_PIN, CLK_PIN);
Disp1637_4 disp(PIN_DIO, PIN_CLK);


class Display {
  
  public:

    void init() {

      // disp.brightness(4);

      disp.setCursor(0);
      disp.print("hello");
      disp.update();

    }  



    void run() {


    }  



    void print(String s) {

      static uint32_t timer;
      static String sOld;
      sOld.reserve(8);
      
      if ( sOld != s ) {

        disp.setCursor(0);
        disp.print(s);
        disp.print("   ");
        disp.update();

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



  private:



};

Display display;
