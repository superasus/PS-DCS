#ifndef CALCULATORTESTS_H
#define CALCULATORTESTS_H

#include <QTest>
#include <QObject>
#include "../main/Calculator/calculator.h"

class CalculatorTests : public QObject {
    Q_OBJECT

private:
    Calculator calc;
private slots:
    void CALCULATE_BIG_EXPRESSION_TEST();
    void CALCULATE_BIG_FUNC_TEST();
    void CALCULATE_BIG_FUNC_RANGE_TEST();
};

#endif // CALCULATORTESTS_H
