#ifndef UIHANDLER_H
#define UIHANDLER_H
#include <QTextStream>
#include <QList>
#include <QString>
#include <QByteArray>
#include <QDebug>

class UIHandler {

public:
    QByteArray takeFunc(const QString& inviteMessage);
    QList<float> takeData(const QString& inviteMessage);
    QString takeIp(const QString& inviteMessage);
    void sendMessage(const QString& Message);
    void showResult(const QList<float> &data);
    UIHandler() = default;
private:
    QList<float> parseData(const QString&data);
    QTextStream m_in{stdin};
    QTextStream m_out{stdout};
};

#endif // UIHANDLER_H
