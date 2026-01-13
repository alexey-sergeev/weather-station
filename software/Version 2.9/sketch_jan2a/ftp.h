// Класс-обертка FTP

#include <SimpleFTPServer.h>
FtpServer FTP;

class FTP_wrapper { 
  public: 

    void init() {

      FTP.setCallback(callback);
      FTP.setTransferCallback(transferCallback);

      #ifdef SET_SETTINGS

        String s = settings.getAdminPassw();
        // if ( s == "" ) s = DEFAULT_PASSWORD;

        static char p[20];
        s.toCharArray(p, sizeof(p));

        FTP.begin("admin", p);

      #else

        FTP.begin(LOGIN_FTP, PASSWORD_FTP);
     
      #endif
      
      pln("FTP opened");
    
    }


    void handle() {

      FTP.handleFTP();

    }


  private: 

    static void callback(FtpOperation ftpOperation, unsigned int freeSpace, unsigned int totalSpace) {

      switch (ftpOperation) {

        case FTP_CONNECT:
          // Serial.println(F("FTP: Connected!"));
          pln("FTP: Connected!");
          break;

        case FTP_DISCONNECT:
          // Serial.println(F("FTP: Disconnected!"));
          pln("FTP: Disconnected!");
          break;

        case FTP_FREE_SPACE_CHANGE:
          // Serial.printf("FTP: Free space change, free %u of %u!\n", freeSpace, totalSpace);
          p("FTP: Free space change, free ");
          p(freeSpace);
          p(" of ");
          pln(totalSpace);
          break;

        default:
          break;

      }

    };



    static void transferCallback(FtpTransferOperation ftpOperation, const char* name, unsigned int transferredSize) {

      switch (ftpOperation) {

        case FTP_UPLOAD_START:
          // Serial.println(F("FTP: Upload start!"));
          pln("FTP: Upload start!");
          break;

        case FTP_UPLOAD:
          // Serial.printf("FTP: Upload of file %s byte %u\n", name, transferredSize);
          p("FTP: Upload of file ");
          p(name);
          p(" byte ");
          pln(transferredSize);
          break;

        case FTP_TRANSFER_STOP:
          // Serial.println(F("FTP: Finish transfer!"));
          pln("FTP: Finish transfer!");
          break;

        case FTP_TRANSFER_ERROR:
          // Serial.println(F("FTP: Transfer error!"));
          pln("FTP: Transfer error!");
          break;

        default:
          break;

      }

    };

};

FTP_wrapper ftp;
