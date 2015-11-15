#include "stdafx.h"
#include "WindowsSockets.h"

namespace WindowsSockets
{
	SOCKET Accept(SOCKET s, sockaddr* connectingEntityAddress)
	{
		int addressLength = sizeof(*connectingEntityAddress);
		SOCKET client = accept(s, connectingEntityAddress, &addressLength);

		if (client == SOCKET_ERROR)
		{
			int lastError = WSAGetLastError();
			throw new WebException("accept()", lastError);
		}

		return client;
	}

	void Bind(SOCKET s, sockaddr* localAddress)
	{
		if (bind(s, localAddress, sizeof(*localAddress)) == SOCKET_ERROR)
		{
			int lastError = WSAGetLastError();
			throw new WebException("bind()", lastError);
		}
	}

	void CloseSocket(SOCKET socketToClose)
	{
		if (closesocket(socketToClose) == SOCKET_ERROR)
		{
			int lastError = WSAGetLastError();
			throw new WebException("closesocket()", lastError);
		}
	}

	void Connect(SOCKET unconnectedSocket, sockaddr* socketAddress)
	{
		if (connect(unconnectedSocket, socketAddress, sizeof(*socketAddress)) == SOCKET_ERROR)
		{
			int lastError = WSAGetLastError();
			throw new WebException("connect()", lastError);
		}
	}

	std::string GetClientIpAddress(sockaddr_in clientAddress)
	{
		return std::to_string((int)(clientAddress.sin_addr.S_un.S_un_b.s_b1)) + "."
			+ std::to_string((int)(clientAddress.sin_addr.S_un.S_un_b.s_b2)) + "."
			+ std::to_string((int)(clientAddress.sin_addr.S_un.S_un_b.s_b3)) + "."
			+ std::to_string((int)(clientAddress.sin_addr.S_un.S_un_b.s_b4));
	}

	hostent* GetHostByAddress(std::string address, int type)
	{
		hostent* host = gethostbyaddr(address.c_str(), address.length(), type);

		if (host == nullptr)
		{
			int lastError = WSAGetLastError();
			throw new WebException("gethostbyaddr()", lastError);
		}

		return host;
	}

	hostent* GetHostByName(std::string name)
	{
		hostent* host = gethostbyname(name.c_str());

		if (host == nullptr)
		{
			int lastError = WSAGetLastError();
			throw new WebException("gethostbyaddr()", lastError);
		}

		return host;
	}

	void GetSocketOption(SOCKET s, int level, int optionName, char* optionValue, int* optionLength)
	{
		if (getsockopt(s, level, optionName, optionValue, optionLength) == SOCKET_ERROR)
		{
			int lastError = WSAGetLastError();
			throw new WebException("setsockopt()", lastError);
		}

	}

	unsigned short HostToNetworkShort(unsigned short host)
	{
		return htons(host);
	}

	unsigned long InternetAddress(std::string address)
	{
		unsigned long uLongIpAddress = inet_addr(address.c_str());

		if (uLongIpAddress == INADDR_NONE)
			throw new ArgumentException("Server IP address is not a legitimate Internet address.");

		return uLongIpAddress;
	}

	void Listen(SOCKET boundUnconnectedSocket, int maximumQueueLength)
	{
		if (listen(boundUnconnectedSocket, maximumQueueLength) == SOCKET_ERROR)
		{
			int lastError = WSAGetLastError();
			throw new WebException("listen()", lastError);
		}
	}

	std::string Receive(SOCKET s, int maxLenght)
	{
		char* received = new char[maxLenght];
		int countReceived = recv(s, received, (int)strlen(received), 0);

		if (countReceived == SOCKET_ERROR)
		{
			int lastError = WSAGetLastError();
			throw new WebException("recv()", lastError);
		}

		if (countReceived == 0)
		{
			// The connection has been gracefully closed.
			delete[] received;
			received = nullptr;

			return std::string();
		}

		received[countReceived] = '\0';
		std::string receivedString(received);

		delete[] received;
		received = nullptr;

		return receivedString;
	}

	std::string ReceiveFrom(SOCKET s, int maxLenght, sockaddr* from, int flags)
	{
		char* received = new char[maxLenght];
		int addressLength = sizeof(*from);

		int countReceived = recvfrom(s, received, maxLenght, flags, from, &addressLength);

		if (countReceived == SOCKET_ERROR)
		{
			int lastError = WSAGetLastError();
			delete[] received;
			received = nullptr;
			throw new WebException("recvfrom()", lastError);
		}

		if (countReceived == 0)
		{
			// The connection has been gracefully closed.
			delete[] received;
			received = nullptr;

			return std::string();
		}

		received[countReceived] = '\0';
		std::string receivedString(received);

		delete[] received;
		received = nullptr;

		return receivedString;
	}

	void Send(SOCKET s, std::string data)
	{
		if (send(s, data.c_str(), data.length(), 0) == SOCKET_ERROR)
		{
			int lastError = WSAGetLastError();
			throw new WebException("send()", lastError);
		}
	}

	void SendTo(SOCKET s, std::string data, const sockaddr* to, int flags)
	{
		if (sendto(s, data.c_str(), data.length(), 0, to, sizeof(*to)) == SOCKET_ERROR)
		{
			int lastError = WSAGetLastError();
			throw new WebException("sendto()", lastError);
		}
	}

	void SetSocketOption(SOCKET s, int level, int optionName, const char* optionValue, int optionLength)
	{
		// level = SOL_SOCKET
		// optionName = SO_BROADCAST - set/get broadcasting on/off
		// Failure: SOCKET_ERROR

		if (setsockopt(s, level, optionName, optionValue, optionLength) == SOCKET_ERROR)
		{
			int lastError = WSAGetLastError();
			throw new WebException("setsockopt()", lastError);
		}
	}

	SOCKET Socket(int addressFamily, int type, int protocol)
	{
		SOCKET s = socket(addressFamily, type, protocol);

		if (s == SOCKET_ERROR)
		{
			int lastError = WSAGetLastError();
			throw new WebException("socket()", lastError);
		}

		return s;
	}

	void SocketIOMode(SOCKET s, long command, unsigned long* parameterForCommand)
	{
		if (ioctlsocket(s, command, parameterForCommand) == SOCKET_ERROR)
		{
			int lastError = WSAGetLastError();
			throw new WebException("ioctlsocket()", lastError);
		}
	}

	void WsaCleanup()
	{
		if (WSACleanup() == SOCKET_ERROR)
		{
			int lastError = WSAGetLastError();
			throw new WebException("WsaCleanup()", lastError);
		}
	}

	WSADATA WsaStartup()
	{
		WORD versionRequested = MAKEWORD(2, 2);
		WSADATA wsaData;

		int wsaStartupError = WSAStartup(versionRequested, &wsaData);

		if (wsaStartupError != 0)
			throw new WebException("WSAStartup()", wsaStartupError);

		return wsaData;
	}
}