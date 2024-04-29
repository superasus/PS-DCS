#include "tcpserver.h"

TcpServer::TcpServer(int nPort, QObject* parent) : QTcpServer(parent), m_nNextBlockSize(0)
{
    if (this->listen(QHostAddress::Any, nPort))
    {
        qDebug() << "Server start at port: ";
    }else {
        qDebug() << "Start failure";
    }
}

TcpServer::~TcpServer()
{

}

void TcpServer::incomingConnection(qintptr socketDescriptor)
{
    qDebug() << Q_FUNC_INFO << " new connection";
    QTcpSocket *socket = new QTcpSocket;
    socket->setSocketDescriptor(socketDescriptor);
    connect(socket, SIGNAL(readyRead()),    this, SLOT(slotReadyRead()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(deleteLater()));
}

void TcpServer::slotReadyRead()
{
    QTcpSocket* pClientSocket = (QTcpSocket*)sender();
    QDataStream in(pClientSocket);
    in.setVersion(QDataStream::Qt_5_15);
    for (;;)
    {
        if (!m_nNextBlockSize)
        {
            if (pClientSocket->bytesAvailable() < sizeof(quint16))
            {
                break;
            }
            in >> m_nNextBlockSize;
        }

        if (pClientSocket->bytesAvailable() < m_nNextBlockSize) {
            break;
        }

        qDebug() << "read...";
        QList<quint32> n;
        in >> n;
        QFile file("/home/damir/tex.txt");
        if (file.open(QIODevice::Append | QIODevice::Text)) {
            QTextStream out(&file);
            for (const quint32& data : n) {
                out << data << "\n";
            }
            file.close();
            qDebug() << "Данные успешно записаны в файл.";
        } else {
            qDebug() << "Ошибка открытия файла для записи.";
        }

        mass = n;
        m_nNextBlockSize = 0;
    }
}

void TcpServer::SendToClient(QTcpSocket* pSocket, const QString& str)
{
    Data.clear();
    QDataStream out(&Data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_15);
    out << quint16(0)<< str;
    out.device()->seek(0);
    out << quint16(Data.size() - sizeof(quint16));
    socket->write(Data);
}
