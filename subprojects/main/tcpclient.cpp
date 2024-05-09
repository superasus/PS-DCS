#include "tcpclient.h"


TcpClient::TcpClient(const QString host, const qint16 port, QObject* parent): QObject(parent), m_nNextBlockSize(0)
{
    socket = new QTcpSocket(this);
    socket->connectToHost(host, port);
    connect(socket, SIGNAL(readyRead()), SLOT(slotReadyRead()));
    connect(socket, &QTcpSocket::disconnected, this, &TcpClient::deleteLater);
}

TcpClient::~TcpClient(){

}

void TcpClient::SendToServer(QList<quint32> n)
{
    QByteArray  arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_15);
    out << quint16(0);
    out << n;
    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));
    socket->write(arrBlock);
}


void TcpClient::SendMeesage(QList<quint32> dataList){
    QStringList sentChunks;
    int dataSize = dataList.size();
     for (int i = 0; i < dataSize; i += 5000)
     {
         int end = qMin(i + 5000, dataSize);
         QList<quint32> chunk = dataList.mid(i, end - i);
         SendToServer(chunk);
     }
}

void TcpClient::slotReadyRead()
{
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_5_15);
    for (;;)
    {
        if (!m_nNextBlockSize)
        {
            if (socket->bytesAvailable() < sizeof(quint16))
            {
                break;
            }
            in >> m_nNextBlockSize;
        }

        if (socket->bytesAvailable() < m_nNextBlockSize)
        {
            break;
        }
    }
    QString str;
    in >> str;
    qDebug() << str;
    m_nNextBlockSize = 0;
}



