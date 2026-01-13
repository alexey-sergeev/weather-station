#include "bot_schedule.h"

// #define GMT 10800 // 60 * 60 * 3

#define TIME_ALARM_POWER_1 60000 // 60 * 1000
#define TIME_ALARM_POWER_2 300000 // 5 * 60 * 1000
#define TIME_ALARM_POWER_3 900000 // 15 * 60 * 1000
#define TIME_ALARM_POWER_4 1800000 // 30 * 60 * 1000
#define TIME_ALARM_POWER_5 3600000 // 60 * 60 * 1000
#define TIME_ALARM_POWER_6 10800000 // 3 * 60 * 60 * 1000

#define TIME_ALARM_TEMP_1 300000 // 5 * 60 * 1000
#define TIME_ALARM_TEMP_2 3600000 // 60 * 60 * 1000
#define TIME_ALARM_TEMP_3 10800000 // 3 * 60 * 60 * 1000

#define COMMAND_DISPLAY 200
#define COMMAND_DISPLAY_RAW 201
#define COMMAND_WEB_SITE 202
#define COMMAND_BUZZER 203

// #define PATH_CONF_BOT "/conf/bot"


class bot_ws : public bot_schedule {

  public:

    void init() {

      #ifdef SET_SETTINGS
      
        // String s;
        // s = storage.readFile(PATH_CONF_BOT);
        // // pln(s);
        // Text ss(s);
        
        // if ( s != "" ) {

        //   pln("BOT: Found token"); 
        //   bot.setToken(ss.trim());

        // }
        
        String s = settings.getBotToken();
        if ( s != "" ) bot.setToken(s);

      #endif

      bot.attach(newMsg);
      _initEEPROM();
      _initSchEEPROM();

      randomSeed((uint32_t) time(NULL));

      // Получить точное время из Интернета
      // configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");

    }


    // Обработчик сообщений бота

    static void newMsg(FB_msg& msg) {

      // if ( ! _isUser(msg.userID) ) _addUser(msg);

      p("BOT: ");
      p(msg.userID);
      p(", ");
      p(msg.username);
      p(", ");
      pln(msg.text); 

      if ( ! sa.admin.equals("") ) {

        if ( msg.text == "/say_hello" ) _commandSayHello(msg);
        if ( msg.text == "/menu" ) _commandMenu(msg);
        if ( msg.text == "/display" ) _commandDisplay(msg);
        if ( msg.text == "/display_raw" ) _commandDisplayRaw(msg);
        if ( msg.text == "/website" ) _commandWebSite(msg);
        if ( msg.text == "/settings" ) _commandSettings(msg);

        if ( _isUser(msg.userID) ) {

          if ( msg.text == "/buzzer" ) _commandBuzzer(msg);
          if ( msg.text == "/find_schedule" ) _commandFind(msg);

          if ( _isAdmin(msg.userID) ) {

            if ( msg.text == "/list" ) _commandList(msg);
            if ( msg.text == "/i" ) _commandI(msg);

            if ( msg.text == "/add_schedule" || command == COMMAND_ADD ) _commandAdd(msg);
            if ( msg.text == "/edit_schedule" || command == COMMAND_EDIT ) _commandEdit(msg); 

            if ( msg.text == "/delete_user" || command == COMMAND_DELETE ) _commandDelete(msg);
            if ( msg.text == "/invitation" ) _commandInvitation(msg);
            if ( msg.text == "/q" || command == COMMAND_Q ) _commandQ(msg);

          }
        } else _setUser(msg);
      } else _setAdmin(msg);

    }


    void run() {

      
      #ifdef SET_ALARM
      
        _pAlarm();
        _tAlarm();
      
      #endif


      #ifndef SET_STRIP

        _outputOnSchedule();

      #endif


      #ifdef SET_STRIP

        _strip();

      #endif





      #ifdef CHAT_TOKEN
  
        static uint32_t timerDay = 0;
        if ( millis() - timerDay + 1000 > INTERVAL_SAVE_TO_DB_DAY ) {

          String s = sensors.creatingDataRow();

          bot.sendMessage(s, CHAT_TOKEN); 
          timerDay = millis();
    
          p("BOT (chat): "); 
          pln(s); 

        }

      #endif



    }

  
  protected:


    void _strip() {

      #ifdef SET_STRIP
        
        static uint32_t timer = 0;

        if ( millis() - timer > 250 ) {
          
          timer = millis();

          // for (uint8_t i = 0; i < SIZE_ITEM; i++) {
            
          //   p(i);
          //   p(" - ");
          //   p(sch.item[i].status);
          //   p(" - ");
          //   pln(sch.item[i].time);

          // }

          if ( isOnOff() ) {

            // pln("red");
            // strip.point(0xFF0000);
            strip.point2(250);

          } else {

            // pln("green");
            // strip.point(0x008000);
            strip.point2(90);

          };

        }
        
      #endif
     
    }


    void _outputOnSchedule() {

      static uint32_t timer = 0;
      static bool f = true;

      if ( millis() - timer > 30000 ) {
        
        timer = millis();

        if ( isOnOff() ) {

          if ( f ) {

            bot.setTextMode(FB_HTML);
            bot.notify(false);
            bot.sendMessage(_getData(), _userToString());

            #ifdef CHAT_TOKEN
        
                bot.sendMessage(_getData(), CHAT_TOKEN); 

            #endif

          }

          f = false;

        } else {

          f = true;

        };

      }
     
    }





    void _tAlarm() {

      #ifdef SET_ALARM
      
        const char* phrase1[] = { "В серверной сейчас холодно",
                                  "Сейчас в серверной холодно",
                                  "На данный момент в серверной холодно", 
                                  "Настоящее время в серверной холодно" };

        const char* phrase2[] = { "В серверной сейчас жарко",
                                  "Сейчас в серверной жарко",
                                  "На данный момент в серверной жарко", 
                                  "Настоящее время в серверной жарко" };

        static int16_t d = 0;

        static uint8_t n = 0;
        static uint32_t time;
        String s = "";
  
        float t = sTemperature.get();

        if ( ( t < TEMPERATURE_MIN || t > TEMPERATURE_MAX ) ) {

          if ( n == 0 ) {

            time = millis();
            n = 1;
            
          } else if ( ( n == 1 && millis() - time > TIME_ALARM_TEMP_1 ) ||
                      ( n == 2 && millis() - time > TIME_ALARM_TEMP_2 + d ) ||
                      ( n == 3 && millis() - time > TIME_ALARM_TEMP_3 + d * 8 ) ||
                      ( n == 4 && millis() - time > TIME_ALARM_TEMP_3 + d * 32 ) ) {

            time = millis(); 
            d = random(120000) - 60000;
            
            if ( sTemperature.get() < TEMPERATURE_MIN ) s += phrase1[random(sizeof(phrase1)/sizeof(phrase1[0]))];;
            if ( sTemperature.get() > TEMPERATURE_MAX ) s += phrase2[random(sizeof(phrase2)/sizeof(phrase2[0]))];;
            
            s += ": ";
            
            s += (String) t;
            s += " °";

            if ( ++n > 4 ) n = 4;

            bot.sendMessage(s, _userToString());

          } 
          
        } else {

          n = 0; 
                          
        }

      #endif

    }





    void _pAlarm() {

      #ifdef SET_ALARM

        const char* phrase[] = {  "Электричество было выключено",
                                  "Электричество отключили",
                                  "Свет выключили",
                                  "Электроснабжение прекратили",
                                  "Электроэнергию отключили",
                                  "Свет отключили",
                                  "Свет вырубили",
                                  "Электросеть отключили",
                                  "Электричество прекратили подавать",
                                  "Свет отключили" };

        static uint8_t n = 0;
        static uint8_t n2 = 0;
        static int32_t d = 0;
        static uint8_t dm = 0;
      
        static bool f = false;
        static uint32_t time;
        String s = "";

        if ( sPower.get() == 0 ) {

          f = true;
          n2 = 0;

          if ( n == 0 ) {

            time = millis();
            n = 1;
            dm = 0;
            
          } else if ( ( n == 1 && millis() - time > TIME_ALARM_POWER_1 + d * pow(2, dm) ) ||
                      ( n == 2 && millis() - time > TIME_ALARM_POWER_2 + d * pow(2, dm) ) ||
                      ( n == 3 && millis() - time > TIME_ALARM_POWER_3 + d * pow(2, dm) ) ||
                      ( n == 4 && millis() - time > TIME_ALARM_POWER_4 + d * pow(2, dm) ) ||
                      ( n == 5 && millis() - time > TIME_ALARM_POWER_5 + d * pow(2, dm) ) ||
                      ( n == 6 && millis() - time > TIME_ALARM_POWER_6 + d * pow(2, dm) ) ||
                      ( n == 7 && millis() - time > TIME_ALARM_POWER_6 + d * pow(2, dm) ) ) {

            time = millis(); 

            uint32_t arr[] = {TIME_ALARM_POWER_1, TIME_ALARM_POWER_2, TIME_ALARM_POWER_3, TIME_ALARM_POWER_4, TIME_ALARM_POWER_5, TIME_ALARM_POWER_6 };

            int32_t m = d * pow(2, dm);
            for ( uint8_t i = 0; i < n; i++ ) m += arr[i];
            m = m / 60000;

            s += phrase[random(sizeof(phrase)/sizeof(phrase[0]))];
            s += " ";

            if ( n != 7 ) {

              s += (String) m;
              s += " мин. назад";

            } else s += "давно";

            if ( ++n > 7 ) n = 7;
            d = random(120000) - 60000;
            ++dm; 

            bot.sendMessage(s, _userToString());

          } 
          
        } else {

          n = 0; 
          
          if ( n2 == 0 ) {

            time = millis();

          } else if ( n2 == 1 && millis() - time > TIME_ALARM_POWER_1 ) { 

            time = millis(); 
            
            s = "Электричество подается в обычном режиме";
            bot.sendMessage(s, _userToString());
            
            ++n2;

          }
        
        }
        
      #endif

    }




    static String _getData() {

      String s = "";
      s += "<b>Показания датчиков</b>\n\n";

      String data = sensors.creatingDataRow(N_NOW);
      data += " ";

      char c[data.length() + 1];

      for (int i = 0; i < 10; i++) c[i] = data.charAt(i);
      c[10] = 0;

      s += _getTime(c);
      s += "\n";

      // #ifdef SET_ID_DEVICE
      
      //   s += "ID устройства: <b>";
      //   // s += wifi.idDevice();
      //   s += String (wifi.idDevice(), HEX);
      //   s += "</b>\n\n";
      
      // #endif

      uint16_t j = 0;

      for (int i = 10; i < data.length(); i++) {
        
        if ( data.charAt(i) == ' ' ) {

          c[j] = 0;
          j = 0;

          #ifndef SET_ID_DEVICE

            if ( c[0] == 't' && c[1] == ':' ) s += "\n<b>В доме:</b>\n\n";
            if ( c[0] == 't' && c[1] == '2' ) s += "\n<b>На даче:</b>\n\n";
            if ( c[0] == 't' && c[1] == '5' ) s += "\n<b>В лагере:</b>\n\n";
            if ( c[0] == 't' && c[1] == '7' ) s += "\n<b>На балконе:</b>\n\n";
            // if ( c[0] == 't' && c[1] == '6' ) s += "\n<b>В библиотеке:</b>\n\n";
            if ( c[0] == 'r' && c[1] == '4' ) s += "\n<b>Осадки:</b>\n\n";
            if ( c[0] == 'v' && c[1] == 'a' ) s += "\n<b>Напряжение:</b>\n\n";
            if ( c [0] == 't' && c[1] == '3' ) s += "\n<b>На улице:</b>\n\n";
            if ( c [0] == 't' && c[1] == '6' ) s += "\n<b>На балконе:</b>\n\n";
            if ( c [0] == 't' && c[1] == '8' ) s += "\n<b>В серверной:</b>\n\n";
            // if ( c[0] == 'b' && c[1] == 't' ) s += "\n<b>В бассейне:</b>\n\n";
            if ( c[0] == 'b' && c[1] == 't'  && c[2] == '4' ) s += "\n<b>В бассейне:</b>\n\n";
            if ( c[0] == 'b' && c[1] == 't'  && c[2] == '5' ) s += "\n<b>В Ахтубе:</b>\n\n";
            if ( c[0] == 'a' && c[1] == 'i' ) s += "\n<b>Воздух в доме:</b>\n\n";

          #endif

          if ( c[0] == 'i' && c[1] == 'd' ) s += "\n";

          s += _getIndicator(c);
            
        } else c[j++] = data.charAt(i);

      }

      #ifdef SET_FORECAST

        s +=_getForecast();

      #endif

      return s;

    }




    static String _getForecast() {

      String s = "";

      #ifdef SET_FORECAST

        // String s2 = forecast.showForecast('w');

        // if ( s2 != "" ) {

        //   s += "\n<b>На улице:</b>\n\n";
        //   s += "Температура: <b>";
        //   s += s2;
        //   s += " °</b>\n";

        // }

        String s3 = forecast.showForecast('f', 0);
        String s4 = forecast.showForecast('f', 1);
        String s5 = forecast.showForecast('f', 2);
        String s6 = forecast.showForecast('f', 3);

        if ( !( s3 == "" && s4 == "" && s5 == "" && s6 == "" ) ) {

          s += "\n<b>Прогноз на завтра:</b>\n\n";
          if ( s3 != "" ) s += "Ночь: <b>" + s3 + "</b>\n";
          if ( s4 != "" ) s += "Утро: <b>" + s4 + "</b>\n";
          if ( s5 != "" ) s += "День: <b>" + s5 + "</b>\n";
          if ( s6 != "" ) s += "Вечер: <b>" + s6 + "</b>\n";

        }
      
      #endif

      return s;

    }




    static void _commandDisplay(FB_msg& msg) {

      command = COMMAND_DISPLAY;

      bot.setTextMode(FB_HTML);
      bot.sendMessage(_getData(), msg.chatID);

    }



    static String _getIndicator(char* c) {

      char s[100] = "";

      if ( strchr(c, ':') != 0 ) {

        char * key = strtok(c, ":");
        char * value = strtok(NULL, ":");

        String k = key;
        char k0 = k.charAt(0);
        char k1 = k.charAt(1);
        

        if ( k0 == 't' ) {

          strcat(s, "Температура воздуха: <b>");
          strcat(s, value);
          strcat(s, " °</b>");

        }

        else if ( k0 == 'b' && k1 == 't' ) {

          strcat(s, "Температура воды: <b>");
          strcat(s, value);
          strcat(s, " °</b>");

        }

        else if ( k0 == 's' && k1 == 't' ) {

          strcat(s, "Температура на улице: <b>");
          strcat(s, value);
          strcat(s, " °</b>");

        }
          
        else if ( k0 == 'h' ) {
        
          strcat(s, "Влажность: <b>");
          strcat(s, value);
          strcat(s, " %</b>");

        }
          
        else if ( k0 == 'l' ) {
        
          strcat(s, "Освещённость: <b>");
          strcat(s, value);
          strcat(s, " лк.</b>");

        }
        
        else if ( k0 == 's' ) {
        
          strcat(s, "Освещённость: <b>");
          strcat(s, value);
          strcat(s, " %</b>");

        }

        else if ( k0 == 'p' ) {

          strcat(s, "Атмосферное давление: <b>");
          strcat(s, value);
          strcat(s, " мм. рт. ст.</b>");

        }

        else if ( k0 == 'a' && k1 == 'i' ) {

          strcat(s, "Индекс качества воздуха: <b>");
          strcat(s, value);
          strcat(s, "</b>");

        }

        else if ( k0 == 'a' && k1 == 't' ) {

          strcat(s, "Воздушная смесь: <b>");
          strcat(s, value);
          strcat(s, " ppb</b>");

        }

        else if ( k0 == 'a' && k1 == 'c' ) {

          strcat(s, "Углекислый газ: <b>");
          strcat(s, value);
          strcat(s, " ‰</b>");
          
        }

        else if ( k0 == 'g' ) {

          strcat(s, "Влажность почвы: <b>");
          strcat(s, value);
          strcat(s, " %</b>");
          
        }

        else if ( k0 == 'r' ) {

          strcat(s, "Осадки: <b>");
          strcat(s, value);
          strcat(s, " %</b>");
          
        }

        else if ( k0 == 'u' ) {

          strcat(s, "Активность солнца: <b>");
          strcat(s, value);
          strcat(s, " %</b>");
          
        }

        else if ( k0 == 'v' && k1 == 'a' ) {

          strcat(s, "Аккумулятор: <b>");
          strcat(s, value);
          strcat(s, " в.</b>");
          
        }

        else if ( k0 == 'v' && k1 == 'b' ) {

          strcat(s, "Блок питания: <b>");
          strcat(s, value);
          strcat(s, " в.</b>");
          
        }

        else if ( k0 == 's' && k1 == 'd' ) {

          strcat(s, "Шум: <b>");
          strcat(s, value);
          strcat(s, " в.</b>");
          
        }

        else if ( k0 == 'i' && k1 == 'd' ) {

          uint16_t id = atoi(value);
          char cid[] = "FFFF";
          itoa(id, cid, HEX);

          strcat(s, "ID устройства: <b>");
          // strcat(s, value);
          strcat(s, cid);
          strcat(s, "</b>");
         
          // char out[] = "FFFF";
          // _idDevice(value, out);

          // strcat(s, "ID устройства: <b>");
          // // strcat(s, value);
          // strcat(s, out);
          // strcat(s, "</b>");
          
        }

        else {

          strcat(s, "value: <b>");
          strcat(s, value);
          strcat(s, "</b>");

        }

        strcat(s, "\n");

      }

      return (String) s;

    }




    static String _getTime(char* c) {

      String s = "Время: <b>";
      
      uint32_t t = atol(c) + GMT;

      uint8_t tt = 0; 
      
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

      s += "</b>";

      return s;
      
    }



    // // static
    // char* _idDevice(char* value) {

    //   char c[] = "FFFF";

    //   uint16_t id = atoi(value);
    //   itoa(id, c, HEX);
 
    //   return c;
      
    // }


    // static void _idDevice(char* in, char* out) {

    //   uint16_t id = atoi(in);
    //   itoa(id, out, HEX);
      
    // }



    static void _commandWebSite(FB_msg& msg) {

      command = COMMAND_WEB_SITE;

      #ifdef SET_HTTP

        String s;

        // char out[] = "FFFF";
        // _idDevice(value, out);

        s += "Убедитесь, что вы подключены к той же сети, что и метеостанция: <b>";
        s += WiFi.SSID();
        s += "</b>\n\n";
        
        #ifdef SET_ID_DEVICE
         
          s += "ID устройства: <b>";
          s += String (idDevice(), HEX);
          // s += String (wifi.idDevice(), HEX);
          s += "</b>\n\n";

        #endif
      
        s += "http://";

        s += WiFi.localIP().toString();

        bot.setTextMode(FB_HTML);
        bot.sendMessage(s, msg.chatID);
       
      #endif

    }


 


    static void _commandSettings(FB_msg& msg) {

      command = COMMAND_WEB_SITE;

      #ifdef SET_SETTINGS
      #ifdef SET_HTTP

        String s;

        s += "Убедитесь, что вы подключены к той же сети, что и метеостанция: <b>";
        s += WiFi.SSID();
        s += "</b>\n\n";
        
        s += "ID устройства: <b>";
        s += String (idDevice(), HEX);
        s += "</b>\n\n";

        s += "http://";
        s += WiFi.localIP().toString();
        s += "/settings";

        s += "\n\n";
        s += "Пользователь: <b>admin</b>\n";
        s += "Пароль по умолчанию: <b>";
        s += DEFAULT_PASSWORD;
        s += "</b>";

        bot.setTextMode(FB_HTML);
        bot.sendMessage(s, msg.chatID);
       
      #endif
      #endif

    }


 
 
    static void _commandBuzzer(FB_msg& msg) {

      command = COMMAND_BUZZER;

      #ifdef SET_ALARM

        String s;

        s += ( alarm.controlBuzzer() ) ? "On" : "Зуммер выключен на время";

        bot.setTextMode(FB_HTML);
        bot.sendMessage(s, msg.chatID);
       
      #endif

    }


 
 
    static void _commandDisplayRaw(FB_msg& msg) {

      command = COMMAND_DISPLAY_RAW;

      String s = sensors.creatingDataRow();

      bot.setTextMode(FB_HTML);
      bot.sendMessage(s, msg.chatID);

      p("BOT (bot): "); 
      pln(s); 


    }


    static void _commandMenu(FB_msg& msg) {

      command = COMMAND_MENU;

      if ( _isUser(msg.userID) ) {
        
        bool f = false;
        if ( _isAdmin(msg.userID) ) f = true;

        String s = "";

        s += "<b>Меню</b>\n\n";
        s += "/display — Вывести показания датчиков\n";
 
        #ifdef SET_HTTP

          s += "/website — Вывести адрес веб-сайта\n";
        
        #endif
 
        #ifdef SET_ALARM

          s += "/buzzer — Выключить зуммер\n\n";
        
        #endif

        s += "\n";

        // s += "/display_raw — Вывести показания датчиков (raw)\n\n";

        if (f) s += "<b>Расписание:</b>\n\n";

        s += "/find_schedule — Узнать расписание\n";
        if (f) s += "/add_schedule — Добавить\n";
        if (f) s += "/edit_schedule — Редактировать\n";

        if (f) s += "\n<b>Пользователь:</b>\n\n";

        if (f) s += "/list — Список пользователей\n";
        if (f) s += "/invitation — Создать приглашение\n";
        if (f) s += "/delete_user — Удалить\n";

        if (f) s += "\n<b>Устройство:</b>\n\n";

        s += "/say_hello — Поздороваться\n";
        if (f) s += "/i — Имя Wi-Fi\n";
        if (f) s += "/q — Cброс настроек\n";

        bot.setTextMode(FB_HTML);
        bot.sendMessage(s, msg.chatID);

      } else {

        bot.sendMessage("Какой пин-код?", msg.chatID);

      }

    }



    static void _commandAdd(FB_msg& msg) {

      static uint8_t n;

      if ( command != COMMAND_ADD ) n = 0; 
      command = COMMAND_ADD;

      if ( n == 0 ) {
      
        String s = "";

        s = _getSchList();
        if ( s == "" ) s = "Список пуст";
      
        s = "<b>Создать элемент расписания</b>\n\n" + s;

        s += "\n\n<b>Формат:</b>\n\n";
        s += "00.00. — начало\n";
        s += ".00.00 — конец\n\n";
        s += "Создать?\n";

        bot.setTextMode(FB_HTML);
        bot.sendMessage(s, msg.chatID);

        n = 1;

      } else if ( n == 1 ) {

        if ( _setItem(msg.text) ) {
          
          String s = "<b>Готово:</b>\n\n";
          s += _getSchList();
          s += "\n\nЕще создать?";

          bot.sendMessage(s, msg.chatID);

        } else if ( ! msg.text.startsWith("/") ) bot.sendMessage("Формат неправильный. Попробуйте ещё раз.", msg.chatID);

      }

    }






    static bool _setItem(String& s) {

      ScheduleItem schItem;

      if (

          ( s[0] == '0' || s[0] == '1' || s[0] == '2' ) &&   
          ( s[1] == '0' || s[1] == '1' || s[1] == '2' || s[1] == '3' || s[1] == '4' || s[1] == '5' ||
            s[1] == '6' || s[1] == '7' || s[1] == '8' || s[1] == '9' ) &&
          ( s[2] == '.' ) &&   
          ( s[3] == '0' || s[3] == '1' || s[3] == '2' || s[3] == '3' || s[3] == '4' || s[3] == '5' ) &&   
          ( s[4] == '0' || s[4] == '1' || s[4] == '2' || s[4] == '3' || s[4] == '4' || s[4] == '5' ||
            s[4] == '6' || s[4] == '7' || s[4] == '8' || s[4] == '9' ) &&
          ( s[5] == '.' ) 
        
        ) {

            schItem.time = _getTime(s[0], s[1], s[3], s[4]);
            schItem.status = true;
            _itemUpdate(schItem);

            #ifndef SET_STRIP

              schItem.time = _getTime(s[0], s[1], s[3], s[4]) + 1;
              schItem.status = false;
              _itemUpdate(schItem);

            #endif

           return true;
          
        } else if (

          ( s[0] == '.' ) &&
          ( s[1] == '0' || s[1] == '1' || s[1] == '2' ) &&   
          ( s[2] == '0' || s[2] == '1' || s[2] == '2' || s[2] == '3' || s[2] == '4' || s[2] == '5' ||
            s[2] == '6' || s[2] == '7' || s[2] == '8' || s[2] == '9' ) &&
          ( s[3] == '.' ) &&   
          ( s[4] == '0' || s[4] == '1' || s[4] == '2' || s[4] == '3' || s[4] == '4' || s[4] == '5' ) &&   
          ( s[5] == '0' || s[5] == '1' || s[5] == '2' || s[5] == '3' || s[5] == '4' || s[5] == '5' ||
            s[5] == '6' || s[5] == '5' || s[5] == '8' || s[5] == '9' ) 

        ) {

           schItem.time = _getTime(s[1], s[2], s[4], s[5]);
           schItem.status = false;
           _itemUpdate(schItem);

           return true;

        }
        
        

      return false;

    }




    static uint16_t _getTime(char h0, char h1, char m0, char m1) {

      uint16_t t = 1440;

      char h[3];
      h[0] = h0; 
      h[1] = h1; 
      h[2] = 0; 

      char m[3];
      m[0] = m0; 
      m[1] = m1; 
      m[2] = 0; 

      t = atoi(h) * 60 + atoi(m);

      return t;

    }



};

bot_ws telegramBot;

