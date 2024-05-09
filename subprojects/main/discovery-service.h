#ifndef DISCOVERYSERVICE_H
#define DISCOVERYSERVICE_H
#include <QUdpSocket>
#include <QVector>
#include <QString>

class DiscoveryService
{
private:
    QUdpSocket sock;
    QVector<QString> ipPull;
public:
    void findNodes();
    QVector<QString> getIpPull();
};

#endif // DISCOVERYSERVICE_H
