
#pragma comment(lib, "ws2_32.lib")

#include <winsock2.h>
#include <iostream>

#pragma warning(disable : 4996)

SOCKET Connections[100];
int counter = 0;

void ClientHadler(int index)
{
	char msg[256];

	while (true)
	{
		recv(Connections[index], msg, sizeof(msg), NULL); // ��������� ��������� �� ������� 
		for (int iter = 0; iter < counter; iter++) {
			if (index == iter)
				continue;

			send(Connections[iter], msg, sizeof(msg), NULL);
		}
	}
}

int main(int atgc, char* argv[])
{
	WSAData wsadata; //! ��������� WSAData
	WORD DLLVersion = MAKEWORD(2, 1); //! ������������� ������ ���������� winsock ��� �������� ����������
	if (WSAStartup(DLLVersion, &wsadata) != 0) { //! ���� �������� �� ����, �� ���������� ��������� �������
		std::cout << "Error" << std::endl; // �������� ����������
		exit(1);
	}

	//! ��������� ���������� �� ������ ������
	SOCKADDR_IN addr; // ��������� SOCKADDR ������������� ��� �������� ������. ��� �������� ���������� SOCKADDR_IN
	int sizeofaddr = sizeof(addr);
	std::cout << sizeof(addr);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // sin_addr - ��������� SOCKADDR_IN, ������� ������ ����� (� ����� ������ ����� ����)
	addr.sin_port = htons(1111); // ���� ��� ������������� ��������� ������������ �������
	addr.sin_family = AF_INET; // sin_family (AF_INET - ������������ ��������� �������� ����������)

	SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL); // ������� �����, �������� ��������� ��������� ���������� socket

	//! ������ ��� ����� ��������� ����� ������
	bind(sListen, (SOCKADDR*)&addr, sizeof(addr)); //! (sListen - �������������� ��������� �����; ��������� �� ��������� SOCKADDR; ������)
	 
	// ������ ����� ���������� � ������������� ����� � �������� ���������� �� ������� �������
	listen(sListen, SOMAXCONN); // SOMAXCONN - ������������ ����� �������� ��������� ���������

	//! ��������� ����� �����, ����� ���������� ���������� � ��������
	SOCKET newConnection;
	for (int iter = 0; iter < 100; iter++)
	{
		newConnection = accept(sListen, (SOCKADDR*)&addr, &sizeofaddr); // ����� ���������� accept addr ����� ��������� �������� �� IP address �������

		if (newConnection == 0) {
			std::cout << "Error 2" << std::endl; // ������ ������������� �������
		}
		else {
			std::cout << "Client connected" << std::endl;
			char message[256] = "Hello, it is new connection";
			send(newConnection, message, sizeof(message), NULL);

			Connections[iter] = newConnection;
			counter++;
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHadler, (LPVOID)iter, NULL, NULL);
		}
	}

	system("pause");

	return 0;
}