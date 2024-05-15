#include "calculatortests.h"


void CalculatorTests::CALCULATE_BIG_EXPRESSION_TEST()
{
    QCOMPARE(calc.CalculateExpression("7*(2+3)+3^9*12+sqrt(25)"), 236236);
}

void CalculatorTests::CALCULATE_BIG_FUNC_TEST()
{
    QCOMPARE(calc.CalculateFunc("x*(12/4)^2+(x-8)*2-x^2", 10), -6);
}

void CalculatorTests::CALCULATE_BIG_FUNC_RANGE_TEST()
{
    QList<float> expect = {-6, 2, 8, 12, 14, 14, 12};
    QCOMPARE(calc.CalculateFuncRange("x*(12/4)^2+(x-8)*2-x^2", {1, 2, 3, 4, 5, 6, 7}), expect);
}
