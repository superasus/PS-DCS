#ifndef MATHSYNTAXTREETEST_H
#define MATHSYNTAXTREETEST_H

#include <QObject>
#include <QTest>
#include "../main/Calculator/mathsyntaxtree.h"

class MathSyntaxTreeTest : public QObject {
    Q_OBJECT
private:
    MathSyntaxTree mst{};
private slots:
    void ONE_NUMBER_TEST();
    void SIMPLE_EXPRESSIONS_TEST();
    void MATH_FUNC_TEST();
    void EXPRESSION_WITH_SUBEXPRESSION_TEST();
    void ONLY_SUBEXPRESSION_TEST();
    void BIG_COOL_EXPRESSION_TEST();

    void SINGLE_ARG_TEST();
    void SIMPLE_ARG_EXPRESSION_TEST();
    void TWO_PLACED_ARGS();
    void BIG_COOL_FUNCTION();

    void CUSTOM_ADD_TEST();
    void CUSTOM_SIN_TEST();
    void BIT_WORK_TEST();
};

#endif // MATHSYNTAXTREETEST_H
