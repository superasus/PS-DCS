#include "mathtokenizer.h"

MathTokenizer::Token::Token(const QByteArray &value, TOKEN_TYPE type)
    :m_type(type), m_value(value){}

QByteArray MathTokenizer::Token::value() const noexcept
{ return m_value; }

bool MathTokenizer::Token::operator==(const Token &other) const noexcept
{ return m_type==other.type() && m_value == other.value(); }

QVector<MathTokenizer::Token> MathTokenizer::tokenizeMathExpr(const QByteArray &expr) const
{
    QVector<Token> answer;
    quint32 i = 0;
    while (i < expr.size())
    {
        if (expr[i] == ' ' || expr[i] == '\t' || expr[i] == '\n' || expr[i] == '\r') {++i; continue;}
        if (i == 0) {answer.append(readFirstToken(expr, i)); continue;}
        TOKEN_TYPE previousTokenType = answer.back().type();
        if (std::isdigit(expr[i]) ||
            (isMinusToken(expr, i, previousTokenType) && std::isdigit(expr[i+1])))

        {
            if (previousTokenType == TOKEN_TYPE::ARGUMENT)
                answer.append(m_multiplicativeTokenOperator);
            answer.append(readNumber(expr,  i));
        }
        else if (std::isalpha(expr[i]) ||
            (isMinusToken(expr, i, previousTokenType) && std::isalpha(expr[i+1])))
        {
            Token t = readAlphabeticSequence(expr, i);
            if (t.type() == TOKEN_TYPE::ARGUMENT &&
                (previousTokenType == TOKEN_TYPE::NUMBER ||
                    previousTokenType == TOKEN_TYPE::END_SUB_EXPR))

                answer.append(m_multiplicativeTokenOperator);

            answer.append(t);
        }
        else if (expr[i] == '(' || expr[i] == ')')
        {
            Token t = readSubExprToken(expr, i);
            if(t.type() == TOKEN_TYPE::START_SUB_EXPR &&
                (previousTokenType==TOKEN_TYPE::ARGUMENT ||
                    previousTokenType==TOKEN_TYPE::NUMBER))
                answer.append(m_multiplicativeTokenOperator);
            answer.append(t);
        }
        else answer.append(readOperatorToken(expr, i));
    }
    return answer;
}

MathTokenizer::Token MathTokenizer::readFirstToken(const QByteArray &expr, quint32 &offset) const
{
    if (std::isdigit(expr[offset]) || (offset+1 != expr.size() && expr[offset] == '-' && std::isdigit(expr[offset+1])))
        return readNumber(expr,  offset);

    if (std::isalpha(expr[offset]) || (offset+1 != expr.size() && expr[offset] == '-' && std::isalpha(expr[offset+1])))
        return readAlphabeticSequence(expr, offset);

    if (expr[offset] == '(' || expr[offset] == ')')
        return readSubExprToken(expr, offset);

    return readOperatorToken(expr, offset);
}

MathTokenizer::Token MathTokenizer::readNumber(const QByteArray &expr, quint32 &offset) const
{
    QByteArray number;
    number.append(expr[offset++]);
    bool wasPointFlag = false;
    while (offset<expr.size() && (std::isdigit(expr[offset]) || expr[offset] == '.'))
    {
        if(expr[offset] == '.')
        {
            if (wasPointFlag)
            {
                number.append(expr[offset]);
                throw std::invalid_argument("double point in number: " + number);
            }
            wasPointFlag = true;
        }
        number.append(expr[offset++]);
    }
    return Token(number, TOKEN_TYPE::NUMBER);
}

MathTokenizer::Token MathTokenizer::readAlphabeticSequence(const QByteArray &expr, quint32 &offset) const
{
    QByteArray word;
    word.append(expr[offset++]);

    while (offset<expr.size() && std::isalpha(expr[offset]))
        word.append(expr[offset++]);

    return Token(word, word.size() ==1 || (word.size() == 2 && word[0] == '-')  ?
            TOKEN_TYPE::ARGUMENT : TOKEN_TYPE::FUNC);
}

MathTokenizer::Token MathTokenizer::readSubExprToken(const QByteArray &expr, quint32 &offset) const noexcept
{
    if(expr[offset++] == '(') return Token("(", TOKEN_TYPE::START_SUB_EXPR);
    return Token(")", TOKEN_TYPE::END_SUB_EXPR);
}

MathTokenizer::Token MathTokenizer::readOperatorToken(const QByteArray &expr, quint32 &offset) const noexcept
{
    QByteArray value;
    value.append(expr[offset++]);
    return Token(value, TOKEN_TYPE::OPERATOR);
}

bool MathTokenizer::isMinusToken(const QByteArray &expr, const quint32 offset, const TOKEN_TYPE previousTokenType) const noexcept
{
    return offset + 1 != expr.size() && expr[offset] == '-' &&
        (offset == 0 || previousTokenType == TOKEN_TYPE::OPERATOR);
}
