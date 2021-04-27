#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
using namespace std;
// Функци errHandler нужна для обработки ошибок. Why - причина ошибки. ExitCode - код завершения ( по дефолту 1 )
void errHandler (const char * why, const int exitCode = 1)
{
    cerr << why << endl;
    exit(exitCode);
}

int main(int argc, char **argv)
{
    // I этап. Подготовка адресных структур формата IPv4
    // Подготовка структуры с адресом клиента
    sockaddr_in * selfAddr = new (sockaddr_in);
    selfAddr->sin_family = AF_INET;      // интернет протокол IPv4
    selfAddr->sin_port = 0;              // Любой порт на усмотрение ОС
    selfAddr->sin_addr.s_addr = 0;       // все адреса компьютера

    // Подготовка структуры с адресом сервера
    sockaddr_in * remoteAddr = new (sockaddr_in);
    remoteAddr->sin_family = AF_INET; // интернет протокол IPv4
    remoteAddr->sin_port = htons(7); // порт
    remoteAddr->sin_addr.s_addr = inet_addr("82.179.90.12"); // локальный адрес сети

    // Подготовка буфера обмена для передачи и приема данных
    char *buf = new char [256];
    strcpy(buf, "Ртищево - России перекресток!!!\n"); // Копируем строку
    int msgLen = strlen(buf);             // Вычисляем длину строки

    // II этап. Создание сокета IPv4
    int mySocket = socket(AF_INET, SOCK_DGRAM, 0); // Используем протокол UDP
    if (mySocket == -1) {
        errHandler("Error open socket", 11);
    }

    //III этап. Привязка сокета к своему адресу
    int rc = bind(mySocket, (const sockaddr*) selfAddr, sizeof (sockaddr_in));
    if (rc == -1) { // проверка
        close(mySocket);
        errHandler("Error bind socket with local address", 12);
    }

    // IV этап. Установка соединения с сервером
    rc = connect(mySocket, (const sockaddr*) remoteAddr, sizeof(sockaddr_in));
    if (rc == -1) {
        close(mySocket);
        errHandler("Error connect socket with remote server", 13);
    }

    // V этап. Выполнение информационного обмена
    // Передача данных
    rc = send(mySocket, buf, msgLen, 0);
    if (rc == -1) { // проверка
        close(mySocket);
        errHandler("Error send message", 14);
    }
    cout << "We send: " << buf << endl;
    // Принятие ответа
    rc = recv(mySocket, buf, 256, 0);
    if (rc == -1) { // проверка
        close(mySocket);
        errHandler("Error recevie answer", 15);
    }
    buf[rc]= '\0';
    std::cout<<"We receive: "<< buf << std::endl;

    // VI этап. Закрытие сокета
    close(mySocket);
    // Освобождение памяти
    delete selfAddr;
    delete remoteAddr;
    delete[] buf;
    return 0;
}
