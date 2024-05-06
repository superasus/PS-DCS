#pragma once
#ifndef SERIALIZATOR_H
#define SERIALIZATOR_H
#include <QString>
#include <QByteArray>
#include <tuple>
#include <qtypes.h>

namespace Serializator
{
namespace{
template <typename T>
void binarySerializeElement(T& element, QByteArray& answer)
{
    uint8_t* buf = reinterpret_cast<uint8_t*>(&element);
    for (quint32 i = 0; i < sizeof(element); i++) answer.append(buf[i]);
}

template <typename T>
void binarySerializeContainer(T& container, QByteArray& answer)
{
    quint32 containerSize = container.size();
    quint32 containerElementSize = sizeof(typename T::value_type);
    binarySerializeElement(containerSize, answer);
    answer.append(containerElementSize);
    for (auto& el : container) binarySerializeElement(el, answer);
}

template <typename T>
T binaryDeserializeElement(const QByteArray& bytes, quint32& byteIndex)
{
    quint8* buf = new quint8[sizeof(T)];
    for (quint32 j = 0; j < sizeof(T); ++j, ++byteIndex) buf[j] = bytes[byteIndex];
    quint32 answer = *(reinterpret_cast<quint32*>(buf));
    delete[] buf;
    return answer;
}

template <typename T>
T binaryDeserializeElement(const QByteArray& bytes, quint32& byteIndex, quint8* allocatedBuf)
{
    for (quint32 j = 0; j < sizeof(T); ++j, ++byteIndex) allocatedBuf[j] = bytes[byteIndex];
    T answer = *(reinterpret_cast<T*>(allocatedBuf));
    return answer;
}


template <typename T>
T binaryDeserializeContainer(const QByteArray& bytes, quint32& byteIndex)
{
    T answer;
    quint32 containerSize = binaryDeserializeElement<quint32>(bytes, byteIndex);
    quint32 elementSize = bytes[byteIndex++];

    if (elementSize != sizeof(typename T::value_type))
    {
        QString errorMessage("Can't serialize bytes to");
        errorMessage.append(typeid(T).name());
        throw std::invalid_argument(errorMessage.toStdString());
    }
    quint8* buf = new quint8[sizeof(typename T::value_type)];
    for (quint32 i = 0; i < containerSize; i++)
    {
        answer.append(binaryDeserializeElement<typename T::value_type>(bytes, byteIndex, buf));
    }
    delete[] buf;
    return answer;
}

template<
    quint32 Lim,
    quint32 I = 0,
    typename Tuple,
    typename Container = std::tuple_element_t <I, Tuple>>

void binaryDeserializeContainersProcess(Tuple& tuple, const QByteArray& bytes, quint32& byteIndex)
{
    std::get<I>(tuple) = binaryDeserializeContainer<Container>(bytes, byteIndex);
    if constexpr (I + 1 != Lim)
        binaryDeserializeContainersProcess<Lim, I + 1>(tuple, bytes, byteIndex);
}
}

template<typename CurrentContainer, typename ... OtherContainers>
QByteArray binarySerializeContainers(CurrentContainer& container, OtherContainers& ... otherContainers)
{
    QByteArray answer;
    binarySerializeContainer(container, answer);
    if constexpr (sizeof...(otherContainers))
    {
        QByteArray additional_bytes = binarySerializeContainers(otherContainers...);
        answer.append(additional_bytes);
    }
    return answer;
}

template<typename ... Types>
std::tuple<Types...> binaryDeserializeContainers(const QByteArray& bytes)
{
    std::tuple<Types...> answer;
    if constexpr (sizeof...(Types))
    {
        quint32 byteIndex = 0;

        binaryDeserializeContainersProcess<sizeof...(Types)>(answer, bytes, byteIndex);
    }
    return answer;
}
};

#endif // !SERIALIZATOR_H


