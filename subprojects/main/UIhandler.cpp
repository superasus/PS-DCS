#include "UIhandler.h"

QByteArray UIHandler::takeFunc(const QString &inviteMessage)
{
    if (m_in.atEnd()) m_in.skipWhiteSpace();
    m_out << inviteMessage;
    m_out.flush();
    return m_in.readLine().toLocal8Bit();
}

QList<float> UIHandler::takeData(const QString &inviteMessage)
{
    if (m_in.atEnd()) m_in.skipWhiteSpace();
    m_out << inviteMessage;
    m_out.flush();
    return parseData(m_in.readLine());
}

QString UIHandler::takeIp(const QString &inviteMessage)
{
    if (m_in.atEnd()) m_in.skipWhiteSpace();
    m_out << inviteMessage;
    m_out.flush();
    return m_in.readLine();
}

void UIHandler::sendMessage(const QString &message)
{
    m_out << message;
    m_out.flush();
}

QList<float> UIHandler::parseData(const QString&data)
{
    QList<float> answer;
    QString buf;
    for (const auto sim : data)
    {
        if(sim != ' ') { buf.append(sim); continue; }
        if(buf.size()) {answer.append(buf.toFloat()); buf.clear();}
    }
   answer.append(buf.toFloat());
    return answer;
}

void UIHandler::showResult(const QList<float> &data)
{
    qDebug() << data;
    m_out << "processed result: ";
    for (const float num : data) {
        m_out << QString::number(num);
        m_out << ' ';
    }
    m_out << '\n';
    m_out.flush();
}
