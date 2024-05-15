#include <QCoreApplication>
#include "Calculator/calculator.h"
#include <QTextStream>
#include "UIhandler.h"
#include "tcpclient.h"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    UIHandler uiHandler;
    QString clusterIP = uiHandler.takeIp("Enter cluster node IPv4: ");
    uiHandler.sendMessage("Connecting... \n");
    TcpClient client(clusterIP, 2323);
    QByteArray func = uiHandler.takeFunc("Enter your func: ");
    QList<float> data = uiHandler.takeData("Enter your data. Use space like delimiter: ");
    TcpClient::connect(&client, &TcpClient::dataReceived, [&uiHandler](const Message& mes, QTcpSocket*)
        {uiHandler.showResult(mes.dataProtokol);});
    return a.exec();
}
