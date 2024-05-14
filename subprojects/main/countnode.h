#ifndef COUNTNODE_H
#define COUNTNODE_H

#include <QObject>
#include "tcpclient.h"
#include "tcpserver.h"
#include "Calculator/calculator.h"
#include "discovery-service.h"

class CountNode
{
    Q_OBJECT
public:
    CountNode();

private:
    TcpServer m_server{2323};
    QVector<TcpClient*> m_vecClient;
    Calculator m_cal();
    DiscoveryService m_ds();
};

#endif // COUNTNODE_H
