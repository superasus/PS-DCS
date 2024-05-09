#include <QTest>
#include "serializatortests.h"
int main()
{
    SerializatorTests st;
    return QTest::qExec(&st);
}
