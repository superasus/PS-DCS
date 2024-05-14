#ifndef CALCULATOR_H
#define CALCULATOR_H
#include <QByteArray>
#include <QVector>
#include "mathsyntaxtree.h"
class Calculator
{
public:
    Calculator() = default;
    float CalculateExpression(const QByteArray& expr) const;
    float CalculateFunc(const QByteArray& func, float funcArg) const;
    QVector<float> CalculateFuncRange(const QByteArray& func, const QVector<float>& funcArgsRange);
private:
    static const MathSyntaxTreeConfig m_config;
    static MathSyntaxTree m_mst;
};

#endif // CALCULATOR_H
