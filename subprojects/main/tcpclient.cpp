#include "tcpclient.h"


TcpClient::TcpClient(const QString host, const qint16 port, QObject* parent): QObject(parent), m_nNextBlockSize(0)
{
    socket = new QTcpSocket(this);
    socket->connectToHost(host, port);
    connect(socket, SIGNAL(readyRead()), SLOT(slotReadyRead()));
}

TcpClient::~TcpClient()
{

}

void TcpClient::sendToServer(struct Message data)
{
    QByteArray  arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_15);
    out << quint16(0);
    out << Serializator::binarySerialize(data.function, data.sizeArray, data.ReasonForTransfer, data.dataProtokol, data.dataOffset);
    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));
    socket->write(arrBlock);
}

void TcpClient::sendMeesageToServer(struct Message dataList){
    struct Message timeProtokol;
    int dataSize = dataList.dataProtokol.size();
     for (int i = 0; i < dataSize; i += 5000)
     {
         int end = qMin(i + 5000, dataSize);
         QList<float> chunk = dataList.dataProtokol.mid(i, end - i);
         timeProtokol.dataProtokol = chunk;
         timeProtokol.function = dataList.function;
         timeProtokol.ReasonForTransfer = dataList.ReasonForTransfer;
         timeProtokol.sizeArray = dataList.sizeArray;
         timeProtokol.dataOffset = dataList.dataOffset;
         sendToServer(timeProtokol);
     }
}

void TcpClient::slotReadyRead()
{
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_5_15);
    Message data;
    for (;;)
    {
        if (!m_nNextBlockSize)
        {
            if (socket->bytesAvailable() < static_cast<qint64>(sizeof(quint16)))
            {
                break;
            }
            in >> m_nNextBlockSize;
        }
        if (socket->bytesAvailable() < m_nNextBlockSize)
        {
            break;
        }
        qDebug() << "read...";
        QByteArray byte;
        in >> byte;
        std::tuple<QByteArray,qsizetype, reason, QList<float>, quint32> ar =
            Serializator::binaryDeserialize<QByteArray,qsizetype, reason, QList<float>, quint32>(byte);
        data.function = std::get<0>(ar);
        data.sizeArray = std::get<1>(ar);
        data.ReasonForTransfer = std::get<2>(ar);
        data.dataProtokol.append(std::get<3>(ar));
        m_nNextBlockSize = 0;
    }
    if(data.dataProtokol.size()== data.sizeArray)
    {
        emit dataReceived(data);
        data.dataProtokol.clear();
    }
}



