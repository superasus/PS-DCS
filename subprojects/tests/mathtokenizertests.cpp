#include "mathtokenizertests.h"
void MathTokenizerTests::NUMBERS_TEST()
{
    MathTokenizer mt;

    QByteArray simpleNumber = "12";
    QByteArray zeroNumber = "0";
    QByteArray floatNumber = "12.155";
    QByteArray zeroFloat = "0.155";

    Tokens received1 = mt.tokenizeMathExpr(simpleNumber);
    Tokens received2 = mt.tokenizeMathExpr(zeroNumber);
    Tokens received3 = mt.tokenizeMathExpr(floatNumber);
    Tokens received4 = mt.tokenizeMathExpr(zeroFloat);

    Tokens expect1 =
        {
            Token("12", TOKEN_TYPE::NUMBER)
        };

    Tokens expect2 =
        {
            Token("0", TOKEN_TYPE::NUMBER)
        };

    Tokens expect3 =
        {
            Token("12.155", TOKEN_TYPE::NUMBER)
        };

    Tokens expect4 =
        {
            Token("0.155", TOKEN_TYPE::NUMBER)
        };

    QCOMPARE(received1, expect1);
    QCOMPARE(received2, expect2);
    QCOMPARE(received3, expect3);
    QCOMPARE(received4, expect4);
}

void MathTokenizerTests::SIMPLE_EXPRESSIONS_TEST()
{
    MathTokenizer mt;
    QByteArray expr1 = "12+4";
    QByteArray expr2 = "12-4";
    QByteArray expr3 = "12*4";
    QByteArray expr4 = "12/4";
    QByteArray expr5 = "12^4";
    Tokens received1 = mt.tokenizeMathExpr(expr1);
    Tokens received2 = mt.tokenizeMathExpr(expr2);
    Tokens received3 = mt.tokenizeMathExpr(expr3);
    Tokens received4 = mt.tokenizeMathExpr(expr4);
    Tokens received5 = mt.tokenizeMathExpr(expr5);
    Tokens expect1 =
    {
        Token("12", TOKEN_TYPE::NUMBER),
        Token("+", TOKEN_TYPE::OPERATOR),
        Token("4", TOKEN_TYPE::NUMBER)
    };

    Tokens expect2 =
        {
            Token("12", TOKEN_TYPE::NUMBER),
            Token("-", TOKEN_TYPE::OPERATOR),
            Token("4", TOKEN_TYPE::NUMBER)
        };

    Tokens expect3 =
        {
            Token("12", TOKEN_TYPE::NUMBER),
            Token("*", TOKEN_TYPE::OPERATOR),
            Token("4", TOKEN_TYPE::NUMBER)
        };

    Tokens expect4 =
        {
            Token("12", TOKEN_TYPE::NUMBER),
            Token("/", TOKEN_TYPE::OPERATOR),
            Token("4", TOKEN_TYPE::NUMBER)
        };

    Tokens expect5 =
        {
            Token("12", TOKEN_TYPE::NUMBER),
            Token("^", TOKEN_TYPE::OPERATOR),
            Token("4", TOKEN_TYPE::NUMBER)
        };
    QCOMPARE(received1, expect1);
    QCOMPARE(received2, expect2);
    QCOMPARE(received3, expect3);
    QCOMPARE(received4, expect4);
    QCOMPARE(received5, expect5);
}

void MathTokenizerTests::SIMPLE_EXPRESSIONS_WITH_2_OPERATORS_TEST()
{
    MathTokenizer mt;

    QByteArray expr1 = "12+4*6";
    QByteArray expr2 = "5.66-2+3.33";
    QByteArray expr3 = "123.123*77.77-144";

    Tokens received1 = mt.tokenizeMathExpr(expr1);
    Tokens received2 = mt.tokenizeMathExpr(expr2);
    Tokens received3 = mt.tokenizeMathExpr(expr3);

    Tokens expect1 =
        {
            Token("12", TOKEN_TYPE::NUMBER),
            Token("+", TOKEN_TYPE::OPERATOR),
            Token("4", TOKEN_TYPE::NUMBER),
            Token("*", TOKEN_TYPE::OPERATOR),
            Token("6", TOKEN_TYPE::NUMBER)
        };

    Tokens expect2 =
        {
            Token("5.66", TOKEN_TYPE::NUMBER),
            Token("-", TOKEN_TYPE::OPERATOR),
            Token("2", TOKEN_TYPE::NUMBER),
            Token("+", TOKEN_TYPE::OPERATOR),
            Token("3.33", TOKEN_TYPE::NUMBER)
        };

    Tokens expect3 =
        {
            Token("123.123", TOKEN_TYPE::NUMBER),
            Token("*", TOKEN_TYPE::OPERATOR),
            Token("77.77", TOKEN_TYPE::NUMBER),
            Token("-", TOKEN_TYPE::OPERATOR),
            Token("144", TOKEN_TYPE::NUMBER)
        };
    QCOMPARE(received1, expect1);
    QCOMPARE(received2, expect2);
    QCOMPARE(received3, expect3);
}

void MathTokenizerTests::EXPRESSION_WITH_SUBEXPRESSION_TEST()
{
    MathTokenizer mt;
    QByteArray expr = "12+(3-4)";
    Tokens received = mt.tokenizeMathExpr(expr);
    Tokens expected =
    {
        Token("12", TOKEN_TYPE::NUMBER),
        Token("+", TOKEN_TYPE::OPERATOR),
        Token("(", TOKEN_TYPE::START_SUB_EXPR),
        Token("3", TOKEN_TYPE::NUMBER),
        Token("-", TOKEN_TYPE::OPERATOR),
        Token("4", TOKEN_TYPE::NUMBER),
        Token(")", TOKEN_TYPE::END_SUB_EXPR),
    };
    QCOMPARE(received, expected);
}

void MathTokenizerTests::ONLY_SUBEXPRESSIN_TEST()
{
    MathTokenizer mt;
    QByteArray expr = "(3+5)";
    Tokens received = mt.tokenizeMathExpr(expr);
    Tokens expected =
        {
            Token("(", TOKEN_TYPE::START_SUB_EXPR),
            Token("3", TOKEN_TYPE::NUMBER),
            Token("+", TOKEN_TYPE::OPERATOR),
            Token("5", TOKEN_TYPE::NUMBER),
            Token(")", TOKEN_TYPE::END_SUB_EXPR),
        };
    QCOMPARE(received, expected);
}

void MathTokenizerTests::NESTED_SUBEXPRESSION_TEST()
{
    MathTokenizer mt;
    QByteArray expr = "((3+2)+(2-3))";
    Tokens received = mt.tokenizeMathExpr(expr);
    Tokens expected =
        {
            Token("(", TOKEN_TYPE::START_SUB_EXPR),
            Token("(", TOKEN_TYPE::START_SUB_EXPR),
            Token("3", TOKEN_TYPE::NUMBER),
            Token("+", TOKEN_TYPE::OPERATOR),
            Token("2", TOKEN_TYPE::NUMBER),
            Token(")", TOKEN_TYPE::END_SUB_EXPR),
            Token("+", TOKEN_TYPE::OPERATOR),
            Token("(", TOKEN_TYPE::START_SUB_EXPR),
            Token("2", TOKEN_TYPE::NUMBER),
            Token("-", TOKEN_TYPE::OPERATOR),
            Token("3", TOKEN_TYPE::NUMBER),
            Token(")", TOKEN_TYPE::END_SUB_EXPR),
            Token(")", TOKEN_TYPE::END_SUB_EXPR),
        };
    QCOMPARE(received, expected);
}

void MathTokenizerTests::FUNCS_TEST()
{
    MathTokenizer mt;
    QByteArray expr1 = "sin(15)";
    QByteArray expr2 = "cos(15)";
    QByteArray expr3 = "lg(15)";
    Tokens received1 = mt.tokenizeMathExpr(expr1);
    Tokens received2 = mt.tokenizeMathExpr(expr2);
    Tokens received3 = mt.tokenizeMathExpr(expr3);
    Tokens expected1 =
        {
            Token("sin", TOKEN_TYPE::FUNC),
            Token("(", TOKEN_TYPE::START_SUB_EXPR),
            Token("15", TOKEN_TYPE::NUMBER),
            Token(")", TOKEN_TYPE::END_SUB_EXPR)
        };

    Tokens expected2 =
        {
            Token("cos", TOKEN_TYPE::FUNC),
            Token("(", TOKEN_TYPE::START_SUB_EXPR),
            Token("15", TOKEN_TYPE::NUMBER),
            Token(")", TOKEN_TYPE::END_SUB_EXPR)
        };

    Tokens expected3 =
        {
            Token("lg", TOKEN_TYPE::FUNC),
            Token("(", TOKEN_TYPE::START_SUB_EXPR),
            Token("15", TOKEN_TYPE::NUMBER),
            Token(")", TOKEN_TYPE::END_SUB_EXPR)
        };
    QCOMPARE(received1, expected1);
    QCOMPARE(received2, expected2);
    QCOMPARE(received3, expected3);
}

void MathTokenizerTests::COMMON_BIG_EXPRESSION_TEST()
{
    MathTokenizer mt;
    QByteArray expr = "(3^17+sin(3))+(2-lg(10))+(2-3+7)";
    Tokens received = mt.tokenizeMathExpr(expr);
    Tokens expected =
        {
            Token("(", TOKEN_TYPE::START_SUB_EXPR),
            Token("3", TOKEN_TYPE::NUMBER),
            Token("^", TOKEN_TYPE::OPERATOR),
            Token("17", TOKEN_TYPE::NUMBER),
            Token("+", TOKEN_TYPE::OPERATOR),
            Token("sin", TOKEN_TYPE::FUNC),
            Token("(", TOKEN_TYPE::START_SUB_EXPR),
            Token("3", TOKEN_TYPE::NUMBER),
            Token(")", TOKEN_TYPE::END_SUB_EXPR),
            Token(")", TOKEN_TYPE::END_SUB_EXPR),
            Token("+", TOKEN_TYPE::OPERATOR),
            Token("(", TOKEN_TYPE::START_SUB_EXPR),
            Token("2", TOKEN_TYPE::NUMBER),
            Token("-", TOKEN_TYPE::OPERATOR),
            Token("lg", TOKEN_TYPE::FUNC),
            Token("(", TOKEN_TYPE::START_SUB_EXPR),
            Token("10", TOKEN_TYPE::NUMBER),
            Token(")", TOKEN_TYPE::END_SUB_EXPR),
            Token(")", TOKEN_TYPE::END_SUB_EXPR),
            Token("+", TOKEN_TYPE::OPERATOR),
            Token("(", TOKEN_TYPE::START_SUB_EXPR),
            Token("2", TOKEN_TYPE::NUMBER),
            Token("-", TOKEN_TYPE::OPERATOR),
            Token("3", TOKEN_TYPE::NUMBER),
            Token("+", TOKEN_TYPE::OPERATOR),
            Token("7", TOKEN_TYPE::NUMBER),
            Token(")", TOKEN_TYPE::END_SUB_EXPR),
        };
    QCOMPARE(received, expected);
}

void MathTokenizerTests::FIND_UNARY_MINUS()
{
    MathTokenizer mt;
    QByteArray simpleNum = "-3";
    QByteArray simpleArg = "-x";
    QByteArray simpleFunc = "-lg";
    QByteArray simpleExpr = "2+-3";

    Tokens receivedNum = mt.tokenizeMathExpr(simpleNum);
    Tokens receivedArg = mt.tokenizeMathExpr(simpleArg);
    Tokens receivedFunc = mt.tokenizeMathExpr(simpleFunc);
    Tokens receivedExpr = mt.tokenizeMathExpr(simpleExpr);

    Tokens expectNum = {Token("-3", TOKEN_TYPE::NUMBER)};
    Tokens expectArg = {Token("-x", TOKEN_TYPE::ARGUMENT)};
    Tokens expectFunc = {Token("-lg", TOKEN_TYPE::FUNC)};
    Tokens expectExpr =
    {
        Token("2", TOKEN_TYPE::NUMBER),
        Token("+", TOKEN_TYPE::OPERATOR),
        Token("-3", TOKEN_TYPE::NUMBER)
    };

    QCOMPARE(receivedNum, expectNum);
    QCOMPARE(receivedArg, expectArg);
    QCOMPARE(receivedFunc, expectFunc);
    QCOMPARE(receivedExpr, expectExpr);
}

void MathTokenizerTests::FIND_IMPLICET_MULT()
{
    MathTokenizer mt;
    QByteArray simpleArg = "2x";
    QByteArray betweenArg = "2x3";
    QByteArray subExprArg = "x(2+3)";
    QByteArray subExprNum = "2(2+3)";
    QByteArray funcNum = "2sin(x)";

    Tokens receivedArg = mt.tokenizeMathExpr(simpleArg);
    Tokens receivedArgBetween = mt.tokenizeMathExpr(betweenArg);
    Tokens receivedSubExprArg = mt.tokenizeMathExpr(subExprArg);
    Tokens receivedSubExprNum = mt.tokenizeMathExpr(subExprNum);
    Tokens receivedFuncNum = mt.tokenizeMathExpr(funcNum);
    Tokens expectArg =
        {
            Token("2", TOKEN_TYPE::NUMBER),
            Token("*", TOKEN_TYPE::OPERATOR),
            Token("x", TOKEN_TYPE::ARGUMENT)
        };
    Tokens expectArgBetween =
        {
            Token("2", TOKEN_TYPE::NUMBER),
            Token("*", TOKEN_TYPE::OPERATOR),
            Token("x", TOKEN_TYPE::ARGUMENT),
            Token("*", TOKEN_TYPE::OPERATOR),
            Token("3", TOKEN_TYPE::NUMBER)
        };
    Tokens expectSubExprArg =
        {
            Token("x", TOKEN_TYPE::ARGUMENT),
            Token("*", TOKEN_TYPE::OPERATOR),
            Token("(", TOKEN_TYPE::START_SUB_EXPR),
            Token("2", TOKEN_TYPE::NUMBER),
            Token("+", TOKEN_TYPE::OPERATOR),
            Token("3", TOKEN_TYPE::NUMBER),
            Token(")", TOKEN_TYPE::END_SUB_EXPR),
        };
    Tokens expectSubExprNum =
        {
            Token("2", TOKEN_TYPE::NUMBER),
            Token("*", TOKEN_TYPE::OPERATOR),
            Token("(", TOKEN_TYPE::START_SUB_EXPR),
            Token("2", TOKEN_TYPE::NUMBER),
            Token("+", TOKEN_TYPE::OPERATOR),
            Token("3", TOKEN_TYPE::NUMBER),
            Token(")", TOKEN_TYPE::END_SUB_EXPR),
        };

    Tokens expectFuncNum =
        {
            Token("2", TOKEN_TYPE::NUMBER),
            Token("*", TOKEN_TYPE::OPERATOR),
            Token("sin", TOKEN_TYPE::FUNC),
            Token("(", TOKEN_TYPE::START_SUB_EXPR),
            Token("x", TOKEN_TYPE::ARGUMENT),
            Token(")", TOKEN_TYPE::END_SUB_EXPR)
        };

    QCOMPARE(receivedArg, expectArg);
    QCOMPARE(receivedArgBetween, expectArgBetween);
    QCOMPARE(receivedSubExprArg, expectSubExprArg);
    QCOMPARE(receivedSubExprNum, expectSubExprNum);
    QCOMPARE(receivedFuncNum, expectFuncNum);
}
