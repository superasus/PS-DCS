#ifndef TASKER_H
#define TASKER_H

#include <QObject>
#include "tcpclient.h"
#include "UIhandler.h"

class Tasker : public QObject {
    Q_OBJECT
public:
    explicit Tasker(const QString& clusterIp);
    void sendDataToCluster(const QByteArray& func, const QList<float>& data);
private:
    TcpClient m_client;
    UIHandler m_uiHandler;
private slots:
    void onDataFromClusterReceived(const Message& mess, QTcpSocket*);

};

#endif // TASKER_H
