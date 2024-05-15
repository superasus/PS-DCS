#include <QCoreApplication>
#include "Calculator/calculator.h"
#include <QTextStream>
#include "UIhandler.h"
#include "tcpclient.h"
#include "tasker.h"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    UIHandler uiHandler;
    Tasker t(uiHandler.takeIp("Enter cluster node IPv4: "));
    QByteArray func = uiHandler.takeFunc("Enter your func: ");
    QList<float> data = uiHandler.takeData("Enter your data. Use space like delimiter: ");
    t.sendDataToCluster(func, data);
    return a.exec();
}
