#ifndef COUNTNODE_H
#define COUNTNODE_H

#include <QObject>
#include "discoveryService.h"
#include "tcpclient.h"
#include "tcpserver.h"
#include "Calculator/calculator.h"
#include "discoveryService.h"
#include <QSharedPointer>
class CountNode : public QObject
{
    Q_OBJECT
public:
    CountNode(QObject* parent = 0);

private:
    using SocketPtr = QSharedPointer<QTcpSocket>;
    using ClientPtr = QSharedPointer<TcpClient>;

    struct TaskerData
    {
        SocketPtr socket;
        QList<float> dataToReturn;
        quint32 partCount;
        quint32 partReceived;
    };

    QList<QList<float>> mapReduce(const QList<float>& data);

    TcpServer m_server{2323};
    QVector<ClientPtr> m_vecClient;
    QMap<quint32, TaskerData> m_taskersData;
    Calculator m_calc;
    DiscoveryService m_ds{45455};
    bool m_isReady;

    void processTaskerMessage(
        const QByteArray& func,
        const QList<float> data,
        QSharedPointer<QTcpSocket> taskerSocket);

    void processProcessingMessage(
        const quint32 taskerId,
        const quint32 offset,
        const QByteArray& func,
        const QList<float> data,
        QTcpSocket* pSocket);

    void processBackMessage(const quint32 taskerId, const quint32 dataOffset, const QList<float> data);
    void setupConnection();

    quint32 currentAvailableTaskerId = 0;
public slots:
//    void ready() ;
    void processMessage(const Message& Message, QTcpSocket* pSocket = nullptr);
};

#endif // COUNTNODE_H
