#include "p2phb.h"

int main() {
    peer api("akkSCAR", "127.0.0.1", 4444);

    api.init_peer();
}