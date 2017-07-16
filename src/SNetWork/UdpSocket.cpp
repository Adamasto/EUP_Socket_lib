/*!
 * @file UdpSocket.cpp
 * @brief Файл с исходным кодом класса UdpSocket
 * @author Костин А. В., подр. 171
 * @date 07.2016
 */



#include "UdpSocket.h"

//---------------------------------------------------------------------------
UdpSocket::UdpSocket()
{
#if defined (EUP_QNX_OS)
    pthread_mutex_init(&socketMutex, NULL);
#elif defined (EUP_WINDOWS_OS)
    InitializeCriticalSection(&socketMutex);

    //Иннициализация WinSocket
    WORD wVersionRequested = MAKEWORD (2,2);
    WSADATA wsaData;
    int err  = WSAStartup(wVersionRequested, &wsaData);
    if(err != 0)
        return;

#endif

    if ((socketDescriptor = (int)socket(AF_INET, SOCK_DGRAM, 0)) < 0)
        return;

    const int optval = 1;
    if(setsockopt(socketDescriptor, SOL_SOCKET, SO_REUSEADDR,(const char *)&optval, sizeof(optval)) < 0)
        return;

	localHost = "0.0.0.0";
	localPort = 0;
}

//---------------------------------------------------------------------------
UdpSocket::~UdpSocket()
{
	close(socketDescriptor);
}

//---------------------------------------------------------------------------
bool UdpSocket::Bind(string host, int port)
{
	sockaddr_in localAddr;
	memset(&localAddr, 0, sizeof(localAddr));
	localAddr.sin_family = AF_INET;
	localAddr.sin_port = htons(port);


#if defined (EUP_QNX_OS) || defined(EUP_LINUX_OS)
    localAddr.sin_addr.s_addr = inet_addr(host.c_str());
    localAddr.sin_len = sizeof(localAddr);

    if(host.size() != 0)
    {
        hostent *hp = gethostbyname(host.c_str());
        if(hp == NULL)
        {
            PrintMessage("UdpSocket: Неизвестный хост " + host);
            return false;
        }
        else
        {
            localAddr.sin_addr = *(in_addr*)hp->h_addr;
        }
    }
    else
    {
        localAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    }
#else
    localAddr.sin_addr.s_addr = htonl(INADDR_ANY);
#endif


	// Привязываем сокет к указанному адресу и порту
    if(bind(socketDescriptor, (sockaddr *)&localAddr, sizeof(localAddr)) < 0)
	{

        ostringstream sout;
        sout << "UdpSocket: Ошибка привязки сокета к " << host << ":" << port <<" Код ошибки: " << errno;
        PrintMessage(sout.str());
        return false;

	}

	localHost = host;
	localPort = port;

	return true;
}

//---------------------------------------------------------------------------
int UdpSocket::GetDescriptor()
{
	return socketDescriptor;
}

//---------------------------------------------------------------------------
string UdpSocket::GetLocalHost()
{
#ifdef EUP_QNX_OS
    sockaddr_in socketAddr;
    socklen_t addrLen = sizeof(sockaddr_in);

    pthread_mutex_lock(&socketMutex);
    int ret = getsockname(socketDescriptor, (sockaddr*)&socketAddr, &addrLen);
    pthread_mutex_unlock(&socketMutex);

    if(ret == -1)
        return "0.0.0.0";

    char buffer[INET_ADDRSTRLEN];
    string str;

    if(inet_ntop(AF_INET, &socketAddr.sin_addr, buffer, sizeof(buffer)) != NULL)
        str = string(buffer);
    else
        str = string("0.0.0.0");

    localHost = str;
#endif

	return localHost;
}

//---------------------------------------------------------------------------
int UdpSocket::GetLocalPort()
{
#ifdef EUP_QNX_OS
    sockaddr_in socketAddr;
    socklen_t addrLen = sizeof(sockaddr_in);

    pthread_mutex_lock(&socketMutex);
    int ret = getsockname(socketDescriptor, (sockaddr*)&socketAddr, &addrLen);
    pthread_mutex_unlock(&socketMutex);

    if(ret == -1)
        return -1;
    else
        localPort = ntohs(socketAddr.sin_port);
#endif

	return localPort;
}

//---------------------------------------------------------------------------
int UdpSocket::SendData(const char *data, int dataSize, string host, int port)
{
    LockAccess();
	sockaddr_in remoteAddr;
	memset(&remoteAddr, 0, sizeof(remoteAddr));
	remoteAddr.sin_family = AF_INET;
	remoteAddr.sin_addr.s_addr = inet_addr(host.c_str());
	remoteAddr.sin_port = htons(port);

    int res = sendto(socketDescriptor, data, dataSize, 0, (sockaddr *)&remoteAddr, sizeof(remoteAddr));
    if(res < 0)
	{
		ostringstream sout;
		sout << "UdpSocket: sendto error. errno = " << errno;
		PrintMessage(sout.str());
	}

    UnlockAccess();
    return res;
}

//---------------------------------------------------------------------------
int UdpSocket::ReceiveData(char *buffer,  int bufferSize, string host, int *port)
{
	if(buffer == NULL)
		return -1;

    int res = 0;
	sockaddr_in rcvAddr;
    memset(&rcvAddr, 0, sizeof(rcvAddr));
    rcvAddr.sin_family = AF_INET;
#if defined(EUP_WINDOWS_OS)
    {
        rcvAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    }
#else
    rcvAddr.sin_addr.s_addr = inet_addr(host.c_str());
#endif
    rcvAddr.sin_port = htons(*port);
    int addrSize = sizeof(rcvAddr);

    struct timeval tv;
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(socketDescriptor,&readfds);
    tv.tv_sec = 1;
    tv.tv_usec = 0;
    select(socketDescriptor+1,&readfds,NULL,NULL,&tv);
    if(FD_ISSET(socketDescriptor,&readfds))
    {
        LockAccess();
        res = recvfrom(socketDescriptor, buffer, bufferSize, 0, (struct sockaddr*)&rcvAddr, &addrSize);
        UnlockAccess();
        if(res < 0)
        {
            ostringstream sout;
            sout << "UdpSocket: recvfrom error. errno = " << errno;
            PrintMessage(sout.str());
            return res;
        }
    }
    else
    {
        ostringstream sout;
        sout << "timeout ,left time " << tv.tv_sec <<  "s\t" << tv.tv_usec <<"usec\n";
        PrintMessage(sout.str());
        return -1;
    }

#ifdef EUP_QNX_OS
	// Возвращаем адрес отправителя
	if(host != NULL)
	{
		char str[INET_ADDRSTRLEN];

		if(inet_ntop(AF_INET, &rcvAddr.sin_addr, str, sizeof(str)) != NULL)
			*host = string(str);
		else
			*host = string("0.0.0.0");
	}
#endif

	// Возвращаем порт отправителя
	if(port != NULL)
	{
		*port = ntohs(rcvAddr.sin_port);
	}

    UnlockAccess();
    return res;
}

bool UdpSocket::setMulticastGroup(string mulricast_group)
{
    struct ip_mreq mreq;
    memset((char*)&mreq,0,sizeof(mreq));
    mreq.imr_multiaddr.s_addr = inet_addr(mulricast_group.c_str());
    mreq.imr_interface.s_addr = ntohl(INADDR_ANY);

    if (setsockopt(socketDescriptor, IPPROTO_IP, IP_ADD_MEMBERSHIP,
                   (const char*)&mreq, sizeof(struct ip_mreq)) < 0)
    {
        ostringstream sout;
        sout << "UdpSocket: add multicast error. errno = " << errno;
        PrintMessage(sout.str());
        return false;
    }
    return true;
}

UdpSocket::PrintMessage(string err_str)
{
    error_string = err_str;
}

//---------------------------------------------------------------------------
