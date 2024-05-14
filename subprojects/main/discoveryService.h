#ifndef DISCOVERYSERVICE_H
#define DISCOVERYSERVICE_H
#include <QObject>
#include <QUdpSocket>
#include <QVector>
#include <QString>

class DiscoveryService : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief Класс, позволяющий выполнять обнаружение других расчетных узлов в сети и хранить информацию о них
     * \param <port> - Порт, через который будет происходить коммуникация с другими узлами сети
     */
    DiscoveryService(qint64 port);
    /*!
     * \brief Деструктор
     */
    ~DiscoveryService();
    /*!
     * \brief Поиск других расчетных узлов в сети и сохранение их ip адресов через порт текущего расчетного узла
     */
    void findNodes();
    /*!
     * \brief Поиск других расчетных узлов в сети и сохранение их ip адресов через заданный порт
     * \param <port> - Порт, через который будет происходить поиск других узлов сети
     */
    void findNodes(qint64 port);
    /*!
     * \brief Получение перечня ip адресов обнаруженных расчетных узлов в сети
     */
    QVector<QString> getIpPull();
    /*!
     * \brief Получение количества ip адресов обнаруженных расчетных узлов в сети
     */
    qint64 getNodeCount();

private slots:
    void processPendingData();

private:
    qint64 nodePort = 45455;
    QString nodeIp;
    QUdpSocket* udpSocket = nullptr;
    QVector<QString> ipPull;
    qint64 SendBroadcast(qint64 port);
    void identifyIp();
};

#endif // DISCOVERYSERVICE_H
