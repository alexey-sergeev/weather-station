// Класс тревоги

// #define START_DELAY_BUZZER 30
#define START_DELAY_BUZZER 30000
#define DISPLAY_LEN 10
#define MENU_LEN 4

#define MENU_TEMP 0
#define MENU_POWER 1
#define MENU_WIFI 2
#define MENU_VOTAGE 3



#include <Ticker.h>
Ticker flipper;



bool mayOnBuzzer = true;
bool statusBuzzer = false;
uint8_t nMenu = 0;


class Alarm { 
  public: 

    void init() {

      pinMode(PIN_LED_ALARM, OUTPUT);
      
      pinMode(PIN_BUZZER, OUTPUT);
      digitalWrite(PIN_BUZZER, HIGH);

      button.attachCallback(changeMenu);

    }



    void run() {

      static bool f = true;

      _display();
      _alarm();
     
      if ( sPower.get() == 0 && f ) {
        
        _pTime = millis();
        f = false;

      }

      if ( sPower.get() != 0 ) f = true;

    }



 
    static void offBuzzer() {

      statusBuzzer = false;
      // mayOnBuzzer = false;
      digitalWrite(PIN_BUZZER, HIGH);

    }


 
    static bool controlBuzzer() {

      if ( statusBuzzer ) {

        offBuzzer();
        mayOnBuzzer = false;
        
        return false;

      } else {

        if ( _isAlarm() ) statusBuzzer = true;
        mayOnBuzzer = true;

        // digitalWrite(PIN_LED_ALARM, HIGH);
        // digitalWrite(PIN_BUZZER, LOW);

        // delay(50);

        // digitalWrite(PIN_BUZZER, HIGH);
        // digitalWrite(PIN_LED_ALARM, LOW);

        return true;

      }
      
    }



    static void changeMenu() {

      if ( statusBuzzer ) {
        
        offBuzzer();
        mayOnBuzzer = false;
       
      } else {
        
        if ( ++nMenu >= MENU_LEN ) nMenu = 0;
      
      }

    }


 
    bool isBeepBuzzer() {

      return mayOnBuzzer && sPower.get(N_NOW) == 0;

    }





  private:


    static bool _isAlarm() {
      
      return sPower.get() == 0 || sTemperature.get() < TEMPERATURE_MIN || sTemperature.get() > TEMPERATURE_MAX;

    }
    

    
    void _alarm() {

      static bool f = true;
      static bool ff = true;

      if ( _isAlarm() ) {

        if ( f ) {

          flipper.attach(0.08, _onLED);
          statusBuzzer = true;

        }

        f = false;

        if ( _tempMinMax() != 0 && ff ) nMenu = 0;
        if ( sPower.get() == 0 && ff ) nMenu = 1;

        ff = false;

      } else {

        if ( !f ) {
          
          _offLED();
          flipper.detach();

        }
        
        f = true;
        ff = true;

        if ( sTemperature.get() >= TEMPERATURE_MIN + 1 && sTemperature.get() <= TEMPERATURE_MAX - 1 ) mayOnBuzzer = true;

      }

    }





    void _display() {

      #ifdef SET_DISPLAY 
      
        static String s;
        s.reserve(DISPLAY_LEN);
        char tc[] = "000.0";

        static uint8_t nMenuOld;
        if ( nMenuOld != nMenu ) disp.power(true);
        nMenuOld = nMenu;

        switch ( nMenu ) {

          case MENU_TEMP:

            dtostrf(sTemperature.get(), 1, 1, tc);
            s = (String) tc + "*";
            display.print(s);

          break;

          case MENU_POWER:

            s = ( sPower.get() == 0 ) ? "W." + (String) ( ( millis() - _pTime ) / 60000 ) : "WON"; 
            display.print(s);

          break;

          case MENU_WIFI:

            s = ( WiFi.status() != WL_CONNECTED ) ? "----" : WiFi.SSID(); 
            display.print(s);

          break;

          case MENU_VOTAGE:

            if ( sVotage0.get() < BATTERY_CHARGED_0 ) s = "____";
            else if ( sVotage0.get() < BATTERY_CHARGED_25 ) s = "W___";
            else if ( sVotage0.get() < BATTERY_CHARGED_50 ) s = "WW__";
            else if ( sVotage0.get() < BATTERY_CHARGED_75 ) s = "WWW_";
            else s = "WWWW";

            display.print(s);

          break;

          // default:
          // break;

        }


      #endif

    }





    static void _onLED() {

      static uint8_t n = 0;
      uint8_t arr[96];
      char c[97];

      uint8_t mark = _tempMinMax();

      if ( sPower.get() == 0 ) 
        strcpy(c, "11111111........11111111........11111111........11111111........11111111........11111111........");
      else if ( mark == 5 )
        strcpy(c, "11111...........11111...........11111...........11111...........11111...........11111...........");
      else if ( mark == 4 )
        strcpy(c, "1111....................1111....................1111....................1111....................");
      else if ( mark == 3 )
        strcpy(c, "111.............................111.............................111.............................");
      else if ( mark == 2 )
        strcpy(c, "11..............................................11..............................................");
      else if ( mark == 1 )
        strcpy(c, "1...............................................................................................");
      // else if ( WiFi.status() != WL_CONNECTED )
      //   strcpy(c, "11111111........11111111........11111111........11111111........11111111........11111111........");
      // else if ( sVotage0.get() < BATTERY_CHARGED_0 )
      //   strcpy(c, "11111111........11111111........11111111........11111111........11111111........11111111........");
      // c[97] = 0;


      for ( uint8_t i = 0; i < 96; i++ ) arr[i] = ( c[i] == '1' ) ? HIGH : LOW;
          
      digitalWrite(PIN_LED_ALARM, arr[n]);
      if ( millis() > START_DELAY_BUZZER && mayOnBuzzer ) digitalWrite(PIN_BUZZER, !arr[n]);

      #ifdef SET_DISPLAY 

        uint8_t f = 0;

        // if ( _tempMinMax() != 0 && nMenu == MENU_TEMP ) f = 2;
        // else if ( sPower.get() == 0 && nMenu == MENU_POWER ) f = 1;
        // // else if (  WiFi.status() != WL_CONNECTED && nMenu == MENU_WIFI ) f = 1;
        // // else if ( sVotage0.get() < BATTERY_CHARGED_0 && nMenu == MENU_VOTAGE ) f = 1; 
        
        // if ( f == 1 ) display.power( ( arr[n] == HIGH ) ? true : false ); 
        // if ( f == 2 ) display.power( ( arr[n] == HIGH ) ? false : true ); 

        display.power( ( arr[n] == HIGH ) ? false : true );

      #endif
      
      if ( ++n >= 96 ) n = 0;

    }
    

        

    static uint8_t _tempMinMax() {

      uint8_t mark;
      float t = sTemperature.get();

      if ( t < TEMPERATURE_MIN - 10 || t > TEMPERATURE_MAX + 10 ) mark = 4;
      else if ( t < TEMPERATURE_MIN - 7 || t > TEMPERATURE_MAX + 7 ) mark = 3;
      else if ( t < TEMPERATURE_MIN - 3 || t > TEMPERATURE_MAX + 3 ) mark = 2;
      else if ( t < TEMPERATURE_MIN || t > TEMPERATURE_MAX ) mark = 1;
      else mark = 0;

      return mark;

    }





    void _offLED() {

      digitalWrite(PIN_LED_ALARM, LOW);
      digitalWrite(PIN_BUZZER, HIGH);
      
      display.power(true); 

    }
    
    uint32_t _pTime = 0;

};

Alarm alarm;