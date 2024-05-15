#include "countnode.h"

CountNode::CountNode()
{
    m_ds.findNodes();
    connect(&m_server, &TcpServer::dataReceived, this, &CountNode::processMessage);
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
    for (quint32 i = 0; i<reducedData.size(); ++i)
    {
        TcpClient client(ipPull[i], 2323);
        client.sendMeesageToServer(
            {
                func,
                reason::PROCESSING,
                reducedData[i].size(),
                reducedData[i]
            });
    }
}

void CountNode::processProcessingMessage(const QByteArray &/*func*/, const QList<float> /*data*/)
{
    QList<float> calculated;
}

void CountNode::processBackMessage(const QList<float> /*data*/)
{

}

void CountNode::ready()
{m_isReady = true; qInfo() << "Node ready!";}

void CountNode::processMessage(const Message &Message)
{
    switch (Message.ReasonForTransfer) {
    case reason::PROCESSING:
        processProcessingMessage(Message.function, Message.dataProtokol);
        break;
    case reason::PROBLEMBOOK:
        processTaskerMessage(Message.function, Message.dataProtokol);
        break;
    case reason::BACK:
        processBackMessage(Message.dataProtokol);
        break;
    }
}


