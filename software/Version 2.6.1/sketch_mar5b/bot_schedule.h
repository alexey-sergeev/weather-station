#include "bot_core.h"

#define COMMAND_FIND 100
#define COMMAND_ADD 101
#define COMMAND_EDIT 102

#define SIZE_ITEM 100

struct ScheduleItem {
  uint16_t time; 
  bool status;
};

struct Schedule { 
  mString<10> key;
  ScheduleItem item[SIZE_ITEM];
};

Schedule sch;


class bot_schedule : public bot_core {

  public:

    void init() {

      bot.attach(newMsg);
      _initEEPROM();
      _initSchEEPROM();

      // Получить точное время из Интернета
      // configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");

    }


    // Обработчик сообщений бота

    static void newMsg(FB_msg& msg) {

      p(msg.userID);
      p(", ");
      p(msg.username);
      p(", ");
      pln(msg.text); 

      if ( ! sa.admin.equals("") ) {

        if ( msg.text == "/say_hello" ) _commandSayHello(msg);
        if ( msg.text == "/menu" ) _commandMenu(msg);

        if ( _isUser(msg.userID) ) {

          if ( msg.text == "/list" ) _commandList(msg);
          if ( msg.text == "/i" ) _commandI(msg);

          if ( _isAdmin(msg.userID) ) {

            if ( msg.text == "/find_schedule" ) _commandFind(msg);
            if ( msg.text == "/add_schedule" || command == COMMAND_ADD ) _commandAdd(msg);
            if ( msg.text == "/edit_schedule" || command == COMMAND_EDIT ) _commandEdit(msg); 

            if ( msg.text == "/delete_user" || command == COMMAND_DELETE ) _commandDelete(msg);
            if ( msg.text == "/invitation" ) _commandInvitation(msg);
            if ( msg.text == "/q" || command == COMMAND_Q ) _commandQ(msg);

          }
        } else _setUser(msg);
      } else _setAdmin(msg);

    }


    static bool isOnOff() {

      static bool flag;

      // Точное время
      time_t now;
      struct tm * timeinfo;
      time(&now);
      timeinfo = localtime(&now);

      for (uint8_t i = 0; i < SIZE_ITEM - 1; i++) 
        if ( sch.item[i].time >= 1440 ) {
      
          break;
          flag = sch.item[i].status; 

        }

      uint16_t time = timeinfo->tm_hour * 60 + timeinfo->tm_min;

      for (uint8_t i = 0; i < SIZE_ITEM - 1; i++) 
        if ( sch.item[i].time <= time && time < sch.item[i + 1].time ) {
      
          flag = sch.item[i].status;

          break;

        }

      return flag;

    }

  
  protected:
            
    static void _commandFind(FB_msg& msg) {

      command = COMMAND_FIND;

      String s = "";

      s = _getSchList();
      if ( s == "" ) s = "Список пуст";

      s = "<b>Расписание:</b>\n\n" + s;

      bot.setTextMode(FB_HTML);
      bot.sendMessage(s, msg.chatID);

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


    static void _commandEdit(FB_msg& msg) {

      static uint8_t n;

      if ( command != COMMAND_EDIT ) n = 0; 
      command = COMMAND_EDIT;

      if ( n == 0 ) {
      
        String s = "";

        s = _getSchListEdit();
        if ( s != "" ) s += "\nНомер в списке, который необходимо удалить?";
        if ( s == "" ) s += "Список пуст";
      
        s = "<b>Удалить элемент расписания</b>\n\n" + s;

        bot.setTextMode(FB_HTML);
        bot.sendMessage(s, msg.chatID);
    
        n = 1;

      } else if ( n == 1 ) {

        uint16_t nn = _isSchList(msg.text);

        if ( nn != 1440 ) {
          
          String s = "";

          if ( _deleteItem(nn) ) {

            s += _getSchListEdit();
            if ( s != "" ) s += "\nУдалить еще?";
            if ( s == "" ) s += "Список пуст";

            s = "<b>Готово:</b>\n\n" + s;

          } else {

            s = "Ошибка";

          }

          bot.sendMessage(s, msg.chatID);

        } else {

          bot.sendMessage("Номер был введен неправильно", msg.chatID);
          
        }

      }
      
    } 



    static void _commandMenu(FB_msg& msg) {

      command = COMMAND_MENU;

      if ( _isUser(msg.userID) ) {
        
        bool f = false;
        if ( _isAdmin(msg.userID) ) f = true;

        String s = "";

        s += "<b>Меню</b>\n\n";

        if (f) s += "<b>Расписание:</b>\n\n";

        s += "/find_schedule — Узнать расписание\n";
        if (f) s += "/add_schedule — Добавить\n";
        if (f) s += "/edit_schedule — Редактировать\n";

        if (f) s += "\n<b>Пользователь:</b>\n\n";

        s += "/list — Список пользователей\n";
        if (f) s += "/invitation — Создать приглашение\n";
        if (f) s += "/delete_user — Удалить\n";

        if (f) s += "\n<b>Устройство:</b>\n\n";

        s += "/say_hello — Поздороваться\n";
        s += "/i — Имя Wi-Fi\n";
        if (f) s += "/q — Cброс настроек\n";

        bot.setTextMode(FB_HTML);
        bot.sendMessage(s, msg.chatID);

      } else {

        bot.sendMessage("Какой пин-код?", msg.chatID);

      }

    }





    static void _commandQ(FB_msg& msg) {

      static uint8_t n;
      if ( command != COMMAND_Q ) n = 0; 
      command = COMMAND_Q;

      if ( n == 0 ) {
        
        bot.sendMessage("Очистить всю информацию, включая имена пользователей и расписания? (Yes/No)", msg.chatID);
        n = 1;

      } else if ( n == 1 ) {

        if ( _issueYes(msg.text) ) {

          _saInit();
          _schInit();
          
          if ( _saUpdate() ) {

            bot.sendMessage("Готово. Информация была удалена", msg.chatID);
            n = 2;

          } else {

            // Serial.println("Error (_commandQ)");
            pln("Error (_commandQ)");
            command = 0;

          }

        } else {

          bot.sendMessage("Отмена", msg.chatID);
          command = 0;

        }
        
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




    static uint16_t _isSchList(String s) {

      uint16_t nn = s.toInt();
      
      if ( nn == 0 ) return 1440;

      nn--;
      if ( sch.item[nn].time != 1440 ) return nn;

      return 1440;

    }



    static String _getSchListEdit() {

      String s = "";

      for (uint8_t i = 0; i < SIZE_ITEM; i++) 
        if ( sch.item[i].time < 1440 ) {

          uint8_t h = trunc( sch.item[i].time / 60 );
          uint8_t m = sch.item[i].time % 60;

          s += (String) (i + 1);
          s += ". ";
          if ( h < 10 ) s += "0";
          s += (String) h;
          s += ":";
          if ( m < 10 ) s += "0";
          s += (String) m;
          s += ( sch.item[i].status ) ? " on" : " off";
          s += "\n";

        };

      return s;

    }




    static String _getSchList() {

      String s = "";

      for (uint8_t i = 0; i < SIZE_ITEM; i++) 
        if ( sch.item[i].time < 1440 ) {

          uint8_t h = trunc( sch.item[i].time / 60 );
          uint8_t m = sch.item[i].time % 60;

          if ( sch.item[i].status ) s += "on ";
          if ( ! sch.item[i].status ) s += "— ";
          if ( h < 10 ) s += "0";
          s += (String) h;
          s += ":";
          if ( m < 10 ) s += "0";
          s += (String) m;
          s += " ";
          if ( ! sch.item[i].status ) s += "\n";

        };

      return s;

    }




    static bool _deleteItem(uint16_t nn) {

      for (uint8_t i = nn; i < SIZE_ITEM - 1; i++) {
        
        sch.item[i].time = sch.item[i+1].time;
        sch.item[i].status = sch.item[i+1].status; 

      }

      sch.item[SIZE_ITEM].time = 1440;

      if ( _schUpdate() ) return true;
      return false;
      
    }



    void _initSchEEPROM() {

      EEPROM.get(1024, sch);
    
      if ( ! sch.key.equals(KEY_SCH) ) {

        _schInit();
        if ( _schUpdate() ) Serial.println("The EEPROM is not initialized. Fixed it!");

      }

      // Serial.println(sch.key.buf);
      pln(sch.key.buf);

    }




    static void _schInit() {

      sch.key = KEY_SCH;

      for (uint8_t i = 0; i < SIZE_ITEM; i++) {

        sch.item[i].time = 1440;
        sch.item[i].status = false;

      };

    }



    static bool _schUpdate() {

      bool b = false;

      EEPROM.put(1024, sch);

      if ( EEPROM.commit() ) b = true;
      // else Serial.println("Error (_schUpdate)");
      else pln("Error (_schUpdate)");
      
      return b;
      
    }


    static bool _itemUpdate(ScheduleItem schItem) {

      bool flag = false;

      for (uint8_t i = 0; i < SIZE_ITEM; i++) 
        if ( sch.item[i].time >= 1440 ) {

          sch.item[i] = schItem;
          flag = true;

          break;

        }

      _schSort();
      _schUpdate();

      return flag;
      
    }


    static void _schSort() {

      ScheduleItem t;

      for (uint8_t i = 0; i < SIZE_ITEM - 1; i++) 
      for (uint8_t j = 0; j < SIZE_ITEM - i; j++) 
        if ( sch.item[j].time > sch.item[j + 1].time ) {
 
          if ( sch.item[j].time >= 1440 ) break;
           
          t.time = sch.item[j].time; 
          t.status = sch.item[j].status;

          sch.item[j].time = sch.item[j + 1].time; 
          sch.item[j].status = sch.item[j + 1].status;

          sch.item[j + 1].time = t.time; 
          sch.item[j + 1].status = t.status;

        }
 
    }

};

