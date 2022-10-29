#include "header.h"

utils::utils(int8_t DebugMode) {
    if (DebugMode)
        DebugMode_extern = DebugMode;
}

int8_t utils::_connect(SOCKET fd, psockaddr __csock, socklen_t __csock_size) {
    int8_t bytes_read = connect(fd, __csock, __csock_size);

    if (DebugMode_extern)
        switch (bytes_read) {
            case INVALID_SOCKET:
                printf("[!]ConnectError: %d\n", INVALID_SOCKET);
                return bytes_read;

            //case 0x0: break;
            //    printf("[!]ConnectError: %d\n", WSAGetLastError());
            //    return bytes_read;

            default:
                printf("[*]ConnectSuccess\n");
                return bytes_read;
        }
    
    else
        return bytes_read;
}

int8_t utils::_bind(SOCKET fd, psockaddr __bsock, socklen_t __bsock_size) {
    int8_t bytes_read = bind(fd, __bsock, __bsock_size);

    if (DebugMode_extern)
        switch (bytes_read) {
            case INVALID_SOCKET:
                printf("[!]BindError: %d\n", INVALID_SOCKET);
                return bytes_read;

            //case 0x0:
            //    printf("[!]BindError: %d\n", WSAGetLastError());
            //    return bytes_read;

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
                printf("[!]ListenError: %d\n", INVALID_SOCKET);
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
                printf("[!]RecvError: %d\n", SOCKET_ERROR);
                return bytes_read;
            
            case 0x0:
                printf("[!]RecvError: %d\n", 0x0);
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
                printf("[!]SendError: %d\n", SOCKET_ERROR);
                return bytes_read;
            
            case 0x0:
                printf("[!]SendError: %d\n", 0x0);
                return bytes_read;
            
            default:
                printf("[*]SendSuccess\n");
                return bytes_read;
        }

    else
        return bytes_read;
}

int64_t utils::_recvfrom(SOCKET fd, pvoid buffer, size_t buffer_size, uint8_t flags, psockaddr __lsock, socklen_t __lsock_size) {
    int64_t bytes_read = recvfrom(fd, (win_netbuffer)buffer, buffer_size, flags, __lsock, &__lsock_size);

    if (DebugMode_extern)
        switch (bytes_read) {
            case SOCKET_ERROR:
                printf("[!]RecvfromError: %d\n", SOCKET_ERROR);
                return bytes_read;
            
            case 0x0:
                printf("[!]RecvfromError: %d\n", 0x0);
                return bytes_read;

            default:
                printf("[*]RecvfromSuccess\n");
                return bytes_read;
        }

    else
        return bytes_read;
}

int64_t utils::_sendto(SOCKET fd, pvoid buffer, size_t buffer_size, uint8_t flags, psockaddr __ssock, socklen_t __ssock_size) {
    int64_t bytes_read = sendto(fd, (win_netbuffer)buffer, buffer_size, flags, __ssock, __ssock_size);

    if (DebugMode_extern)
        switch (bytes_read) {
            case SOCKET_ERROR:
                printf("[!]SendtoError: %d\n", SOCKET_ERROR);
                return bytes_read;
            
            case 0x0:
                printf("[!]SendtoError: %d\n", 0);
                return bytes_read;
            
            default:
                printf("[*]SendtoSuccess\n");
                return bytes_read;
        }
    
    else
        return bytes_read;
}

SOCKET utils::_accept(SOCKET fd, psockaddr __lsock, psocklen_t __lsock_ssize) {
    SOCKET bytes_read = accept(fd, __lsock, __lsock_ssize);

    if (DebugMode_extern)
        switch (bytes_read) {
            case (SOCKET)INVALID_SOCKET:
                printf("[!]AcceptError: %d\n", INVALID_SOCKET);
                return bytes_read;

            default:
                printf("[*]AcceptSuccess\n");
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
}

void peer::peerInit() {
    auto _initNodeConnection = ([&]() -> int8_t {
        sockaddr_in sock;
        int32_t size_sock = sizeof(sockaddr_in);

        sock.sin_addr.s_addr = inet_addr(users.at("node -> " + Nicknames.at(0)).c_str());
        sock.sin_port = htons(ports.at(users.at("node -> " + Nicknames.at(0)).c_str()));
        sock.sin_family = AF_INET;

        SOCKET fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

        if (fd == INVALID_SOCKET && DebugMode == 1) {
            printf("[!]SocketError-_initNodeConnection: %d\n", INVALID_SOCKET);
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
        socklen_t size_ylsock = sizeof(sockaddr_in);

        ylsock.sin_addr.s_addr = htonl(INADDR_ANY);
        ylsock.sin_port = htons(ports.at(users.at("you -> " + Nicknames.at(1)).c_str()));
        ylsock.sin_family = AF_INET;

        SOCKET fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

        if (fd == INVALID_SOCKET && DebugMode == 1) {
            printf("[!]SocketError-_initYourListenSock: %d\n", INVALID_SOCKET);
            return -2;
        }

        else if (fd == INVALID_SOCKET && DebugMode != 1)
            return -2;
        
        utils utils_local(DebugMode);

        utils_local._bind(fd, (psockaddr)&ylsock, size_ylsock);

        while (1) {
            utils_local._listen(fd, 0x400);
            utils_local.delay(2000);
            fdAcceptYourListenSock = utils_local._accept(fd, (psockaddr)&lsock, &size_ylsock);

            if (fdAcceptYourListenSock == INVALID_SOCKET && DebugMode == 1)
                printf("[!]Accept error: %d\n", INVALID_SOCKET);
            
            else if (fdAcceptYourListenSock != INVALID_SOCKET && DebugMode == 1)
                printf("[*]Accept success\n");
        }
    });

    std::thread(_initNodeConnection).join();
    std::thread(_initYourListenSock).join();
}

void peer::peerToPeerConn(std::string nickPeer) {
    sockaddr_in sock;
    socklen_t size_sock = sizeof(sockaddr_in);

    for (int64_t i = 0; i < Nicknames.size(); i++)
        if (strcmp(nickPeer.c_str(), Nicknames.at(i).c_str()) != 0 && DebugMode == 1)
            printf("[!]PeerDontFoundInThisServerError: %d\n", -3);

    sock.sin_addr.s_addr = inet_addr(users.at(nickPeer.c_str()).c_str());
    sock.sin_port = htons(ports.at(users.at(nickPeer.c_str()).c_str()));
    sock.sin_family = AF_INET;

    SOCKET fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (fd == INVALID_SOCKET && DebugMode == 1)
        printf("[!]SocketError-peerToPeerConn: %d\n", INVALID_SOCKET);
    
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

    std::string buffer = "getmembers";

    utils_local._send(fd, (win_netbuffer)buffer.c_str(), buffer.size(), 0);
    if (fd == INVALID_SOCKET && DebugMode == 1)
        printf("[!]SocketError-getMembers: %d\n", INVALID_SOCKET);
    
    buffer.clear();
    utils_local._recv(fd, (win_netbuffer)buffer.c_str(), bytes32size, 0);

    return buffer.c_str();
}

void peer::peerBroadcast() {
    sockaddr_in sock;
    socklen_t size_sock = sizeof(sockaddr_in);

    broadcastBuffer = new char[bytes32size];

    SOCKET fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    std::vector<sockaddr_in> vecSockets(Nicknames.size());

    std::thread([&]() -> void {
        for (int64_t i = 0; i < Nicknames.size(); i++) {
            vecSockets.at(i) = sockaddr_in();

            vecSockets.at(i).sin_addr.s_addr = htonl(INADDR_ANY);
            vecSockets.at(i).sin_port = htons(ports.at(users.at(Nicknames.at(i))));
            vecSockets.at(i).sin_family = AF_INET;

            utils utils_local(DebugMode);

            utils_local._bind(fd, (psockaddr)&vecSockets.at(i), size_sock);
            utils_local._listen(fd, 0x1);
            utils_local._accept(fd, NULL, NULL);
            utils_local._recv(fd, (win_netbuffer)broadcastBuffer, bytes32size, 0);

            bigDataBuffer.push_back(broadcastBuffer);

            memset(broadcastBuffer, 0, strlen(broadcastBuffer));
        }

        vecSockets.~vector();
    }).join();
}

sigSignalServer::sigSignalServer(std::string nameNode, 
                                 std::string addressNode, 
                                 uint16_t portNode, 
                                 uint8_t DebugMode_i) {

    Nicknames.push_back(nameNode.c_str());
    Addresses.push_back(nameNode.c_str());

    users.insert(std::map<std::string, std::string>::value_type("node -> " + Nicknames.at(0), Addresses.at(0).c_str()));
    ports.insert(std::map<std::string, uint16_t>::value_type(users.at("node -> " + Nicknames.at(0)).c_str(), portNode));

    DebugMode = DebugMode_i;
}

void sigSignalServer::sigInit() {
    sockaddr_in sock, lsock;
    socklen_t size_sock = sizeof(sockaddr_in);

    nameBuffer = new char[bytes32size];

    auto dbNode = ([&]() -> int8_t {
        sock.sin_addr.s_addr = htonl(INADDR_ANY);
        sock.sin_port = htons(ports.at(users.at("node -> " + Nicknames.at(0)).c_str()));
        sock.sin_family = AF_INET;

        SOCKET fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

        if (fd == INVALID_SOCKET && DebugMode == 1) {
            printf("[!]SocketError-dbNode: %d\n", INVALID_SOCKET);
            return -2;
        }

        else if (fd == INVALID_SOCKET && DebugMode != 1)
            return -2;
        
        utils utils_local(DebugMode);

        utils_local._bind(fd, (psockaddr)&sock, size_sock);

        while (1) {
            utils_local._listen(fd, 0x400);
            SOCKET fd2 = utils_local._accept(fd, (psockaddr)&lsock, &size_sock);

            if (fd2 == INVALID_SOCKET && DebugMode == 1)
                printf("[!]Accept socket error: %d\n", INVALID_SOCKET);
            
            else if (fd2 != INVALID_SOCKET && DebugMode == 1)
                printf("[*]Accept socket success\n");

            Addresses.push_back(inet_ntoa(lsock.sin_addr));
            ports.insert(std::map<std::string, uint16_t>::value_type(Addresses.at(Addresses.size() - 1).c_str(), htons(lsock.sin_port)));

            memset(nameBuffer, 0, bytes32size);

            utils_local._recv(fd2, (win_netbuffer)nameBuffer, bytes32size, 0);
            
            if (strcmp(nameBuffer, "getmembers") == 0) {
                std::string buffer;

                for (int64_t i = 0; i < Nicknames.size(); i++)
                    buffer += Nicknames.at(i) + "\n";

                    utils_local._send(fd2, (win_netbuffer)buffer.c_str(), buffer.size(), 0);
            }

            else
                users.insert(std::map<std::string, std::string>::value_type(nameBuffer, Addresses.at(Addresses.size() - 1).c_str()));
        }
    });

    if (dbNode() == -2)
        exit(-2);
}
