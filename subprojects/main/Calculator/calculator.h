#ifndef CALCULATOR_H
#define CALCULATOR_H
#include <QByteArray>
#include <QVector>
#include "mathsyntaxtree.h"
class Calculator
{
public:
    Calculator() = default;
    /*!
     * \brief Вычисляет значение математического выражения
     * \param <expr> Выражение
     * \return Значение выражения
     */
    float CalculateExpression(const QByteArray& expr) const;

    /*!
     * \brief Вычисляет значение математической функции от переданного аргумента
     * \param <func> Математическая функция
     * \param <funcArg> Значение аргумента функции
     * \return Вычисленное значение
     */
    float CalculateFunc(const QByteArray& func, float funcArg) const;

    /*!
     * \brief Вычисляет значения математической функции от переданных аргументов
     * \param <func> Математическая функция
     * \param <funcArgsRange> Значения аргумента функции
     * \return Массив вычисленных значений функции от соответсвующих аргументов
     */
    QList<float> CalculateFuncRange(
        const QByteArray& func,
        const QList<float>& funcArgsRange);
private:
    static const MathSyntaxTreeConfig m_config;
    static MathSyntaxTree m_mst;
};

#endif // CALCULATOR_H
