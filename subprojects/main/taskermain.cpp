#include <QCoreApplication>
#include "Calculator/calculator.h"
#include <QTextStream>
#include "UIhandler.h"
#include "Calculator/calculator.h"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    UIHandler uiHandler;
    Calculator calc;
    QString clusterIP = uiHandler.takeIp("Enter cluster node IPv4: ");
    uiHandler.sendMessage("Connecting... \n");
    QByteArray func = uiHandler.takeFunc("Enter your func: ");
    QList<float> data = uiHandler.takeData("Enter your data. Use space like delimiter: ");
    uiHandler.showResult(calc.CalculateFuncRange(func, data));
    return a.exec();
}
