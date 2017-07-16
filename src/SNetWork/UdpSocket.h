/*!
 * @file UdpSocket.h
 * @brief Заголовочный файл класса UdpSocket
 * @author Костин А. В., подр. 171
 * @date 07.2017
 */

#ifndef UDPSOCKET_H
#define UDPSOCKET_H

#include "eup_detect_os.h"
#include <sstream>
#include <strings.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#ifdef EUP_QNX_OS
#include <sys/socket.h>
#include <sys/netmgr.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/udp.h>
#include <arpa/inet.h>
#include <pthread.h>
#endif

using namespace std;

/*!
 * @class UdpSocket
 * @brief Класс-обертка для  UDP-сокета
 */
class UdpSocket
{
public:

	//-----------------------------------------------------------------------
	/*!
	 * Конструктор по умолчанию
	 */
    UdpSocket();

	//-----------------------------------------------------------------------
	/*!
	 * Деструктор
	 */
	virtual ~UdpSocket();

	//-----------------------------------------------------------------------
	/*!
	 * Метод привязки сокета к локальному адресу и порту
	 * @param host - Локальный адрес
	 * @param port - Локальный порт
	 * @return Результат выполнения
	 */
	bool Bind(string host, int port);

	//-----------------------------------------------------------------------
	/*!
	 * Получить нативный дескриптор сокета
	 * @return Дескриптор сокета
	 */
	int GetDescriptor();

	//-----------------------------------------------------------------------
	/*!
	 * Получить локальный адрес сокета
	 * @return - Строка с адресом
	 */
	string GetLocalHost();

	//-----------------------------------------------------------------------
	/*!
	 * Получить локальный порт сокета
	 * @return Порт
	 */
	int GetLocalPort();

#ifdef EUP_QNX_OS
    //-----------------------------------------------------------------------
    /*!
     * Получить удаленный адрес сокета
     * @return - Строка с адресом
     */
    string GetRemoteHost();

    //-----------------------------------------------------------------------
    /*!
     * Получить удаленный порт сокета
     * @return - Порт
     */
    int GetRemotePort();
#endif
	//-----------------------------------------------------------------------
	/*!
	 * Метод отправки данных через сокет
	 * @warning Метод блокирующий
	 * @param data - Данные для отправки
	 * @param dataSize - Размер данных (в байтах)
	 * @param host - Адрес получателя
	 * @param port - Порт получателя
	 * @return Количество отправленных байт, -1 в случае ошибки (код ошибки в errno)
	 */
	int SendData(const char *data, int dataSize, string host, int port);

	//-----------------------------------------------------------------------
	/*!
	 * Метод приема данных через сокет
	 * @warning Метод блокирующий
	 * @param buffer - Буфер для приема данных
	 * @param bufferSize - Максимальный размер буфера
	 * @param host - Адрес отправителя
	 * @param port - Порт отправителя
	 * @return Количество принятых байт, -1 в случае ошибки (код ошибки в errno) или в случае разрыва соединения
	 */
    int ReceiveData(char *buffer,  int bufferSize, std::__cxx11::string host = NULL, int *port = NULL);

    //-----------------------------------------------------------------------
    /*!
     * Метод получение строки с ошибкой
     * \return Строку с текстом ошибки
     */
    string getErrorString() { return error_string;}

    //-----------------------------------------------------------------------
    /*!
     * Метод добавления сокета в мультикастовую группу
     * @param [in]mulricast_group - Сетевой адрес мультикастовой группы
     * \return  - Результат выполнения
     */
    bool setMulticastGroup(string mulricast_group);

private:
#ifdef EUP_WINDOWS_OS
    SOCKET socketDescriptor;
#else
    int socketDescriptor;
#endif
	string localHost;
    string error_string;
	int localPort;

    //-----------------------------------------------------------------------
    /**
     * Метод формирования стоки с сообщением ошибки
     * @param[in] err_str - Текст с ошибкой
     */
    PrintMessage(string err_str);

//---------------------------------------------------------------------------
#ifdef EUP_QNX_OS
pthread_mutex_t socketMutex;             //! Мютекс для доступа к дескриптору сокета
#else
CRITICAL_SECTION socketMutex;            //! Мютекс для доступа к дескриптору сокета
#endif

//---------------------------------------------------------------------------
void LockAccess()
{
#ifdef EUP_QNX_OS
    pthread_mutex_lock(&socketMutex);
#else
    EnterCriticalSection(&socketMutex);
#endif
}

//---------------------------------------------------------------------------
void UnlockAccess()
{
#ifdef EUP_QNX_OS
    pthread_mutex_unlock(&socketMutex);
#else
    LeaveCriticalSection(&socketMutex);
#endif
}
};

#endif // UDPSOCKET_H
