// #define LED_NUM 256

#include <FastLED.h>

CRGB leds[LED_NUM];



class Strip {
  
  public:

    void init() {

      FastLED.addLeds<WS2812, PIN_STRIP, GRB>(leds, LED_NUM);
      FastLED.setBrightness(10);

    }  



    void run() {

      
    }  


    void point(uint32_t color = 0xFFFFFF, uint16_t n = 0) {

      // pln(color);

      leds[n] = color;
      FastLED.show();

    }  




    void point2(uint32_t color = 0, uint16_t n = 0) {

      static int dc = 0;
      static bool f = true;

      leds[n].setHue(dc + color);
      FastLED.show();

      // Serial.println(dc);
      
      if ( f ) {

        dc++;
        if ( dc >= 5 ) f = !f;

      } else {

        dc--;
        if ( dc <= 0 ) f = !f;

      }

    }  





  



  private:

   


};

Strip strip;
