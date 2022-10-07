#include <WinSock2.h>
#include <vector>
#include <string>
#include <thread>
#include <map>
#include "utils.h"

constexpr int ErPeerResponce = 0xf;
constexpr int ConnTo         = 0xe;
constexpr int GetM           = 0xd;
constexpr int Brd            = 0xb;

typedef sockaddr_in   net;
typedef SOCKET   net_sock;
typedef unsigned int uint;
typedef sockaddr *  net_p;
typedef char *  WinBuffer;

class node : public p2p_utils {
private:
    std::map<std::string, uint16_t> __Ap_peers;
    std::vector<std::string> id_names;
    std::vector<int8_t> ErrorCodes;
    uint16_t node_port;
    std::string __Addr_node;
    uint size_net = sizeof(net);
    void er_useless_peers();
    void init();
public:
    node(std::string, std::string, uint16_t);
    void getlasterror();
    void init_peer_server();
};

class peer : public p2p_utils {
private:
    uint size_net = sizeof(net);
    std::vector<int8_t> ErrorCodes;
    std::string peer_addr, nick;
    uint16_t peer_port;
public:
    peer(std::string, std::string, uint16_t);
    void init_peer();
    void init_connection();
};