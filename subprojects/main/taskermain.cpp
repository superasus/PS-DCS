#include <QCoreApplication>
#include "Calculator/calculator.h"
#include <QTextStream>
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Calculator calc;
    QTextStream in(stdin);
    QTextStream out(stdout);
    QByteArray func, data;
    QList<float> dataToProcess;
    out << "Enter func: ";
    in >> func;
    out << "Enter data. Use space like delmiter";
    in >> data;
    QByteArray buf;
    for (const auto& sim : data) {


    }
    return a.exec();
}
