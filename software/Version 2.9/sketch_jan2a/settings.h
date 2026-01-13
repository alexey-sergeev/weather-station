// 

#define PATH_CONF_WIFI "/conf/wifi"
#define PATH_CONF_BOT "/conf/bot"
#define PATH_CONF_FORECAST "/conf/forecast"
#define PATH_CONF_ADMIN "/conf/admin"
#define PATH_CONF_DELTA "/conf/delta"
#define PATH_CONF_AP "/conf/ap"

#define DEFAULT_PASSWORD "12345"


class Settings { 
  public: 

    void init() {

    }


    void run() {

      if ( Serial.available() > 0)  {
 
        String s = Serial.readStringUntil('\n');
        
        if ( s == "reset" ) {

          save();
          pln(F("The device has been reset. Overload the device"));
          // storage.writeFile(PATH_CONF_ADMIN, "");
          // pln(F("Пароль администратора был сброшен"));

        }

      } 

    }


    String html() {

      String s;
      String a;
      String b;

      s += "<html><head><title>Settings</title></head><body>";
      s += "<h1>Settings</h1>";
      s += "<form method=\"POST\" action=\"settingsform\">";
      
      s += "<h2>WiFi</h2>";
      
      a = getWiFi(0);
      b = getWiFi(0, 'p');
      s += "SSID 1: <input type=\"text\" name=\"ssid_1\" value=\"" + a + "\"> <input type=\"password\" name=\"ssid_passw_1\" value=\"" + b + "\"><p>";
      
      a = getWiFi(1);
      b = getWiFi(1, 'p');
      s += "SSID 2: <input type=\"text\" name=\"ssid_2\" value=\"" + a + "\"> <input type=\"password\" name=\"ssid_passw_2\" value=\"" + b + "\"><p>";
      
      a = getWiFi(2);
      b = getWiFi(2, 'p');
      s += "SSID 3: <input type=\"text\" name=\"ssid_3\" value=\"" + a + "\"> <input type=\"password\" name=\"ssid_passw_3\" value=\"" + b + "\"><p>";
      

      #ifdef SET_AP

        s += "<h2>Access Point</h2>";
        
        a = ( getAP() == "1" ) ? " checked" : "";
        s += "On/Off: <input type=\"checkbox\" name=\"ap\" value=\"1\"" + a + "><p>";

      #endif


      #ifdef SET_BOT 

        s += "<h2>Telegram Bot</h2>";
        
        a = getBotToken();
        s += "Token: <input type=\"text\" name=\"bot_token\" value=\"" + a + "\"><p>";

      #endif
      
      #ifdef SET_FORECAST 
      
        s += "<h2>Weather forecast</h2>";
        
        a = getForecast(0);
        b = getForecast(1);
        s += "Appid: <input type=\"text\" name=\"forecast_arrid\" value=\"" + a + "\"><p>";
        s += "City: <input type=\"text\" name=\"forecast_city\" value=\"" + b + "\"><p>";
        
      #endif
      

      #ifdef TEMPERATURE_B_DELTA

        s += "<h2>Temperature correction</h2>";
        
        a = getDelta();
        s += "Correction: <input type=\"text\" name=\"delta\" value=\"" + a + "\"><p>";
      
      #endif      


      s += "<h2>Admin</h2>";

      a = getAdminPassw();
      s += "Password: <input type=\"password\" name=\"admin_passw_1\" value=\"" + a + "\"><p>";
      s += "Password again: <input type=\"password\" name=\"admin_passw_2\" value=\"" + a + "\"><p>";
      
      s += "<br>";
      s += "<input type=\"submit\" name=\"save\" value=\"Save\">";
      s += "</form>";

      #ifdef SET_UPDATE

        s += "<br>";
        s += "<a href=\"/firmware\">firmware</a>";

      #endif      

//  String message;
//  for (uint8_t i = 0; i < server.args(); i++) { message += " " + server.argName(i) + ": " + server.arg(i) + "\n"; }
//  s += message;


      s += "</body></html>";


      // pln( getWiFi(0) );
      // pln( getWiFi(0, 'p') );
      // pln( getWiFi(1));
      // pln( getWiFi(1, 'p') );
      // pln( getWiFi(2));
      // pln( getWiFi(2, 'p') );

      // pln( getBotToken() );

      // pln( getForecast(0) );
      // pln( getForecast(1) );

      // pln( getAdminPassw() );





      return s;

    }



    void set(String key, String d) {

      Text dd(d);
      String data = dd.trim().toString();

      if ( key == "ssid_1" ) _ssid_1_from = data;
      if ( key == "ssid_passw_1" ) _ssid_passw_1_from = data;
      
      if ( key == "ssid_2" ) _ssid_2_from = data;
      if ( key == "ssid_passw_2" ) _ssid_passw_2_from = data;
      
      if ( key == "ssid_3" ) _ssid_3_from = data;
      if ( key == "ssid_passw_3" ) _ssid_passw_3_from = data;

      if ( key == "ap" ) _ap_from = data;
      
      if ( key == "bot_token" ) _bot_token_from = data;
      
      if ( key == "forecast_arrid" ) _forecast_arrid_from = data;
      if ( key == "forecast_city" ) _forecast_city_from = data;
      
      if ( key == "delta" ) _delta_from = data;
      
      if ( key == "admin_passw_1" ) _admin_passw_1_from = data;
      if ( key == "admin_passw_2" ) _admin_passw_2_from = data;

      // pln(key);
      // pln(data);

    }


    bool isRight() {

      bool f = true;

      // pln(_ssid_1_from);
      // pln(_ssid_passw_1_from);
      
      // pln(_ssid_2_from);
      // pln(_ssid_passw_2_from);
      
      // pln(_ssid_3_from);
      // pln(_ssid_passw_3_from);

      // pln(_bot_token_from);
      
      // pln(_forecast_arrid_from);
      // pln(_forecast_city_from);
      
      // pln(_admin_passw_1_from);
      // pln(_admin_passw_2_from);

      if ( _admin_passw_1_from != _admin_passw_2_from ) f = false;

      return f;

    }



    void save() {

      String data;

      data = "";
      
      if ( _ssid_1_from != "" ) {

        data += _ssid_1_from;
        data += ":";
        data += _ssid_passw_1_from;
        data += "\n";

      }
      
      if ( _ssid_2_from != "" ) {
      
        data += _ssid_2_from;
        data += ":";
        data += _ssid_passw_2_from;
        data += "\n";

      }
      
      if ( _ssid_3_from != "" ) {
      
        data += _ssid_3_from;
        data += ":";
        data += _ssid_passw_3_from;

      }

      storage.writeFile(PATH_CONF_WIFI, data);

      #ifdef SET_AP 

        data = _ap_from;
        storage.writeFile(PATH_CONF_AP, data);

      #endif
      
      #ifdef SET_BOT 

        data = _bot_token_from;
        storage.writeFile(PATH_CONF_BOT, data);

      #endif
      
      #ifdef SET_FORECAST 

        data = "";
        data += _forecast_arrid_from;
        data += "\n";
        data += _forecast_city_from;
      
        storage.writeFile(PATH_CONF_FORECAST, data);

      #endif      
            
      #ifdef TEMPERATURE_B_DELTA

        data = _delta_from;
        storage.writeFile(PATH_CONF_DELTA, data);

      #endif      

      data = _admin_passw_1_from;
      storage.writeFile(PATH_CONF_ADMIN, data);

    }



    String get(const char * p) {

      String s;
      s = storage.readFile(p);
      Text ss(s);

      return ss.trim().toString();

    }



    String getBotToken() {

      return get(PATH_CONF_BOT);

    }



    String getAdminPassw() {


      String s = get(PATH_CONF_ADMIN);
      if ( s == "" ) s = DEFAULT_PASSWORD;
      
      return s;

    }




    String getDelta() {

      String s = get(PATH_CONF_DELTA);

      #ifdef TEMPERATURE_B_DELTA
        if ( s == "" ) s = TEMPERATURE_B_DELTA;
      #else
        if ( s == "" ) s = "0";
      #endif
      
      return s;

    }



    String getAP() {

      String s = get(PATH_CONF_AP);

      if ( s == "" || s == "1" ) return "1";
      return "0";

    }



    String getForecast(uint8_t n = 0) {

      String s;

      s = get(PATH_CONF_FORECAST);
      Text ss(s);

      TextParser row(ss, '\n');
      
      if ( row.parse() ) if ( n == 0 ) return row.trim().toString();
      if ( row.parse() ) if ( n == 1 ) return row.trim().toString();

      return "";

    }



    String getWiFi(uint8_t n = 0, char k = 's') {

      String s;
      s = get(PATH_CONF_WIFI);

      Text ss(s);

      // String u;
      // String p;

      // u.reserve(20);
      // p.reserve(20);

      uint8_t nn = 0;

      for ( TextParser row(ss, '\n'); row.parse(); ) {
        
        TextParser col(row, ':');
        
        if ( col.parse() ) if ( n == nn && k == 's')  return col.trim().toString();
        if ( col.parse() ) if ( n == nn && k == 'p')  return col.trim().toString();

        ++nn;

      }

      return "";

    }




  private: 

    String _ssid_1_from;
    String _ssid_passw_1_from;
    String _ssid_2_from;
    String _ssid_passw_2_from;
    String _ssid_3_from;
    String _ssid_passw_3_from;
    String _ap_from = "0";
    String _bot_token_from;
    String _forecast_arrid_from;
    String _forecast_city_from;
    String _delta_from;
    String _admin_passw_1_from;
    String _admin_passw_2_from;

};


Settings settings;

