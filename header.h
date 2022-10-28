#include <cstdio>
#include <cstring>
#include <string>
#include <WinSock2.h>
#include <thread>
#include <map>
#include <vector>

typedef void *                 pvoid;
typedef sockaddr *         psockaddr;
typedef sockaddr_in *   psockaddr_in;
typedef char *         win_netbuffer;
typedef int32_t            socklen_t;
typedef int32_t *         psocklen_t;

enum KernelDebugModule {
    DebugMode_on,
    DebugMode_off
};

#include <time.h>

constexpr uint32_t bytes32size = 4294967295 / 2;
constexpr int32_t signedbytes32size = (4294967295 / 2) * -1;

class utils {
public:
    int8_t DebugMode_extern;

    int64_t _recvfrom(SOCKET fd, pvoid buffer, 
                                size_t buffer_size, 
                                uint8_t flags, 
                                psockaddr __lsock, 
                                socklen_t __lsock_size);

    int64_t _sendto(SOCKET fd, pvoid buffer, 
                              size_t buffer_size, 
                              uint8_t flags, 
                              psockaddr __ssock, 
                              socklen_t __ssock_size);

    int64_t _recv(SOCKET fd, pvoid buffer, 
                            size_t buffer_size, 
                            uint8_t flags);

    int64_t _send(SOCKET fd, pvoid buffer, 
                            size_t buffer_size, 
                            uint8_t flags);

    int8_t _connect(SOCKET fd, psockaddr __csock, 
                               socklen_t __csock_size);

    int8_t _listen(SOCKET fd, int64_t queue);
    SOCKET _accept(SOCKET fd, psockaddr __lsock, 
                              psocklen_t __lsock_ssize);

    int8_t _bind(SOCKET fd, psockaddr __bsock, 
                            socklen_t __bsock_size);

    void delay(uint32_t mseconds);
public:
    utils(int8_t DebugMode);
};

class peer {
private:
    std::map<std::string, int32_t> hybrids;
    std::map<std::string, std::string> users;
    std::map<std::string, uint16_t> ports;

    std::vector<std::string> bigDataBuffer;

    std::vector<std::string> Nicknames;
    std::vector<std::string> Addresses;

    win_netbuffer broadcastBuffer;

    SOCKET fdAcceptYourListenSock = NULL;

    int8_t DebugMode;
public:
    peer(std::string nickname, 
         std::string nicknameNode, 
         std::string address, 
         std::string addressNode, 
         uint16_t port, 
         uint16_t portNode, 
         uint8_t DebugMode_i);

    void peerInit();

    void peerToPeerConn(std::string nickPeer);
    void SendToPeer(std::string msg);
    std::string getMembers();
    void peerBroadcast();
};

class sigSignalServer {
private:
    std::map<std::string, std::string> users;
    std::map<std::string, uint16_t> ports;

    std::vector<std::string> bigDataBuffer;

    std::vector<std::string> Nicknames;
    std::vector<std::string> Addresses;

    win_netbuffer nameBuffer;

    int8_t DebugMode;
public:
    sigSignalServer(std::string nodeName, 
                    std::string addressNode, 
                    uint16_t portNode, 
                    uint8_t DebugMode_i);
    
    void sigInit();
};