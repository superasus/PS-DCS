#ifndef COUNTNODE_H
#define COUNTNODE_H

#include <QObject>
#include "discoveryService.h"
#include "tcpclient.h"
#include "tcpserver.h"
#include "Calculator/calculator.h"
#include "discoveryService.h"
class CountNode : public QObject
{
    Q_OBJECT
public:
    CountNode();

private:
    QList<QList<float>> mapReduce(const QList<float>& data);

    TcpServer m_server{2323};
    QVector<TcpClient*> m_vecClient;
    Calculator m_cal();
    DiscoveryService m_ds{45455};
    bool m_isReady;
    void processTaskerMessage(const QByteArray& func, const QList<float> data);
    void processProcessingMessage(const QByteArray& func, const QList<float> data);
    void processBackMessage(const QList<float> data);
private slots:
    void ready();
    void processMessage(const Message& Message);
};

#endif // COUNTNODE_H
