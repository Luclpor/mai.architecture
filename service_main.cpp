#include "service_service/http_service_server.h"


int main(int argc, char*argv[]) 
{
    HTTPServiceWebServer app;
    return app.run(argc, argv);
}