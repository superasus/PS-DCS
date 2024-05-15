#include "calculator.h"

const MathSyntaxTreeConfig Calculator::m_config
    {
        .binaryOperatorsSet
        {
            {"-", {0, [](float a, float b){return a-b;}}},
            {"+", {0, [](float a, float b){return a+b;}}},
            {"*", {1, [](float a, float b){return a*b;}}},
            {"/", {1, [](float a, float b){return a/b;}}},
            {"^", {2, [](float a, float b){return pow(a, b);}}}
        },
        .unaryFuncSet
        {
            {"sin", [](float a){return sinf(a);}},
            {"cos", [](float a){return cosf(a);}},
            {"tan", [](float a){return tanf(a);}},
            {"cotan", [](float a){return cosf(a)/sinf(a);}},
            {"sqrt", [](float a){return sqrt(a);}}
        }
    };

MathSyntaxTree Calculator::m_mst(m_config);

float Calculator::CalculateExpression(const QByteArray &expr) const
{ m_mst.buildTree(expr); return m_mst.value(); }

float Calculator::CalculateFunc(const QByteArray &func, float funcArg) const
{
    m_mst.buildTree(func);
    m_mst.setArgumentValue(funcArg);
    return m_mst.value();
}

QList<float> Calculator::CalculateFuncRange(const QByteArray &func, const QList<float> &funcArgsRange)
{
    m_mst.buildTree(func);
    QList<float> answer;
    for (const auto& arg : funcArgsRange)
    {
        m_mst.setArgumentValue(arg);
        answer.append(m_mst.value());
    }
    return answer;
}
