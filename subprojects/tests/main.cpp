#include <QTest>
#include "serializatortests.h"
#include "tokenizertests.h"
int main()
{
    TokenizerTests tt;
    return QTest::qExec(&tt);
}
