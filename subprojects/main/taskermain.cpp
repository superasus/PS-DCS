#include <QCoreApplication>
#include "Calculator/calculator.h"
#include <QTextStream>
#include "UIhandler.h"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Calculator calc;
    UIHandler uiHandler;
    QByteArray func = uiHandler.takeFunc("Enter your func: ");
    QList<float> data = uiHandler.takeData("Enter your data. Use space like delimiter: ");
    uiHandler.showResult(calc.CalculateFuncRange(func, data));

    return a.exec();
}
