#include <iostream>
#include <string>
#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

int main() {
    setlocale(LC_ALL, "rus");
    // Инициализация WinSock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Ошибка при инициализации WinSock." << std::endl;
        return 1;
    }

    // Создание сокета
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Ошибка при создании сокета." << std::endl;
        WSACleanup();
        return 1;
    }

    // Ввод адреса и порта сервера
    std::cout << "Введите IP-адрес сервера: ";
    std::string serverAddress;
    std::cin >> serverAddress;
    std::cout << "Введите порт сервера: ";
    unsigned short port;
    std::cin >> port;

    // Установка соединения с сервером
    sockaddr_in serverSocketAddress{};
    serverSocketAddress.sin_family = AF_INET;
    inet_pton(AF_INET, serverAddress.c_str(), &(serverSocketAddress.sin_addr));
    serverSocketAddress.sin_port = htons(port);
    if (connect(clientSocket, reinterpret_cast<sockaddr*>(&serverSocketAddress), sizeof(serverSocketAddress)) ==
        SOCKET_ERROR) {
        std::cerr << "Ошибка при установке соединения с сервером." << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Соединение с сервером установлено. Ожидание данных..." << std::endl;

    // Получение данных от сервера
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    if (recv(clientSocket, buffer, sizeof(buffer) - 1, 0) == SOCKET_ERROR) {
        std::cerr << "Ошибка при получении данных от сервера." << std::endl;
    }
    else {
        std::cout << "Получены данные от сервера: " << buffer << std::endl;
    }

    // Закрытие соединения и освобождение ресурсов
    closesocket(clientSocket);
    WSACleanup();

    return 0;
}