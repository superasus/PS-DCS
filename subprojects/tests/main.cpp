#include <QTest>
#include "serializatortests.h"
int main()
{
    SerializatorTests st;
    QTest::qExec(&st);
}
