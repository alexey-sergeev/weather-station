// Класс 

#include <LittleFS.h>

#ifdef SET_SD
  
  #include <SPI.h> 
  #include <SD.h>
  
  #define CS_PIN 15

#endif

// #define IS_DB_SD true


class FS_wrapper { 
  public: 

    void init() {

      LittleFS.begin();
      pln("LittleFS opened");

      #ifdef SET_SD

        _isDbSd = true;

        if ( !SD.begin(CS_PIN) ) {

          _isDbSd = false;
          pln("NOT (!!!) SD opened. The database is stored in LittleFS");

        } else {
          
          pln("SD opened");

        }
      
      #endif
      
    }


  
    bool isSD() {
      
      return _isDbSd;
      
    }



    bool exists(String path) {

      #ifdef SET_SD

        if ( _isDbSd ) return SD.exists(path);
        else

      #endif

      return LittleFS.exists(path);

    }



    File open(String path, const char* rw) {

      #ifdef SET_SD
      
        if ( _isDbSd ) return SD.open(path, ( rw == "r" ) ? FILE_READ : FILE_WRITE);
        else 
      
      #endif

      return LittleFS.open(path, rw);

    }



    bool deleteFile(String path) {
        
      #ifdef SET_SD

        if ( _isDbSd ) return SD.remove(path);
        else
    
      #endif

      return LittleFS.remove(path);

    }



    uint32_t size(String path) {

      File file = open(path, "r");
      uint32_t s = file.size();
      file.close();

      return s;
 
    }
 

    String readFile(const char * path) {

      File file = LittleFS.open(path, "r");

      if (!file) {
        
        return "";
      
      } else {

        // p("Found: ");
        // pln(path);

      }

      String s;
      while ( file.available() ) s += (char) file.read();

      file.close();

      return s;

    }


 

    bool writeFile(const char * path, String data ) {

      bool f = true;

      File file = LittleFS.open(path, "w");
      if ( !file.print(data) ) f = false;
      file.close();

      return f;

    }



  private: 

    // bool _isDbSd = IS_DB_SD;
    bool _isDbSd = false;

};

FS_wrapper storage;
