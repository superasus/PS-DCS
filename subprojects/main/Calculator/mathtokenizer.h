#ifndef MATHTOKENIZER_H
#define MATHTOKENIZER_H
#include <qtypes.h>
#include <QByteArray>
#include <QVector>
#include <cctype>
#include <exception>
enum TOKEN_TYPE
{
    NUMBER,
    ARGUMENT,
    OPERATOR,
    FUNC,
    START_SUB_EXPR,
    END_SUB_EXPR
};

class MathTokenizer
{
private:
public:
    class Token
    {
    public:
        Token(const QByteArray& value, TOKEN_TYPE type);
        QByteArray  value() const noexcept;
        constexpr TOKEN_TYPE type() const noexcept {return m_type; }
        bool operator==(const Token& other) const noexcept;
    private:
        TOKEN_TYPE m_type;
        QByteArray m_value;
    };
    explicit MathTokenizer() = default;
    QVector<Token> tokenizeMathExpr(const QByteArray& expr) const;
private:
    Token readFirstToken(const QByteArray& expr, quint32& offset) const;
    Token readNumber(const QByteArray& expr, quint32& offset) const;
    Token readAlphabeticSequence(const QByteArray& expr, quint32& offset) const;
    Token readSubExprToken(const QByteArray& expr, quint32& offset) const noexcept;
    Token readOperatorToken(const QByteArray& expr, quint32& offset) const noexcept;
    bool isMinusToken(
        const QByteArray& expr,
        const quint32 offset,
        const TOKEN_TYPE previousTokenType) const noexcept;
    Token m_multiplicativeTokenOperator = Token("*", TOKEN_TYPE::OPERATOR);
};

#endif // MATHTOKENIZER_H
