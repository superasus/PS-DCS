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
    connect(socket, SIGNAL(disconnected()), socket, SLOT(deleteLater()));
    localClient.push_back(socket);
}

void TcpServer::slotReadyRead()
{
    QTcpSocket* pClientSocket = (QTcpSocket*)sender();
    QDataStream in(pClientSocket);
    in.setVersion(QDataStream::Qt_5_15);
    Message timeStruct;
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
        if (pClientSocket->bytesAvailable() < m_nNextBlockSize)
        {
            break;
        }
        qDebug() << "read...";
        QByteArray byte;
        in >> byte;
        std::tuple<QString,qsizetype, reason, QList<float>> ar = Serializator::binaryDeserialize<QString,qsizetype, reason, QList<float>>(byte);

        timeStruct.function = std::get<0>(ar);
        timeStruct.sizeArray = std::get<1>(ar);
        timeStruct.ReasonForTransfer = std::get<2>(ar);
        timeStruct.dataProtokol.append(std::get<3>(ar));

        QFile file("../serverOut.txt");
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
            return;

        QTextStream out(&file);
        for (int i = 0; i < std::get<3>(ar).size(); ++i) {
            out <<std::get<3>(ar).at(i) << " ";
        }
        file.close();

        m_nNextBlockSize = 0;
    }

    messageProcessing(pClientSocket,timeStruct);
}

void TcpServer::sendToClient(QTcpSocket* pSocket,struct Message data)
{
    QByteArray  arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_15);
    out << quint16(0);
    out << Serializator::binarySerialize(data.function, data.sizeArray, data.ReasonForTransfer, data.dataProtokol);
    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));
    pSocket->write(arrBlock);
}

void TcpServer::sendMeesageToClient(QTcpSocket* pSocket,struct Message dataList)
{
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
         sendToClient(pSocket,timeProtokol);
     }
}

void TcpServer::messageProcessing(QTcpSocket* pSocket, Message dataProcessing)
{
    data.dataProtokol.append(dataProcessing.dataProtokol);
    data.ReasonForTransfer = dataProcessing.ReasonForTransfer;
    data.function = dataProcessing.function;
    data.sizeArray = dataProcessing.sizeArray;
    switch(dataProcessing.ReasonForTransfer)
    {
        case reason::PROBLEMBOOK:
            if(data.dataProtokol.size() == dataProcessing.sizeArray)
            {
                socketProblembook = pSocket;
                if(localClient.size() > 1)
                {
                    localClient.pop_back();
                }
                separationInProblembook(data);
                data.dataProtokol.clear();
                data.ReasonForTransfer = dataProcessing.ReasonForTransfer;
                data.function = "";
                data.sizeArray = 0;
            }
        break;
        case reason::BACK:
            if(data.dataProtokol.size() == dataProcessing.sizeArray)
            {
                data.sizeArray = data.dataProtokol.size();
                data.ReasonForTransfer = END;
                sendMeesageToClient(socketProblembook, data);
                data.dataProtokol.clear();
                data.ReasonForTransfer = dataProcessing.ReasonForTransfer;
                data.function = "";
                data.sizeArray = 0;
            }
        break;
    }
}

void TcpServer::separationInProblembook(Message dataProcessing)
{
    struct Message timeData;
    qsizetype n = localClient.size();
    qsizetype dataSize = dataProcessing.dataProtokol.size();
    qsizetype partSize  = dataSize / n;
    qsizetype extraElements = dataSize % n;
    qsizetype currentIndex = 0;
    for (int i = 0; i < n; i++)
    {
        int currentPartSize = partSize + (i < extraElements ? 1 : 0);
        QList<float> currentPart;

        for (int j = currentIndex; j < currentIndex + currentPartSize; j++)
        {
            currentPart.append(dataProcessing.dataProtokol[j]);
        }
        timeData.dataProtokol.append(currentPart);
        currentIndex += currentPartSize;
        timeData.sizeArray = timeData.dataProtokol.size();
        timeData.ReasonForTransfer = PROCESSING;
        timeData.function = dataProcessing.function;
        sendMeesageToClient(localClient[i], timeData);
        timeData.dataProtokol.clear();
        timeData.sizeArray = 0;
        timeData.function = "";
    }
}
