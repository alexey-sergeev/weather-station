// Класс-обертка HTTP
// 1.1

#include <ESP8266WebServer.h>
ESP8266WebServer HTTP(80);


#ifdef SET_UPDATE

  #include <ESP8266HTTPUpdateServer.h>
  ESP8266HTTPUpdateServer httpUpdater;

#endif



class HTTP_wrapper { 
  public: 
    
    void init() {

      #ifdef SET_UPDATE

        #ifdef SET_SETTINGS

          httpUpdater.setup(&HTTP, "/firmware", "admin", settings.getAdminPassw().c_str());

        #else

          httpUpdater.setup(&HTTP, "/firmware", LOGIN_UPDATE, PASSWORD_UPDATE);

        #endif
        


        pln("HTTP Update Server ready");

      #endif

      #ifdef SET_SETTINGS


        HTTP.on("/settings", [](){

          // pln(settings.getAdminPassw().c_str());

          if ( ! HTTP.authenticate("admin", settings.getAdminPassw().c_str())) {
            return HTTP.requestAuthentication();
          }

          HTTP.send(200, "text/html", settings.html());
          
        });
        
        HTTP.on("/settingsform", handleSettings);
        pln("HTTP settings ready");

      #endif
     
      HTTP.on("/data", [](){HTTP.send(200, "text/plain", sensors.creatingDataRow());});
      
      HTTP.onNotFound([](){
        if ( !handleFileRead(HTTP.uri()) )
        HTTP.send(404, "text/html", "Not Found");
      });

      HTTP.begin();
      pln("HTTP opened");

  
    }


    void handle() {

      HTTP.handleClient();

    }


  private: 


    static void handleSettings() {

      for (uint8_t i = 0; i < HTTP.args(); i++) settings.set( (String) HTTP.argName(i), (String) HTTP.arg(i) );

      if ( settings.isRight() ) {
        
        settings.save();
        HTTP.send(200, "text/plain", "Saved. Overload the device");

      } else {

        HTTP.send(200, "text/plain", "The administrator password is incorrect");

      }   

    }



    static bool handleFileRead(String path) {

      #ifdef SET_DB 

        if ( path.startsWith("/db/") ) {

          Index index = db.getIndex();
          String sd = ( storage.isSD() ) ? " (SD)" : "";

          String pathRaw = index.day[0].buf;
          if ( path == "/db/d1.csv" ) pathRaw = index.day[1].buf;
          if ( path == "/db/d2.csv" ) pathRaw = index.day[2].buf;
          if ( path == "/db/m0.csv" ) pathRaw = index.month[0].buf;
          if ( path == "/db/m1.csv" ) pathRaw = index.month[1].buf;
          if ( path == "/db/m2.csv" ) pathRaw = index.month[2].buf;
          if ( path == "/db/y0.csv" ) pathRaw = index.year[0].buf;
          if ( path == "/db/y1.csv" ) pathRaw = index.year[1].buf;
          if ( path == "/db/y2.csv" ) pathRaw = index.year[2].buf;

          if ( storage.exists(pathRaw) ) {

            p("HTTP: ");
            p(path);
            p(" (");
            p(pathRaw);
            p(") - yes");
            pln(sd);

            File file = storage.open(pathRaw, "r");
            HTTP.streamFile(file, "text/plain");
            file.close();

            return true;

          }

          p("HTTP: ");
          p(path);
          p(" (");
          p(pathRaw);
          p(") - none");
          pln(sd);
          
          HTTP.send(200, "text/plain", "none");

          return true;

        } else {

          if ( path.endsWith("/") ) path += "index.html";
          String contentType = getContentType(path);

          if ( LittleFS.exists(path) ) {

            p("HTTP: ");
            p(path);
            pln(" - yes");

            File file = LittleFS.open(path, "r");
            HTTP.streamFile(file, contentType);
            file.close();

            return true;

          }
      
          p("HTTP: ");
          p(path);
          pln(" - no");
        
          return false;

        }

      #else

        return false;

      #endif

    }



    static String getContentType(String filename) {

      #ifdef SET_DB

        if (HTTP.hasArg("download")) return "application/octet-stream";
        else if (filename.endsWith(".htm")) return "text/html";
        else if (filename.endsWith(".html")) return "text/html";
        else if (filename.endsWith(".css")) return "text/css";
        else if (filename.endsWith(".js")) return "application/javascript";
        else if (filename.endsWith(".png")) return "image/png";
        else if (filename.endsWith(".gif")) return "image/gif";
        else if (filename.endsWith(".jpg")) return "image/jpeg";
        else if (filename.endsWith(".ico")) return "image/x-icon";
        else if (filename.endsWith(".xml")) return "text/xml";
        else if (filename.endsWith(".pdf")) return "application/x-pdf";
        else if (filename.endsWith(".zip")) return "application/x-zip";
        else if (filename.endsWith(".gz")) return "application/x-gzip";
        return "text/plain";

      #else
    
        return "text/plain";

      #endif

    }


};

HTTP_wrapper http;
