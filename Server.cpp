
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
		recv(Connections[index], msg, sizeof(msg), NULL); // ѕринимает сообщение от клиента 
		for (int iter = 0; iter < counter; iter++) {
			if (index == iter)
				continue;

			send(Connections[iter], msg, sizeof(msg), NULL);
		}
	}
}

int main(int atgc, char* argv[])
{
	WSAData wsadata; //! —труктура WSAData
	WORD DLLVersion = MAKEWORD(2, 1); //! «апрашиваема€ верси€ библиотеки winsock дл€ загрузки библиотеки
	if (WSAStartup(DLLVersion, &wsadata) != 0) { //! ≈сли вернулс€ не ноль, то библиотека загружена неверно
		std::cout << "Error" << std::endl; // «агрузка библиотеки
		exit(1);
	}

	//! «аполн€ем информацию об адресе сокета
	SOCKADDR_IN addr; // —труктура SOCKADDR предназначена дл€ хранени€ адреса. ƒл€ интернет протоколов SOCKADDR_IN
	int sizeofaddr = sizeof(addr);
	std::cout << sizeof(addr);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // sin_addr - структура SOCKADDR_IN, котора€ хранит адрес (в нашем случае локал хост)
	addr.sin_port = htons(1111); // ѕорт дл€ идентификации программы поступающими данными
	addr.sin_family = AF_INET; // sin_family (AF_INET - используетс€ семейство интернет протоколов)

	SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL); // —оздали сокет, которому присвоили результат выполнени€ socket

	//! “еперь нам нужно прив€зать адрес сокета
	bind(sListen, (SOCKADDR*)&addr, sizeof(addr)); //! (sListen - предварительно созданный сокет; указатель на структуру SOCKADDR; –азмер)
	 
	// “епреь нужно приступить к прослушиванию порта в ожидании соединени€ со стороны клиента
	listen(sListen, SOMAXCONN); // SOMAXCONN - максимальное число запросов ожидаемых обработки

	//! ќбь€вл€ем новый сокет, чтобы удерживать соединение с клиентом
	SOCKET newConnection;
	for (int iter = 0; iter < 100; iter++)
	{
		newConnection = accept(sListen, (SOCKADDR*)&addr, &sizeofaddr); // ѕосле выполнени€ accept addr будет содержать сведени€ об IP address клиента

		if (newConnection == 0) {
			std::cout << "Error 2" << std::endl; // ќшибка присоединени€ клиента
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