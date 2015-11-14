// Broadcast Chat.cpp: ���������� ����� ����� ��� ����������� ����������.
//

#include "stdafx.h"

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <winsock2.h>
#include <conio.h>
#include <iostream>
#include <sstream>
#include <algorithm> 
#include <vector>
#include <string>
#include <thread>

#include "WindowsSockets.h"

#pragma comment(lib, "ws2_32.lib")

#define DEFAULT_PORT 3228

using namespace std;
using namespace WindowsSockets;

/*
Broadcast "Chat":

��������� ��������� ��������� ������ � �������� �� �� ����� � �������:
<IP-����� �����������>: �����

����� ������� �� ������ ���� ������������ ��������� ���������.
������ ������������ ��� ��������������� ����.

���������� ����������� (UDP) ������������� ������.
������� ���� 3228.
*/

class Broadcast
{
	int _port;
	string _message;

public:
	Broadcast(int port = DEFAULT_PORT)
	{
		if (port < 1024)
			throw new ArgumentException("Port number is lower than 1024.");

		_port = port;
		_message = "Bro!"; // Inspired by HBO's Silicon Valley.
	}
	~Broadcast()
	{
		_port = 0;
		_message.clear();
	}

	void Run()
	{
		// ������������� ��������� Winsock.
		WSAData wsaData = WindowsSockets::WsaStartup();

		// �������� �������.
		SOCKET senderSocket = WindowsSockets::Socket(AF_INET, SOCK_DGRAM);		// Broadcasting.
		SOCKET receiverSocket = WindowsSockets::Socket(AF_INET, SOCK_DGRAM);	// ��������� �������� ���������.

		// Setting senderSocket broadcasting option on.
		const int broadcast = 1;
		WindowsSockets::SetSocketOption(senderSocket, SOL_SOCKET, SO_BROADCAST, (const char*)&broadcast, sizeof(broadcast));
		cout << "Broadcasting option is now on." << endl;

		// Non-blocking socket.
		 const int nonblocking = 1;
		 WindowsSockets::SocketIOMode(receiverSocket, FIONBIO, (unsigned long*)&nonblocking);
		 cout << "Non-blocking receiverSocket." << endl;

		// ������� �������.
		
		// ������ ��� ��������� ��������� ���������.
		sockaddr_in unknownSenderAddress;
		memset((void *)&unknownSenderAddress, '\0', sizeof(sockaddr_in));

		// ��� �����, ��� ����������.
		sockaddr_in receiverAddress;
		memset((void *)&receiverAddress, '\0', sizeof(sockaddr_in));

		receiverAddress.sin_family = AF_INET;
		receiverAddress.sin_port = WindowsSockets::HostToNetworkShort(_port);
		receiverAddress.sin_addr.s_addr = WindowsSockets::HostToNetworkShort(INADDR_ANY);

		// ����� ���������� ����� ���������.
		sockaddr_in broadcastSenderAddress;
		memset((void *)&broadcastSenderAddress, '\0', sizeof(sockaddr_in));

		broadcastSenderAddress.sin_family = AF_INET;
		broadcastSenderAddress.sin_port = WindowsSockets::HostToNetworkShort(_port);
		broadcastSenderAddress.sin_addr.s_addr = INADDR_BROADCAST;

		// ����������� ����� ��� ������ ��������� � ������ � �����.
		WindowsSockets::Bind(receiverSocket, (SOCKADDR*)&receiverAddress);

		keybd_event(VK_SPACE, 0, KEYEVENTF_KEYUP, 0);
		keybd_event(VK_ESCAPE, 0, KEYEVENTF_KEYUP, 0);

		while (true)
		{
			// ��������� ���������.
			try
			{
				std::string recieved = WindowsSockets::ReceiveFrom(receiverSocket, 512, (SOCKADDR*)&unknownSenderAddress);
				cout << "<" << WindowsSockets::GetClientIpAddress(unknownSenderAddress) << ">: " << recieved << endl;
			}
			catch (WebException* e)
			{
				if (e->GetError() != WSAEWOULDBLOCK)
					throw e;
			}

			// �������� ��������� ��� ������� �������.
			if (GetAsyncKeyState(VK_SPACE))
			{
				keybd_event(VK_SPACE, 0, KEYEVENTF_KEYUP, 0);

				WindowsSockets::SendTo(senderSocket, _message, (SOCKADDR*)&broadcastSenderAddress);
				cout << "Message sent." << endl;
			}

			// ���������� �����.
			if (GetAsyncKeyState(VK_ESCAPE))
				break;
		}

		// �������� ������.
		WindowsSockets::CloseSocket(receiverSocket);

		// ������������ ��������, ���������� ��� ������ � Windows Sockets.
		WindowsSockets::WsaCleanup();
	}

	void SetMessage(string message)
	{
		if (message.empty())
			throw new ArgumentException("Message cannot be empty.");

		_message = message;
	}
};

int main()
{
	try
	{
		Broadcast* b = new Broadcast(DEFAULT_PORT);
		b->Run();
	}
	catch (Exception* e)
	{
		// �������� ���������� ��������� ?
		cout << e->GetExceptionMessage();
	}

	_getch();
    return 0;
}

