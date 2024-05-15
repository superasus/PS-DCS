#include "countnode.h"

CountNode::CountNode(QObject* parent): QObject(parent)
{
    m_server = new TcpServer(2323);
//    m_ds.findNodes();
    setupConnection();
}

QList<QList<float> > CountNode::mapReduce(const QList<float> &data)
{
    QList<QList<float>> answer;
    auto segmentCount = ceil(data.size() / m_ds.getNodeCount());
    quint32 i = 0;
    for(quint32 j = 0; j < segmentCount; ++j)
    {
        QList<float> buf;
        for (;; ++i)
            buf.append(data[i]);
        answer.append(buf);
    }
    return answer;
}

void CountNode::processTaskerMessage(const QByteArray &func, const QList<float> data)
{
    QList<QList<float>> reducedData = mapReduce(data);
    QVector<QString> ipPull = m_ds.getIpPull();
    ipPull.append("127.0.0.1");
//    m_vecClient.clear();
//    for (quint32 i = 0; i<reducedData.size(); ++i)
//    {
//        TcpClient client(ipPull[i], 2323);
//        m_vecClient.append(client);
//        connect(&m_vecClient.back(), &TcpClient::dataReceived, this, &CountNode::processMessage);
//        m_vecClient.back().sendMeesageToServer(
//        {
//            func,
//            reason::PROCESSING,
//            reducedData[i].size(),
//            reducedData[i],
//            i
//        });

//    }
}

void CountNode::processProcessingMessage(const QByteArray &/*func*/, const QList<float> /*data*/, QTcpSocket* pSocket)
{    
    QList<float> calculated;
    m_server->sendMeesageToClient(pSocket, {});

}

void CountNode::processBackMessage(const QList<float> /*data*/)
{
//получение массивов, сборка, отправка задачнику
}

void CountNode::setupConnection()
{
    connect(m_server, &TcpServer::dataReceived, this, &CountNode::processMessage);

}

void CountNode::processMessage(const Message &Message, QTcpSocket* pSocket)
{
    switch (Message.ReasonForTransfer) {
    case reason::PROBLEMBOOK:
        processTaskerMessage(Message.function, Message.dataProtokol);
        qDebug() << Message.dataProtokol;
        break;
    case reason::PROCESSING:
        processProcessingMessage(Message.function, Message.dataProtokol, pSocket);
        qDebug() << Message.dataProtokol;
        break;
    case reason::BACK:
        //собрать массив
        processBackMessage(Message.dataProtokol);
        break;
    }
}


