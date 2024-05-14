#ifndef TCPCLIENT_H
#define TCPCLIENT_H
#include <QObject>
#include <QTcpSocket>
#include <QDataStream>
#include <QFile>
#include "serializator.h"
#include "message.h"

class TcpClient: public QObject
{
    Q_OBJECT
    /*!
 * \brief TcpClient
 */
public:
    /*!
     * \brief TcpClient
     * \param host - ip сервера
     * \param port - порт сервера
     * \param parent - родительский объект QObject
     */
    TcpClient(const QString host, const qint16 port, QObject* parent = 0);
    /*!
     * Деструктор
     */
    ~TcpClient();
    /*!
     * \brief SendMeesage для отправки массива на сервер
     * \param dataList массив float
     */
    void sendMeesageToServer(struct Message data);

private:
    QTcpSocket *socket;
    quint16 m_nNextBlockSize;
    void sendToServer(struct Message data);
     //cn
    QString func;
    void messageProcessing(QTcpSocket* pSocket, struct Message dataProcessing);
//    struct Message data;
public slots:
    void slotReadyRead();

signals:
    void dataReceived(const Message &data);
};

#endif // TCPCLIENT_H
