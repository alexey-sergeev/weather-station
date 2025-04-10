// v 1.1

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
ESP8266WiFiMulti wifiMulti;

#ifdef SET_OTA

  #include <ArduinoOTA.h>

#endif


class WiFi_wrapper {

  public:


    // Инициализация Wi-Fi

    void init() {

      WiFi.begin(SSID_0, PASS_0);

      while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
      }

      Serial.println("");
      Serial.print("Connected to ");
      Serial.println(WiFi.SSID());
      Serial.print("IP address: ");
      Serial.println(WiFi.localIP());
      
    }



    // Инициализация Wi-Fi (Multi)

    void initMulti() {

      pinMode(LED_BUILTIN, OUTPUT);

      #ifdef SSID_0
        wifiMulti.addAP(SSID_0, PASS_0);
      #endif

      #ifdef SSID_1
        wifiMulti.addAP(SSID_1, PASS_1);
      #endif

      #ifdef SSID_2
        wifiMulti.addAP(SSID_2, PASS_2);
      #endif

      #ifdef SSID_3
        wifiMulti.addAP(SSID_3, PASS_3);
      #endif

      #ifdef SSID_4
        wifiMulti.addAP(SSID_4, PASS_4);
      #endif

      #ifdef SSID_5
        wifiMulti.addAP(SSID_5, PASS_5);
      #endif

      #ifdef SSID_6
        wifiMulti.addAP(SSID_6, PASS_6);
      #endif

      #ifdef SSID_7
        wifiMulti.addAP(SSID_7, PASS_7);
      #endif

      #ifdef SSID_8
        wifiMulti.addAP(SSID_8, PASS_8);
      #endif

      #ifdef SSID_9
        wifiMulti.addAP(SSID_9, PASS_9);
      #endif
  
      #ifdef SET_OTA
        _startOTA();
      #endif

    }



    // Проверка wi-fi

    void runMulti() {

      static bool flag = true;

      uint8_t onLed;
      uint8_t offLed;

      if ( wifiMulti.run() == WL_CONNECTED ) {

        #ifdef SET_LED_REVERSE_ORDER

          onLed = LOW;
          offLed = HIGH;

        #else

          onLed = HIGH;
          offLed = LOW;

        #endif

        digitalWrite(LED_BUILTIN, ( bitRead( millis(), 11 ) == 0 && bitRead( millis() + 1900, 11 ) == 0 ) ? onLed : offLed);
        // digitalWrite(LED_BUILTIN, ( bitRead( millis(), 11 ) == 0 && bitRead( millis() + 1900, 11 ) == 0 ) ? HIGH : LOW);

        if ( flag ) {

          Serial.println();
          Serial.print("Connected to ");
          Serial.println(WiFi.SSID());
          Serial.print("IP address: ");
          Serial.println(WiFi.localIP());
          Serial.print("MAC address: ");
          Serial.println(WiFi.macAddress());
          
          #ifdef SET_ID_DEVICE
    
            Serial.print("ID Device: ");
            Serial.println(idDevice(), HEX);
    
          #endif
    
          flag = false;

        }

      } else {
        
        flag = true;
        Serial.print(".");

        digitalWrite(LED_BUILTIN, HIGH);
        delay(1000);
        digitalWrite(LED_BUILTIN, LOW);

      }

      #ifdef SET_OTA
        ArduinoOTA.handle();
      #endif

    }




    // uint16_t idDevice() {

    //   uint16_t id;

    //   uint8_t mac[6];
    //   WiFi.macAddress(mac);

    //   id += mac[0] >> 6 << 14;
    //   id += mac[1] >> 6 << 12;
    //   id += mac[2] >> 6 << 10;
    //   id += mac[3] >> 6 << 8;
    //   id += mac[4] >> 4 << 4;
    //   id += mac[5] >> 4 << 0;

    //   return id;

    // }




  private:

    void _startOTA() {

      #ifdef SET_OTA

        char h[] = "esp8266-" HELLO;

        ArduinoOTA.setHostname(h);
        ArduinoOTA.setPassword(PASSWORD_OTA);

        ArduinoOTA.onStart([]() {
        
          String type;
        
          if ( ArduinoOTA.getCommand() == U_FLASH) type = "sketch";
          else type = "filesystem";
          
          Serial.println("Start updating " + type);

        });

        ArduinoOTA.onEnd([]() {
        
          Serial.println("\nEnd");
        
        });


        ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {

          Serial.printf("Progress: %u%%\n", (progress / (total / 100)));

        });


        ArduinoOTA.onError([](ota_error_t error) {

          Serial.printf("Error[%u]: ", error);
          
          if ( error == OTA_AUTH_ERROR ) Serial.println("Auth Failed");
          else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
          else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
          else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
          else if (error == OTA_END_ERROR) Serial.println("End Failed");
        
        });

        ArduinoOTA.begin();

      #endif

    }


};

WiFi_wrapper wifi; 
