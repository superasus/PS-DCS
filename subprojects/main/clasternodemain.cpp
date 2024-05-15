#include <QCoreApplication>
#include "countnode.h"
#include "tcpclient.h"
#include "tcpserver.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    CountNode cn;


    return a.exec();
}
