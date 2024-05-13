#include "discoveryService.h"
#include <QNetworkInterface>
#include <QTimer>
#include <QRandomGenerator64>

DiscoveryService::DiscoveryService(qint64 port)
{
    identifyIp();
    nodePort = port;
    udpSocket = new QUdpSocket();
    udpSocket->bind(nodePort, QUdpSocket::ShareAddress);
    connect(udpSocket, &QUdpSocket::readyRead,
            this, &DiscoveryService::processPendingData);
}

void DiscoveryService::findNodes(qint64 port)
{
    QTimer timer;
    timer.setInterval(QRandomGenerator::global()->bounded(500));
    timer.start();
    SendBroadcast(port);
}

void DiscoveryService::findNodes()
{
    findNodes(nodePort);
}

void DiscoveryService::identifyIp()
{
    const QHostAddress &localhost = QHostAddress(QHostAddress::LocalHost);
    for (const QHostAddress &address: QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != localhost) {
            nodeIp = address.toString();
        }
    }
    if (nodeIp.isEmpty())
    {
        throw std::runtime_error("Не удалось определить ip адрес машины");
    }
}

qint64 DiscoveryService::SendBroadcast(qint64 port)
{
    if(nodeIp.isEmpty()){
        identifyIp();
    }
    QByteArray datagram = "PS-DCS-ADDNODE:" + nodeIp.toUtf8() + ":" + QByteArray::number(nodePort);
    return udpSocket->writeDatagram(datagram, QHostAddress::Broadcast, port);
}

void DiscoveryService::processPendingData()
{
    QByteArray datagram;
    while (udpSocket->hasPendingDatagrams()) {
        datagram.resize(int(udpSocket->pendingDatagramSize()));
        udpSocket->readDatagram(datagram.data(), datagram.size());
        QString data = datagram.constData();
        if(data.contains("PS-DCS-ADDNODE:")){
            qint16 ipIdx = data.indexOf(":") + 1;
            qint16 portIdx = data.lastIndexOf(":") + 1;
            QString pendingIp = data.mid(ipIdx, portIdx - ipIdx - 1);
            QString pendingPort = data.mid(portIdx);
            if(!ipPull.contains(pendingIp)) {
                ipPull.push_back(pendingIp);
            }
            if(nodeIp.isEmpty()){
                identifyIp();
            }
            QByteArray ansDatagram = "PS-DCS-ANSWER:" + nodeIp.toUtf8();
            QHostAddress host(pendingIp);
            udpSocket->writeDatagram(ansDatagram, host, pendingPort.toInt());
        }
        else if(data.contains("PS-DCS-ANSWER:")){
            QString ip = data.mid(data.indexOf(":") + 1);
            if(!ipPull.contains(ip)) {
                ipPull.push_back(ip);
            }
        }
    }
}

QVector<QString> DiscoveryService::getIpPull()
{
    return ipPull;
}

qint64 DiscoveryService::getNodeCount()
{
    return ipPull.size();
}

DiscoveryService::~DiscoveryService()
{
    udpSocket->abort();
    delete udpSocket;
}
