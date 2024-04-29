#ifndef TCPCLIENT_H
#define TCPCLIENT_H
#include <QObject>
#include <QTcpSocket>
#include <QDataStream>

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
     * \param n массив quint32
     */
    void SendMeesage(QList<quint32> n);

private:
    void SendToServer(QList<quint32> n);
    QTcpSocket *socket;
    QByteArray Data;
    quint16     m_nNextBlockSize;

public slots:
    void slotReadyRead();
};

#endif // TCPCLIENT_H
