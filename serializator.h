#pragma once
#ifndef SERIALIZATOR_H
#define SERIALIZATOR_H
#include <QString>
#include <QByteArray>
#include <tuple>

namespace Serializator
{
namespace{

/*!
 * \brief Выполняет двоичную сериализацию одного элемента
 * \details Элемент должен быть фундаментальным типом или POD структурой.
 * Сложные типы (классы, управляющие данными в куче, другие контейнеры и т.д) не поддерживаются
 * \param <element> - элемент для сериализации
 * \param <answer> - ранее созданный QByteArray, куда будет помещён результат сериализации
 */
template <typename T>
void binarySerializeElement(T& element, QByteArray& answer)
{
    quint8* buf = reinterpret_cast<quint8*>(&element);
    for (quint32 i = 0; i < sizeof(element); i++) answer.append(buf[i]);
}

/*!
 * \brief Выполняет двоичную сериализацию одного контейнера
 * \details Контейнер должен содержать фундаментальные языковые типы или POD структуры.
 * Контейнеры, содержащие сложные типы (классы, управляющие данными в куче, другие контейнеры и т.д)
 * не поддерживаются
 * \param <container> - контейнер для сериализации
 * \param <answer> - ранее созданный QByteArray, куда будет помещён результат сериализации
 */

template <typename T>
void binarySerializeContainer(T& container, QByteArray& answer)
{
    quint32 containerSize = container.size();
    quint32 containerElementSize = sizeof(typename T::value_type);
    binarySerializeElement(containerSize, answer);
    answer.append(containerElementSize);
    for (auto& el : container) binarySerializeElement(el, answer);
}

/*!
 * \brief Выполняет двоичную десериализацию одиночного элемента
 * \details Элемент должен быть фундаментальным типом или POD структурой.
 * Сложные типы (классы, управляющие данными в куче, другие контейнеры и т.д) не поддерживаются
 * \tparam <T> - выходной элемент
 * \param <bytes> - набор байтов для десериализации, содержащий десериализуемый элемент
 * \param <byteIndex> - Индекс первого байта десериализуемого элемента
 * \return элемент типа T, полученный в результате десериализации
 */
template <typename T>
T binaryDeserializeElement(const QByteArray& bytes, quint32& byteIndex)
{
    quint8* buf = new quint8[sizeof(T)];
    for (quint32 j = 0; j < sizeof(T); ++j, ++byteIndex) buf[j] = bytes[byteIndex];
    quint32 answer = *(reinterpret_cast<quint32*>(buf));
    delete[] buf;
    return answer;
}
/*!
 * \brief Выполняет двоичную десериализацию одиночного элемента
 * \details Элемент должен быть фундаментальным типом или POD структурой.
 * Сложные типы (классы, управляющие данными в куче, другие контейнеры и т.д) не поддерживаются
 * \tparam <T> - выходной элемент
 * \param <bytes> - набор байтов для десериализации, содержащий десериализуемый элемент
 * \param <byteIndex> - Индекс первого байта десериализуемого элемента
 * \param <allocatedBuf> - Аллоцированный буфер размера sizeof(T), в котором можно работать.
 * По завершению функции память не освобождается
 * \return элемент типа T, полученный в результате десериализации
 */

template <typename T>
T binaryDeserializeElement(const QByteArray& bytes, quint32& byteIndex, quint8* allocatedBuf)
{
    for (quint32 j = 0; j < sizeof(T); ++j, ++byteIndex) allocatedBuf[j] = bytes[byteIndex];
    T answer = *(reinterpret_cast<T*>(allocatedBuf));
    return answer;
}

/*!
 * \brief Выполняет двоичную десериализацию одного контейнера
 * \details Контейнер должен содержать фундаментальные языковые типы или POD структуры.
 * Контейнеры, содержащие сложные типы (классы, управляющие данными в куче, другие контейнеры и т.д)
 * не поддерживаются
 * \tparam <T> - выходной котейнер
 * \param <bytes> - набор байтов для десериализации, содержащий QT контейнер
 * \param <byteIndex> - Индекс первого байта десериализуемого контейнера
 * \return контейнер T, полученный в результате десериализации
 */
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

/*!
 * \brief запрашивает двоичную десериализацию каждого контейнера в отдельности в том порядке,
 * в котором они идут в выходном кортеже и добавляет результат в него
 * \details Контейнеры должны содержать фундаментальные языковые типы или POD структуры.
 * Контейнеры, содержащие сложные типы (классы, управляющие данными в куче, другие контейнеры и т.д)
 * не поддерживаются
 * \param <tuple> - выходной кортеж
 * \param <bytes> - набор байтов для десериализации, содержащий QT контейнеры
 * \param <byteIndex> - Индекс первого байта десериализуемого контейнера
 */

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

/*!
 * \brief Выполняет двоичную сериализацию N QT-контэйнеров в QByteArray, где N>0.
 * \details Контейнеры должны содержать фундаментальные языковые типы или POD структуры.
 * Контейнеры, содержащие сложные типы (классы, управляющие данными в куче, другие контейнеры и т.д)
 * не поддерживаются
 * \details Контейнеры будут сериализованы в том порядке, в котором были переданы
 * \param <container> - первый контейнер для сериализации
 * \param <otherContainers> - пакет добавочных контейнеров для сериализации (может быть пустым)
 * \return QByteArray, содержащий сериализованный контейнер
 */
template<typename CurrentContainer, typename ... OtherContainers>
QByteArray binarySerializeContainers(const CurrentContainer& container, const OtherContainers& ... otherContainers)
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

/*!
 * \brief Выполняет двоичную десериализацию набора байтов (QByteArray) в N QT-контейнеров, где N>0.
 * \details Набор байтов должен содержать в себе сериализованные контейнеры, содержащие
 * фундаментальные языковые типы или POD структуры.
 * Контейнеры, содержащие сложные типы (классы, управляющие данными в куче, другие контейнеры и т.д)
 * не поддерживаются
 * \tparam <Types...> - Пакет выходных контейнеров, которые должны получиться в ходе десериализации.
 *  Для корректной десериализации контейнеры в пакете должны располагаться в том же порядке,
 *  в котором они были сериализованы
 * \param <bytes> - набор байтов, содержащий сериализованные контейнеры
 * \return std::tuple<Types...>, содержащий запрошенные в пакете Types... контейнеры в том порядке,
 * в котором они были переданы в пакет
 */
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


