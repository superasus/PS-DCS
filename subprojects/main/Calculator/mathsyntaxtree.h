#ifndef MATHSYNTAXTREE_H
#define MATHSYNTAXTREE_H
#include <QByteArray>
#include <QSharedPointer>
#include <QWeakPointer>
#include <QMap>
#include <cmath>
#include <functional>
#include "mathtokenizer.h"

struct OperatorData
{
    quint32 priority;
    std::function<float(float, float)> func;
};

struct MathSyntaxTreeConfig
{
    QMap<QByteArray, OperatorData> binaryOperatorsSet = {};
    QMap<QByteArray, std::function<float(float)>> unaryFuncSet = {};
};

class MathSyntaxTree {
private:
    using Tokens = QVector<MathTokenizer::Token>;
    using ArgPointer = QSharedPointer<QSharedPointer<float>>;
    class Node;

    MathSyntaxTreeConfig m_defaultConfig
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

public:
    using BinaryOperatorsSet = QMap<QByteArray, OperatorData>;
    using UnaryFuncSet = QMap<QByteArray, std::function<float(float)>>;
    explicit MathSyntaxTree() = default;
    MathSyntaxTree(const MathSyntaxTreeConfig& config);
    MathSyntaxTree(const MathSyntaxTree&) = delete;
    MathSyntaxTree(const MathSyntaxTree&&) = delete;
    void buildTree(const QByteArray& expr);
    void buildTree(const Tokens& tokens);
    float value() const;
    QSharedPointer<Node> root() const noexcept;
    void setArgumentValue(float value);
    MathSyntaxTreeConfig config = m_defaultConfig;
    MathSyntaxTreeConfig defaultConfig() const noexcept;
private:

    enum NODE_TYPE
    {
        NUMBER,
        OPERATOR,
        FUNC,
        ARGUMENT
    };

    class Node
    {
    public:
        explicit Node(NODE_TYPE type);
        virtual float value() const noexcept = 0;
        constexpr NODE_TYPE type() const noexcept;
        virtual ~Node() = default;
        QWeakPointer<Node> parent;
    private:
        NODE_TYPE m_type;
    };

    class NumberNode final : public Node
    {
    public:
        NumberNode(float value);
        float value() const noexcept override;
    private:
        float m_value;
    };

    class OperatorNode final : public Node
    {
    public:
        explicit OperatorNode(OperatorData data);
        float value() const noexcept override;
        constexpr quint32 priority() const noexcept;
        QSharedPointer<Node> left = nullptr;
        QSharedPointer<Node> right = nullptr;
    private:
        std::function<float(float, float)> m_func;
        quint32 m_priority;
    };

    class FuncNode final : public Node
    {
    public:
        explicit FuncNode(std::function<float(float)> func, QSharedPointer<Node> arg, bool isMinus);
        float value() const noexcept override;
    private:
        bool m_isMinus;
        std::function<float(float)> m_func;
        QSharedPointer<Node> m_arg = nullptr;
    };

    class ArgumentNode final : public Node
    {
    public:
        explicit ArgumentNode(QSharedPointer<float> value, bool isMinus);
        float value() const noexcept override;
    private:
        bool m_isMinus;
        QSharedPointer<float> m_value = nullptr;
    };
    MathSyntaxTree(const MathSyntaxTreeConfig& config, QSharedPointer<float> argValue);
    void emplaceOperatorNode(QSharedPointer<OperatorNode> node);
    void emplaceLastNode(QSharedPointer<Node> node);
    void emplaceLastNode(QSharedPointer<Node> node, QSharedPointer<Node> subtree);

    void emplaceUpperLevelOperatorNode(
        QSharedPointer<OperatorNode> place,
        QSharedPointer<OperatorNode> newNode);

    Tokens readSubExpr(const Tokens& tokens, quint32& offset) const;
    QSharedPointer<Node> buildSubTree(const Tokens& tokens);


    QSharedPointer<float> m_ArgValue = QSharedPointer<float>::create(0);
    QSharedPointer<Node> m_root = nullptr;
    bool m_hasArgumen = false;
    bool m_isArgumentSetted = false;
    MathTokenizer m_tokinizer;
};

#endif // MATHSYNTAXTREE_H
