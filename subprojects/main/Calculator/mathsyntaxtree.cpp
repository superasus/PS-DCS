#include "mathsyntaxtree.h"

MathSyntaxTree::NumberNode::NumberNode(float value)
    : Node(NODE_TYPE::NUMBER), m_value(value) {}

float MathSyntaxTree::NumberNode::value() const noexcept
{return m_value;}

MathSyntaxTree::OperatorNode::OperatorNode(OperatorData data)
    : Node(NODE_TYPE::OPERATOR), m_func(data.func), m_priority(data.priority) {}

float MathSyntaxTree::OperatorNode::value() const noexcept
{
    return m_func(left->value(), right->value());
}

constexpr quint32 MathSyntaxTree::OperatorNode::priority() const noexcept
{return m_priority;}

float MathSyntaxTree::value() const
{
    if(!m_root) throw std::invalid_argument("Tree not builded!");
    if(m_hasArgumen && !m_isArgumentSetted) throw std::invalid_argument("Arg value not setted!");
    return m_root->value();
}

void MathSyntaxTree::emplaceOperatorNode(QSharedPointer<OperatorNode> node)
{
    if (!m_root) {m_root = node; return;}
    QSharedPointer<Node> buf = m_root;
    while (true)
    {
        QSharedPointer<OperatorNode> currentOperator = buf.staticCast<OperatorNode>();
        if(node->priority() > currentOperator->priority())
        {
            if(!currentOperator->right)
            {
                currentOperator->right = node;
                node->parent = currentOperator;
                return;
            }
            buf = currentOperator->right;
            continue;
        }
        else {emplaceUpperLevelOperatorNode(currentOperator, node); return;}
    }
}

void MathSyntaxTree::emplaceLastNode(QSharedPointer<Node> node)
{
    if (!m_root) {m_root = node; return;}
    QSharedPointer<Node> buf = m_root;
    while(true)
    {
        if (buf->type() == NODE_TYPE::OPERATOR)
        {
            auto currentNode = buf.staticCast<OperatorNode>();
            if (!currentNode->right) {currentNode->right = node; return;}
            buf = currentNode->right;
            continue;
        }
        throw std::logic_error("syntax error!");
    }
}

void MathSyntaxTree::emplaceLastNode(
    QSharedPointer<Node> node,
    QSharedPointer<Node> subtree)
{
    QSharedPointer<Node> buf = subtree;
    while(true)
    {
        if (buf->type() == NODE_TYPE::OPERATOR)
        {
            auto currentNode = buf.staticCast<OperatorNode>();
            if (!currentNode->right) {currentNode->right = node; return;}
            buf = currentNode->right;
            continue;
        }
        throw std::logic_error("syntax error!");
    }
}

void MathSyntaxTree::emplaceUpperLevelOperatorNode(
    QSharedPointer<OperatorNode> place,
    QSharedPointer<OperatorNode> newNode)
{
    if (place == m_root) m_root = newNode;
    newNode->left = place;
    if(place->parent.isNull()) return;
    auto parent = place->parent.toStrongRef();
    newNode->parent = place->parent;
    place->parent = newNode;
    if (parent->type() == NODE_TYPE::OPERATOR)
    {
        auto parentOperator =parent.staticCast<OperatorNode>();
        parentOperator->right = newNode;
    }

}

MathSyntaxTree::Tokens MathSyntaxTree::readSubExpr(
    const Tokens &tokens,
    quint32 &offset) const
{
    if (tokens[offset].type() == TOKEN_TYPE::START_SUB_EXPR) ++offset;
    Tokens answer;
    while (tokens[offset].type() != TOKEN_TYPE::END_SUB_EXPR)
        answer.append(tokens[offset++]);
    return answer;
}

MathSyntaxTree::MathSyntaxTree(const MathSyntaxTreeConfig &config)
    : config (config){}

void MathSyntaxTree::buildTree(const QByteArray &expr)
{
    buildTree(m_tokinizer.tokenizeMathExpr(expr));
}

void MathSyntaxTree::buildTree(const Tokens& tokens)
{
    m_root.clear();
    m_root = nullptr;
    QSharedPointer<Node> buf;
    for (quint32 i = 0; i < tokens.size(); ++i)
    {
        QByteArray tokenValue = tokens[i].value();
        bool isMinus = tokenValue[0] == '-';
        switch (tokens[i].type()) {
        case TOKEN_TYPE::OPERATOR:
        {
            if (!config.binaryOperatorsSet.contains(tokenValue)) throw std::invalid_argument("syntaxError!");
            auto newNode = QSharedPointer<OperatorNode>::create(config.binaryOperatorsSet[tokenValue]);
            emplaceOperatorNode(newNode);
            if(!newNode->left) {newNode->left = buf; continue;}
            emplaceLastNode(buf, newNode->left);
            continue;
        }

        case TOKEN_TYPE::START_SUB_EXPR:
        {
            buf = buildSubTree(readSubExpr(tokens, i));
            continue;
        }

        case TOKEN_TYPE::FUNC:
        {
            if (isMinus) tokenValue = tokenValue.remove(0, 1);
            if(!config.unaryFuncSet.contains(tokenValue))  throw std::invalid_argument("syntaxError!");
            auto func = config.unaryFuncSet[tokenValue];
            buf = QSharedPointer<FuncNode>::create(func, buildSubTree(readSubExpr(tokens, ++i)), isMinus);
            continue;
        }

        case TOKEN_TYPE::NUMBER:
        {
            buf = QSharedPointer<NumberNode>::create(tokenValue.toFloat());
            continue;
        }

        case TOKEN_TYPE::ARGUMENT:
        {
            buf = QSharedPointer<ArgumentNode>::create(m_ArgValue, isMinus);
            m_hasArgumen = true;
            continue;
        }

        case TOKEN_TYPE::END_SUB_EXPR:
            throw std::invalid_argument("syntaxError!");
        }
    }
    emplaceLastNode(buf);
}

QSharedPointer<MathSyntaxTree::Node> MathSyntaxTree::buildSubTree(const Tokens &tokens)
{
    MathSyntaxTree newTree(config, m_ArgValue);
    newTree.buildTree(tokens);
    return newTree.m_root;
}

void MathSyntaxTree::setArgumentValue(float value)
{*m_ArgValue = value; m_isArgumentSetted = true; }

MathSyntaxTreeConfig MathSyntaxTree::defaultConfig() const noexcept
{return m_defaultConfig;}

MathSyntaxTree::Node::Node(NODE_TYPE type): m_type(type){}

constexpr MathSyntaxTree::NODE_TYPE MathSyntaxTree::Node::type() const noexcept
{ return m_type; }


MathSyntaxTree::FuncNode::FuncNode(std::function<float (float)> func, QSharedPointer<Node> arg, bool isMinus)
    : Node(NODE_TYPE::FUNC), m_isMinus(isMinus), m_func(func), m_arg(arg) {}

float MathSyntaxTree::FuncNode::value() const noexcept
{return m_func(m_arg->value()) * (m_isMinus ? -1 : 1);}

MathSyntaxTree::ArgumentNode::ArgumentNode(QSharedPointer<float> value, bool isMinus)
    : Node(NODE_TYPE::ARGUMENT), m_isMinus(isMinus), m_value(value){}

float MathSyntaxTree::ArgumentNode::value() const noexcept
{return *m_value * (m_isMinus ? -1 : 1);}

MathSyntaxTree::MathSyntaxTree(const MathSyntaxTreeConfig &config, QSharedPointer<float> argValue)
    : config(config), m_ArgValue(argValue){}
