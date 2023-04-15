#include "auth_service/http_auth_server.h"
 
 int main(int argc, char *argv[]){
    HTTPAuthWebServer app;
    return app.run(argc, argv);
 }