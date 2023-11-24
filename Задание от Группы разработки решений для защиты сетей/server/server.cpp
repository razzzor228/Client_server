#include <iostream>
#include <string>
#include <WinSock2.h>

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
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Ошибка при создании сокета." << std::endl;
        WSACleanup();
        return 1;
    }

    // Ввод прослушивающего порта
    std::cout << "Введите прослушивающий порт: ";
    unsigned short port;
    std::cin >> port;

    // Привязка сокета к адресу и порту
    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(port);
    if (bind(serverSocket, reinterpret_cast<sockaddr*>(&serverAddress), sizeof(serverAddress)) == SOCKET_ERROR) {
        std::cerr << "Ошибка при привязке сокета к адресу и порту." << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    // Прослушивание входящих соединений
    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Ошибка при прослушивании входящих соединений." << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Сервер ожидает соединения на порту " << port << "..." << std::endl;

    // Принятие входящего соединения
    SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Ошибка при принятии входящего соединения." << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Соединение установлено. Отправка данных клиенту..." << std::endl;

    // Отправка данных клиенту
    std::string message = "Hello Maxim!";
    if (send(clientSocket, message.c_str(), message.length(), 0) == SOCKET_ERROR) {
        std::cerr << "Ошибка при отправке данных клиенту." << std::endl;
    }

    // Закрытие соединения и освобождение ресурсов
    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();

    std::cout << "Соединение закрыто. Сервер завершает работу." << std::endl;

    return 0;
}