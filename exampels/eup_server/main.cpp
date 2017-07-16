#include <QCoreApplication>
#include "up_server.h"
#include <QObject>

int main(int argc, char *argv[])
{


    QCoreApplication a(argc, argv);

    up_server *server = new up_server();
    if(server->startSender())
        printf("sender is started \n");
    else
        printf("ERORR start sender \n");


    return a.exec();
}
