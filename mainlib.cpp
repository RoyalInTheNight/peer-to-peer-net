#include "p2phb.h"

node::node(std::string node_nickname, std::string node_address, uint16_t port) {
    __Ap_peers.insert(std::map<std::string, uint16_t>::value_type(node_address, port));
    id_names.push_back(node_nickname);

    __Addr_node = node_address.c_str();
}

void node::init() {
    WSADATA wsa;

    ErrorCodes.push_back(0);
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) ErrorCodes.push_back(-1);
}

/*int8_t p2p_utils::getlasterror() {

}

void p2p_utils::warnings() {

}*/

void node::init_peer_server() {
    WinBuffer buffer = new char[MAXIMUM_REPARSE_DATA_BUFFER_SIZE];
    std::string str_buffer, check_buffer;
    int *IntBuffer = new int();
    uint pos_map = 0;

    auto __Pr_node = ([&]() -> void {
        net __node, __lsock;

        __node.sin_addr.S_un.S_addr = inet_addr(__Addr_node.c_str());
        __node.sin_port = htons(__Ap_peers.at(__Addr_node.c_str()));
        __node.sin_family = AF_INET;

        net_sock fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

        if (fd == INVALID_SOCKET)
            ErrorCodes.push_back(-2);
        
        if (bind(fd, (net_p)&__node, size_net) == SOCKET_ERROR)
            ErrorCodes.push_back(-3);
        
        while (1) {
            int8_t bytes_read = recvfrom(fd, buffer, 1024, 0, (net_p)&__lsock, (int *)&size_net);

            if (bytes_read == 0) ErrorCodes.push_back(-4);
            else if (bytes_read < 0) ErrorCodes.push_back(-5);

            id_names.push_back(buffer);
            __Ap_peers.insert(std::map<std::string, uint16_t>::value_type(inet_ntoa(__lsock.sin_addr), htons(__lsock.sin_port)));
        }
    });

    auto __C_node = ([&]() -> void {
        net __cnode;

        __cnode.sin_addr.S_un.S_addr = inet_addr(__Addr_node.c_str());
        __cnode.sin_port = htons(__Ap_peers.at(__Addr_node.c_str()) * 2);
        __cnode.sin_family = AF_INET;

        net_sock fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

        if (fd == INVALID_SOCKET)
            ErrorCodes.push_back(-2);

        if (bind(fd, (net_p)&__cnode, size_net) == SOCKET_ERROR)
            ErrorCodes.push_back(-3);
        
        while (1) {
            int8_t bytes_read = recvfrom(fd, (WinBuffer)IntBuffer, 1, 0, NULL, NULL);

            if (bytes_read == 0) 
                ErrorCodes.push_back(-4);

            else if (bytes_read < 0) 
                ErrorCodes.push_back(-5);

            if (IntBuffer[0] == ConnTo) {
                bytes_read = recvfrom(fd, (WinBuffer)buffer, 1024, 0, NULL, NULL);

                if (bytes_read == 0) ErrorCodes.push_back(-4);
                else if (bytes_read < 0) ErrorCodes.push_back(-5);

                for (uint i = 0; i < id_names.size(); i++) {
                    check_buffer = strcmp(buffer, id_names.at(i).c_str()) == 0 ? str_buffer : check_buffer;
                    if (strcmp(str_buffer.c_str(), buffer) == 0) break;
                }
                
                bytes_read = send(fd, str_buffer.c_str(), str_buffer.size(), 0);

                if (bytes_read < 0) 
                    ErrorCodes.push_back(-4);
            }

            if (IntBuffer[0] == GetM) {
                for (uint i = 0; i < id_names.size(); i++)
                    str_buffer += id_names.at(i);
                str_buffer += "\n";

                bytes_read = send(fd, (WinBuffer)buffer, 1024, 0);
            }

            //if (IntBuffer[0] == Brd) {
            //        
            //}
        }
    });

    init();

    std::thread __Pr_n(__Pr_node);
    std::thread __C__(__C_node);

    __Pr_n.join();
    __C__.join();
}

peer::peer(std::string nickname, std::string peer_address, uint16_t port) {
    nick = nickname.c_str();
    peer_addr = peer_address.c_str();
    peer_port = port;
}

void peer::init_peer() {
    WSADATA wsa;

    ErrorCodes.push_back(0);

    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) 
        ErrorCodes.push_back(-1);

    auto __peer_init = ([&]() -> void {
        net __peer;

        __peer.sin_addr.S_un.S_addr = inet_addr(peer_addr.c_str());
        __peer.sin_port = htons(peer_port);
        __peer.sin_family = AF_INET;

        net_sock fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

        if (fd == INVALID_SOCKET) 
            ErrorCodes.push_back(-2);

        int8_t bytes_read = sendto(fd, nick.c_str(), nick.size(), 0, (net_p)&__peer, size_net);

        if (bytes_read < 0)
            ErrorCodes.push_back(-3);
    });

    auto __peer_initl = ([&]() -> void {
        WinBuffer buffer = new char[MAXIMUM_REPARSE_DATA_BUFFER_SIZE];
        net __peer, __peer_list;

        __peer.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
        __peer.sin_port = htons(peer_port * 3);
        __peer.sin_family = AF_INET;

        net_sock fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

        if (fd == INVALID_SOCKET)
            ErrorCodes.push_back(-2);
        
        if (bind(fd, (net_p)&__peer, size_net) == SOCKET_ERROR)
            ErrorCodes.push_back(-3);
        
        int8_t bytes_read = 0;

        while (1) {
            bytes_read = recvfrom(fd, (WinBuffer)buffer, 1024, 0, (net_p)&__peer_list, (int *)&size_net);

            if (bytes_read == 0)
                ErrorCodes.push_back(-4);

            if (bytes_read < 0)
                ErrorCodes.push_back(-5);

            // connected status for generate session
        }
    });

    std::thread __pinitl(__peer_initl);

    __peer_init();
    __pinitl.join();
}

