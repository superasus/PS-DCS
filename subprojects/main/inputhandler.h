#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H
#include <QTextStream>
#include <QList>
#include <QString>
#include <QByteArray>
class InputHandler {

public:
    QByteArray takeFunc(const QString& inviteMessage);
    QList<float> takeData(const QString& inviteMessage);
    InputHandler() = default;
private:
    QList<float> parseData(const QByteArray& data);
    QTextStream m_in{stdin};
    QTextStream m_out{stdout};
};

#endif // INPUTHANDLER_H
