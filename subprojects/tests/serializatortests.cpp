#include "serializatortests.h"

void SerializatorTests::QSTRING_TEST()
{
    QString testString = "I'm test string";
    auto serializatorResult =
        Serializator::binaryDeserializeContainers<QString>(
            Serializator::binarySerializeContainers(testString));
    QString answerString = std::get<0>(serializatorResult);
    QCOMPARE(answerString, testString);
}

void SerializatorTests::QVECTOR_TEST()
{
    QVector<float> testVector = {10.43, 5, 95.10};
    auto serializatorResult =
        Serializator::binaryDeserializeContainers<QVector<float>>(
            Serializator::binarySerializeContainers(testVector));
    QVector<float> answerString = std::get<0>(serializatorResult);
    QCOMPARE(answerString, testVector);
}

void SerializatorTests::QLIST_TEST()
{
    QList<float> testList = {10.43, 5, 95.10};
    auto serializatorResult =
        Serializator::binaryDeserializeContainers<QList<float>>(
            Serializator::binarySerializeContainers(testList));
    QList<float> answerString = std::get<0>(serializatorResult);
    QCOMPARE(answerString, testList);
}

void SerializatorTests::MULTIPLE_CONTAINERS_TEST()
{
    QString string = "I'm test string";
    QVector<float> vec = {10.43, 5, 95.10};
    QList<int> list = {1,2,3, 10};

    auto serializatorResult =
        Serializator::binaryDeserializeContainers<QString, QVector<float>, QList<int>>(
            Serializator::binarySerializeContainers(string, vec, list));

    QCOMPARE(std::get<0>(serializatorResult), string);
    QCOMPARE(std::get<1>(serializatorResult), vec);
    QCOMPARE(std::get<2>(serializatorResult), list);
}
