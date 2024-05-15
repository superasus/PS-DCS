#ifndef MESSAGE_H
#define MESSAGE_H
#include <QString>
#include <QList>

enum reason{
    PROBLEMBOOK,
    PROCESSING,
    BACK,
    END
};

/*!
 * \brief The Message struct
 */
struct Message
{
    QByteArray function;
    reason ReasonForTransfer;
    qsizetype sizeArray;
    QList<float> dataProtokol;
    quint32 dataOffset;
    quint32 taskerId;
};

#endif // MESSAGE_H
