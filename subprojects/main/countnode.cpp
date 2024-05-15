#include "countnode.h"

CountNode::CountNode(QObject* parent): QObject(parent)
{
    m_ds.findNodes();
    setupConnection();
}

QList<QList<float> > CountNode::mapReduce(const QList<float> &data)
{
    QList<QList<float>> answer;
    quint64 nodeCount = m_ds.getNodeCount() + 1;
    quint32 segmentLength = ceil(data.size() / (nodeCount + 1));
    quint32 i = 0;
    for(quint32 j = 0; j < nodeCount; ++j)
    {
        QList<float> buf;
        for (quint64 k = 0; i<data.size() && k<segmentLength; ++i, ++k)
            buf.append(data[i]);
        answer.append(buf);
    }
    return answer;
}

void CountNode::processTaskerMessage(
    const QByteArray &func,
    const QList<float> data,
    QSharedPointer<QTcpSocket> taskerSocket)
{
    QList<QList<float>> reducedData = mapReduce(data);
    quint32 segmentCount = reducedData.size();
    QList<float> returnDataBuf(data.size());
    m_taskersData[currentAvailableTaskerId] = {taskerSocket, returnDataBuf, segmentCount, 0};
    QVector<QString> ipPull = m_ds.getIpPull();
    ipPull.append("127.0.0.1");
    m_vecClient.clear();
    quint32 dataOffset = 0;
    for (quint32 i = 0; i < reducedData.size(); ++i)
    {
        ClientPtr client = ClientPtr::create(ipPull[i], 2323);
        m_vecClient.append(client);
        connect(m_vecClient.back().data(), &TcpClient::dataReceived, this, &CountNode::processMessage);
        m_vecClient.back()->sendMeesageToServer(
            {
                func,
                reason::PROCESSING,
                reducedData[i].size(),
                reducedData[i],
                dataOffset,
                currentAvailableTaskerId
            });
        dataOffset+=reducedData[i].size();
        ++currentAvailableTaskerId;
    }
}

void CountNode::processProcessingMessage(
    const quint32 taskerId,
    const quint32 dataOffset,
    const QByteArray &func,
    const QList<float> data,
    QTcpSocket* pSocket)
{    
    QList<float> calculated = m_calc.CalculateFuncRange(func, data);
    m_server.sendMeesageToClient(pSocket, {func, reason::BACK, calculated.size(), calculated, dataOffset, taskerId});
}

void CountNode::processBackMessage(const quint32 taskerId, const quint32 dataOffset, const QList<float> data)
{
    for (quint32 i = 0; i < data.size(); ++i)
        m_taskersData[taskerId].dataToReturn[i+dataOffset] = data[i];
    ++m_taskersData[taskerId].partReceived;
    if (m_taskersData[taskerId].partReceived == m_taskersData[taskerId].partCount)
    {
        m_server.sendMeesageToClient(m_taskersData[taskerId].socket.data(),
            {"", reason::END, m_taskersData[taskerId].dataToReturn.size(), m_taskersData[taskerId].dataToReturn, 0, taskerId});
        m_taskersData.remove(taskerId);
    }
}

void CountNode::setupConnection()
{
    connect(&m_server, &TcpServer::dataReceived, this, &CountNode::processMessage);

}

void CountNode::processMessage(const Message &message, QTcpSocket* pSocket)
{
    switch (message.ReasonForTransfer) {
    case reason::PROBLEMBOOK:
        processTaskerMessage(message.function, message.dataProtokol, SocketPtr(pSocket));
        qDebug() << message.dataProtokol;
        break;
    case reason::PROCESSING:
        processProcessingMessage(message.dataOffset, message.taskerId, message.function, message.dataProtokol, pSocket);
        qDebug() << message.dataProtokol;
        break;
    case reason::BACK:
        //собрать массив
        processBackMessage(message.taskerId, message.dataOffset, message.dataProtokol);
        break;
    }
}


