#include "header.h"

utils::utils(int8_t DebugMode) {
    if (DebugMode)
        DebugMode_extern = DebugMode;
}

int8_t utils::_connect(SOCKET fd, psockaddr __csock, int32_t __csock_size) {
    int8_t bytes_read = connect(fd, __csock, __csock_size);

    if (DebugMode_extern)
        switch (bytes_read) {
            case SOCKET_ERROR:
                printf("[!]ConnectError: %d\n", WSAGetLastError());
                return bytes_read;

            case 0x0: break;
                printf("[!]ConnectError: %d\n", WSAGetLastError());
                return bytes_read;

            default:
                printf("[*]ConnectSuccess\n");
                return bytes_read;
        }
    
    else
        return bytes_read;
}

int8_t utils::_bind(SOCKET fd, psockaddr __bsock, int32_t __bsock_size) {
    int8_t bytes_read = bind(fd, __bsock, __bsock_size);

    if (DebugMode_extern)
        switch (bytes_read) {
            case SOCKET_ERROR:
                printf("[!]BindError: %d\n", WSAGetLastError());
                return bytes_read;

            case 0x0:
                printf("[!]BindError: %d\n", WSAGetLastError());
                return bytes_read;

            default:
                printf("[*]BindSuccess\n");
                return bytes_read;
        }

    else
        return bytes_read;
}

int8_t utils::_listen(SOCKET fd, int64_t queue) {
    int8_t bytes_read = listen(fd, queue);

    if (DebugMode_extern)
        switch (bytes_read) {
            case INVALID_SOCKET:
                printf("[!]ListenError: %d\n", WSAGetLastError());
                return bytes_read;

            default:
                printf("[*]ListenSuccess\n");
                return bytes_read;
        }

    else
        return bytes_read;
}

int64_t utils::_recv(SOCKET fd, pvoid buffer, size_t buffer_size, uint8_t flags) {
    int64_t bytes_read = recv(fd, (win_netbuffer)buffer, buffer_size, flags);

    if (DebugMode_extern)
        switch (bytes_read) {
            case SOCKET_ERROR:
                printf("[!]RecvError: %d\n", WSAGetLastError());
                return bytes_read;
            
            case 0x0:
                printf("[!]RecvError: %d\n", WSAGetLastError());
                return bytes_read;

            default:
                printf("[*]RecvSuccess\n");
                return bytes_read;
        }
    
    else
        return bytes_read;
}

int64_t utils::_send(SOCKET fd, pvoid buffer, size_t buffer_size, uint8_t flags) {
    int64_t bytes_read = send(fd, (win_netbuffer)buffer, buffer_size, flags);

    if (DebugMode_extern)
        switch (bytes_read) {
            case SOCKET_ERROR:
                printf("[!]SendError: %d\n", WSAGetLastError());
                return bytes_read;
            
            case 0x0:
                printf("[!]SendError: %d\n", WSAGetLastError());
                return bytes_read;
            
            default:
                printf("[*]SendSuccess\n");
                return bytes_read;
        }

    else
        return bytes_read;
}

int64_t utils::_recvfrom(SOCKET fd, pvoid buffer, size_t buffer_size, uint8_t flags, psockaddr __lsock, int32_t *__lsock_size) {
    int64_t bytes_read = recvfrom(fd, (win_netbuffer)buffer, buffer_size, flags, __lsock, __lsock_size);

    if (DebugMode_extern)
        switch (bytes_read) {
            case SOCKET_ERROR:
                printf("[!]RecvfromError: %d\n", WSAGetLastError());
                return bytes_read;
            
            case 0x0:
                printf("[!]RecvfromError: %d\n", WSAGetLastError());
                return bytes_read;

            default:
                printf("[*]RecvfromSuccess\n");
                return bytes_read;
        }

    else
        return bytes_read;
}

int64_t utils::_sendto(SOCKET fd, pvoid buffer, size_t buffer_size, uint8_t flags, psockaddr __ssock, int32_t __ssock_size) {
    int64_t bytes_read = sendto(fd, (win_netbuffer)buffer, buffer_size, flags, __ssock, __ssock_size);

    if (DebugMode_extern)
        switch (bytes_read) {
            case SOCKET_ERROR:
                printf("[!]SendtoError: %d\n", WSAGetLastError());
                return bytes_read;
            
            case 0x0:
                printf("[!]SendtoError: %d\n", WSAGetLastError());
                return bytes_read;
            
            default:
                printf("[*]SendtoSuccess\n");
                return bytes_read;
        }
    
    else
        return bytes_read;
}

int8_t utils::_accept(SOCKET fd, psockaddr __lsock, int32_t *__lsock_ssize) {
    int8_t bytes_read = accept(fd, __lsock, __lsock_ssize);

    if (DebugMode_extern)
        switch (bytes_read) {
            case SOCKET_ERROR:
                printf("[!]AcceptError: %d\n", WSAGetLastError());
                return bytes_read;

            default:
                printf("[*]AcceptSuccess\n", WSAGetLastError());
                return bytes_read;
        }
    
    else
        return bytes_read;
}

void utils::delay(uint32_t mseconds) {
    clock_t goal = mseconds + clock();
    while (goal > clock());
}

peer::peer(std::string nickname, 
           std::string nicknameNode, 
           std::string address, 
           std::string addressNode, 
           uint16_t port, 
           uint16_t portNode, 
           uint8_t DebugMode_i) {

    Nicknames.push_back(nicknameNode);
    Nicknames.push_back(nickname);

    Addresses.push_back(addressNode);
    Addresses.push_back(address);

    users.insert(std::map<std::string, std::string>::value_type("you -> " + Nicknames.at(1), Addresses.at(1).c_str()));
    users.insert(std::map<std::string, std::string>::value_type("node -> " + Nicknames.at(0), Addresses.at(0).c_str()));

    ports.insert(std::map<std::string, uint16_t>::value_type(Addresses.at(1).c_str(), port));
    ports.insert(std::map<std::string, uint16_t>::value_type(Addresses.at(0).c_str(), portNode));

    DebugMode = DebugMode_i;

    WSADATA wsa;

    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0 && DebugMode == 1)
        printf("[!]WSAinitError: %d\n", WSAGetLastError());
}

void peer::peerInit() {
    auto _initNodeConnection = ([&]() -> int8_t {
        sockaddr_in sock;
        int32_t size_sock = sizeof(sockaddr_in);

        sock.sin_addr.S_un.S_addr = inet_addr(users.at("node -> " + Nicknames.at(0)).c_str());
        sock.sin_port = htons(ports.at(users.at("node -> " + Nicknames.at(0)).c_str()));
        sock.sin_family = AF_INET;

        SOCKET fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

        if (fd == INVALID_SOCKET && DebugMode == 1) {
            printf("[!]SocketError-_initNodeConnection: %d\n", WSAGetLastError());
            return -2;
        }

        else if (fd == INVALID_SOCKET && DebugMode != 1)
            return -2;
        
        utils utils_local(DebugMode);

        utils_local._connect(fd, (psockaddr)&sock, size_sock);
        utils_local._send(fd, (win_netbuffer)Nicknames.at(1).c_str(), Nicknames.at(1).size(), 0);
    });

    auto _initYourListenSock = ([&]() -> int8_t {
        sockaddr_in ylsock, lsock;
        int32_t size_ylsock = sizeof(sockaddr_in);

        ylsock.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
        ylsock.sin_port = htons(ports.at(users.at("you -> " + Nicknames.at(1)).c_str()));
        ylsock.sin_family = AF_INET;

        SOCKET fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

        if (fd == INVALID_SOCKET && DebugMode == 1) {
            printf("[!]SocketError-_initYourListenSock: %d\n", WSAGetLastError());
            return -2;
        }

        else if (fd == INVALID_SOCKET && DebugMode != 1)
            return -2;
        
        utils utils_local(DebugMode);

        utils_local._bind(fd, (psockaddr)&ylsock, size_ylsock);

        while (1) {
            utils_local._listen(fd, 0x400);
            utils_local.delay(2000);
            utils_local._accept(fd, (psockaddr)&lsock, &size_ylsock);
        }
    });

    std::thread(_initYourListenSock).join();
    std::thread(_initNodeConnection).join();
}

void peer::peerToPeerConn(std::string nickPeer) {
    sockaddr_in sock;
    int32_t size_sock = sizeof(sockaddr_in);

    sock.sin_addr.S_un.S_addr = inet_addr(users.at(nickPeer.c_str()).c_str());
    sock.sin_port = htons(ports.at(users.at(nickPeer.c_str()).c_str()));
    sock.sin_family = AF_INET;

    SOCKET fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (fd == INVALID_SOCKET && DebugMode == 1)
        printf("[!]SocketError-peerToPeerConn: %d\n", WSAGetLastError());
    
    utils utils_local(DebugMode);

    utils_local._connect(fd, (psockaddr)&sock, size_sock);
}

void peer::SendToPeer(std::string msg) {
    utils utils_local(DebugMode);

    SOCKET fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    utils_local._send(fd, (win_netbuffer)msg.c_str(), msg.size(), 0);
}

std::string peer::getMembers() {
    utils utils_local(DebugMode);

    SOCKET fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (fd == INVALID_SOCKET && DebugMode == 1)
        printf("[!]SocketError-getMembers: %d\n", WSAGetLastError());
    
    std::string buffer;

    utils_local._recv(fd, (win_netbuffer)buffer.c_str(), bytes32size, 0);

    return buffer.c_str();
}