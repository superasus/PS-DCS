#include "mathsyntaxtreetest.h"

void MathSyntaxTreeTest::ONE_NUMBER_TEST()
{
    mst.buildTree("3");
    QCOMPARE(mst.value(), 3);
}

void MathSyntaxTreeTest::SIMPLE_EXPRESSIONS_TEST()
{
    mst.buildTree("3+4");
    QCOMPARE(mst.value(), 7);
}

void MathSyntaxTreeTest::MATH_FUNC_TEST()
{
    mst.buildTree("sin(5)");
    QVERIFY(abs(mst.value()+0.9589) < 1e-3);
}

void MathSyntaxTreeTest::EXPRESSION_WITH_SUBEXPRESSION_TEST()
{
    mst.buildTree("7*(3+4)");
    QCOMPARE(mst.value(), 49);
}

void MathSyntaxTreeTest::ONLY_SUBEXPRESSION_TEST()
{
    mst.buildTree("(3+4)");
    QCOMPARE(mst.value(), 7);
}

void MathSyntaxTreeTest::BIG_COOL_EXPRESSION_TEST()
{
    mst.buildTree("7*(2+3)+3^9*12+sqrt(25)");
    QCOMPARE(mst.value(), 236236);
}

void MathSyntaxTreeTest::SINGLE_ARG_TEST()
{
    mst.buildTree("x");
    mst.setArgumentValue(3);
    QCOMPARE(mst.value(), 3);
}

void MathSyntaxTreeTest::SIMPLE_ARG_EXPRESSION_TEST()
{
    mst.buildTree("x+15");
    mst.setArgumentValue(3);
    QCOMPARE(mst.value(), 18);
}

void MathSyntaxTreeTest::TWO_PLACED_ARGS()
{
    mst.buildTree("x+3*x");
    mst.setArgumentValue(5);
    QCOMPARE(mst.value(), 20);
}

void MathSyntaxTreeTest::BIG_COOL_FUNCTION()
{
    mst.buildTree("x*(12/4)^2+(x-8)*2-x^2");
    mst.setArgumentValue(10);
    QCOMPARE(mst.value(), -6);
}

void MathSyntaxTreeTest::CUSTOM_ADD_TEST()
{
    mst.config.binaryOperatorsSet["+"].func = [](float a, float b){ return static_cast<int>(a+b)%2; };
    mst.buildTree("1+1");
    QCOMPARE(mst.value(), 0);
    mst.config=mst.defaultConfig();
}

void MathSyntaxTreeTest::CUSTOM_SIN_TEST()
{
    mst.config.unaryFuncSet["sin"] = [](float a){return pow(a, 3);};
    mst.buildTree("sin(4)");
    QCOMPARE(mst.value(), 64);
    mst.config=mst.defaultConfig();
}

void MathSyntaxTreeTest::BIT_WORK_TEST()
{
    mst.config.binaryOperatorsSet =
    {
        {"|", {0, [](float a, float b){return static_cast<int>(a) | static_cast<int>(b) ;}}},
        {"&", {1, [](float a, float b){return static_cast<int>(a) & static_cast<int>(b) ;}}},
        {"^", {0, [](float a, float b){return static_cast<int>(a) ^ static_cast<int>(b) ;}}}
    };
    mst.buildTree("(5|15&2)^3");
    QCOMPARE(mst.value(), 4);
    mst.config=mst.defaultConfig();
}
