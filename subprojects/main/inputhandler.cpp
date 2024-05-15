#include "inputhandler.h"

QByteArray InputHandler::takeFunc(const QString &inviteMessage)
{
    QByteArray answer;
    m_out << inviteMessage;
    m_in >> answer;
    return answer;
}

QList<float> InputHandler::takeData(const QString &inviteMessage)
{
    QByteArray data;
    m_out << inviteMessage;
    m_in >> data;
    return parseData(data);
}

QList<float> InputHandler::parseData(const QByteArray &data)
{
    QList<float> answer;
    QByteArray buf;
    for (const auto sim : data)
    {
        if(sim != ' ') { buf.append(sim); continue; }
        if(buf.size()) {answer.append(buf.toFloat()); buf.clear();}
    }
    return answer;
}
