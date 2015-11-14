#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <winsock2.h>
#include <iostream>

#include "WebException.h"

#pragma comment(lib, "ws2_32.lib")

namespace WindowsSockets
{
	SOCKET Accept(SOCKET s, sockaddr* connectingEntityAddress);
	void Bind(SOCKET s, sockaddr* localAddress);
	void CloseSocket(SOCKET socketToClose);
	void Connect(SOCKET unconnectedSocket, sockaddr* socketAddress);

	std::string GetClientIpAddress(sockaddr_in clientAddress);
	hostent* GetHostByAddress(std::string address, int type = AF_INET);
	hostent* GetHostByName(std::string name);

	void GetSocketOption(SOCKET s, int level, int optionName, char* optionValue, int* optionLength);

	unsigned short HostToNetworkShort(unsigned short host);
	unsigned long InternetAddress(std::string address);

	void Listen(SOCKET boundUnconnectedSocket, int maximumQueueLength = SOMAXCONN);

	std::string Receive(SOCKET s, int maxLenght);
	std::string ReceiveFrom(SOCKET s, int maxLenght, sockaddr* from, int flags = 0);

	void Send(SOCKET s, std::string data);
	void SendTo(SOCKET s, std::string data, const sockaddr* to, int flags = 0);

	void SetSocketOption(SOCKET s, int level, int optionName, const char* optionValue, int optionLength);
	SOCKET Socket(int addressFamily = AF_INET, int type = SOCK_STREAM, int protocol = 0);
	void SocketIOMode(SOCKET s, long command, unsigned long* parameterForCommand);

	void WsaCleanup();
	WSADATA WsaStartup();
}

