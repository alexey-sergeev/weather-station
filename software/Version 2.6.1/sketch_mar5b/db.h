// Класс базы данных

// #define INTERVAL_SAVE_TO_DB_DAY 120000 // 2 * 60 * 1000

// #define LOG_LENGTH 36000 
#define LOG_LENGTH 200000 

#define FILE_DAY "/db/d000.csv"
#define FILE_MONTH "/db/m000.csv"
#define FILE_YEAR "/db/y000.csv"

#define DELETE_OLD_FILE true
#define DELETE_OLD_FILE_SD false

struct Index {
  mString<16> day[3];
  mString<16> month[3];
  mString<16> year[3];
};


class DB { 
  public: 

    void init() {

      _getFileDB();

      for (uint8_t i = 0; i < 3; i++) sizePrint(_index.day[i].buf);
      for (uint8_t i = 0; i < 3; i++) sizePrint(_index.month[i].buf);  
      for (uint8_t i = 0; i < 3; i++) sizePrint(_index.year[i].buf); 

    }


    // Вставка данных
    
    void insert() {

      // Сохранить данные в дневной таблице
      static uint32_t timerDay = 0;
      if ( millis() - timerDay + 1000 > INTERVAL_SAVE_TO_DB_DAY ) {

        // saveBD(N_DAY, data(N_DAY));
        saveBD(N_DAY, sensors.creatingDataRow(N_DAY));
        timerDay = millis();

      }

      // Точное время
      time_t now;
      struct tm * timeinfo;
      time(&now);
      timeinfo = localtime(&now);  
      
      // Сохранить данные в месячной таблице
      static bool _flagMonth = true;
      if ( timeinfo->tm_min > 0 && timeinfo->tm_min < 20 ) {

        if ( _flagMonth ) {
          
          // saveBD(N_MONTH, data(N_MONTH));
          saveBD(N_MONTH, sensors.creatingDataRow(N_MONTH));
          _flagMonth = false;
        
        }

      } else {

        _flagMonth = true;

      }
      
      // Сохранить данные в годовой таблице
      static bool _flagYear = true;
      if ( timeinfo->tm_min > 1 && timeinfo->tm_min < 20 ) {

        if ( _flagYear ) {

          // if ( timeinfo->tm_hour % 4 == 0 ) saveBD(N_YEAR, data(N_YEAR));
          if ( timeinfo->tm_hour % 4 == 0 ) saveBD(N_YEAR, sensors.creatingDataRow(N_YEAR));
          _flagYear = false;

        }

      } else {

        _flagYear = true;

      }

    }


    

    // Сохранить данные в базу данных
    
    void saveBD(uint8_t n, String d) {

      File file = storage.open(getNameFileDB(n).buf, "a");

      p("DB: Insert data ");
      p(d);
      p(" into ");
      p(getNameFileDB(n).buf);
      if ( storage.isSD() ) p(" (SD)");
      pln();

      if ( !file ) {

        // Serial.println("DB: Failed to open file for appending");
        pln("DB: Failed to open file for appending");
        return;

      }

      d += "\n";
      // if ( !file.print(d) ) Serial.println("DB: Append failed");
      if ( !file.print(d) ) pln("DB: Append failed");
          
      file.close();

    }


    Index getIndex() {

      return _index;

    }


    
    // Очистка базы данных

    void purge() {

      time_t now;
      struct tm * timeinfo;
      time(&now);
      timeinfo = localtime(&now);  

      static bool _flagClean = true;
      if ( timeinfo->tm_hour >= 3 && timeinfo->tm_hour <= 24 ) {

        if ( _flagClean ) {

          _purge(N_DAY);
          if ( timeinfo->tm_mday % 6 == 0 ) _purge(N_MONTH);
          if ( timeinfo->tm_mday % 24 == 0 ) _purge(N_YEAR);

          _flagClean = false;

        }

      } else {

        _flagClean = true;

      }

    }




    mString<16> getNameFileDB(uint8_t n) {

      mString path = _index.day[0];
      if ( n == N_MONTH ) path = _index.month[0];
      if ( n == N_YEAR ) path = _index.year[0]; 

      return path;

    }




    mString<16> getNameFileDBOld(uint8_t n) {

      mString path = _index.day[2];
      if ( n == N_MONTH ) path = _index.month[2];
      if ( n == N_YEAR ) path = _index.year[2]; 

      return path;

    }



    void sizePrint(String path) {

      if ( path != "" && storage.exists(path) ) {

        p(path);
        p(" ");
        p(storage.size(path));
        p(" byte");
        if ( storage.isSD() ) p(" (SD)");
        pln();

      }

    }



  private:

    void _getFileDB() {

      if ( storage.isSD() ) {

        #ifdef SET_SD

          if ( SD.exists("/db/") ) {

            File db1 = SD.open("/db/");

            while (true) {

              File file = db1.openNextFile();
              if (!file) break;
              String path = "/db/";
              path += file.name();

              _updateIndex(path);
              file.close();

            }

          }

        #endif

      } else {

        Dir db2 = LittleFS.openDir("/db/");
        while (db2.next()) {
          
          String path = "/db/";
          path += db2.fileName();
          _updateIndex(path);        

        }

      }

      if ( _index.day[0] == "" ) _index.day[0] = FILE_DAY;
      if ( _index.month[0] == "" ) _index.month[0] = FILE_MONTH;
      if ( _index.year[0] == "" ) _index.year[0] = FILE_YEAR;

    }




    void _updateIndex(String f) {

      if ( f.startsWith("/db/d") ) {

        if ( f > (String) _index.day[0].buf ) {

          _index.day[2] = _index.day[1]; 
          _index.day[1] = _index.day[0]; 
          _index.day[0] = f; 

        } else if ( f > (String) _index.day[1].buf ) {
        
          _index.day[2] = _index.day[1]; 
          _index.day[1] = f; 

        } else if ( f > (String) _index.day[2].buf ) {
        
          _index.day[2] = f; 

        }

      }

      if ( f.startsWith("/db/m") ) {

        if ( f > (String) _index.month[0].buf ) {

          _index.month[2] = _index.month[1]; 
          _index.month[1] = _index.month[0]; 
          _index.month[0] = f; 

        } else if ( f > (String) _index.month[1].buf ) {
        
          _index.month[2] = _index.month[1]; 
          _index.month[1] = f; 

        } else if ( f > (String) _index.month[2].buf ) {
        
          _index.month[2] = f; 

        }

      }

      if ( f.startsWith("/db/y") ) {

        if ( f > (String) _index.year[0].buf ) {

          _index.year[2] = _index.year[1]; 
          _index.year[1] = _index.year[0]; 
          _index.year[0] = f; 

        } else if ( f > (String) _index.year[1].buf ) {
        
          _index.year[2] = _index.year[1]; 
          _index.year[1] = f; 

        } else if ( f > (String) _index.year[2].buf ) {
        
          _index.year[2] = f; 

        }

      }

    }





    void _purge(uint8_t n) {

      String path = getNameFileDB(n).buf;
      String old = getNameFileDBOld(n).buf;

      uint32_t count = storage.size(path);

      if ( count > LOG_LENGTH ) {

        uint8_t slash = path.lastIndexOf('/');
        uint8_t point = path.indexOf('.');
        String s = path.substring(++slash, point);
        char s0 = s[0];
        s[0] = '1';
        uint16_t a = s.toInt();
        a++;
        s = (String) a;
        s[0] = s0;
        for (uint8_t i = 0; i < point - slash; i++) path[slash + i] = s[i];

        _updateIndex(path);

        File file = storage.open(path, "a");
        file.close();

        p("File add ");
        p(path);
        if ( storage.isSD() ) p(" (SD)");
        pln();

        if ( old != "" ) {

          if ( ( storage.isSD() && DELETE_OLD_FILE_SD ) || ( ! storage.isSD() && DELETE_OLD_FILE ) ) {
            
            if ( storage.deleteFile(old) ) {

              p("File deleted ");
              p(old);
              if ( storage.isSD() ) p(" (SD)");
              pln();

            } else {

              p("Delete failed ");
              p(old);
              if ( storage.isSD() ) p(" (SD)");
              pln();

            };

          }
          
          if ( ! storage.isSD() && DELETE_OLD_FILE ) storage.deleteFile(old);

        }

      }

    }

    Index _index;

};

DB db;