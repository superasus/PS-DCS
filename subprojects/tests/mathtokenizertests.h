#ifndef MATHTOKENIZERTESTS_H
#define MATHTOKENIZERTESTS_H

#include <QObject>
#include <QTest>
#include "../main/calculator/mathtokenizer.h"

class MathTokenizerTests : public QObject
{
    using Tokens = QVector<MathTokenizer::Token>;
    using Token = MathTokenizer::Token;
    Q_OBJECT
private slots:
    void NUMBERS_TEST();
    void SIMPLE_EXPRESSIONS_TEST();
    void SIMPLE_EXPRESSIONS_WITH_2_OPERATORS_TEST();
    void EXPRESSION_WITH_SUBEXPRESSION_TEST();
    void ONLY_SUBEXPRESSIN_TEST();
    void NESTED_SUBEXPRESSION_TEST();
    void FUNCS_TEST();
    void COMMON_BIG_EXPRESSION_TEST();

    void FIND_UNARY_MINUS();
    void FIND_IMPLICET_MULT();
};

#endif // MATHTOKENIZERTESTS_H
