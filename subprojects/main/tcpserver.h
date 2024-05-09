#ifndef TCPSERVER_H
#define TCPSERVER_H
#include <QTcpServer>
#include <QTcpSocket>
#include <QVector>
#include <QDataStream>
#include <QString>
#include <QFile>


class TcpServer : public QTcpServer
{
    Q_OBJECT
public:
    TcpServer(int nPort, QObject* parent = 0);
    ~TcpServer();
    QTcpSocket *socket;
    QList<quint32> mass;

private:
    QVector <QTcpSocket*> Sockets;
    QTcpServer* m_ptcpServer;
    QByteArray Data;
    quint16     m_nNextBlockSize; //хранения длины следующего, полученного от сокета блока
    void SendToClient(QTcpSocket* pSocket, const QString& st);

public slots:
    void incomingConnection(qintptr socketDescriptor);;
    void slotReadyRead();
};

#endif // TCPSERVER_H
