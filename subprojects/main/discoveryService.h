#ifndef DISCOVERYSERVICE_H
#define DISCOVERYSERVICE_H
#include <QObject>
#include <QUdpSocket>
#include <QVector>
#include <QString>

class DiscoveryService : public QObject
{
    Q_OBJECT
public:
    DiscoveryService(qint64 port);
    ~DiscoveryService();
    void findNodes();
    void findNodes(qint64 port);
    QVector<QString> getIpPull();
    qint64 getNodeCount();

private slots:
    void processPendingData();

private:
    qint64 nodePort = 45455;
    QString nodeIp;
    QUdpSocket* udpSocket = nullptr;
    QVector<QString> ipPull;
    qint64 SendBroadcast(qint64 port);
    void identifyIp();
};

#endif // DISCOVERYSERVICE_H
