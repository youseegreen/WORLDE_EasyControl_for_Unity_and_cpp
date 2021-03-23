#pragma once

//TCP,UDP通信用ライブラリ
#pragma comment(lib, "ws2_32.lib")

#include <winsock2.h> //windows
#include <iostream>
#include <string>

#pragma warning(disable:4996)

class UDP {
    WSAData wsaData;
    SOCKET sock;
    struct sockaddr_in addr;
    bool enable;

public:

    // try catchで、UDPを利用可能かチェックするようにしたい
    UDP(std::string address = "100.80.145.173", int port = 52525) {
        WSAStartup(MAKEWORD(2, 0), &wsaData);
        sock = socket(AF_INET, SOCK_DGRAM, 0);
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.S_un.S_addr = inet_addr(address.c_str());
    }
    bool send(std::string msg) {
        sendto(sock, msg.c_str(), msg.length(), 0, (struct sockaddr*)&addr, sizeof(addr));
        return true;
    }
    ~UDP() {
        closesocket(sock);
        WSACleanup();
    }
};
