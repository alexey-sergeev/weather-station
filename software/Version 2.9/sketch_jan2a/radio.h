//

#define INTERVAL_SEND_ITEM_DATA 1000
#define TIME_LED_IS_ON 100

#include <Gyver433.h>

#ifdef SET_RADIO_TX

  Gyver433_TX<PIN_RADIO_TX> radioTx; 

#endif

#ifdef SET_RADIO_RX

  Gyver433_RX<PIN_RADIO_RX, DATA_BUF_COUNT_RADIO> radioRx;

#endif


class Radio {
  public:

    Radio() {

      _bufDataNow.clear();
      _bufDataDay.clear();
      _bufDataMonth.clear();
      _bufDataYear.clear();

    }


    #ifdef SET_RADIO_TX

      void init() {

        pln("RADIO opened - TX");

        pinMode(PIN_RADIO_LED, OUTPUT);
        digitalWrite(PIN_RADIO_LED, LOW);
        
      }

    #endif


    #ifdef SET_RADIO_RX

      void init() {

        pln("RADIO opened - RX");

        pinMode(INTERRUPT_NUMDER, INPUT);
        attachInterrupt(INTERRUPT_NUMDER, isr, CHANGE);

        pinMode(PIN_RADIO_LED, OUTPUT);
        digitalWrite(PIN_RADIO_LED, LOW);

      }


      #ifdef ESP8266

        static IRAM_ATTR void isr() {

      #else

        static void isr() {

      #endif

        radioRx.tickISR();

      }

    #endif


    void run() {


      #ifdef SET_RADIO_TX

        static uint32_t timer = 0;

        if ( millis() - timer > INTERVAL_SEND_ITEM_DATA ) {
       
          timer = millis();        
          _sendItemData(); 
       
        }

      #endif


      #ifdef SET_RADIO_RX

        static uint32_t timeLed;

        if (radioRx.gotData()) {

          digitalWrite(PIN_RADIO_LED, LOW);
          timeLed = millis();

          char s[radioRx.size + 1];
          strncpy(s, (char*) radioRx.buffer, radioRx.size);
          s[radioRx.size] = 0;

          _decryption(s);
          // _encryption(s);

          p("RADIO (got): ");
          p(s);
          p(", RSSI: ");
          p(radioRx.getRSSI());

          char mac = '0';
          uint8_t macIsCorrect = 0;

          #ifdef SET_PORT_0
            if ( s[0] == MAC_IN_PORT_0 ) {
              
              port0.read(s);
              macIsCorrect++;

            }
          #endif

          #ifdef SET_PORT_1
            if ( s[0] == MAC_IN_PORT_1 )  {
              
              port1.read(s);
              macIsCorrect++;

            }
          #endif

          #ifdef SET_PORT_2
            if ( s[0] == MAC_IN_PORT_2 )  {
              
              port2.read(s);
              macIsCorrect++;

            }
          #endif

          #ifdef SET_PORT_3
            if ( s[0] == MAC_IN_PORT_3 )  {
              
              port3.read(s);
              macIsCorrect++;

            }
          #endif

          if ( macIsCorrect == 0 ) p(" - MAC invalid");
          pln();

        }

        if ( millis() - timeLed > TIME_LED_IS_ON ) digitalWrite(PIN_RADIO_LED, HIGH);
      
      #endif

    }



    void sendData(String &s) {

        if ( s.startsWith(PREFIX_NOW) ) _bufDataNow = s.substring(5);
        if ( s.startsWith(PREFIX_DAY) ) _bufDataDay = s.substring(5);
        if ( s.startsWith(PREFIX_MONTH) ) _bufDataMonth = s.substring(5);
        if ( s.startsWith(PREFIX_YEAR) ) _bufDataYear = s.substring(5);

        p("RADIO (send): ");
        pln(s);

    }





  private:

    mString<DATA_BUF_COUNT> _bufDataNow;
    mString<DATA_BUF_COUNT> _bufDataDay;
    mString<DATA_BUF_COUNT> _bufDataMonth;
    mString<DATA_BUF_COUNT> _bufDataYear;

    void _sendItemData() {

      #ifdef SET_RADIO_TX

        mString<DATA_BUF_COUNT> *arr[4];

        arr[0] = &_bufDataNow;
        arr[1] = &_bufDataDay;
        arr[2] = &_bufDataMonth;
        arr[3] = &_bufDataYear;

        char item[DATA_BUF_COUNT_ITEM - 2];
        char data[DATA_BUF_COUNT_ITEM];

        char mark[] = { 'N', 'D', 'M', 'Y' };
        int16_t a;

        for ( int8_t i = 0; i < 4; i++ ) {

          if ( ! arr[i]->equals("") ) {

              digitalWrite(PIN_RADIO_LED, HIGH); 
       
              a = arr[i]->lastIndexOf(' ');

              arr[i]->substring(a + 1, arr[i]->length(), item);
              arr[i]->truncate(arr[i]->length() - a);

              data[0] = MAC_ADDRESS;
              data[1] = mark[i];
              data[2] = 0;
              strcat(data, item);

              p("RADIO (item): ");
              pln(data);

              _encryption(data);
              
              #ifdef SET_SLEEP

                power.setSystemPrescaler(PRESCALER_1);
              
              #endif
              
              radioTx.sendData(data);

              #ifdef SET_SLEEP

                power.setSystemPrescaler(PRESCALER_2);
              
              #endif

              digitalWrite(PIN_RADIO_LED, LOW);

              return;

          }

        }

      #endif

    }




    char* _encryption(char* s) {

      char key[] = KEY_RADIO;

      uint8_t n = 0;

      for (int i = 0; i < strlen(s); i++) {

        uint8_t k = (uint8_t) key[n] % strlen(s); 

        char c = s[k];
        s[k] = s[i];
        s[i] = c;

        n = ++n % strlen(key);

      }

      n = 0;

      for (int i = 0; i < strlen(s); i++) {

        s[i] = (char) ( (uint8_t) key[n] ^ (uint8_t) s[i] );
        n = ++n % strlen(key);

      }

      s[strlen(s)] = 0;
      
      return s;

    }



    char* _decryption(char* s) {

      char key[] = KEY_RADIO;

      uint8_t n = 0;

      for (int i = 0; i < strlen(s); i++) {

        s[i] = (char) ( (uint8_t) key[n] ^ (uint8_t) s[i] );
        n = ++n % strlen(key);

      }

      n = ( strlen(s) - 1 ) % strlen(key);

      for (int i = 0; i < strlen(s); i++) {

        uint8_t k = (uint8_t) key[n] % strlen(s); 

          char c = s[strlen(s) - i - 1];
          s[strlen(s) - i - 1] = s[k];
          s[k] = c;

          n = --n % strlen(key);

      }

      s[strlen(s)] = 0;
      
      return s;

    }




    // char* _encryption(char* s) {

    //   char key[] = KEY_RADIO;

    //   uint8_t n = 0;

    //   for (int i = 0; i < strlen(s); i++) {
  
    //     s[i] = (char) ( (uint8_t) key[n] ^ (uint8_t) s[i] );
    //     n = ++n % strlen(key);

    //   }
    
    //   s[strlen(s)] = 0;
      
    //   return s;

    // }


    
};

Radio radio;
