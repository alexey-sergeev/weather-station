// 

#include <PubSubClient.h>


WiFiClient net;
PubSubClient client(net);

#define MQTT_TOPIC_GENERIC "ESP/DataRow"

class MQTT_wrapper { 
  public: 

    void init() {

      client.setServer(MQTT_SERVER, MQTT_PORT);
      client.setCallback(msgReceived);

    }


    void run() {
      
      if ( !client.connected() ) {

        String clientId = "ESP8266-" + WiFi.macAddress();

        p("MQTT: Connecting - ");
        pln( ( client.connect(clientId.c_str(), MQTT_USER, MQTT_PASSWORD) ) ? "yes" : "Connected failed" );
      
        #ifdef SET_MQTT_RX

          client.subscribe(MQTT_TOPIC_GENERIC, 1);

          for ( int i = 0; i < BUF_NOW_COUNT; i++ ) {
          
            String topic = MQTT_TOPIC_GENERIC;
            topic += String(i);

            client.subscribe(topic.c_str(), 1);
          
          }

        #endif

      }
      
      client.loop();

      #ifndef SET_MQTT_RX

        // Now

        static uint32_t timerNow = 0;
        if ( millis() - timerNow > INTERVAL_PUBLISH_NOW ) {

          p("MQTT (publish):");
 
          String s = _wrapperDataRow(N_NOW);
          pln( ( client.publish(MQTT_TOPIC, s.c_str()) ) ? " - ok" : " - Publish failed" );
 
          timerNow = millis();

        }


        // Day

        static uint32_t timerDay = 0;
        if ( millis() - timerDay + 1000 > INTERVAL_SAVE_TO_DB_DAY ) {

          p("MQTT (publish):");

          String s = _wrapperDataRow(N_DAY);          
          pln( ( client.publish(MQTT_TOPIC, s.c_str()) ) ? " - ok" : " - Publish failed" );

          timerDay = millis();

        }


        // Точное время

        time_t now;
        struct tm * timeinfo;
        time(&now);
        timeinfo = localtime(&now);  

        // Month 

        static bool _flagMonth = true;
        if ( timeinfo->tm_min > 0 && timeinfo->tm_min < 20 ) {

          if ( _flagMonth ) {

            p("MQTT (publish):");

            String s = _wrapperDataRow(N_MONTH);          
            pln( ( client.publish(MQTT_TOPIC, s.c_str()) ) ? " - ok" : " - Publish failed" );
  
            _flagMonth = false;
          
          }

        } else {

          _flagMonth = true;

        }
      

      // Year

      static bool _flagYear = true;
      if ( timeinfo->tm_min > 1 && timeinfo->tm_min < 20 ) {

        if ( _flagYear ) {

          if ( timeinfo->tm_hour % 4 == 0 ) {

            p("MQTT (publish):");

            String s = _wrapperDataRow(N_YEAR);
            pln( ( client.publish(MQTT_TOPIC, s.c_str()) ) ? " - ok" : " - Publish failed" );

          }
         
          _flagYear = false;

        }

      } else {

        _flagYear = true;

      }

      #endif

    }



    static void msgReceived(char* topic, byte* payload, unsigned int length) {

      #ifdef SET_MQTT_RX 

        p("MQTT (message arrived [");
        p(topic);
        p("]): ");

        String msg = "";

        for ( int i = 0; i < length; i++ ) msg += (char) payload[i];

        pln(msg);

        #ifdef SET_HTTP
        
          if ( msg.startsWith(PREFIX_NOW) ) {

            sensors.updateBufDataNow(_topicN(topic), msg.substring(17));

          }

        #endif

        if ( msg.startsWith(PREFIX_DAY) ) db.saveBD(N_DAY, msg.substring(6));
        if ( msg.startsWith(PREFIX_MONTH) ) db.saveBD(N_MONTH, msg.substring(6));
        if ( msg.startsWith(PREFIX_YEAR) ) db.saveBD(N_YEAR, msg.substring(6));

      #endif

    }



  private: 

    String _wrapperDataRow(uint8_t n = 0) {

      String s = "";

      if ( n == N_NOW ) s += PREFIX_NOW;
      if ( n == N_DAY ) s += PREFIX_DAY;
      if ( n == N_MONTH ) s += PREFIX_MONTH;
      if ( n == N_YEAR ) s += PREFIX_YEAR;

      s += " ";
      s += sensors.creatingDataRow(n);

      // Serial.print(" ");
      // Serial.print(s);
      p(" ");
      p(s);

      return s;

    }



    static uint8_t _topicN(char* topic) {

      char c;
      String t;
       
      for ( int i = 0; i < strlen(topic); i++ ) {

        c = topic[i];

        if ( c == '0' || c == '1' || c == '2' || c == '3' || c == '4' || 
              c == '5' || c == '6' || c == '7' || c == '8' || c == '9' ) t += c;

        }

      return t.toInt();

    }

};

MQTT_wrapper mqtt;

