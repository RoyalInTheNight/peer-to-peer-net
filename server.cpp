#include "p2phb.h"

int main() {
    node api("NodeName", "0.0.0.0", 4444);

    api.init_peer_server();
}