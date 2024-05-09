#ifndef SERIALIZATORTESTS_H
#define SERIALIZATORTESTS_H
#include <QTest>
#include <QObject>
#include <QString>
#include <QVector>
#include <QList>
#include <tuple>
#include"../main/serializator.h"

class SerializatorTests : public QObject
{
    Q_OBJECT
private slots:
    void QSTRING_TEST();
    void QVECTOR_TEST();
    void QLIST_TEST();
    void EMPTY_CONYAINER_TEST();
    void MULTIPLE_CONTAINERS_TEST();
    void SIMPLE_STRUCT_VECTOR_TEST();
};

#endif // SERIALIZATORTESTS_H
