#include <QCoreApplication>
#include "countnode.h"
#include "tcpclient.h"
#include "tcpserver.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    CountNode cn;
    TcpClient k{"127.0.0.1", 2323};


    QByteArray str = "x^2";
    QList<float> numberList;
    for (int i = 1; i <= 10000; ++i)
    {
         numberList.append(i);
    }

    Message b = {"x^2" ,PROBLEMBOOK,numberList.size(),numberList};
    k.sendMeesageToServer(b);

    return a.exec();
}
