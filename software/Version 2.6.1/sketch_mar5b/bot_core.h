// v 1.2

#include <FastBot.h>
#include <EEPROM.h>
#include "mString.h"

#define INVITATION_TIME 300000

#define COMMAND_SAY_HELLO 1
#define COMMAND_MENU 2
#define COMMAND_INVITATION 3
#define COMMAND_I 4
#define COMMAND_Q 5
#define COMMAND_LIST 6
#define COMMAND_DELETE 7


FastBot bot(BOT_TOKEN);

// #define SIZE_USER 25

struct SecurityAttribute { 
  mString<10> key;
  mString<12> admin;
  mString<25> adminName;
  mString<12> user[SIZE_USER];
  mString<25> userName[SIZE_USER];
};

SecurityAttribute sa;
uint16_t command = 0;
uint16_t invitation;
uint32_t invitationTime;

class bot_core {

  public:

    // Инициализация 

    void init() {

      bot.attach(newMsg);
      _initEEPROM();

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

            if ( msg.text == "/delete_user" || command == COMMAND_DELETE ) _commandDelete(msg);
            if ( msg.text == "/invitation" ) _commandInvitation(msg);
            if ( msg.text == "/q" || command == COMMAND_Q ) _commandQ(msg);

          }
        } else _setUser(msg);
      } else _setAdmin(msg);

    }


   
    void tick() {

      bot.tick();
      
    }


  protected:



    // "Поздороваться"

    static void _commandSayHello(FB_msg& msg) {

      command = COMMAND_SAY_HELLO;

      const char* hello[] = { "Привет", "Здравствуйте", "Добрый день", "Приветствую", "Доброго времени суток" };
      bot.sendMessage(hello[random(sizeof(hello)/sizeof(hello[0]))], msg.chatID);

    }


    static void _commandMenu(FB_msg& msg) {

      command = COMMAND_MENU;

      if ( _isUser(msg.userID) ) {
        
        bool f = false;
        if ( _isAdmin(msg.userID) ) f = true;

        String s = "";

        s += "<b>Меню</b>\n";

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



    static void _commandI(FB_msg& msg) {

      command = COMMAND_I;
      bot.sendMessage(WiFi.SSID(), msg.chatID);

    }



    static void _commandList(FB_msg& msg) {

      command = COMMAND_LIST;
      
      String s = "";

      s = _getList();
      if ( s == "" ) s = "Список пуст";
    
      s = "<b>Пользователи:</b>\n\n" + s;

      bot.setTextMode(FB_HTML);
      bot.sendMessage(s, msg.chatID);

    }



    static void _commandDelete(FB_msg& msg) {

      static uint8_t n;
      static uint8_t nn = 0;

      if ( command != COMMAND_DELETE ) n = 0; 
      command = COMMAND_DELETE;

      if ( n == 0 ) {
      
        String s = _getList();

        if ( s == "" ) s = "Список пуст";
        else s += "\nНомер в списке, который необходимо удалить?";
        bot.sendMessage(s, msg.chatID);
        n = 1;

      } else if ( n == 1 ) {

        nn = _isUserList(msg.text);
        // Serial.println(nn);

        if (  nn != 255 ) {
          
          bot.sendMessage("Удалить пользователя " + (String) sa.userName[nn].buf + "?  (Yes/No)", msg.chatID);
          n = 2;

        } else {
          
          bot.sendMessage("Номер был введен неправильно", msg.chatID);
          n = 1;

        }
        
      } else if ( n == 2 ) {

        if ( _issueYes(msg.text) ) {

          String s;

          if ( _deleteUser(nn) ) {

            s = "Пользователь был удален\n\n";
            s += _getList();

          } else {

            s = "Ошибка";

          }

          bot.sendMessage(s, msg.chatID);
          command = 0;
          nn = 0;

        } else {

          bot.sendMessage("Отмена", msg.chatID);
          command = 0;

        }

      }

    }



    static bool _deleteUser(uint8_t nn) {

      sa.user[nn].clear();
      sa.userName[nn].clear();

      if ( _saUpdate() ) return true;
      return false;
      
    }


    static uint8_t _isUserList(String s) {

      uint8_t nn = s.toInt();
      
      if ( nn == 0 ) return 255;

      uint8_t ii = 1;

      for (uint8_t i = 0; i < SIZE_USER; i++) 
        if ( ! sa.user[i].equals("") )
          if ( nn == ii++ ) return i;

      return 255;

    }



    static String _getList() {

      String s = "";
      uint8_t ii = 1;

      for (uint8_t i = 0; i < SIZE_USER; i++) 
        if ( ! sa.user[i].equals("") ) {

          s += (String) ii++;
          s += ". ";
          s += sa.userName[i].buf;
          s += " (";
          s += sa.user[i].buf[0];
          s += sa.user[i].buf[1];
          s += sa.user[i].buf[2];
          s += "...)";
          s += "\n";

      }

      return s;

    }



    static void _commandInvitation(FB_msg& msg) {

      static uint8_t n;
      command = COMMAND_INVITATION;

      invitation = 1000 + random(9000);
      invitationTime = millis();

      String s = "";
      s += "Приглашение было создано: ";
      s += (String) invitation;

      bot.sendMessage(s, msg.chatID);

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
          if ( _saUpdate() ) {

            bot.sendMessage("Готово. Информация была удалена", msg.chatID);
            n = 2;

          } else {

            Serial.println("Error (_commandQ)");
            command = 0;

          }

        } else {

          bot.sendMessage("Отмена", msg.chatID);
          command = 0;

        }
        
      } 

    }



    static void _setUser(FB_msg& msg) {

      String t = msg.text;

      if ( invitation == 0 || 
           invitation != t.toInt() || 
           millis() - invitationTime > INVITATION_TIME ) return; 

      for (uint8_t i = 0; i < SIZE_USER; i++)
        if ( sa.user[i].equals("") ) {

        sa.user[i] = msg.userID;
        sa.userName[i] = msg.username;

        if ( _saUpdate() ) {

          bot.sendMessage("Готово. Вы являетесь пользователем", msg.chatID);
          return;

        } 

        invitation = 0;

      }

      bot.sendMessage("Память закончилась", msg.chatID);

    }



    static void _addUser(FB_msg& msg) {

          for (uint8_t i = 0; i < SIZE_USER; i++)
            if ( sa.user[i].equals("") ) {

            sa.user[i] = msg.userID;
            sa.userName[i] = msg.username;

            _saUpdate();

            break;

          }

    }



    static void _setAdmin(FB_msg& msg) {

      if ( ! _issueYes(msg.text) ) {
        
        bot.sendMessage("Администратор не определен. Вы хотите быть администратором? (Yes/No)", msg.chatID);
        // Serial.println("Администратор не определен. Вы хотите быть администратором? (Yes/No)");
        pln("Администратор не определен. Вы хотите быть администратором? (Yes/No)");

      } else {

        sa.admin = msg.userID;
        sa.adminName = msg.username;

        if ( _saUpdate() ) {

          bot.sendMessage("Готово. Вы являетесь администратором", msg.chatID);
          // Serial.println("Готово. Вы являетесь администратором");
          // Serial.println(sa.admin.buf);
          pln("Готово. Вы являетесь администратором");
          pln(sa.admin.buf);
       
        } 

      }

    }



    static bool _issueYes(String& q) {

      bool b = false;

      if ( q == "Yes" || q == "yes" || q == "Y" || q == "y" || 
           q == "Да" || q == "да" || q == "Д" || q == "д" ) b = true;

      return b;

    }



    static bool _issueNo(String& q) {

      bool b = false;

      if ( q == "No" || q == "no" || q == "N" || q == "n" || 
           q == "Нет" || q == "нет" || q == "Н" || q == "н" ) b = true;

      return b;

    }




    void _initEEPROM() {

      EEPROM.begin(2048);
      EEPROM.get(0, sa);
    
      pln(sa.key.buf);
      p(sa.admin.buf);
      p(" (");
      p(sa.adminName.buf);
      pln(")");

      if ( ! sa.key.equals(KEY) ) {

        _saInit();
        if ( _saUpdate() ) Serial.println("The EEPROM is not initialized. Fixed it!");

      }

      pln(sizeof(sa));

    }



    static void _saInit() {

        sa.key = KEY;
        sa.admin.clear();
        sa.adminName.clear();

        for (uint8_t i = 0; i < SIZE_USER; i++) {

          sa.user[i].clear();
          sa.userName[i].clear();

        };

    }


    static bool _saUpdate() {

      bool b = false;

      EEPROM.put(0, sa);

      if ( EEPROM.commit() ) b = true;
      else pln("Error (_saUpdate)");
      
      return b;
     
    }


   // v 1.1 

    static String _userToString() {

      String s = "";

      if ( ! sa.admin.equals("") ) {

          s += sa.admin.buf;
          s += ",";

      }

      for (uint8_t i = 0; i < SIZE_USER; i++)
        if ( ! sa.user[i].equals("") ) {

          s += sa.user[i].buf;
          s += ",";
 
      }

      return s;

    }



    static bool _isAdmin(String& u) {

      return u == (String) sa.admin.buf ;
     
    }


    static bool _isUser(String& u) {

      if ( _isAdmin(u) ) return true;

      for (uint8_t i = 0; i < SIZE_USER; i++)
        if ( u == (String) sa.user[i].buf ) return true;
      
      return false;
     
    }

    
};

