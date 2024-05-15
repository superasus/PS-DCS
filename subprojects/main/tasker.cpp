#include "tasker.h"

Tasker::Tasker(const QString& clusterIp) : m_client(clusterIp, 2323)
{
    connect(&m_client, &TcpClient::dataReceived, this, &Tasker::onDataFromClusterReceived);
}

void Tasker::sendDataToCluster(const QByteArray &func, const QList<float> &data)
{
    m_client.sendMeesageToServer({func, PROBLEMBOOK, data.size(), data, 0, 0});
}

void Tasker::onDataFromClusterReceived(const Message &mess, QTcpSocket *)
{
    m_uiHandler.showResult(mess.dataProtokol);
}
